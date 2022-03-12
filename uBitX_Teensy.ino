
/*
 *    uBitX_Teensy
 *    
 *    Using a Teensy 3.2 for control and USB audio for soundcard modes ( Teensy can appear in Windows as a 16 bit sound card )
 *    
 *    First the ILI9341 will need to be converted to 3.3 volt operation.  Currently all on 5 volts which apparently works but runs hot.
 *      On the ILI9341 remove the solder jumper at J1.  Install an 82 ohm or 100 ohm resistor from pin 1 ( vcc ) to pin 8 (LED) on top
 *        of the connector pins.
 *      On the raduino board remove the 7805.  Cut the etch to pin 4 ( reset ) on the outside of the connector near the edge of the board.
 *        There is not much room here.  This removes +5 from the reset pin.    Remove the 22 ohm surface mount resistor near pin 8.  This 
 *        removes the LED pin from +5 volts.  The LED was wired as above and now the etch run from the 22 ohm resistor is on the net 
 *        for pin 4.  Wire the via or surface mount pad on this etch run to 3.3 volts on the nano socket to put 3.3 volts on the reset pin.
 *      Install the 7805 without the heatsink on the opposite side from where it was.  Bend the leads up at 90 degrees.  Make sure it is   
 *        installed correctly - the back of the metal tab will be up with the body of the 7805 about covering the via where 3.3 volts was 
 *        wired.  Have the 7805 a little off the board so it doesn't heat the wire we just put on side 2. I pop riveted a small piece of
 *        aluminium to the 7805 for some extra heat sinking.
 *      Remove R2, a pullup to 5 volts on signal KEYER. 
 *      Mark the board for 3.3 volt operation. The Nano can no longer be used with this Raduino.  Test with a current limiting supply, a  
 *      worn out 9 volt battery from a smoke detector works.  Power up with no processor, no screen.  Short out the 3.3 volt run to protect 
 *      the Si5351 in case the 7805 is in backwards.  It should draw about 3 ma.  Test that the 5 volt net has 5 volts.  Insert the screen 
 *      and power up.  It should draw about 60 ma on the depleted battery and the screen should light.
 *            
 *            
 * A plan outline:    
 * 
 *    The easy idea:
 *       Use the signals on the Audio connector.  Very little modifications to the main board.  ( need CW key jack mods )
 *       Remove the LM386 from the socket.  Sample audio on VOL-M.  Volume control works like an RF gain control adjusting level into the DSP.
 *       Connect the DAC to MIC via capacitor, pot for level control, capacitor.  USB audio for DIGI mode transmit uses the DAC.
 *       Receive Audio to the speaker/headphones using PWM to the SPK on the audio connector. Suspect speaker volume will be low, headphone  
 *       volume will be loud. Receive audio will also be sent to USB for DIGI modes. 
 *       
 *    CW key jack:  replace the DIT series resistor R3 with 10 ohm.  Wire PTT from the MIC jack  over to the DAH contact on the key jack.  
 *       Remove the series resistor R2.  Keyer works with digital signals, KEYER and PTT as DIT and DAH.
 *    
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
 *      The existing Si5351 scheme seems to work ok, may not implement this idea.  
 *      
 *      ATU board.   An ATU-100 mini clone from uSDX SOTA radio.
 *        Probably will want to run the I2C through a level converter and run the PIC on the ATU at 5 volts.   
 *        Mounting will be tight.  Maybe just mount using a bnc on the back panel. 
 *        A connector exists to pick up RF on the main board. Will need to wire 5 volts and I2C.
 *        Maybe can separate 5 volt runs so the PIC can run with the relays off, relays only on during TX like the rest of the radio.
 *          Or would need to use I2C to tell ATU to set the relays. 
 *        Poll during TX to display the FWD and REF power.  Could display the L and C also.  
 *        Top cover 1/4 inch higher may be needed for space to mount the ATU.
 *        
 *      Could construct a Teensy 4.1 version  ( make sure all pullups to 5 volts removed )
 *        Use the Teensy audio shield.  Ordered a Softrock with K2 IF, about 4.914 mhz.  Can receive using the IQ DSP and transmit using 
 *        the built in SSB filter at 11 mhz.
 *        This would be a totally different project.  Could order a different v6 next year.  
 *        Could put Softrock IQ on a jack and run HDSDR or a Teensy remote head. 
 *          Would need to know the Audio IF in use to make this work as transceiver. 
 */

/*  Change log
 *   
 *      Implemented basic control of the receiving functions, mode, band switching.
 *      Added 160 meters to the band stack.  Can listen there, would need an external filter to try transmitting.
 *      Added a USA license class indicator to show when in a ham band and what class license is needed to transmit.
 *      Added some frequency presets to the 60 meter menu - 630 meters, some ssb aircraft frequencies.
 *      Added rudimentary CAT control using Argonaut V protocol.  Can fix any inaccuracies as needed. 
 *      Added TR switching.
 *      Added a keyer with TR sequencing.  Avoids sending while the relays are switching.  No sidetone yet. 
 *      Added a keyer mode submenu. Mode A, Mode B, Straight, Ultimatic 
 *      Added some touch targets to the hidden menu.  Touch area is much larger than the targets.  Hidden menu no longer hidden.
 *      Added a multi function menu where values are changed with the encoder. ( keyer speed for example )
 *      Added a toogles menu where values are turned on and off ( swap keyer Dit and Dah for example )
 *      Tried the idea of different cw power levels using the Si5351 drive levels 2,4,6,8, ma.  No change in output power so removed. 
 *      Re-wired some pins on the Teensy adapter making Teensy pins 3,4 available for use with the PWM library object.
 *      Wiring changes for the key jack, R2 removed, R3 replaced with 10 ohm, PTT jumpered to the Key jack for DAH.
 *        Jumper pin PTT on MIC jack to same pin on the Key Jack.  PTT is the etch run that goes directly to pin 2 on connector.
 *      Added audio library elements.  Compile for USB type Serial,Midi,Audio.   
 *      Removed the LM386 and converted to Digital audio.  USB receive audio works.
 *      Enabled TX USB audio for DIGI mode in tx() function.  It seems I wired the tx level pot backwards, CCW increases the tx drive.
 *      Added a fine tune clarify to help with frequency drift due to temperature changes. Changes both transmit and receive frequency.
 *        This is not a RIT control.
 *      Added PWM filter for headphone audio, 10n and 4.7mh.
 *      A/D may be quieter without connecting Analog ground to Digital ground.
 *      Added some TX/RX sequencing to the PTT function.  Relays and bi-directional amps take somewhere between 32 and 42 ms to switch.
 *      Added a tune toggle for the to be installed auto tuner.
 *      Re-wrote all the tx/rx sequencing to use a common function.  CW is now 64 ms behind sidetone.  
 *      Note: Library object PWM only works when cpu speed is 48 or 96.  For the T3.2 that is underclocked or overclocked.
 *      Added cw sidetone. And audio muting during TX.
 *      Added a more complicated audio library model for current and planned features.
 *      Implemented an AM detector. AM is tuned off frequency and the recified. 
 *        It is followed with a highpass to restore the dc level and a lowpass to try to filter out the carrier.
 *        Ended up with tuning the BFO off frequency to pass the AM signal at an audio IF of 3.5 to 6.5 or thereabouts.
 *      Removed R2 on the Raduino.  I think all signals are now 3.3 volts and safe for a Teensy 4.1 for future experiments.   
 *      Wired a jumper for analog ground.  Can see if signals are better with or without connecting the grounds.
 *      Added DIGI drive level per band.  Set for output of 5 watts at 12.5 volts.  Set tune power to 25% approximately 1 watt.
 *        Power out is lower on 15, 12 and 10 meters. 
 *      Wired up the ATU.  PIC16F1938 out of stock for 1 year.  PIC18F2220 has I2C on different pins, other pins look ok.  
 *        Wired a Pickit 2 header to the I/O interface provided, Power, Ground, Data, Clock did not line up in any useful way. 
 *           Wired the Pickit interface such that Data and Clock line up which put Vpp, Vcc and Ground on the I/O lines of 
 *           RB0-RB2.  So this method lost the 3 I/O lines and required leaving the one pin where Vpp connected out of the socket.
 *      Added an S meter using an image converted with 565 online image converter by Rinky Dink Electronics.
 *      Added a way to override 5 watt DIGI power and allow full power if desired, so a non-qrp DIGI mode.  
 *        15 12 and 10 will still be < 5 watts.
 *      Added AGC.  Changed volume to range to 0.99, no gain.  Think following BiQuad distorts on loud signals if volume is 1.0 or above.
 *        Gain boosted with agc_gain now. 
 *      
 *  To do.
 *      Review power levels again.  I twiddled the MIC gain pot a bit to make sure it was in a linear range.
 *        Think I should set this for 5 watts on 17 meters instead of 15 meters as I did before.
 *      Review PWM audio.  Is the 88k suppressed enough.  Would it sound better using the DAC and an analog mux to allow dual use
 *        for both transmit and receive?  A Teensy 3.5 would allow easier wiring with 2 DAC's. 
 *      Noticed some spurs on SSB transmit, think a 11 meg IF suckout trap on TP13 or TP14 may be useful.  Not much room there. 
 *        Farhan says move L5 and L7 to side two - signal feedback to the 45 meg filter. This would be a simple fix. 
 *        Scope FFT on TP13 and TP14 to see if have any 11 meg signal there.  Could use Scope FFT to see what stage the spurs appear. 
 *        They seem to be 11 meg mixed with the transmit frequency.  Didn't measure how high they are.  
 *        From Groups IO, maybe the mixing is in the final IRF510's.
 *        Didn't see any on 80 meters.
 *        20 meters seem to be a multi mix with spurs at +- 2.4.
 *        30 meters +- 0.9
 *        40 meters +- 3.8
 *        15 meters one at 10.0   
 *        10 meters one at 17.8
 *      Terminal Mode.
 *      The touch calibration is from a previous project, seems to be working ok but could be reviewed. 
 *      CW decoder.   Hell decoder.
 *      Audio scope, audio FFT displays.  Band scope by scanning? ( mute, scan one freq, return vfo, unmute )
 *      Noise reduction, auto notch.
 *      CW filters.   SSB high cut filter for QRM reduction. 
 *      Transmit timout timer, in case miss the CAT command to return to RX.  Maximum tx on time.  
 *      ATU firmware. 
 *      Some pictures for documentation.
*/         


// Wiring  ( not easy with Nano upside down )  Teensy mounted with USB on the other side for the shortest wiring to the display.
//  Teensy mounted upside right.
//  Special wiring will be A2, and maybe A3 for audio input.  A2 wired to VOL-M via the circuit shown in Audio Library for A/D input.
//    The DAC pin wired to MIC via some caps and a pot for level.   DAC-1k-cap-500 ohm pot-cap-1k-MIC.  Caps 6.8 or 10 uf.
//    Pins  D3, D4 wired to SPK via the circuit shown in the Audio Library for PWM output.  LM386 removed from the socket. 
//       4.7 mh added in series with the SPK for better filtering.  60 inches of wire on FT37-43. ( a little over 100 turns )
//    The Teensy audio library will control these 5 pins.
//  ILI9341 wired for standard SPI as outlined on Teensy web site and it matches the Nano wiring pin for pin
//    Uses pins 8 to 13 wired to Nano pins 8 to 13
//  I2C uses the standard A4 A5, so they are wired to Nano A4 A5
//  The keyer will make use of the PTT pin as either DIT or DAH - digital keyer inputs instead of one analog pin
//  Teensy free pins A6, A7

#define TR        7     // to nano pin 7
                        // nano pin 6 CW_TONE not wired
