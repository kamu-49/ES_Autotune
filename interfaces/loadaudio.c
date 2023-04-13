#include <xilfifo.h>
#include <xscugic.h>
#include <sys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <fcntl.h>

u32 Mic_Init(void)
{
	//assigning fresh fcp to address received by xilfifo_lookupcnfig
	//return on error
	xil_printf("[INFO] Looking for FIFO configuration...\r\n");
	_Fifo_ConfigPtr = XLlFfio_LookupConfig(XPAR_FIFO_DEVICE_ID);
	if(_Fifo_ConfigPtr == NULL)
	{
		xil_printf("[ERROR] Invalid FIFO configuration!\r\n");
		return XST_FAILURE;
	}
	//FIFO initialization. return on eror or inability to register/initalize. else continue
	xil_printf("[INFO] Initialize FIFO...\r\n");
	if(XLlFifo_CfgInitialize(&_Fifo, _Fifo_ConfigPtr, _Fifo_ConfigPtr->BaseAddress) != XST_SUCCESS)
	{
		xil_printf("[ERROR] FIFO initialization failed!\n\r");
		return XST_FAILURE;
	}
	//GIC(generic interrupt controller) configuration. return on error if no space/wrong initialization. else continue
	xil_printf("[INFO] Looking for GIC configuration...\r\n");
	_GIC_ConfigPtr = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	if(_GIC_ConfigPtr == NULL)
	{
		xil_printf("[ERROR] Invalid GIC configuration!\n\r");
		return XST_FAILURE;
	}
	//GIC initialization. return on error and if addressing is incorrect/unavailabe and/or cfginitialize func does not return 0
	xil_printf("[INFO] Initialize GIC...\r\n");
	if(XScuGic_CfgInitialize(&_GIC, _GIC_ConfigPtr, _GIC_ConfigPtr->CpuBaseAddress) != XST_SUCCESS)
	{
		xil_printf("[ERROR] GIC initialization failed!\n\r");
		return XST_FAILURE;
	}

	//handler setup. 
	//using GIC pointer similar to initalization in order to create a priority 'trigger type'
	//return on error if connection was incomplete/not correctly created. else enable the GIC
	xil_printf("[INFO] Setup interrupt handler...\r\n");
	XScuGic_SetPriorityTriggerType(&_GIC, XPAR_FABRIC_FIFO_INTERRUPT_INTR, 0xA0, 0x03);
	if(XScuGic_Connect(&_GIC, XPAR_FABRIC_FIFO_INTERRUPT_INTR, (Xil_ExceptionHandler)AudioPlayer_FifoHandler, &_Fifo) != XST_SUCCESS)
	{
		xil_printf("[ERROR] Can not connect interrupt handler!\n\r");
		return XST_FAILURE;
	}
	XScuGic_Enable(&_GIC, XPAR_FABRIC_FIFO_INTERRUPT_INTR);

	//exception enabling and interrrupt
	//not sure if this one is truly necessary yet but put in here just in case
	// enables the exception handler and interrupt handler aswell.
	xil_printf("[INFO] Enable exceptions...\r\n");
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &_GIC);
	Xil_ExceptionEnable();

	xil_printf("[INFO] Enable FIFO interrupts...\r\n");
	XLlFifo_IntClear(&_Fifo, XLLF_INT_ALL_MASK);

	//attepmt towards utilizing the clock of our system. initializes clock wizard
	//returns on error if addressing is incorrect or neither initialization or getting the already initialized clocking wizard does not return 0
	xil_printf("[INFO] Initialize Clocking Wizard...\r\n");
	if((ClockingWizard_Init(&_ClkWiz, XPAR_CLOCKINGWIZARD_BASEADDR) || ClockingWizard_GetOutput(&_ClkWiz, &_AudioClock))!= XST_SUCCESS)
	{
		xil_printf("[ERROR] Clocking Wizard initialization failed!\n\r");
		return XST_FAILURE;
	}
	/*
	xil_printf("[INFO] Mount SD card...\r\n");
	if(SD_Init())
	{
		xil_printf("[ERROR] Can not initialize SD card!\n\r");
		return XST_FAILURE;
	}
	*/

	return XST_SUCCESS;
}

if(Mic_Init() != XST_SUCCESS)
{
	xil_printf("[ERROR] Can not initialize mic. Abort...\n\r");
	return XST_FAILURE;
}

if(Mic_LoadFile("Audio.wav"))
{
	xil_printf("[ERROR] Can not open Audio file!\n\r");
	return XST_FAILURE;
}

