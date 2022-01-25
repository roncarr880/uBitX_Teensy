
/*
 *    uBitX_Teensy
 *    
 *    Using a Teensy 3.2 for control and USB audio for soundcard modes ( Teensy can appear in Windows as a 16 bit sound card )
 *    
 *    First the ILI9341 will need to be converted to 3.3 volt operation.  Currently all on 5 volts which apparently works but runs hot.
 *      On the ILI9341 remove the solder jumper at J1.  Install an 82 ohm - 100 ohm resistor from pin 1 ( vcc ) to pin 8 (LED) on top
 *        of the connector pins.
 *      On the raduino board remove the 7805.  Cut the etch to pin 4 ( reset ) on the outside of the connector near the edge of the board.
 *        There is not much room here.  This removes +5 from the reset pin.    Remove the 22 ohm surface mount resistor near pin 8.  This 
 *        removes the LED pin from +5 volts.  The LED was wired as above and now the etch run from the 22 ohm resistor is on the net 
 *        for pin 4.  Wire the via or surface mount pad on this etch run to 3.3 volts on the nano socket to put 3.3 volts on the reset pin.
 *      Install the 7805 without the heatsink on the opposite side from where it was.  Bend the leads at 90 degrees.  Make sure it is   
 *        installed correctly - the back of the metal tab will be up with the body of the 7805 about covering the via where 3.3 volts was 
 *        wired.  Have the 7805 a little off the board so it doesn't heat the wire we just put on side 2.
 *      Mark the board for 3.3 volt operation. The Nano can no longer be used with this Raduino.  Test with a current limiting supply.  
 *      Power up with no processor, no screen.  Short out the 3.3 volt run to protect the Si5351 in case the 7805 is in backwards.  
 *      It should draw about ______ ma.  Test that the 5 volt net has 5 volts.  
 *            
 *    The DAC pin will do triple duty, sound to speaker, sound to microphone in for digital modes, AGC and ALC control.
 *      Audio down to the main board via the sidetone pin.  Circuit mods needed and some trim pots for levels.
 *      For digital modes turn the volume control down so you don't hear the transmit tones.
 *      For digital modes will need to remove the microphone from the jack or will tx room noise. 
 *    Will need a jumper to the main board to pick up audio for A2 analog input. May need an op-amp to increase the signal level.
 *      
 *    RX audio read via A2 AC coupled, processed and output via the Teensy DAC pin.  Using Teensy Audio Library.
 *    12 or 13 usable bits out of 16 bits sampled, will need some form of AGC to keep the A/D in range.
 *      
 *    For CW mode
 *      Can control power out via the to be installed ALC/AGC attenuator in the audio line to/from the last mixer.
 *      Perhaps can control CW power also via the Si5351 drive ( 2ma, 4ma, 6ma, 8ma ).
 *      Sidetone keyed with the keyer, actual transmit always behind by the relay on delay time( 20 ms or maybe shorter is ok ).
 *         So no short dits or dahs when first key up.
 *      Read the key via A3.  Use resistor level shifters for 5 volts to 3.3 volts.  DC coupled.  Extract value from Audio Library.
 *         
 *    Maybe widen the 11 meg filter.  Can then set the bandwidth on RX via some processing.  Tx with mic not processed so can't 
 *      make it too wide.  The caps on the main board for the filter are very small, not 0805 size. 
 *      
 *    Values from standard radio,  the PLL remains fixed.
 *      Clock 0 : BFO  11,056,400
 *      Clock 1 : LSB  33,948,600
 *      Clock 1 : USB  56,061,400
 *      Clock 2 : VFO  48 meg to 75 meg
 *      1st IF  :      45,005,000
 *      XTAL Base :    25,000,000
 *      PLL divider    35
 *      PLL       :   875,000,000
 *      CAL       :       189,000
 *      PLL for calc  875,189,000
 *      Actual XTAL    25,005,400  if CAL was zero   ( 189000/35 )
 *      
 *    Will break out the VFO from this scheme so that it uses the unused PLL and even dividers for maybe lower jitter.
 *      Maybe can run a divider of 12.  900/12 = 75.  600/12 = 50.  48*12 = 575 - so 80 meters runs PLL slightly out of spec.
 *      Maybe can use the R dividers to put the CW transmit signal where we want for each band.
 *        691.2/12/16 =  3.6 meg
 *        681.6/12/8  =  7.1 meg
 *          30 meters ?
 *        676.8/12/4  = 14.1 meg
 *          15 meters ?
 *        674.4/12/2  = 28.1 meg
 *      This doesn't seem to be a good idea.  So just change the divider to something other than 12 for CW transmit and reset the PLL. 
 *      
 *      ATU board.
 *        Probably will want to run the I2C through a level converter and run the PIC on the ATU at 5 volts.   
 *        Mounting will be tight.  Maybe just mount using a bnc on the back panel. 
 *        A connector exists to pick up RF on the main board. Will need to wire 5 volts and I2C.
 *        Maybe can separate 5 volt runs so the PIC can run with the relays off, relays only on during TX like the rest of the radio.
 *          Or would need to use I2C to tell ATU to set the relays. 
 *        Poll during TX to display the FWD and REF power.  Could display the L and C also.  
 *      
 *      User interface. 
 *      Use the Touch screen or not?  Thinking touch paddles could work as buttons when not in CW mode.  Easier than pushing the encoder.  
 *        Dit Dah touch menu:  Dit or encoder brings up menu, Dah or encoder scrolls selections, Dit or encoder makes a selection and exits.
 *        Use auto RIT with this radio so can't knock TX off freq.
 *        
 *      Display: use the LED fonts for main tuning display or play with an analog simulation.  
 *        Can display audio scope view or audio FFT, no RF view unless put an IF tap in place. ( and then its a completely different project. )
 *        Can decode CW, Hellschrieber, etc.
 *        
 *      DSP bandwidth filters for CW. Notch, de-noise possible.  AM would be difficult with the narrow filter, could try it. 
 *      
 *      Can we mount the finals differently, on some thin wood maybe?  CPU computer fan for cooling?
 *        
 */


void setup() {


}

void loop() {
  

}