#define LP_A      6     // to nano pin 5 
#define LP_B      5     // to nano pin 4
#define LP_C     A9     // to nano pin 3
#define CW_KEY   A8     // to nano pin 2
#define DIT_pin  A1     // to nano pin A6  - was KEYER,  short a resistor on main board, remove pullup to 5 volts.
#define DAH_pin  A0     // to nano pin A3
#define PTT      A0     // same pin as DAH_pin,  ptt wired over to the key jack, remove a resistor on the main board
#define ENC_B     0     // to nano A0   These are the Teensy digital pins not the analog pins for the encoder
#define ENC_A     1     // to nano A1
#define ENC_SW    2     // to nano A2

// Teensy pins 3 and 4 are audio library pwm output pins.  A14 is the DAC. 

/* button states */
#define IDLE_ 0
#define ARM 1
#define DTDELAY 2
#define DONE 3    
#define TAP  4
#define DTAP 5
#define LONGPRESS 6
#define DBOUNCE 50


#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
//#include <Wire.h>
#include <i2c_t3.h>            // non-blocking wire library
//  There are control files in the audio library where include Wire.h will need to be replaced with include i2c_t3.h.  ( 7 total ? )
//  Or maybe Wire.h will work ok with this program instead of i2c_t3 if desired. 
#include <SPI.h>
#include "led_fonts.h"
#include "smeter5.h"

#define BFO 11059590L          // this puts the bfo on the high side of the filter, sharper cutoff?
#define IF  45000000L          // what is the actual center of the IF.  Testing says 45k even is about right.

         //  4bpp pallett  0-3, 4-7, 8-11, 12-15
const uint16_t EGA[] = { 
         ILI9341_BLACK,    ILI9341_NAVY,    ILI9341_DARKGREEN, ILI9341_DARKCYAN,
         ILI9341_MAROON,   ILI9341_PURPLE,  ILI9341_OLIVE,     ILI9341_LIGHTGREY,
         ILI9341_DARKGREY, ILI9341_BLUE,    ILI9341_GREEN,     ILI9341_CYAN,
         ILI9341_RED,      ILI9341_MAGENTA, ILI9341_YELLOW,    ILI9341_WHITE
         }; 

#define TFT_DC  9
#define TFT_CS 10
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
XPT2046_Touchscreen ts(8);

/*****************************   Audio Library **********************************/

#include <Audio.h>

// Audio processing model with AGC, AM detector, CW detector and other dummy objects for NR Notch and Decoders
// An issue with this model is the AGC loop is outside of the bandwidth object.
//   If outside the bandwidth object, strong signals outside of the desired bandwidth will cause the desired signal to decrease in volume.
//   If AGC loop includes the bandwidth object, strong signals outside of the desired bandwidth could overload the earlier stages.
//     Solution is two separate agc loops which is perhaps a bit complicated for this radio.
//       1st agc loop prevents overload, 2nd sets the desired listening volume.
//   The chosen implementation will only have an issue with narrow bandwidth modes like CW on a very busy band. 
// GUItool: begin automatically generated code
AudioInputAnalog         VOL_M;           //xy=223.30557250976562,286.52777099609375
AudioAnalyzePeak         peak1;          //xy=347.2500190734863,161.13887405395508
AudioOutputUSB           usb2;           //xy=348.13885498046875,207.36110305786133
AudioAmplifier           AGC;           //xy=397.77777099609375,286.3888854980469
AudioMixer4              NR_Notch;         //xy=450,357.2221984863281
AudioInputUSB            usb1;           //xy=477.52777099609375,192.5555419921875
AudioAnalyzeRMS          rms1;           //xy=479.16668701171875,234.72219848632812
AudioSynthWaveformSine   SideTone;          //xy=481.11114501953125,147.5
AudioEffectRectifier     AM_Det;       //xy=538.75,411.25
AudioMixer4              RX_SEL;         //xy=629.9999389648438,307.6666564941406
AudioMixer4              TX_SEL;         //xy=691.27783203125,181.61109924316406
AudioAmplifier           amp2;           //xy=778.75,410
AudioFilterBiquad        BandWidth;        //xy=801.666748046875,307.08331298828125
AudioMixer4              Decoders;         //xy=862.5,240.138916015625
AudioAnalyzeToneDetect   CW_Det;          //xy=917.5,410
AudioOutputAnalog        MIC;           //xy=994.166748046875,181.361083984375
AudioOutputPWM           SPK;           //xy=994.444580078125,306.3055419921875
AudioConnection          patchCord1(VOL_M, 0, usb2, 0);
AudioConnection          patchCord2(VOL_M, 0, usb2, 1);
AudioConnection          patchCord3(VOL_M, peak1);
AudioConnection          patchCord4(VOL_M, AGC);
AudioConnection          patchCord5(AGC, 0, NR_Notch, 0);
AudioConnection          patchCord6(AGC, 0, RX_SEL, 0);
AudioConnection          patchCord7(AGC, AM_Det);
AudioConnection          patchCord8(AGC, rms1);
AudioConnection          patchCord9(NR_Notch, 0, RX_SEL, 2);
AudioConnection          patchCord10(usb1, 0, TX_SEL, 1);
AudioConnection          patchCord11(SideTone, 0, RX_SEL, 1);
AudioConnection          patchCord12(SideTone, 0, TX_SEL, 0);
AudioConnection          patchCord13(AM_Det, 0, RX_SEL, 3);
AudioConnection          patchCord14(RX_SEL, BandWidth);
AudioConnection          patchCord15(TX_SEL, MIC);
AudioConnection          patchCord16(amp2, CW_Det);
AudioConnection          patchCord17(BandWidth, amp2);
AudioConnection          patchCord18(BandWidth, 0, Decoders, 0);
AudioConnection          patchCord19(BandWidth, SPK);
// GUItool: end automatically generated code

/*
// Added peak object for signal level 
// GUItool: begin automatically generated code
AudioSynthWaveformSine   SideTone;          //xy=175,170
AudioInputUSB            usb1;           //xy=176,257
AudioInputAnalog         VOL_M;           //xy=179,110
AudioMixer4              RX_SEL;         //xy=340,176
AudioMixer4              TX_SEL;         //xy=341,258
AudioAnalyzePeak         peak1;          //xy=351,51
AudioOutputPWM           SPK;           //xy=490,177
AudioOutputAnalog        MIC;           //xy=490,259
AudioOutputUSB           usb2;           //xy=493,105
AudioConnection          patchCord1(SideTone, 0, RX_SEL, 1);
AudioConnection          patchCord2(SideTone, 0, TX_SEL, 0);
AudioConnection          patchCord3(usb1, 0, TX_SEL, 1);
AudioConnection          patchCord4(VOL_M, 0, usb2, 0);
AudioConnection          patchCord5(VOL_M, 0, usb2, 1);
AudioConnection          patchCord6(VOL_M, 0, RX_SEL, 0);
AudioConnection          patchCord7(VOL_M, peak1);
AudioConnection          patchCord8(RX_SEL, SPK);
AudioConnection          patchCord9(TX_SEL, MIC);
// GUItool: end automatically generated code
*/
/*
#include <Audio.h>
//#include <Wire.h>
//#include <SPI.h>
//#include <SD.h>
//#include <SerialFlash.h>

// Digital Audio with no processing. A/D VOL_M to usb, A/D VOL_M to PWM SPK, usb to DAC MIC.  Sidetone to SPK and/or MIC.
// GUItool: begin automatically generated code
AudioSynthWaveformSine   SideTone;          //xy=175,170
AudioInputUSB            usb1;           //xy=176,257
AudioInputAnalog         VOL_M;           //xy=179,110
AudioMixer4              RX_SEL;         //xy=340,176
AudioMixer4              TX_SEL;         //xy=341,258
AudioOutputPWM           SPK;           //xy=490,177
AudioOutputAnalog        MIC;           //xy=490,259
AudioOutputUSB           usb2;           //xy=493,105
AudioConnection          patchCord1(SideTone, 0, RX_SEL, 1);
AudioConnection          patchCord2(SideTone, 0, TX_SEL, 0);
AudioConnection          patchCord3(usb1, 0, TX_SEL, 1);
AudioConnection          patchCord4(VOL_M, 0, usb2, 0);
AudioConnection          patchCord5(VOL_M, 0, usb2, 1);
AudioConnection          patchCord6(VOL_M, 0, RX_SEL, 0);
AudioConnection          patchCord7(RX_SEL, SPK);
AudioConnection          patchCord8(TX_SEL, MIC);
// GUItool: end automatically generated code
*/

/*******************************************************************************/
// globals

// Implementing a simplistic split tuning model.
//   VFO A is the receive vfo, VFO B is the transmit vfo, split is used for RIT
//   Can listen on VFO B, if split is also selected.
//   VFO B follows VFO A unless split is enabled.
//   B --> A exit split with VFO B selected.   A --> B exit spit with VFO A selected.
//  encoder switch  tap = enable split A selected (RIT), tap then toogles B or A vfo, long press B to A or A to B as above.

// radio defines for vfo_mode.
#define VFO_A  1
#define VFO_B  2
#define VFO_SPLIT  4
#define VFO_DIGI   8
#define VFO_CW    16
#define VFO_LSB   32
#define VFO_USB   64
#define VFO_AM   128

//  radio variables, set reasonable for 1st band change
//  can start a different band in setup, but will save this data to the 40 meter band_stack
//  ECARS usually has some activity to listen to
int32_t  vfo_a = 7255000, vfo_b = 7255000;
uint8_t  vfo_mode = VFO_A + VFO_LSB;
int band = 3;                          // 40 meters

int transmitting;                      // status of TR
int      cw_tr_delay = 65;             // semi breakin time, in multi fun, actual value is *10 ms.
int      fast_tune = 1;                // double tap encoder to toggle
uint8_t  cat_tx;                       // a flag that CAT is in control of the transmitter


#define STRAIGHT    0          // CW keyer modes
#define ULTIMATIC   1
#define MODE_A      2
#define MODE_B      3
#define PRACTICE_T  4          // last options toggle practice, and swap
#define KEY_SWAP    5
uint8_t key_mode = ULTIMATIC;
int  cw_practice = 1;               // toggle and multi fun variables must be int, have value 0 to 99.
int  key_swap = 0;
int  wpm = 14;
int  volume_ = 98;                  // actual * 0.01
int  clari  = 50;                   // adjustment for temperature changes, maps to +-50 hz
int  tuning;                        // low power tune via sidetone object
int side_vol = 10;                  // sidetone volume
uint8_t oob;                        // out of band flag, don't save vfo's to the bandstack.  Don't transmit.
int digi5w = 1;                     // use the digi power levels in the bandstack for digi power
int dis_info;                       // display some information in Smeter area
int lock;                           // lock vfo, allow CAT freq changes
int agc_gain = 25;                  // agc gain 0 to 9.9 with 10 == 1.0
float sig_rms;                      // global so can print
float agc_sig;                      // glabal so can print
float agc_print;


//   Touch menu's
void (* menu_dispatch )( int32_t );    // pointer to function that is processing screen touches

struct menu {
   char title[20];
   const char *menu_item[10];    // array of pointers to strings, (even number needed for 2 wide menu)
   int32_t param[10];
   int y_size;                  // x size will be half the screen, two items on a line for now
   int color;
   int current;
};

// mode menu items
const char m_vfoa[]  = " VFO A";
const char m_vfob[]  = " VFO B";
const char m_split[] = " Split";
const char m_cw[]    =  " CW";
const char m_usb[]   = " USB";
const char m_lsb[]   = " LSB";
const char m_am[]    = " AM";
const char m_digi[]  = " DIGI";