u32 Mic_LoadFile(char* File)
{
	if(FPGA_LoadFileFromCard(File, &_File))
	{
		xil_printf("[ERROR] Can not open Audio file!\n\r");
		return XST_FAILURE;
	}

	xil_printf("	File size: %lu bytes\n\r", _File.Header.ChunkSize + 8);
	xil_printf("	File format: %lu\n\r", _File.Format.AudioFormat);
	xil_printf("	Channels: %lu\n\r", _File.Format.NumChannels);
	xil_printf("	Sample rate: %lu Hz\n\r", _File.Format.SampleRate);
	xil_printf("	Bits per sample: %lu bits\n\r", _File.Format.BitsPerSample);
	xil_printf("	Block align: %lu bytes\n\r", _File.Format.BlockAlign);
	xil_printf("	Data bytes per channel: %lu bytes\n\r", _File.Header.ChunkSize / _File.Format.NumChannels);
	xil_printf("	Samples: %lu\n\r", 8 * _File.Header.ChunkSize / _File.Format.NumChannels / _File.Format.BitsPerSample);
	AudioPlayer_ChangeFreq(_File.Format.SampleRate);

	if(( _File.Format.BitsPerSample != 16) || (_File.Format.NumChannels > 2))
	{
		xil_printf("[ERROR] Invalid file format!\n\r");
		return XST_FAILURE;
	}

	XLlFifo_TxReset(&_Fifo);
	XLlFifo_IntEnable(&_Fifo, XLLF_INT_ALL_MASK);
//	SD_CopyDataIntoBuffer(_FifoBuffer, 256);
//	AudioPlayer_CopyBuffer();

	return XST_SUCCESS;
}

u32 FPGA_LoadFileFromCard(const char* FileName, Wave_t* File)
{
	xil_printf("[INFO] Opening file: %s...\n\r", FileName);

	if(f_open(&_FileHandle, FileName, FA_READ))
	{
		xil_printf("[ERROR] Can not open audio file!\n\r");
		return XST_FAILURE;
	}

	if(f_read(&_FileHandle, &File->RIFF, sizeof(Wave_RIFF_t), &_BytesRead) || f_read(&_FileHandle, &File->Format, sizeof(Wave_Format_t), &_BytesRead))
	{
		xil_printf("[ERROR] Can not read SD card!\n\r");
		return XST_FAILURE;
	}

	Wave_Header_t Header;
	uint32_t Offset = sizeof(Wave_RIFF_t) + sizeof(Wave_Format_t);
	if(f_read(&_FileHandle, Header.ChunkID, sizeof(Wave_Header_t), &_BytesRead) || f_lseek(&_FileHandle, Offset))
	{
		xil_printf("[ERROR] Can not read!\n\r");
		return XST_FAILURE;
	}
	
	if(strncmp("LIST", Header.ChunkID, 4) == 0)
	{
		Offset += Header.ChunkSize + sizeof(Wave_Header_t);
		if(f_read(&_FileHandle, &File->ListHeader, sizeof(Wave_Header_t), &_BytesRead) || f_lseek(&_FileHandle, Offset))
		{
			xil_printf("[ERROR] Can not place pointer!\n\r");
			return XST_FAILURE;
		}
	}

	if(f_read(&_FileHandle, &File->DataHeader, sizeof(Wave_Header_t), &_BytesRead))
	{
		xil_printf("[ERROR] Can not read!\n\r");
		return XST_FAILURE;
	}

	if(File->Format.AudioFormat != WAVE_FORMAT_PCM)
	{
		xil_printf("[ERROR] Audio format not supported! Keep sure that the file use the PCM format!\n\r");
		return XST_FAILURE;
	}

	_RemainingBytes = File->DataHeader.ChunkSize;

	_IsBusy = true;

	return XST_SUCCESS;
}

static void Mic_ChangeFreq(const u32 SampleRate)
{
	if(SampleRate == 44100)
	{
		xil_printf("	Use clock setting 1...\n\r");
		_ClkWiz.DIVCLK_DIVIDE = 5;
		_ClkWiz.CLKFBOUT_MULT = 42;
		_ClkWiz.CLKFBOUT_Frac_Multiply = 0;
		_AudioClock.DIVIDE = 93;
		_AudioClock.FRAC_Divide = 0;
	}
	else if(SampleRate == 48000)
	{
		xil_printf("	Use clock setting 2...\n\r");
		_ClkWiz.DIVCLK_DIVIDE = 3;
		_ClkWiz.CLKFBOUT_MULT = 23;
		_ClkWiz.CLKFBOUT_Frac_Multiply = 0;
		_AudioClock.DIVIDE = 78;
		_AudioClock.FRAC_Divide = 0;
	}
	else if(SampleRate == 96000)
	{
		xil_printf("	Use clock setting 3...\n\r");
		_ClkWiz.DIVCLK_DIVIDE = 3;
		_ClkWiz.CLKFBOUT_MULT = 23;
		_ClkWiz.CLKFBOUT_Frac_Multiply = 0;
		_AudioClock.DIVIDE = 39;
		_AudioClock.FRAC_Divide = 0;
	}

	ClockingWizard_SetClockBuffer(&_ClkWiz);
	ClockingWizard_SetOutput(&_ClkWiz, &_AudioClock);
}

/**
int main()
{
	unsigned int fifospace;
	volatile int * audio_ptr = (int *) 0xFF203040; // audio port
	
	while (1)
	{
		fifospace = *(audio_ptr+1); // read the audio port fifospace register
		if ((fifospace & 0x000000FF) > 0 &&		// Available sample right
			(fifospace & 0x00FF0000) > 0 &&		// Available write space right
			(fifospace & 0xFF000000) > 0)		// Available write space left
		{
			int sample = *(audio_ptr + 3);	// read right channel only
			*(audio_ptr + 2) = sample;		// Write to both channels
			*(audio_ptr + 3) = sample;
		}
	}
}
**/
