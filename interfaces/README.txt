consider installing:
install_dir\Vitis\2019.2\data\embeddedsw\XilinxProcessorIPLib\drivers\llfifo_v5_3\src

SOME STRUGGLES:
-


MEETING STUFF:
- how much mem?
- how are we going to store mem?
- how do you talk from software to hardware/the autotune hardware?
- how do you tell it what to do and how do you get data from it?
- ***given that we know the autotune algorithm, how is that going to be implemented into hardware?
- block diagram:
    - output file*
    - how does the autotune block interact with memory
    - what is inside the autotune block
    - planning to do in real time?
        - do audotune stuff while everythign interacts with autotune codec?
- consider getting better estimates of values (audio, I/O, bits, etc)
- keep 2 frames worth of AOT outputs
    - will be around 1k samples    
- can consider running this at full speed
- what is the interface to the FFT block that the Intel folks provide?
    - if use block, need to know and understand
    - why you want to try to implmeent HW in C
-# of big issues with python
    - be precise with the amount of memory being used and how being used and whem
    - don't have floating point. all fixed point
        -16 to 24 bit should be enough, but going to code that with range in mind will cause issues
    - need a lot of parallelism to make this work, may be difficult with python
    - C is better wih handling signal processing (i think)
- still can be made to work with python tho. just would be more work
- focus primarily on mic in, speaker out
    - can bring output as a wav file, won't be end of the world 