struct menu mode_menu_data = {
   { " VFO Mode (keyer)" },
   { m_vfoa,m_vfob,m_split,m_cw,m_usb,m_lsb,m_am,m_digi },
   {0,1,2,3,4,5,6,7,-1,-1},
   48,
   EGA[1],
   5
};

const char m_keys[] = " Straight";
const char m_keyu[] = " Ultimatic";
const char m_keya[] = " Mode A";
const char m_keyb[] = " Mode B";
const char m_keyp[] = " Practice";
const char m_keyw[] = " Swap";

struct menu keyer_menu_data = {
  { " Keyer Mode" },
  { m_keys,m_keyu,m_keya,m_keyb,m_keyp,m_keyw },
  { 0,1,2,3,4,5,-1,-1,-1,-1 },
  40,
  EGA[2],
  1
};


// band menu
const char b_160[] = " 160m rx";
const char b_80[] = " 80m";
const char b_40[] = " 40m";
const char b_30[] = " 30m";
const char b_20[] = " 20m";
const char b_17[] = " 17m";
const char b_15[] = " 15m";
const char b_12[] = " 12m";
const char b_10[] = " 10m";

struct menu band_menu_data = {
  { " BAND  (60m)" },                                        // 60 meters in a submenu
  { b_160, b_80, b_40, b_30, b_20, b_17, b_15, b_12, b_10 },
  { 0,1,3,4,5,6,7,8,9,-1 },                                    
  40,
  EGA[1],
  2
};

const char c_1[] = " Ch 1";
const char c_2[] = " Ch 2";
const char c_3[] = " Ch 3";
const char c_4[] = " Ch 4";
const char c_5[] = " Ch 5";
const char c_6[] = " 630 m";
const char c_7[] = " Wx Fax";
const char c_8[] = " Av NY A";
const char c_9[] = " Av NY E";
const char c_10[]= " Av Car";

struct menu band_60_menu_data = {                  // 60 meter channels with some other presets
  { "  Channel" },
  {  c_1, c_2, c_3, c_4, c_5, c_6, c_7, c_8, c_9, c_10 },
  { 5330500, 5346500, 5357000, 5371500, 5403500, 474200, 6338600, 5598000, 6628000, 5550000 },
  40,
  EGA[2],
  2
};

struct BAND_STACK {
  uint32_t  vfoa;
  uint32_t  vfob;
  uint8_t   mode;
  uint8_t   relay;
  float     digi_pwr;                            // TX_SEL gain for 5 watts out at 12.5 volts VCC, 15,12,10 lower power.
};

  // don't set up any splits here
struct BAND_STACK band_stack[10] = {
  {  1875000,  1875000, VFO_A+VFO_LSB, 4, 0.00 },      // can listen on 160 meters, tx has harmonics and spur
  {  3928000,  3928000, VFO_A+VFO_LSB, 4, 0.60 },      // relay C  for 80 and 60 meters
  {  6000000,  6000000, VFO_A+VFO_USB, 4, 0.70 },
  {  7168000,  7168000, VFO_A+VFO_LSB, 2, 0.85 },      // relay B
  { 10106000, 10106000, VFO_A+VFO_USB, 2, 0.80 },
  { 14200000, 14200000, VFO_A+VFO_USB, 1, 0.75 },      // relay A
  { 18100000, 18100000, VFO_A+VFO_DIGI, 1, 0.80 },
  { 21100000, 21100000, VFO_A+VFO_CW, 0, 0.99 },       // default lowpass
  { 24900000, 24900000, VFO_A+VFO_USB, 0, 0.99 },
  { 28250000, 28250000, VFO_A+VFO_USB, 0, 0.99 }
};

struct multi {                         // all multi variables type int, functions that use them to map to floats if needed
   char title[20];                     // value 0 to 99 - functions to map as needed
   int num;                            // now many of the 10 slots are actually in use
   const char *menu_item[10];          // variable names
   int *val[10];                       // variable pointers
   int current;                        // adjusting this one now
};

const char mf_vl[] = " Volume";
const char mf_bf[] = " Clarify";       // +- 50 hz for drift due to temperature
const char mf_ks[] = " Key Spd";
const char mf_tr[] = " cwDelay";       // 0-99 maps to 0 to 990 ms, min value is 100
const char mf_sv[] = " SideVol";       // sidetone volume
const char mf_ag[] = " AGC Vol";       // agc gain
// PWM audio volume

struct multi multi_fun_data = {
   " Multi Adj  (exit)",
   6,
   { mf_vl, mf_ag, mf_bf, mf_ks, mf_tr, mf_sv },
   { &volume_, &agc_gain, &clari, &wpm, &cw_tr_delay, &side_vol },
   0
};

const char tt_ks[] = "Key Swap";
const char tt_kp[] = "Practice";
const char tt_ft[] = "Fast Vfo";
const char tt_tt[] = "ATU TUNE";
const char tt_d5[] = "Digi  5W";
const char tt_di[] = "Info Dis";
const char tt_lk[] = "Lock VFO";

struct multi toggles_data = {
   " Toggle Values",
   7,
   { tt_ks, tt_kp, tt_ft, tt_tt, tt_d5, tt_di, tt_lk },
   { &key_swap, &cw_practice, &fast_tune, &tuning, &digi5w, &dis_info, &lock },
   2
};

// screen owners
#define DECODE  0
#define MULTI   1
#define MENUS   2
uint8_t  screen_owner = DECODE;

extern void initOscillators();
extern void si5351bx_setfreq(uint8_t clknum, uint32_t fout);
//extern void si5351_set_calibration( int32_t cal );

/*****************************************************************************************/


void setup() {

  pinMode( TR, OUTPUT );                 // set a default radio state, receive mode
  pinMode( LP_A, OUTPUT);
  pinMode( LP_B, OUTPUT);
  pinMode( LP_C, OUTPUT);
  pinMode( CW_KEY, OUTPUT);
  digitalWriteFast( TR, LOW );
  digitalWriteFast( LP_A, LOW );         // LP relays should be set up for the starting band.  Done below.
  digitalWriteFast( LP_B, LOW );         // they don't switch on until TR goes high
  digitalWriteFast( LP_C, LOW );
  digitalWriteFast( CW_KEY, LOW );

  Serial.begin(1200);                    // 2 meg usb serial, baud rate makes no difference.  Argonaut V baud rate.

  pinMode( ENC_A,   INPUT_PULLUP );
  pinMode( ENC_B,   INPUT_PULLUP );
  pinMode( ENC_SW,  INPUT_PULLUP );
  pinMode( DIT_pin, INPUT_PULLUP );      // R2 removed, R3 replaced with 10 ohm ( zero to 100 works )
  //pinMode( DAH_Pin, INPUT_PULLUP );    // connected to PTT
  pinMode( PTT,     INPUT_PULLUP );      // DAH and PTT connected together with wire on side 2


  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  
  ts.begin();                               // touchscreen
  ts.setRotation(3);                        // ? should have been 1, same as print rotation. This screen is different.
  
  tft.setTextSize(2);                       // sign on message
  tft.setTextColor(ILI9341_WHITE);          // or foreground/background for non-transparent text
  tft.setCursor(10,200);
  tft.print("K1URC uBitX w/ Teensy 3.2");   // about 25 characters per line with text size 2
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(1);
  tft.setCursor(14,230);     tft.print("Mic");
  tft.setCursor(150,230);     tft.print("Spkr");
  tft.setCursor(319-24,230);    tft.print("Key");

  Wire.begin();                             // I2C_OP_MODE_DMA, I2C_OP_MODE_ISR possible options.
  Wire.setClock(400000);
  initOscillators();                        // sets bfo
  si5351bx_setfreq( 0, BFO );               // set new bfo if changed from the default
//  si5351bx_setfreq( 1, 33948600 );          // set LSB mode
//  si5351bx_setfreq( 1, 56061400 );          // set LSB mode
  si5351bx_setfreq( 1, IF + BFO );          // set LSB mode

  vfo_freq_disp();
  vfo_mode_disp();
  set_relay( band_stack[band].relay );
 // band_change(3);                         // can start with a different band if desired
  mf_bar_disp();
  
  menu_dispatch = &hidden_menu;             // function pointer for screen touch

  // audio library setup

    AudioNoInterrupts();
    AudioMemory(40);

    SideTone.frequency(600);
    SideTone.amplitude( 0.0 );

    RX_SEL.gain(0,1.0);                 // listen to Receive audio 
    RX_SEL.gain(1,0.0);                 // sidetone muted
    RX_SEL.gain(2,0.0);                 // NR Notch
    RX_SEL.gain(3,0.0);                 // AM mode

    TX_SEL.gain(0,0.0);                 // low power tuning mode using sidetone
    TX_SEL.gain(1,0.0);                 // USB audio tx for DIGI mode
    TX_SEL.gain(2,0.0);                 // unused
    TX_SEL.gain(3,0.0);                 // unused

    MIC.analogReference(INTERNAL);      // 1.2 volt p-p

    AGC.gain( (float)(agc_gain)/10.0 );   // default signal pass through until agc loop takes over
    BandWidth.setHighpass( 0, 200, 0.707 );
    BandWidth.setLowpass( 1, 3200, 0.707 );


    AudioInterrupts();
  

}

void loop() {
static uint32_t  tm;
uint32_t t;
int t2;


   t2 = encoder();
   if( t2 ){
      if( screen_owner == DECODE && lock == 0 ) freq_update( t2 );
      if( screen_owner == MULTI ) multi_fun_encoder( t2 );
   }

   t = touch();
   if( t ) (*menu_dispatch)(t);             // off to whoever owns the touchscreen

   if( rms1.available() ){                  // agc
        sig_rms = rms1.read();
        agc_process( sig_rms);
   }


   //  One ms processing
   t = millis() - tm;
   tm += t;
   if( t > 5 ) t = 5;                       // first time or out of processing power
   while( t-- ){                            // repeat for any missed time ticks
      
      t2 = button_state(0);
      if( t2 > DONE ) button_process(t2);
      if( vfo_mode & VFO_CW ) keyer();
      if( tuning ) tune();                  // Antenna tuning
      if( transmitting ) tx_rx_seq();       // 48 ms delays for TX RX switching
     // test_1st_IF();                        // find the center of the 45mhz filter. Seems 45k about right.
   }

   radio_control();                         // CAT
   check_ptt();
   if( screen_owner == DECODE ) info_corner();
   
}



#define AGC_FLOOR  0.20             // 0.15 0.05
#define AGC_SLOPE  9                // 6
#define AGC_HANG   500              //  hang == ms time
void agc_process( float reading ){
//static float sig = AGC_FLOOR;
static int hang;
float g, g2;
int ch;                             // flag change needed
static int again;

    ch = 0;

    if( again != agc_gain ) again = agc_gain, ch = 1;     // implement gain changes when signal is below floor
    
    if( reading > agc_sig && reading > AGC_FLOOR ){       // attack 0.001
       agc_sig += 0.009,  hang = 0, ch = 1;
    }
    else if( agc_sig > AGC_FLOOR && hang++ > AGC_HANG/3 ){  // decay
       agc_sig -= 0.0001, ch = 1;
    }

    if( ch ){                             // change needed
      g2 = (float)agc_gain / 10.0;
      g = agc_sig - AGC_FLOOR;
      g *= AGC_SLOPE;
      g = g2 - g;                         // agc action reduces gain from the set value agc_gain 
      if( g <= 0 ) g = 0.1;
      set_agc_gain(g);
    }                                               

}

void set_agc_gain(float g ){

  //g = g * (float)agc_gain / 10.0;
  AudioNoInterrupts();
    AGC.gain(g);
  AudioInterrupts();
  agc_print = g;
}



