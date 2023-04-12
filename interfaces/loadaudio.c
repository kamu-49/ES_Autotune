u32 Mic_Init(void)
{
	xil_printf("[INFO] Looking for FIFO configuration...\r\n");
	_Fifo_ConfigPtr = XLlFfio_LookupConfig(XPAR_FIFO_DEVICE_ID);
	if(_Fifo_ConfigPtr == NULL)
	{
		xil_printf("[ERROR] Invalid FIFO configuration!\r\n");
		return XST_FAILURE;
	}

	xil_printf("[INFO] Initialize FIFO...\r\n");
	if(XLlFifo_CfgInitialize(&_Fifo, _Fifo_ConfigPtr, _Fifo_ConfigPtr->BaseAddress) != XST_SUCCESS)
	{
		xil_printf("[ERROR] FIFO initialization failed!\n\r");
		return XST_FAILURE;
	}

	xil_printf("[INFO] Looking for GIC configuration...\r\n");
	_GIC_ConfigPtr = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	if(_GIC_ConfigPtr == NULL)
	{
		xil_printf("[ERROR] Invalid GIC configuration!\n\r");
		return XST_FAILURE;
	}

	xil_printf("[INFO] Initialize GIC...\r\n");
	if(XScuGic_CfgInitialize(&_GIC, _GIC_ConfigPtr, _GIC_ConfigPtr->CpuBaseAddress) != XST_SUCCESS)
	{
		xil_printf("[ERROR] GIC initialization failed!\n\r");
		return XST_FAILURE;
	}

	xil_printf("[INFO] Setup interrupt handler...\r\n");
	XScuGic_SetPriorityTriggerType(&_GIC, XPAR_FABRIC_FIFO_INTERRUPT_INTR, 0xA0, 0x03);
	if(XScuGic_Connect(&_GIC, XPAR_FABRIC_FIFO_INTERRUPT_INTR, (Xil_ExceptionHandler)AudioPlayer_FifoHandler, &_Fifo) != XST_SUCCESS)
	{
		xil_printf("[ERROR] Can not connect interrupt handler!\n\r");
		return XST_FAILURE;
	}
	XScuGic_Enable(&_GIC, XPAR_FABRIC_FIFO_INTERRUPT_INTR);

	xil_printf("[INFO] Enable exceptions...\r\n");
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &_GIC);
	Xil_ExceptionEnable();

	xil_printf("[INFO] Enable FIFO interrupts...\r\n");
	XLlFifo_IntClear(&_Fifo, XLLF_INT_ALL_MASK);

	xil_printf("[INFO] Initialize Clocking Wizard...\r\n");
	if((ClockingWizard_Init(&_ClkWiz, XPAR_CLOCKINGWIZARD_BASEADDR) || ClockingWizard_GetOutput(&_ClkWiz, &_AudioClock))!= XST_SUCCESS)
	{
		xil_printf("[ERROR] Clocking Wizard initialization failed!\n\r");
		return XST_FAILURE;
	}
	
	xil_printf("[INFO] Mount SD card...\r\n");
	if(SD_Init())
	{
		xil_printf("[ERROR] Can not initialize SD card!\n\r");
		return XST_FAILURE;
	}

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
	if(SD_LoadFileFromCard(File, &_File))
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
	SD_CopyDataIntoBuffer(_FifoBuffer, 256);
	AudioPlayer_CopyBuffer();

	return XST_SUCCESS;
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
