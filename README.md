# uBitX_Teensy
uBitx with a Teensy 3.2 instead of the Nano.  Since the Teensy can appear in Window as a 16 bit soundcard, the BitX can run Digital Modes without any audio cables.

The wiring of the Teensy is described in the .ino file.  Circuits are from the recommended circuits of the Teensy Audio Library.
The DAC wiring has a series 1k resistor, 6.8uf cap, 500 ohm pot, 6.8uf cap, 1k resistor to MIC on the audio connector.
The PWM filter is 10n and 4.7mh, so is the Audio Library circuit with the inductor added.  It goes to the SPK on the audio connector.  LM386 is removed from the socket.
Audio is sampled on VOL-M on the audio connector.  It goes through the circuit as in the Audio Library and then to A2 on the Teensy.  I have a jumper option for connecting Analog ground to Digital ground or leave them separate.

The ATU is a separate project using an ATU board from the uSDX SOTA design.  Due to part shortages and the desire to have the ATU communicate with the Teensy, the ATU has a 18F2220 instead of a 16F1938.  A software serial port allows communication at 1200 baud. It was also desired to be able to program in circuit with a PicKit 2 and some I/O lines were re-purposed for Vcc, ground and Vpp to allow signals to line up on the header.  One leg of the 18F2220 must be left out of the socket as the net is wired to pin 1 for Vpp.  The on board regulator and 12 volt input  are not used. 