#define TUNE_OFF 10000       // 10000 is 10 seconds
void tune(){          // enable a low power tune signal via DAC and SideTone object.  Sequence and timeout.
                      // called once per millisecond
    ++tuning;
    if( tuning < 0 ) tuning = TUNE_OFF;
    switch( tuning ){
        case  2:
           tx();
        break;
        case 50:                              // enable the sidetone after relays have switched
          // si5351bx_setfreq( 2, vfo_b + IF + ( clari - 50 )); handled in tx sequencer
           SideTone.frequency(1500);
           SideTone.amplitude(0.99);
           TX_SEL.gain(0,0.25);                // set mic level pot on 17 meters, 15m - 10m will be lower than other bands
           //TX_SEL.gain(0,(float)side_vol/100.0);    // !!! testing only, side_vol is the audio sidetone during CW tx
        break;
        case TUNE_OFF:                        // timeout at nn seconds
           tuning = 0;
           rx();
        break;
        default:                              // check if have 1:1 match and end early ?
        break;
    }
  
}

void info_corner(){
static uint32_t tm;                                // slow down the display
float val;
float val2;
static float val3;                                 // save peak for info as now calling the smeter more frequently
const int ls = 11;                                 // vertical spacing of lines from line 80
static int mod;

    if( millis() - tm < 25 ) return;
    if( peak1.available() == 0 ) return;
    tm = millis();
    val2 = peak1.read();
    if( val2 > val3 ) val3 = val2;

    if( ++mod >= 40 && dis_info ){
       mod = 0;
       tft.setTextSize( 1 );
       tft.setTextColor(EGA[10], EGA[0] );
       tft.setCursor( 254, 80+ 1*ls );
       tft.print("cpu ");
       tft.setCursor( 280, 80+ 1*ls );
       val = (float)AudioProcessorUsage() / 100.0 ;
       tft.print( val );

       tft.setCursor( 254, 80 + 2*ls );
       tft.print("AGC ");
       tft.setCursor( 280, 80 + 2*ls );
       tft.print( agc_print );

       tft.setCursor( 254, 80 );                       // displaying top line last for color changes
       tft.print("Sig ");
       tft.setCursor( 280, 80 );
       if( val3 > 0.95 ) tft.setTextColor( EGA[12], EGA[0] );
       else if( val3 > 0.85 ) tft.setTextColor( EGA[14], EGA[0] );
       tft.print( val3 );
       val3 = 0.0;
    }

    val2 = map( val2,0.0,1.0,1.0,10.0 );               // log scale on meter, convert 0.0 - 1.0 to a value where the log
    val2 = log10(val2);                                //  has a range of 0.0 - 1.0  ( log of 1 is zero, log of 10 is 1.0 )
    sig_pwr_meter( val2 );
}


void band_change( int to_band ){
int val;

   if( oob == 0 ){                             // only save the vfo's if we are inside an amateur band.
      band_stack[band].vfoa = vfo_a;
      band_stack[band].vfob = vfo_b;
      band_stack[band].mode = vfo_mode;
   }
   band = to_band;
   vfo_a = band_stack[band].vfoa;
   vfo_b = band_stack[band].vfob;
   vfo_mode = band_stack[band].mode;
   set_relay( band_stack[band].relay );
   vfo_freq_disp();
   vfo_mode_disp();

   // update the mode data struct as we just changed vfo_mode with the band change
   if( vfo_mode & VFO_CW ) val = 3;
   if( vfo_mode & VFO_USB ) val = 4;
   if( vfo_mode & VFO_LSB ) val = 5;
   if( vfo_mode & VFO_AM ) val = 6;
   if( vfo_mode & VFO_DIGI ) val = 7;
   mode_menu_data.current = val;
   
}

void set_relay( int num ){

   digitalWriteFast( LP_A, LOW );             // clear relays
   digitalWriteFast( LP_B, LOW );
   digitalWriteFast( LP_C, LOW );

   if( num & 1 ) digitalWriteFast( LP_A, HIGH );    // relays will actually switch during TX
   if( num & 2 ) digitalWriteFast( LP_B, HIGH );
   if( num & 4 ) digitalWriteFast( LP_C, HIGH );
}



// touch the screen top,middle,bottom to bring up different menus.  Assign menu_dispatch.
// This is default touch processing.
void hidden_menu( int32_t t ){
int32_t  yt, xt;


   screen_owner = MENUS;
   yt = t & 0xff;
   xt = t >> 8;

   // Serial.print("X ");  Serial.print(xt);  Serial.print(" Y "); Serial.println(yt);
   // touch rotation is different than the print rotation on this ILI9341
   
   // check the y value of touch to see what menu area
   if( yt < 50 ){
      menu_display( &mode_menu_data,3 );     // pass the mode hack value 3
      menu_dispatch = &mode_menu;            // screen touch goes to mode_menu() now
   }
   else if( yt < 100 ){ 
      menu_display( &band_menu_data,0 );
      menu_dispatch = &band_menu;
   }
   else if( yt < 160 && xt < 50 ){            // left side of screen, multi menu
      multi_display( &multi_fun_data, 1 );
      menu_dispatch = &multi_fun_touch;
      screen_owner = MULTI;
   }
   else if( yt < 160 && xt > 320-50 ){
      multi_display( &toggles_data, 1 );
      menu_dispatch = &toggles_touch;
   }
   /*
   else if( yt > 190 && xt > 270 && (vfo_mode & VFO_CW) ){  // keyboard CW sending
      menu_dispatch = &key_tx;
      key_tx(0);
   }
   else if( yt > 140 ){
      menu_display( &decode_menu_data );
      menu_dispatch = &decode_menu;
   }
   */
                           
   else menu_cleanup();                  // not active part of the screen, return to normal op.
}


void band_60_menu( int32_t t ){               // setup for 60 meters
int selection;                                // pick the 80 meter filter

  selection = touch_decode( t, band_60_menu_data.y_size );
  if( selection != -1 && band_60_menu_data.param[selection] != -1 ){
       // 630 meters special case in this menu
      if( band_60_menu_data.param[selection] < 3000000 ) band_change(0);   // 160 meters relays setting
      else band_change(2);
      vfo_a = vfo_b = band_60_menu_data.param[selection];
      vfo_mode = VFO_A+VFO_USB;
      // set_relay( band_stack[2].relay );           // redundant
      band_60_menu_data.current = selection;
      band_menu_data.current = 10;                 // 60 meters is not in band_menu_data, selection 2 is 40 meters - band 3
  }                                                // set it out of bounds,  no highlighting.
 // vfo_freq_disp();     redundant with cleanup
 // vfo_mode_disp();
  menu_cleanup();
}

void multi_fun_touch( int32_t t ){
int selection;

     selection = touch_decode( t, 40 );
     if( selection == -1 || selection >= multi_fun_data.num ){       // exit when touch exit, else this menu stays on the screen
        // check limits on some variables
        wpm = constrain( wpm, 10, 50 );                              // avoid divide by zero
        cw_tr_delay = constrain( cw_tr_delay, 10, 99 );              // min tr delay of 100ms max 990
        menu_cleanup();
        return;
     }
     multi_fun_data.current = selection;             // selected a value
     multi_display( &multi_fun_data, 0 );            // display all again for color highlighting
}

void toggles_touch( int32_t t ){
int selection;
int *p;

     selection = touch_decode( t, 40 );
     if( selection != -1 && selection < toggles_data.num ){ 
        toggles_data.current = selection;
        p = toggles_data.val[selection];
        if( *p > 10 ) *p = -10;                     // a hack to have a turn off toggle for tuning as it increments once enabled
        else *p ^= 1;   
     }
     menu_cleanup();
  
}

void band_menu( int32_t t ){
int selection;

   selection = touch_decode( t, band_menu_data.y_size );   //  expand for 60 meters submenu
   if( selection == -1  ){
      menu_display( &band_60_menu_data,0 );                // sub menu
      menu_dispatch = &band_60_menu;
      return;   
   }
   if( band_menu_data.param[selection] != -1 ){
     band_menu_data.current = selection;
     band_change( band_menu_data.param[selection] );
   }
   menu_cleanup();
}

void keyer_menu( int32_t t ){
int selection;

   selection = touch_decode( t, keyer_menu_data.y_size );
   if( selection == -1 ){                                    // title was touched
      menu_cleanup();
      return;
   }

   if( keyer_menu_data.param[selection] != -1 ){
      
      if( selection < 4 ){
          key_mode = keyer_menu_data.param[selection];
          keyer_menu_data.current = selection;
      }
      else if( selection == 4 ) cw_practice ^= 1;            // these toggles are also in the toggles menu
      else if( selection == 5 ) key_swap ^= 1;
   }
   
   menu_cleanup();
}


void mode_menu( int32_t t ){
int selection;
// int current;

   // current = mode_menu_data.current;                         // current is usb,cw,lsb etc.
                                                            
   selection = touch_decode( t, mode_menu_data.y_size );
   if( selection == -1 ){                                    // title was touched
      // menu_cleanup();
      menu_display( &keyer_menu_data,0 );                    // sub menu
      menu_dispatch = &keyer_menu;
      return;
   }
   if( mode_menu_data.param[selection] != -1 ){
      if( selection > 2 ) mode_menu_data.current = selection;
      selection = mode_menu_data.param[selection];

      switch( selection ){
        case 0:                                           // vfo A selected
                 vfo_mode |= VFO_A;
                 vfo_mode &= ( 0xff ^ VFO_B );
               
        break;
        case 1:  vfo_mode |= VFO_B + VFO_SPLIT;           // vfo B also implies split mode 
                 vfo_mode &= ( 0xff ^ VFO_A );
        break;
        case 2:  vfo_mode ^= VFO_SPLIT;                   // toggle split.  Split can also be enabled / disabled with encoder switch
                 if( (vfo_mode & VFO_SPLIT) == 0 ){       // if toggled off then assign vfo a --> b or b--> a
                    if( vfo_mode & VFO_A ) vfo_b = vfo_a;
                    else vfo_a = vfo_b;
                    vfo_mode |= VFO_A;                    // leaving split always returns to standard vfo A operation
                    vfo_mode &= ( 0xff ^ VFO_B );
                 }
        break;
        case 3:  vfo_mode &= 0x7;                         // clear all modes, save vfo, split
                 vfo_mode |= VFO_CW;                      // set cw mode
                 fast_tune = 0;                           // try default tune rates, slow for cw, fast for SSB,
        break;                                            // always can toggle with double tap encoder
        case 4:  vfo_mode &= 0x7;
                 vfo_mode |= VFO_USB;
                 //fast_tune = 1;                         // not sure I like this feature
        break;
        case 5:  vfo_mode &= 0x7;
                 vfo_mode |= VFO_LSB;
                 //fast_tune = 1;
        break;         
        case 6:  vfo_mode &= 0x7;
                 vfo_mode |= VFO_AM;   
        break;  
        case 7:  vfo_mode &= 0x7;
                 vfo_mode |= VFO_DIGI; 
        break;    
      }
     
   //qsy_mode( current, mode_menu_data.current );   
   }
   
   menu_cleanup();
   set_bandwidth();
}

