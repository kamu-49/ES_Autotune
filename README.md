# ES_Autotune

## Plan
- initialize mic
- find a way to load and store the file (maybe SD?)
- focusing solely on input? Not focusing on both input and output, solely input.
- see Adam's code and see how the software should ride off the back of that
    - once we understand what he has, we'll be able to update and branch off of his systemverilog-to-be code; but will attempt 1-3 in the meantime

## Things to Add
- AudioCODEC: controls to send it to the signal
- I2S
- Speaker?
- Ability to load and play audio through speaker

## Links to remember
- https://github.com/Xilinx/embeddedsw/blob/master/lib/bsp/standalone/src/arm/cortexr5/xil_cache.h
- https://kampi.gitbook.io/fpga/i2s/let-s-play-some-audio
### Not-confident-are-super-related links to remmeber
- https://github.com/hansfbaier/adat-usb2-audio-interface
- https://www.reddit.com/r/FPGA/comments/ka50s1/de1soc_cyclone_v_audio_implementation/
- http://www-ug.eecg.utoronto.ca/desl/nios_devices_SoC/dev_audio.html
- http://www-ug.eecg.toronto.edu/msl/manuals/tutorial_DE1-SoC-v5.4.pdf

### Links found during the journey
- https://support.xilinx.com/s/question/0D52E00006hpX9RSAU/help-fatal-error-xllfifoh-no-such-file-or-directory?language=en_US