void set_bandwidth(){           // set a filter appropriate for the mode
  // Q's two cascade
  // 0.54119610
  // 1.3065630
  // three cascade
  // 0.51763809
  // 0.70710678
  // 1.9318517
  // four cascade
  // 0.50979558
  // 0.60134489
  // 0.89997622
  // 2.5629154

   if( vfo_mode & VFO_AM ){
      BandWidth.setHighpass( 0, 200, 0.70710678 );  // highpass to restore dc level
      BandWidth.setLowpass( 1, 3000, 0.61763809 );  // favor high freq tone
      BandWidth.setLowpass( 2, 3000, 0.70710678 );
      BandWidth.setLowpass( 3, 3000, 1.9318517 );
   }
   else{                                            // default wide filter, crystal filter should be setting the bandwidth
      BandWidth.setHighpass( 0, 200, 0.70710678 );  
      BandWidth.setLowpass( 1, 3300, 0.51763809 );
      BandWidth.setLowpass( 2, 3300, 0.70710678 );
      BandWidth.setLowpass( 3, 3300, 1.9318517 );    
   }
  
}

void menu_cleanup(){

   // exit touch menu and back to normal screen
   menu_dispatch = &hidden_menu;
   tft.fillScreen(ILI9341_BLACK);
   screen_owner = DECODE;
   vfo_mode_disp();
   vfo_freq_disp();
   mf_bar_disp();
   set_volume( volume_ );
   sig_pwr_meter( -1.0 );           // reset meter to zero
}

// a mf bar on the left side of the screen grew into a full Smeter display
void mf_bar_disp(){                 // display a bar on left side to show a menu exists there
const char msg[]  = "Multi Fun";    // also add some other boxes for menu hotspots, the hidden menu is no longer hidden
const char msg2[] = "Band";
const char msg3[] = " Toggles ";
int i;
uint16_t c;

   c = ( transmitting ) ? EGA[12] : EGA[4];          // brighter red for transmitting
   tft.drawLine(0,69,319,69,c);
   tft.drawLine(0,70,319,70,c);
   tft.drawLine(0,169,319,169,c);
   tft.drawLine(0,170,319,170,c);

   tft.setTextSize(1);
   tft.setTextColor(EGA[15]);
   tft.fillRect( 0,70,8,100,c );           // multi function
   for( i = 0; i < 9; ++i ){
      tft.setCursor( 1, 77+10*i );
      tft.write( msg[i] );
   }
   tft.fillRect( 319-8,70,8,100,c );       // toggles
   for( i = 0; i < 9; ++i ){
      tft.setCursor( 319-7, 77+10*i );
      tft.write( msg3[i] );
   }
   // display the S meter image   
   //   tft.writeRect(9, 70, 209, 100, (uint16_t*)smeter);
   // tft.writeRect(9, 72, 209, 98, (uint16_t*)smeter);    // try down two pixel and not write bottom 2 lines
   //tft.writeRect(9, 72, 182, 98, (uint16_t*)smeter2);
   //tft.writeRect(9, 72, 208, 98, (uint16_t*)smeter3);
   //tft.writeRect(9, 72, 208, 98, (uint16_t*)smeter4);
   tft.writeRect(43-34*dis_info, 72, 234, 96, (uint16_t*)smeter5);      // bottom two lines left off for divider box lines, centered or left
   tft.fillRect( 148,65,32,8,c );           // band select
   tft.setCursor( 153,65 );
   tft.print( msg2 );                            // overwrites part of the smeter
   /**********
   tft.fillRect( 319-8,0,8,20,EGA[4] );          // mode select, not much room here for text
   tft.setCursor( 319-7,1 );
   tft.write( 'M' );
   tft.setCursor( 319-7,11);
   tft.write( 'd' );
   ***********/
}

// non-log meter unless convert the value before calling,  value is 0 to 1.0 for zero to full scale. 
// on rx, more of an indication of the input to the A/D converter than S signal value
void sig_pwr_meter( float val ){                // move the indicator bar around with a sprite
int sx = 40;
const int sy = 131;                                    // zero position
static int pos;                                        // current x position
static int mod;                                        // slow down the fall of the meter
//uint8_t sprite[] = { 0,0,0xaa,0xa0,0xaa,0xa0,0,0 };
//uint8_t sprite[] = { 0,0,0,0x0a,0xaa,0xa0,0x0a,0xaa,0xa0,0x0a,0xaa,0xa0,0,0,0 };
//uint8_t sprite[] = { 0,0xa0,0xa0,0xa0,0xa0,0 };                       // 2x6  erase only on falling
//uint8_t sprite[] = { 0,0xa0,0xa0,0xa0,0xa0,0xa0,0 };                  // 2x7  erase only on falling
const uint8_t spriteg[]  = { 0x00,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0x00 };     // there is room to go wider, 2x9
const uint8_t spritey[] = { 0x00,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0x00 };
const uint8_t spriter[] = { 0x00,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x00 };
int x,y;
const int xm = 170;                      // x value scaling factor
int r;                                   // flag if nothing to do
int i;                                   // loop 3 times for faster response on signal increases

    if( val < 0.0 ){                     // reset on menu exit
        pos = 0;
        return;
    }

    if( dis_info == 0 ) sx += 34;        // 34 == difference between centered meter and meter on left side
    for( i = 0; i < 3; ++i ){
       // where should the indicator be.  Move one pixel at a time.
       r = 1;
       x = xm * val;
       if( x > pos+2 ) ++pos, r = 0;
       else if( x < pos ){
          if( ++mod == 3 ) mod = 0, --pos, r = 0;
       }
       if( r ) return;                      // no change in indicator position
    
       x = pos;
    
      // y = (pos > xm/2) ? xm-pos : pos;
      // y = -y/8;                              // should be sine or cosine involved, this is linear 

       float th = -0.785 + 0.785 * (float(x)/(float)(xm/2));    // -45 to +45 degrees
       th = cos(th) - cos( 0.785 );                             // amount to raise the curve
       y = -48.0 * th;                                          // pixel fudge factor

       //writeRect4BPP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *pixels, const uint16_t * palette );
       if( pos > 0.85*xm ) tft.writeRect4BPP( sx+x, sy+y, 2, 9, spriter, EGA );          // red
       else if( pos > 0.75*xm ) tft.writeRect4BPP( sx+x, sy+y, 2, 9, spritey, EGA );     // yellow
       else tft.writeRect4BPP( sx+x, sy+y, 2, 9, spriteg, EGA );      // 4BPP, 2 pixels per byte.  0xa0 has one green pixel, one black pixel
    }                                                                 // with colors from the EGA pallett.

}

uint32_t touch(){
static uint32_t  tm;
static int16_t  x,y;
static uint8_t   z;

   // control rate of updates,  library is at 3 ms.  Averaging 4 reads, 20ms touch.
   if( millis() - tm < 5 ) return 0;
   tm = millis();

   if( ts.touched() ){
     ++z;
     TS_Point p = ts.getPoint();
     //ts.readData(&x,&y,&z);
     x += map( p.x, 250, 3650, 0, 320 );
     y += map( p.y, 330, 3700, 0, 240 );
     //x = p.x;  y = p.y;
   }
   else z = 0;
   if( z == 0 ) x = y = 0;

   if( z == 4 ){
     x >>= 2;
     y >>= 2;
     return ( x << 8 ) | y;
   }
   return 0;
}

void multi_display( struct multi *m, int clr ){           // display name value pairs.   use also for toggles
int i,x,y; 
char buf[20];

   if( clr ){                                       // new menu, else have menu on screen and just updating the value pairs
      tft.setTextColor( ILI9341_WHITE, EGA[2] );    // text is white on background color 
      tft.fillScreen(ILI9341_BLACK);                // border of menu items is black

      // title box
      tft.fillRect(5,5,320-10,30,EGA[2]);
      tft.setCursor( 10,10 );
      tft.setTextSize(2);
      tft.print(m->title);
   }
   
   // draw some menu boxes, two per row for now
   y = 40; x = 0;
   for( i = 0; i < 10; ++i ){
      if( m->menu_item[i][0] == 0 ) break;       // null option
      if( y + 30  > 239 ) break;                 // screen is full
      if( i >= m->num ) break;                   // redundant to null string test above just in case
      tft.setTextSize(2);
      tft.fillRect(x+5,y+5,160-10,30,EGA[2]);
      tft.setCursor( x+10,y+10 );
      if( i == m->current ) tft.setTextColor( ILI9341_YELLOW, EGA[2] );
      else tft.setTextColor( ILI9341_WHITE, EGA[2] );
      strncpy(buf,m->menu_item[i],12);    buf[12] = 0;
      tft.print(buf);
      tft.setCursor( x+120, y+10 );
      if( *(m->val[i]) >= 100 ) tft.setTextSize(1);   // for Tune counter value, exceeds 100 while tune in progress
      if( *(m->val[i]) < 10 ) tft.write(' ');
      tft.print( *(m->val[i]) );
      x += 160;
      if( x >= 320 ) x = 0, y += 40;
   }
  
}

void multi_fun_encoder( int b ){             // encoder adjusts a multi function variable up or down, range 0 to 99
int x,y;                                     // assume structure is multi_fun_data for now
int i;                                       // toggles will probably just use touch and not the encoder
int *p;
                                             
     i = multi_fun_data.current;
     if( i >= multi_fun_data.num ) return;   // belts and suspenders
     p = multi_fun_data.val[i];
     *p += b;                                // bump the value
     *p = constrain( *p, 0, 99 );
     x = ( i & 1 ) ? 160+120 : 120;          // where is the value on the screen?
     y = 40 * (i/2) + 50;
     tft.setTextSize(2);
     tft.setTextColor( ILI9341_YELLOW, EGA[2] );
     tft.setCursor( x,y);
     if( *p < 10 ) tft.write(' ');
     tft.print(*p);

     switch( i ){                                       // immediate adjustment needed?
        case 0: set_volume( volume_ ); break;           // digital audio gain
        case 1:                                         // fine tune 25 meg adjustment for temp drift
           vfo_freq_disp();                             // new tuning values loaded
        break;
     }
}

void set_volume(int vol){
float g;
int sel;
int i;

  sel = 0;                               // assume normal SSB DIGI
  if( vfo_mode & VFO_AM ) sel = 3;
  // else if notch/nr sel = 2;
  g = 0.01 * (float)vol;                  // 0 to 99 mapped to 0.0 to 0.99
  RX_SEL.gain(sel,g);
  
  for( i = 0; i < 4; ++i ){              // turn off other volume sources
    if( i == sel ) continue;
    RX_SEL.gain(i, 0.0 );
  }
}

void menu_display( struct menu *m, int mode_hack ){    // display any of the menus on the screen, special highlighting for the mode menu
int i,x,y;                                             // other functions handle selections
char buf[20];

   tft.setTextColor( ILI9341_WHITE, m->color );  // text is white on background color 
   tft.fillScreen(ILI9341_BLACK);                // border of menu items is black

   // title box
   tft.fillRect(5,5,320-10,m->y_size-10,m->color);
   tft.setCursor( 10,10 );
   if( m->y_size > 45 ) tft.setTextSize(3);
   else tft.setTextSize(2);
   tft.print(m->title);
   
   // draw some menu boxes, two per row for now
   y = m->y_size; x = 0;
   for( i = 0; i < 10; ++i ){
      if( m->menu_item[i][0] == 0 ) break;       // null option
      if( y + m->y_size-10  > 239 ) break;       // screen is full
      tft.fillRect(x+5,y+5,160-10,m->y_size-10,m->color);
      tft.setCursor( x+10,y+10 );
      if( i == m->current ) tft.setTextColor( ILI9341_YELLOW, m->color );
      else if( i < mode_hack ){                                             // special highlighting of vfo mode, split
          if( i == 0 && (vfo_mode & VFO_A) ) tft.setTextColor( ILI9341_YELLOW, m->color );
          else if( i == 1 && (vfo_mode & VFO_B) ) tft.setTextColor( ILI9341_YELLOW, m->color );
          else if( i == 2 && (vfo_mode & VFO_SPLIT) ) tft.setTextColor( ILI9341_YELLOW, m->color );
          else tft.setTextColor( ILI9341_WHITE, m->color );
      }
      else tft.setTextColor( ILI9341_WHITE, m->color );
      strncpy(buf,m->menu_item[i],12);    buf[12] = 0;
      tft.print(buf);
      x += 160;
      if( x >= 320 ) x = 0, y += m->y_size;
   }
}

int touch_decode( int32_t t, int y_size ){    // selection for 2 items wide touch menu
int32_t x,y;

   y = t & 0xff;
   x = t >> 8;
   y -= y_size;                               // top row is the title. if sub menus needed, need to decode title touch also
   if( y < 0 ) return -1;                     // title area was touched
   y /= y_size;                               // row of the touch
   x /= 160;                                  // column
   //Serial.print( "X " );  Serial.print(x); Serial.print(" Y "); Serial.println(y);   
   return 2*y + x;                            // two menu items per row
   
}



void button_process( int val ){

   switch( val ){
      case TAP:
         if( ( vfo_mode & VFO_SPLIT ) == 0 || ( vfo_mode & VFO_B ) ){         // split tuning A, split as RIT
            vfo_mode |= VFO_A + VFO_SPLIT;
            vfo_mode &= ( 0xff ^ VFO_B );
         }
         else{
            vfo_mode |= VFO_B + VFO_SPLIT;                                    // toggle to B
            vfo_mode &= ( 0xff ^ VFO_A );          
         }
         fast_tune = 0;                                                       // no big jumps when jiggling the encoder switch for RIT/split
      break;
      case DTAP:
         fast_tune ^= 1;                                                      // toggle tune by 1000
      break;
      case LONGPRESS:
         if( vfo_mode & VFO_SPLIT ){                                          // cancel split if enabled
           if( vfo_mode & VFO_A ) vfo_b = vfo_a;
           if( vfo_mode & VFO_B ) vfo_a = vfo_b;
           vfo_mode &= ( 0xff ^ ( VFO_A + VFO_B + VFO_SPLIT ) );
           vfo_mode |= VFO_A;
         }                                                                    // else another function
      break;
   }
   vfo_freq_disp();
   vfo_mode_disp();
   button_state(DONE);
   
  
}


int encoder(){                              // read the encoder, return -1,0,1
// static int mod;        /* encoder is divided by 4 because it has detents */
static int dir;        /* need same direction as last time, effective debounce */
static int last;       /* save the previous reading */
int new_;              /* this reading */
int b;

   //if( transmitting ) return 0;
   
   new_ = (digitalReadFast(ENC_B) << 1 ) | digitalReadFast(ENC_A);
   if( new_ == last ) return 0;       /* no change */

   b = ( (last << 1) ^ new_ ) & 2;    /* direction 2 or 0 from xor of last shifted and new data */
   last = new_;
   if( b != dir ){
      dir = b;
      return 0;                       /* require two in the same direction serves as debounce */
   }
   
//   mod = (mod + 1) & 3;       /* divide by 4 for encoder with detents */
//   if( mod != 0 ) return 0;

   return ( (dir == 2 ) ? 1: -1 );   /* swap defines EN_A, EN_B if it works backwards */
}


int button_state( int fini ){     /* state machine running at 1ms rate */
static int press_,nopress;
static int st;
int sw;

      if( fini ){                // switch state latched until processed and we say done
          st = DONE;
          return st;
      }
      
      sw = digitalReadFast(ENC_SW) ^ 1;   
      if( sw ) ++press_ , nopress= 0;
      else ++nopress , press_= 0;
      
      /* switch state machine */
         if( st == IDLE_ && press_ >= DBOUNCE ) st = ARM;
         if( st == DONE && nopress >= DBOUNCE ) st = IDLE_;       /* reset state */

         /* double tap and long detect */
         if( st == ARM && nopress >= DBOUNCE/2 )  st = DTDELAY;
         if( st == ARM && press_ >= 8*DBOUNCE )  st = LONGPRESS; 
         if( st == DTDELAY && nopress >= 4*DBOUNCE ) st = TAP;
         if( st == DTDELAY && press_ >= DBOUNCE )   st = DTAP;
          
     return st;        
}


// vfo's operation:  vfo_a is the RX vfo, vfo_b is the TX vfo, split serves as RIT
void freq_update( int count ){
static int holdoff;                                            // notch tuning at 100 hz boundaries
static int rpm;
static uint32_t tm;
static int rc;
uint32_t va, vb;
//static int last_count;

  if( count ) ++rc;
  if( millis() - tm  > 250 ){
     rpm = 7*rpm + 4*60*rc/90;          // counts in 1/4 second * 4 for approx counts per minute, encoder about 90 counts per rev
     rpm >>= 3;                         // average filter 
     tm = millis();
     rc = 0;
     //Serial.println( rpm );
  }

  if( count == 0 ) return;

  //if( count != last_count ) last_count = count, rpm >>= 1;    // defeat fast tune if reverse direction
  
  if( holdoff ){                             // in a tuning notch
     --holdoff;
     return;
  }

  if( fast_tune ) count *= 1000;
  else{
     if( rpm > 20  || (vfo_mode & VFO_CW) == 0 ) count *= 10;       // only cw mode counts by 1 hz
     if( rpm > 50 ) count *= 10;
  }

  if( vfo_mode & VFO_B ){
      vfo_b += count;
      if( (vfo_b % 100) == 0 && abs(count) < 100 ) holdoff = 5;     // rx tuning notch at 100 hz steps for fine tuning
      count = vfo_b % count;
      vfo_b -= count;                                               // clear not counting digits
  }
  else{
      vfo_a += count;
      if( (vfo_mode & VFO_SPLIT) == 0 ) vfo_b = vfo_a;              // tx vfo follows vfo_a if not in split
      if( (vfo_a % 100) == 0 && abs(count) < 100 ) holdoff = 5;     // rx tuning notch at 100 hz steps for fine tuning
      count = vfo_a % count;
      vfo_a -= count;                                               // clear not counting digits
  }

  va = vfo_a;   vb = vfo_b;                                         // band check may change the vfo's if tune across bands rather than
                                                                    // switch bands with menu. Will see mode changes to the new band.
  band_check( vfo_b );                                              // check that the tx vfo is in the correct band, the correct tx relays
                                                                    // selected
  vfo_a = va;   vfo_b = vb;
  vfo_freq_disp();
  
}


/***************      display freq of vfo's in a simulated 7 segment font   *****************************/
void vfo_freq_disp(){
//int val;
int32_t vfo;
int32_t am_off;
//int pos;           // sceen x position
//int32_t mult;
int ca,cb;         // colors
char bp;

   cb = 4;         // unused vfo color in EGA pallet
   ca = 6;         // was 2 or 4

   if( vfo_mode & VFO_B ) vfo = vfo_b, cb = 10;
   else vfo = vfo_a, ca = 10;       // was 10

   if( vfo_mode & VFO_SPLIT ) cb = 12;    // vfo b is active transmit

   am_off = ( vfo_mode & VFO_AM ) ? -1500 : 0;                  // tune AM off frequency to pass carrier at 5-8 khz
   si5351bx_setfreq( 2, vfo + IF + ( clari - 50 ) + am_off);

   if( screen_owner != DECODE ) return;
                          

   if( ca == 10 ) disp_vfo( vfo, 20, ca );
   else disp_vfo(vfo_a,20,ca);
   if( cb == 10 ) disp_vfo(vfo,170,cb);
   else disp_vfo(vfo_b,170,cb);

   bp = band_priv( band, vfo_b );                    // check if xmit vfo is in band
   oob = ( bp == 'X' ) ? 1 : 0;                      // flag if out of band, don't save vfo to bandstack
   tft.setCursor( 280,60);
   tft.setTextColor( EGA[10],0 );
   tft.setTextSize(1);
   if( bp == 'X' ) { tft.setTextColor( EGA[12],0); tft.print("  OOB"); }
   //if( bp == 'E' || bp == 'e' ) tft.print("Extra"); 
   if( bp == 'E' || bp == 'e' ) { tft.setTextColor( EGA[12],0);  tft.print("Extra"); } // my class is Advanced, so display Extra in Red
   if( bp == 'A' || bp == 'a' ) tft.print("  Adv");
   if( bp == 'G' || bp == 'g' ) tft.print("  Gen");
   if( bp == 'e' || bp == 'g' || bp == 'a' ) tft.write('-');     // cw digi band
   else tft.write(' ');
   //tft.write(bp);


    // standard font freq display
//   tft.setTextSize(3);
//   tft.setCursor(50,20);
//   tft.setTextColor(EGA[10],0);
//   if( vfo < 10000000 ) tft.write(' ');
//   tft.print(vfo / 1000);
  // tft.write('.');
//   tft.setTextSize(2);
//   p_leading(vfo % 1000,3);

  // tft.setFontAdafruit();   // !!!
   // will this radio have RIT or just vfo_a and vfo_b ?
//   tft.setTextSize(2);
//   val = rit;
//   if( val < 0 ){
//      val = - val;
//      tft.setTextColor(EGA[12],0);       // change color instead of displaying the minus sign
//   }
//   tft.setCursor(265,20);
//   p_leading(val,4);


}

void disp_vfo( int32_t vfo, int column, int color ){          // row is hardcoded currently


   int leading = 1; uint8_t c;
   int32_t mult = 10000000L;
   for( int i = 0; i < 8; ++i ){
       int digit = vfo / mult;
       if( leading && digit == 0 ) c = '/';
       else leading = 0, c = digit + '0';
       if( mult > 100 ) column += disp_segments(column,c,color,BigNumbers);
       else column += disp_segments(column,c,color,MediumNumbers);
       vfo -= digit*mult;
       mult /= 10;
       column += 3;                // wider spacing, was 2
   }  
  
}


// change a mono font to 4BPP for display.  bits need to change from a vertical 0-7 to horizontal format.
// only using two colors so each 4 bits will be 0 or the forground color
// displaying at a fixed row for now, but should probably pass that as an argument also
int disp_segments( int pos, uint8_t digit, int color, const uint8_t font[] ){
int i,j,k,w,h,base,sz,adr;
uint8_t chr[1350];
uint8_t data;

   for( i = 0; i < 1350; ++i ) chr[i] = 0;
   w = font[0];  h = font[1]; base = font[2]; sz = font[3];
   digit = digit - base;                    // get index into table
   if( digit < 0 || digit > sz ) return 0;    // outside of the table.
   
   // reuse base as our zero index in the table
   base = digit * (w * h / 8 ) + 4 ;

   for( k = 0; k < h/8; ++k ){          // each height
      for( i = 0; i < w; ++i ){         // row of width
        data = font[base+i];
        for( j = 0; j < 8; j++ ){       // each bit
 //         adr = k * (w * h / 4) + i/2 + j * w/2;          // h/16   j*w/2  h/4 works for medium, h/8 works for large
            adr = k * 4 * w + i/2 + j * w/2;                // k * 8*w/2 where 8 is the range of j, 2 pixel/byte
          if( adr >= 1350 ) adr = 1349; // something wrong     
          if( i & 1 ){                  // lower nibble
             if( data & 1 ) chr[adr] |= color;
          }
          else{                         //  upper nibble
             if( data & 1) chr[adr] |=  ( color << 4 );
          }
          data >>= 1;                   // next bit
        }
      }
      base += w;
   }

// void writeRect4BPP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *pixels, const uint16_t * palette );
   tft.writeRect4BPP( pos, 35, w, h, chr, EGA );      // row is hardcoded here !!! fix someday

   return w;
}



/*************************************************************************************/

void vfo_mode_disp(){
int a,b,s,r;
int u,l,c,m;
int row = 6;                                // was 2
int32_t as;

   as = -1;                                 // add/sub sign for LSB/USB.  add/sub from 1st IF freq.  Set up for USB.
   a = b = s = r = 0;                       // set colors of the vfo mode
   c = u = l = m = 0;
   
   if( vfo_mode & VFO_B ) b = 10;
   if( vfo_mode & VFO_A ) a = 10;
   if( vfo_mode & VFO_SPLIT ) s = 10;
   if( vfo_mode & VFO_DIGI ) r = 10;
   if( vfo_mode & VFO_CW ) c = 10, as = 1;       // CW on LSB
   if( vfo_mode & VFO_LSB ) l = 10, as = 1;      // LSB on LSB
   if( vfo_mode & VFO_USB ) u = 10;
   if( vfo_mode & VFO_AM )  m = 10;              // AM through the filter on USB, tune 1.5 k off frequency down, 
                                                 // get carrier at 4.5k +- 1k

   si5351bx_setfreq( 1, IF + as*BFO );           // not sure I like having radio setups in the display code but currently it is the 
                                                 // common place where all code branches merge.  Also we know what the mode is here.
                                                 // what is the actual center of the 1st IF freq?  Notes say it is low due to loading but
                                                 // the standard code sets it at 45005000
   if( vfo_mode & VFO_AM ){
      si5351bx_setfreq( 0, BFO + 3500 );         // tune AM to pass 3.5k to 6.5k IF.  Think C78 across volume pot may be reducing volume if 
   }                                             // try higher IF frequencies. 
   else si5351bx_setfreq(0, BFO );               // normal bfo on high side of the filter
                                                 
   if( screen_owner != DECODE ) return;          // make sure screen owner is changed before this call when exiting menu's

   tft.setTextSize(1);
   tft.setTextColor(EGA[4+a],0);
   tft.setCursor(15,row);
   tft.print("VFO A");
   
   tft.setCursor(60,row);
   tft.setTextColor(EGA[4+b],0);
   tft.print("VFO B");
   
   tft.setCursor(105,row);
   tft.setTextColor(EGA[4+s],0);
   tft.print("Split");

   tft.setCursor(150,row);
   tft.setTextColor(EGA[4+c],0);
   tft.print("CW");
   if( cw_practice ) tft.write('p');
   else tft.write(' ');

   tft.setCursor(175,row);
   tft.setTextColor(EGA[4+u],0);
   tft.print("USB");

   tft.setCursor(205,row);
   tft.setTextColor(EGA[4+l],0);
   tft.print("LSB");

   tft.setCursor(235,row);
   tft.setTextColor(EGA[4+m],0);
   tft.print("AM");
   
   tft.setCursor(265,row);
   tft.setTextColor(EGA[4+r],0);
   tft.print("DIGI");
    
}


//  USA: can we transmit here and what mode
//  lower case for CW portions, upper case for phone segments
//  X - out of band, E - Extra, A - Advanced class, G - General
char band_priv( int band,  uint32_t f ){
char r = 'X';

   
   if( band != 2 ) f = f / 1000;             // test 1k steps
   else f = f / 100;                         // 60 meters, test 500hz steps
   switch( band ){                           // is there an easy way to do this.  This seems like the hard way.
       case 0:
          if( f >= 1800 && f <= 2000 ) r = 'G';
       break;   
       case 1:
          if( f >= 3500 && f <= 4000 ){
             if( f < 3525 ) r = 'e';
             else if( f < 3600 ) r = 'g';
             else if( f < 3700 ) r = 'E';
             else if( f < 3800 ) r = 'A';
             else r = 'G';
          }
       break;
       case 2:
          if( (vfo_mode & VFO_USB) || (vfo_mode & VFO_DIGI) ){
             if( f == 53305 || f == 53465 || f == 53570 || f == 53715 || f == 54035 ) r = 'G';
          }
          if( vfo_mode & VFO_CW ){
             if( f == 53320 || f == 53480 || f == 53585 || f == 53730 || f == 54050 ) r = 'g'; 
          }
       break;
       case 3:
          if( f >= 7000 && f <= 7300 ){
             if( f < 7025 ) r = 'e';
             else if ( f < 7125 ) r = 'g';
             else if ( f < 7175 ) r = 'A';
             else r = 'G';
          }
       break;
       case 4:
          if( f >= 10100 && f <= 10150 ) r = 'g';
       break;
       case 5:
          if( f >= 14000 && f <= 14350 ){
             if( f < 14025 ) r = 'e';
             else if( f < 14150 ) r = 'g';
             else if( f < 14175 ) r = 'E';
             else if( f < 14225 ) r = 'A';
             else r = 'G';
          }
       break;
       case 6:
          if( f >= 18068 && f <= 18168 ){
             if( f < 18110 ) r = 'g';
             else r = 'G';
          }
       break;
       case 7:
          if( f >= 21000 && f <= 21450 ){
             if( f < 21025 ) r = 'e';
             else if( f < 21200 ) r = 'g';
             else if( f < 21225 ) r = 'E';
             else if( f < 21275 ) r = 'A';
             else r = 'G';
          }
       break;
       case 8:
         if( f >= 24890 && f <= 24990 ){
            if( f < 24930 ) r = 'g';
            else r = 'G';
         }
       break;
       case 9:
         if( f >= 28000 && f <= 29700 ){
            if( f < 28300 ) r = 'g';
            else r = 'G';
         }
       break;
   }

  return r;
}


/*****************************************************************************************/
// TenTec Argonaut V CAT emulation

#define stage(c) Serial.write(c)

//int un_stage(){    /* send a char on serial */
//char c;

//   if( stg_in == stg_out ) return 0;
//   c = stg_buf[stg_out++];
//   stg_out &= ( STQUESIZE - 1);
//   Serial.write(c);
//   return 1;
//}

#define CMDLEN 20
char command[CMDLEN];
// uint8_t vfo = 'A';

void radio_control() {
static int expect_len = 0;
static int len = 0;
static char cmd;

char c;
int done;

    if (Serial.available() == 0) return;
    
    done = 0;
    while( Serial.available() ){
       c = Serial.read();
       command[len] = c;
       if(++len >= CMDLEN ) len= 0;  /* something wrong */
       if( len == 1 ) cmd = c;       /* first char */
       /* sync ok ? */
       if( cmd == '?' || cmd == '*' || cmd == '#' );  /* ok */
       else{
          len= 0;
          return;
       }
       if( len == 2  && cmd == '*' ) expect_len = lookup_len(c);    /* for binary data on the link */       
       if( (expect_len == 0 &&  c == '\r') || (len == expect_len) ){
         done = 1;
         break;   
       }
    }
    
    if( done == 0 ) return;  /* command not complete yet */
        
    if( cmd == '?' ){
      get_cmd();
     // operate_mode = CAT_MODE;            // switch modes on query cat command
     // if( wwvb_quiet < 2 ) ++wwvb_quiet;  // only one CAT command enables wwvb logging, 2nd or more turns it off
     // mode_display();
    }
    if( cmd == '*' )  set_cmd();
    if( cmd == '#' ){
        pnd_cmd(); 
       // if( wwvb_quiet < 2 ) ++wwvb_quiet;  // allow FRAME mode and the serial logging at the same time
    }

 /* prepare for next command */
   len = expect_len= 0;
   stage('G');       /* they are all good commands */
   stage('\r');

}

int lookup_len(char cmd2){     /* just need the length of the command */
int len;

   
   switch(cmd2){     /* get length of argument */
    case 'X': len = 0; break;
    case 'A':
    case 'B': len = 4; break;
    case 'E':
    case 'P':
    case 'M': len = 2; break;
    default:  len = 1; break ;
   }
   
   return len+3;     /* add in *A and cr on the end */
}

void set_cmd(){
char cmd2;
unsigned long val4;

   cmd2 = command[1];
   switch(cmd2){
    case 'X':   stage_str((char*)"RADIO START\r"); break; 
    case 'O':   /* split */ 
      if( command[2] == 1 ) vfo_mode |= VFO_SPLIT;
      else vfo_mode &= 0xff ^ VFO_SPLIT;
      status_display();
    break;
    case 'A':   // set frequency
    case 'B':
       val4 = get_long();
       cat_qsy(val4);  
    break;
    case 'E':
       if( command[2] == 'V' ){
          if( command[3]  == 'B' ){
              vfo_mode |= VFO_B+VFO_SPLIT;
              vfo_mode &= 0xff ^ VFO_A;
          }
          else{
              vfo_mode |= VFO_A;
              vfo_mode &= 0xff ^ VFO_B;
          }
          status_display();
       }
    break;
    case 'W':    /* bandwidth */
    break;
    case 'K':    /* keying speed */
    break;
    case 'T':    /* added tuning rate as a command */
    break;
    case 'M':
       int i = command[2] - '0';          // FM will map to DIGI
       mode_change(i);
       status_display();
    break;       
   }  /* end switch */   
}

void get_cmd(){
char cmd2;
// long arg;
int len, i;

   cmd2 = command[1];   
   stage(cmd2);
   switch(cmd2){
    case 'A':     // get frequency
      stage_long( vfo_a );
    break;
    case 'B': 
      stage_long( vfo_b );
    break;
    case 'V':   /* version */
      stage_str((char *)"ER 1010-516");
    break;
    case 'W':          /* receive bandwidth */
       stage(30);
    break;
    case 'M':          /* mode. 11 is USB USB  ( 3 is CW ) vfo A, vfo B */
      if( vfo_mode & VFO_AM ) i = 0;
      else if( vfo_mode & VFO_DIGI ) i = 4;                 // DIGI reports as FM
      else if( vfo_mode & VFO_CW ) i = 3;
      else if( vfo_mode & VFO_USB ) i = 1;
      else i = 2;
      i = i + '0';
      stage(i); stage(i);
    break;
    case 'O':          /* split */
       if( vfo_mode & VFO_SPLIT ) stage(1);   
       else stage(0);
    break;
    case 'P':         /*  passband slider */
       stage_int( 3000 );
    break;
    case 'T':         /* added tuning rate command */
    break;   
    case 'E':         /* vfo mode */
      stage('V');
      if( vfo_mode & VFO_A ) stage('A');
      else stage('B');
    break;
    case 'S':         /* signal strength */
       stage(7);
       stage(0);
    break;
    case 'C':      // transmitting status 
       stage(0);
       if( transmitting ) stage(1);
       else stage(0);
    break;
    case 'K':   /* wpm on noise blanker slider */
       stage( 15 - 10 );
    break;   
    default:           /* send zeros for unimplemented commands */
       len= lookup_len(cmd2) - 3;
       while( len-- ) stage(0);  
    break;    
   }
  
   stage('\r');  
}


void stage_str( char * st ){
//unsigned int i;
char c;

  while( ( c = *st++ ) ) stage(c);
  //for( i = 0; i < st.length(); ++i ){
  //   c = st.charAt( i );
  //   stage(c);
  //}    
}

void stage_long( long val ){
unsigned char c;
   
   c = val >> 24;
   stage(c);
   c = val >> 16;
   stage(c);
   c = val >> 8;
   stage(c);
   c = val;
   stage(c);
}


unsigned long get_long(){
union{
  unsigned long v;
  unsigned char ch[4];
}val;
int i;

  for( i = 0; i < 4; ++i) val.ch[i] = command[5-i]; // or i+2 for other endian
  return val.v;
}

void stage_int( int val ){
unsigned char c;
   c = val >> 8;
   stage(c);
   c = val;
   stage(c);
}

void stage_num( int val ){   /* send number in ascii */
char buf[35];
char c;
int i;

   itoa( val, buf, 10 );
   i= 0;
   while( (c = buf[i++]) ) stage(c);  
}

void pnd_cmd(){
char cmd2;
   
   cmd2 = command[1];
   switch(cmd2){
     case '0':  rx(); cat_tx = 0; break;    // enter rx mode
     case '1':  tx(); cat_tx = 1; break;    // TX
   }

}

// function stubs for cat calls
void cat_qsy( uint32_t freq ){       // test if a band change is needed
int t;

   t = band_check( freq );
   if( vfo_mode & VFO_A ){
      vfo_a = freq;
      if( t || (vfo_mode & VFO_SPLIT ) == 0 ) vfo_b = freq;    // cross band split not allowed
                                                               // transmit vfo follows rx vfo when not split
   }
   else{
      vfo_b = freq;
      if( t ) vfo_a = freq;
   }
   vfo_freq_disp();
}

int band_check( uint32_t freq ){

int i = 0;

   if( freq  >  3000000 ) i = 1;
   if( freq  >  5000000 ) i = 2;
   if( freq  >  6700000 ) i = 3;
   if( freq  >  9500000 ) i = 4;
   if( freq  > 13000000 ) i = 5;
   if( freq  > 17000000 ) i = 6;
   if( freq  > 20000000 ) i = 7;
   if( freq  > 23000000 ) i = 8;
   if( freq  > 27000000 ) i = 9;
   if( i != band ){
       band_change(i);
       return 1;
   }
   return 0;
  
}

void mode_change( int i ){

  vfo_mode &= 7;                         // save A, B, SPLIT info
  if( i == 0 ) vfo_mode |= VFO_AM;
  if( i == 1 ) vfo_mode |= VFO_USB;
  if( i == 2 ) vfo_mode |= VFO_LSB;
  if( i == 3 ) vfo_mode |= VFO_CW;
  if( i == 4 ) vfo_mode |= VFO_DIGI;     // FM maps to DIGI
  set_volume( volume_ );
  set_bandwidth();
}

void status_display(){

  vfo_mode_disp();
}

/********************* end Argo V CAT ******************************/

// transmit control from different sources: Keyer, PTT, CAT
// wish to sequence the keyer to avoid short first elements and transmitting during relay switching

  //  time a 48ms delay between TX to RX and RX to TX switching, call once per ms
  //  relays and bi-directional amps need to switch
void tx_rx_seq(){

     if( transmitting < 50 ) ++transmitting;
     
     if( transmitting > 0 ){         // changing from RX to TX
        if( transmitting == 48 ){
           if( (vfo_mode & VFO_CW) && tuning == 0 ) si5351bx_setfreq( 2, vfo_b - 600 );
           else si5351bx_setfreq( 2, vfo_b + IF + ( clari - 50 ) );
           mf_bar_disp();            // Smeter area in RED as transmitter on indication
        }
     }
     else{                           // TX to RX
        if( transmitting == 0 ){
           if( vfo_mode & VFO_B ) si5351bx_setfreq( 2, vfo_b + IF + ( clari - 50 ));
           else si5351bx_setfreq( 2, vfo_a + IF + ( clari - 50 ) );
           mf_bar_disp();            // Smeter area in muted red
        }
     } 
}



// sequencer called each ms when mode is CW
void cw_sequencer( uint64_t val ){                             // call with val == 0x8000000000000000 to key tx, 0 for key up
static uint64_t cw_seq;                                        // cw sent behind by 64 ms, avoid sending during relay switching
static uint16_t mod;                                           // counter for semi breakin

   if( cw_practice ){
       if( transmitting > 0 ) rx();                            // someone entered practice mode while transmitting
       return;                                                 // practice only keys the sidetone
   }
   
   cw_seq >>= 1;
   cw_seq |= val;                                              // save a bit to send 64 ms later
   if( cw_seq ) mod = 10*cw_tr_delay;                          // still sending

   if( cw_seq & 1 ) digitalWriteFast( CW_KEY, HIGH);           // key cw on and off
   else digitalWriteFast( CW_KEY, LOW );
   
   if( mod ) --mod;                                            // delay the return to rx mode

   if( mod && transmitting <= 0 ) tx();                        // turn the transmitter on and off, tx overrides return to rx mode
   if( transmitting > 0 && mod == 0 ) rx();

}

void check_ptt(){

  if( cat_tx || (vfo_mode & VFO_CW) || tuning ) return;          // it's not my job

  if( transmitting == 0 && digitalReadFast( PTT ) == LOW ){      // enable transmit, ssb with microphone
     tx();                                                       // enable tx
  }

  if( transmitting > 40 && digitalReadFast( PTT ) == HIGH ){     // 40 ms debounce as well as sequencing
     rx();
  }
  
}

// what needs to happen to return to rx mode
void rx(){

  transmitting = -48;                                      // sequence delay 48 ms
  si5351bx_setfreq( 2, 0 );                                // tx off, vfo off
  digitalWriteFast( TR, LOW );
  
  if( vfo_mode &  VFO_CW ){                                // enable the bfo and IF now
     si5351bx_setfreq( 1, IF + BFO );                      // CW is on LSB mode
     si5351bx_setfreq( 0, BFO );
  }
  
  TX_SEL.gain(0,0.0);                                // Turn off DIGI USB TX audio and Sidetone tune audio
  TX_SEL.gain(1,0.0);

  set_volume( volume_ );
  
}


// what needs to happen to enter tx mode
void tx(){
float pwr;

  if( oob ){                                // disable tx out of band.   WSJT freq changes can leave vfo_b out of band.
     rx();
     return;
  }

  set_volume( 0 );                          // mute rx
  transmitting = 1;
  si5351bx_setfreq( 2, 0 );                 // vfo off for rx to tx switch

  if( tuning ) ;                            // do nothing
  else if( vfo_mode & VFO_CW ){             // turn off the IF and Prod detector mixers for cw
     si5351bx_setfreq( 0, 0 );
     si5351bx_setfreq( 1, 0 );
  }
  else if( vfo_mode & VFO_DIGI ){
     pwr = (digi5w) ? band_stack[band].digi_pwr : 0.99;     // limit digi to 5w or full power ?
     TX_SEL.gain(1,pwr);                                    // turn on the USB audio for DIGI TX
  }
 
  digitalWriteFast( TR, HIGH );

}


#define DIT 1
#define DAH 2

int read_paddles(){                    // keyer function
int pdl;

   pdl = digitalReadFast( DAH_pin ) << 1;
   pdl += digitalReadFast( DIT_pin );
   // pdl |= 1;    // !!! disable the dit pin  

   pdl ^= 3;                                                // make logic positive
   if( key_swap ){ 
      pdl <<= 1;
      if( pdl & 4 ) pdl += 1;
      pdl &= 3;
   }

   return pdl;
}

void side_tone_on(){

   SideTone.frequency(600);
   SideTone.amplitude(0.95);
   RX_SEL.gain(0,0.0);
   RX_SEL.gain(1,((float)side_vol)/100.0);
}

void side_tone_off(){

   SideTone.amplitude(0.0);
   RX_SEL.gain(1,0.0);
   if( cw_practice ) set_volume(volume_);
}


// http://cq-cq.eu/DJ5IL_rt007.pdf      all about the history of keyers

#define WEIGHT 200        // extra weight for keyed element

void keyer( ){            // this function is called once every millisecond
static int state;
static int count;
static int cel;           // current element
static int nel;           // next element - memory
static int arm;           // edge triggered memory mask
static int iam;           // level triggered iambic mask
int pdl;
uint64_t seq;             // sequencer bit. 0x8000 0000 0000 0000 or 0

   pdl = read_paddles();
   if( count ) --count;
   seq = 0;                                // default to not sending

   switch( state ){
     case 0:                               // idle
        cel = ( nel ) ? nel : pdl;         // get memory or read the paddles
        nel = 0;                           // clear memory
        if( cel == DIT + DAH ) cel = DIT;
        if( cel == 0 ) break;
        if( key_mode == STRAIGHT ){        // straight key stays in state 0
            seq = 0xf000000000000000;      // 4ms debounce enough?
            break;
        }
        iam = (DIT+DAH) ^ cel;
        arm = ( iam ^ pdl ) & iam;         // memory only armed if alternate paddle is not pressed at this time, edge trigger
                                                    // have set up for mode A
        if( key_mode == MODE_B ) arm = iam;         // mode B - the descent into madness begins.  Level triggered memory.
        if( key_mode == ULTIMATIC ) iam = cel;      // ultimatic mode
        
        count = (1200+WEIGHT)/wpm;
        if( cel == DAH ) count *= 3;
        state = 1;
        side_tone_on();
        seq = 0x8000000000000000;                   // put a bit in the sequencer delay line
     break; 
     case 1:                                  // timing the current element. look for edge of the other paddle
        seq = 0x8000000000000000;            // assume still sending
        if( count ) nel = ( nel ) ? nel : pdl & arm;
        else{
           count = 1200/wpm;
           state = 2;
           side_tone_off();
           seq = 0;                           // not sending
        }
     break;   
     case 2:                                  // timing the inter-element space
        if( count ) nel = ( nel ) ? nel : pdl & arm;
        else{
           nel = ( nel ) ? nel : pdl & iam;   // sample alternate at end of element and element space
           state = 0;
        }
     break;   
   }

   cw_sequencer( seq );                       // add a bit to the delay line each ms
  
}

/*****************************************************************/
#ifdef NOWAY
// a test to find the center of the 45 mhz filter, call once per ms

void test_1st_IF(){
static int tm;
int32_t off;
int i;
int mi = 0;
float mp = 0.0;
float val;
static float vals[40];

     // run once per second 
     ++tm;
     if( tm < 1000 ) return;
     tm = 0;
     
     peak1.read();
     for( i = 0; i < 40; ++i ){
        off = -20000 + 1000 * i;
        si5351bx_setfreq(2, vfo_a + IF + off );
        si5351bx_setfreq(1, IF + off - BFO );      // usb
        while( peak1.available() == 0 );
        //Serial.print( i );    Serial.write( ' ' );  Serial.println( peak1.read() );
        val = peak1.read();
        vals[i] += val;
        si5351bx_setfreq(1, IF + off + BFO );      // lsb
        while( peak1.available() == 0 );
        val = peak1.read();
        vals[i] += val;    
     }
     for( i = 0; i < 40; ++i ){                    // find highest, index zero may have some of the normal signal so discard
        if( i != 0 && vals[i] > mp ){
          mi = i;
          mp = vals[i];
        }
     }
     Serial.print( mi ); 
     for( i = mi - 10; i <= mi + 10; ++i ){
        if( i < 0 || i >= 40 ) continue;
        Serial.write( ' ' );  Serial.print( vals[i] );
        if( i == mi ) Serial.write('*');
     }
     Serial.println();

}

// the idea here was to mute the rx, tune to another freq, take a measurement of the level, return to rx frequency, unmute, plot band scan.
void band_scan(){
static int t;

   return;    // this seems to need more delay than expected to avoid mixing the two frequencies, or avoid mixing the audio to the speaker
              // the time that the audio needs to be muted is too long 
   
   if( ++t < 173 ) return;
   t = 0;
   // fake it 
   set_volume( 0.0 );
   delay(3);
   si5351bx_setfreq(2,7076000+IF);
   delay(3);
   while( peak1.available() == 0 );
   peak1.read();
   while( peak1.available() == 0 );
   peak1.read();
   while( peak1.available() == 0 );
   Serial.println(10*peak1.read());
   si5351bx_setfreq(2,7255000+IF);
   delay(6);
   while( peak1.available() == 0 );
   peak1.read();
   set_volume(volume_);
}

#endif
