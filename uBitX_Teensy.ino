
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
 *      Install the 7805 without the heatsink on the opposite side from where it was.  Bend the leads up at 90 degrees.  Make sure it is   
 *        installed correctly - the back of the metal tab will be up with the body of the 7805 about covering the via where 3.3 volts was 
 *        wired.  Have the 7805 a little off the board so it doesn't heat the wire we just put on side 2. I pop riveted a small piece of
 *        aluminium to the 7805 for some extra heat sinking.
 *      Mark the board for 3.3 volt operation. The Nano can no longer be used with this Raduino.  Test with a current limiting supply, a  
 *      worn out 9 volt battery from a smoke detector works.  Power up with no processor, no screen.  Short out the 3.3 volt run to protect 
 *      the Si5351 in case the 7805 is in backwards.  It should draw about 3 ma.  Test that the 5 volt net has 5 volts.  Insert the screen 
 *      and power up.  It should draw about 60 ma on the depleted battery and the screen should light.
 *            
 *            
 * A plan outline:           
 * 
 *    The DAC pin will do double or triple duty, sound to speaker, sound to microphone in for digital modes, AGC and ALC control.
 *      Audio down to the main board via the sidetone pin.  Circuit mods needed and some trim pots for levels.
 *      For digital modes turn the volume control down so you don't hear the transmit tones.
 *      For digital modes will need to remove the microphone from the jack or will tx room noise. 
 *    Will need a jumper to the main board to pick up audio for A2 analog input. May need an op-amp to increase the signal level.
 *      
 *    RX audio read via A2 AC coupled, processed and output via the Teensy DAC pin.  Using Teensy Audio Library.
 *    12 or 13 usable bits out of 16 bits sampled, will need some form of AGC to keep the A/D in range.
 *      
 *    For CW mode
 *      Can control power out via the to be installed ALC/AGC attenuator in the rf line to/from the first mixer.
 *      Perhaps can control CW power also via the Si5351 drive ( 2ma, 4ma, 6ma, 8ma ).
 *      Sidetone keyed with the keyer, actual transmit always behind by the relay on delay time( 20 ms or maybe shorter is ok ).
 *         So no short dits or dahs when first key up.
 *      Terminal mode.  Can use terminal tx/rx or CAT, not both at the same time.
 *         
 *    Reduce the gain on the signal input to A3 by 4 bits.  Then have 16 bits of dynamic range with a resolution
 *      of 12 bits.  Perhaps the ALC/AGC attenuator then not needed and can do AGC only in software.  Custom library object to merge the
 *      two audio streams, pick A3 or A2 shifted down 4 bits.  This all assumes those 4 bits are just noise.  Or have this stage do
 *      early AGC processing with 4 bits of compression on loud signals.  We will need to see what the signal level is at the sample point, 
 *      may actually need an op-amp with gain of 4 bits. 
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
 *      
 *      ATU board.   An ATU-100 mini clone from uSDX SOTA radio.
 *        Probably will want to run the I2C through a level converter and run the PIC on the ATU at 5 volts.   
 *        Mounting will be tight.  Maybe just mount using a bnc on the back panel. 
 *        A connector exists to pick up RF on the main board. Will need to wire 5 volts and I2C.
 *        Maybe can separate 5 volt runs so the PIC can run with the relays off, relays only on during TX like the rest of the radio.
 *          Or would need to use I2C to tell ATU to set the relays. 
 *        Poll during TX to display the FWD and REF power.  Could display the L and C also.  
 *                
 *      DSP bandwidth filters for CW. Notch, de-noise possible.  Try AM decoder with Carrier on high side of the filter.
 *        Could use a high Q lowpass to enhance the carrier on the edge of the filter.
 *      
 *      Mount the finals differently, on some thin wood maybe?
 *      Need to rotate the speaker away from the Teensy/ATU?  Top cover 1/4 inch higher may be needed for the ATU.
 *        
 *      Construct a Teensy 4.1 version  ( make sure all pullups to 5 volts removed )
 *        Use the Teensy audio shield.  Ordered a Softrock with K2 IF, about 4.914 mhz.  Can receive using the IQ DSP and transmit using 
 *        the built in SSB filter at 11 mhz.
 *        Just swap the Teensy board for a different Radio.
 *          Or can route I Q audio to a jack and control the radio with a remote head via CAT control. ( usb host mode ? ).
 *          Bigger screen, speaker, audio amp, etc.  Guess would need audio cables with this idea. 
 */

/*  Change log.   The above discussion is a loose plan, some things will be implemented some not.
 *   
 *      Implemented basic control of the receiving functions, mode, band switching.
 *      Added 160 meters to the band stack.  Can listen there, would need an external filter to try transmitting.
 *      Added a USA license class indicator to show when in a ham band and what class license is needed to transmit.
 *        Did not implement for 160 meters.
 *      Added some frequency presets to the 60 meter menu - 630 meters, some ssb aircraft frequencies.
 *      Added rudimentary CAT control using Argonaut V protocol.  Can fix any inaccuracies as needed. 
 *      
 *      
 *  To do.    
 *      Finish my last project.
 *      Test TX.  Freq change needed if CW mode.  ( send CW via Audio Keying of Sidetone is an option also, J2B? )
 *      Terminal Mode.
 *      Audio design using the Teensy Audio tool. Maybe start with just input to output to get signal levels correct.
 *      Change VFO to use PLLB and even divider.
 *      CW keyer.  ( wait for circuit changes as going to use digital pins ).  CW sequencer.  Try CW power levels idea.
 *      Measure signal level and see if we need more or less signal for the Teensy A/D. DAC will need attenuation.  Convert to DSP audio.
 *        Think about a FET to gate the standard audio through. Could then do A/B comparison. Might be useful for very weak signals. 
 *      CW decoder.   Hell decoder.
 *      Audio scope, audio FFT displays.
 *      Work with the ATU.  Mounting, add inductors, firmware. Kind of a separate project. 
 *      Some pictures for documentation.
*/         


// Wiring  ( not easy with Nano upside down )  Teensy mounted with USB on the other side for the shortest wiring to the display.
//  Teensy mounted upside right.
//  Special wiring will be A2 A3 for audio input and the DAC pin wired to Nano pin 6 - was CW_TONE for audio output.
//    The Teensy audio library will control these 3 pins.
//  ILI9341 wired for standard SPI as outlined on Teensy web site and it matches the Nano wiring pin for pin
//    Uses pins 8 to 13 wired to Nano pins 8 to 13
//  I2C uses the standard A4 A5, so they are wired to Nano A4 A5
//  The keyer will make use of the PTT pin as either DIT or DAH - digital keyer inputs instead of one analog pin
//  Teensy free pins A6, A7, A8, A9

#define TR        7     // to nano pin 7
                        // nano pin 6 CW_TONE wired to blocking capacitor( value ____ ) to DAC ( A14 )
#define LP_A      6     // to nano pin 5 
#define LP_B      5     // to nano pin 4
#define LP_C      4     // to nano pin 3
#define CW_KEY    3     // to nano pin 2
#define DIT_pin  A1     // to nano pin A6  - was KEYER,  short a resistor on main board, remove pullup to 5 volts.
#define DAH_pin  A0     // to nano pin A3
#define PTT      A0     // same pin as DAH_pin,  ptt wired over to the key jack, remove a resistor on the main board
#define ENC_B     0     // to nano A0   These are the Teensy digital pins not the analog pins for the encoder
#define ENC_A     1     // to nano A1
#define ENC_SW    2     // to nano A2

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
//#include <Audio.h>
//#include <Wire.h>
#include <i2c_t3.h>            // non-blocking wire library
#include <SPI.h>
#include "led_fonts.h"

#define BFO 11059590L          // !!! must match number in ubitx_si5351, for now defined in two separate places
#define IF  45000000L          // what is the actual center of the IF?

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

/*******************************************************************************/
// globals

// Implementing a simplistic split tuning model.
//   VFO A is the receive vfo, VFO B is the transmit vfo, split is used for RIT
//   Can listen on VFO B, if selected split is also selected.  If transmit, listen vfo returns to VFO A.
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

uint8_t  fast_tune;                    // double tap encoder to toggle

//   Touch menu's
void (* menu_dispatch )( int32_t );    // pointer to function that is processing screen touches

struct menu {
   char title[16];
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
   { "   VFO Mode" },
   { m_vfoa,m_vfob,m_split,m_cw,m_usb,m_lsb,m_am,m_digi },
   {0,1,2,3,4,5,6,7,-1,-1},
   48,
   EGA[1],
   5
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
};

struct BAND_STACK band_stack[10] = {
  {  1875000,  1875000, VFO_A+VFO_LSB, 4 },      // can listen on 160 meters
  {  3928000,  3928000, VFO_A+VFO_LSB, 4 },      // relay C  for 80 and 60 meters
  {  6000000,  6000000, VFO_A+VFO_USB, 4 },
  {  7168000,  7176000, VFO_A+VFO_LSB, 2 },      // relay B
  { 10106000, 10140000, VFO_A+VFO_USB, 2 },
  { 14200000, 14076000, VFO_A+VFO_USB, 1 },      // relay A
  { 18100000, 18200000, VFO_A+VFO_DIGI, 1 },
  { 21100000, 21100000, VFO_A+VFO_CW, 0 },       // default lowpass
  { 24900000, 24900000, VFO_A+VFO_USB, 0 },
  { 28250000, 28250000, VFO_A+VFO_USB, 0 }
};



// screen owners
#define DECODE 0
#define KEYBOARD 1
#define MENUS    2
uint8_t  screen_owner = DECODE;

extern void initOscillators();
extern void si5351bx_setfreq(uint8_t clknum, uint32_t fout);

/*****************************************************************************************/


void setup() {

  pinMode( TR, OUTPUT );                 // set a default radio state, receive mode
  pinMode( LP_A, OUTPUT);
  pinMode( LP_B, OUTPUT);
  pinMode( LP_C, OUTPUT);
  pinMode( CW_KEY, OUTPUT);
  digitalWriteFast( TR, LOW );
  digitalWriteFast( LP_A, LOW );         // !!! LP relays should be set up for the starting band
  digitalWriteFast( LP_B, LOW );         // they don't switch on until TR goes high
  digitalWriteFast( LP_C, LOW );
  digitalWriteFast( CW_KEY, LOW );

  Serial.begin(1200);                    // 2 meg usb serial, baud rate makes no difference.  Argonaut V baud rate.

  pinMode( ENC_A,   INPUT_PULLUP );
  pinMode( ENC_B,   INPUT_PULLUP );
  pinMode( ENC_SW,  INPUT_PULLUP );
  pinMode( DIT_pin, INPUT );             // !!! wiring changes needed, then use INPUT_PULLUP
  //pinMode( DAH_Pin, INPUT );             //      !!! currently have pullup to 5 volts on DIT_pin
  pinMode( PTT,     INPUT_PULLUP );      // redundant with dah depending upon wiring


  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  
  ts.begin();                               // touchscreen
  ts.setRotation(3);                        // ? should have been 1, same as print rotation
  
  tft.setTextSize(2);                       // sign on message
  tft.setTextColor(ILI9341_WHITE);          // or foreground/background for non-transparent text
  tft.setCursor(10,200);
  tft.print("K1URC uBitX w/ Teensy 3.2");   // about 25 characters per line with text size 2
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(1);
  tft.setCursor(14,230);     tft.print("Mic");
  tft.setCursor(150,230);     tft.print("Spkr");
  tft.setCursor(319-24,230);     tft.print("Key");

  Wire.begin();                        // !!! interrupt and dma options, speed options
  initOscillators();                        // sets bfo
//  si5351bx_setfreq( 1, 33948600 );          // set LSB mode
//  si5351bx_setfreq( 1, 56061400 );          // set LSB mode
  si5351bx_setfreq( 1, IF + BFO );     // set LSB mode

  vfo_freq_disp();
  vfo_mode_disp();
  set_relay( band_stack[band].relay );
 // band_change(3);                         // can start with a different band if desired
  
  menu_dispatch = &hidden_menu;             // function pointer for screen touch

}

void loop() {
static uint32_t  tm;
uint32_t t;
int t2;


   t2 = encoder();
   freq_update( t2 );                    // will have encoder users eventually

   t = touch();
   if( t ) (*menu_dispatch)(t);          // off to whoever owns the touchscreen


   //  One ms processing
   t = millis() - tm;
   tm += t;
   if( t > 5 ) t = 5;                       // first time or out of processing power
   while( t-- ){                            // repeat for any missed time ticks
      
      t2 = button_state(0);
      if( t2 > DONE ) button_process(t2);

   }

   radio_control();
   
}


void band_change( int to_band ){
int val;

   band_stack[band].vfoa = vfo_a;
   band_stack[band].vfob = vfo_b;
   band_stack[band].mode = vfo_mode;
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

   if( num & 1 ) digitalWriteFast( LP_A, HIGH );
   if( num & 2 ) digitalWriteFast( LP_B, HIGH );
   if( num & 4 ) digitalWriteFast( LP_C, HIGH );
}



// touch the screen top,middle,bottom to bring up different menus.  Assign menu_dispatch.
// This is default touch processing.   No menu on the screen.
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
   else if ( yt < 110 ){ 
      menu_display( &band_menu_data,0 );
      menu_dispatch = &band_menu;
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
      band_change(2);
      vfo_a = vfo_b = band_60_menu_data.param[selection];
      vfo_mode = VFO_A+VFO_USB;
      set_relay( band_stack[2].relay );
      band_60_menu_data.current = selection;
      band_menu_data.current = 2;
  }
 // vfo_freq_disp();     redundant with cleanup
 // vfo_mode_disp();
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

void mode_menu( int32_t t ){
int selection;
// int current;

   // current = mode_menu_data.current;                         // current is usb,cw,lsb etc.
                                                            
   selection = touch_decode( t, mode_menu_data.y_size );
   if( selection == -1 ){                                    // title was touched
      menu_cleanup();
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
        break;
        case 4:  vfo_mode &= 0x7;
                 vfo_mode |= VFO_USB;
        break;
        case 5:  vfo_mode &= 0x7;
                 vfo_mode |= VFO_LSB;
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
}

void menu_cleanup(){

   // exit touch menu and back to normal screen
   menu_dispatch = &hidden_menu;
   tft.fillScreen(ILI9341_BLACK);
   screen_owner = DECODE;
   vfo_mode_disp();
   vfo_freq_disp();
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
      band_check( vfo_b );                                          // check if tuning to next band, relays may change
  }
  else{
      vfo_a += count;
      if( (vfo_mode & VFO_SPLIT) == 0 ) vfo_b = vfo_a;              // tx vfo follows vfo_a if not in split
      if( (vfo_a % 100) == 0 && abs(count) < 100 ) holdoff = 5;     // rx tuning notch at 100 hz steps for fine tuning
      count = vfo_a % count;
      vfo_a -= count;                                               // clear not counting digits
      band_check( vfo_a );
  }

  vfo_freq_disp();
  
}


/***************      display freq of vfo's in a simulated 7 segment font   *****************************/
void vfo_freq_disp(){
//int val;
int32_t vfo;
//int pos;           // sceen x position
//int32_t mult;
int ca,cb;         // colors
char bp;

   cb = 4;         // unused vfo color in EGA pallet
   ca = 6;         // was 2 or 4

   if( vfo_mode & VFO_B ) vfo = vfo_b, cb = 10;
   else vfo = vfo_a, ca = 10;       // was 10

   if( vfo_mode & VFO_SPLIT ) cb = 12;    // vfo b is active transmit

   si5351bx_setfreq(2, vfo + IF);           // !!! not a good spot for this? but we do know which vfo is in use here

   if( screen_owner != DECODE ) return;
                          

   if( ca == 10 ) disp_vfo( vfo, 20, ca );
   else disp_vfo(vfo_a,20,ca);
   if( cb == 10 ) disp_vfo(vfo,170,cb);
   else disp_vfo(vfo_b,170,cb);

   bp = band_priv( band, vfo_b );                    // check if xmit vfo is in band
   tft.setCursor( 280,60);
   tft.setTextColor( EGA[10],0 );
   tft.setTextSize(1);
   if( bp == 'X' ) { tft.setTextColor( EGA[12],0); tft.print("  OOB"); }
   //if( bp == 'E' || bp == 'e' ) tft.print("Extra"); 
   if( bp == 'E' || bp == 'e' ) { tft.setTextColor( EGA[12],0);  tft.print("Extra"); } // my class is Advanced, so display Extra Red
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
       column += 3;                // wider spacing was 2
   }  
  
}


// change a mono font to 4BPP for display.  bits need to change from a vertical 0-7 to horizontal format.
// only using two colors so each 4 bits will be 0 or the forground color
// displaying at a fixed row for now, but should probably pass that as an argument also
// pass color also would be good
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
   if( vfo_mode & VFO_AM )  m = 10;              // AM through the filter on USB, tune 3 k off frequency down, 
                                                 // get carrier at 3k audio + lower sideband for the decoder.  Will this work?

   si5351bx_setfreq( 1, IF + as*BFO );           // !!! not sure I like having radio setups in the display code but currently it is the 
                                                 // common place where all code branches merge.  Also we know what the mode is here.
                                                 // what is the actual center of the 1st IF freq?  Notes say it is low due to loading but
                                                 // the standard code sets it at 45005000
                                                 
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
    
   tft.drawLine(0,69,319,69,EGA[4]);                 // 64
   tft.drawLine(0,70,319,70,EGA[4]);
//   if( decode_menu_data.current != DHELL ){
//      tft.drawLine(0,128,319,128,EGA[4]);
//      tft.drawLine(0,129,319,129,EGA[4]);
//   }


}


//  USA: can we transmit here and what mode
//  lower case for CW portions, upper case for phone segments
//  X - out of band, E - Extra, A - Advanced class, G - General
char band_priv( int band,  uint32_t f ){
char r = 'X';

   // skipping 160 meters for now, adjust band to match this code
   --band;
   
   if( band != 1 ) f = f / 1000;             // test 1k steps
   else f = f / 100;                         // 60 meters, test 500hz steps
   switch( band ){                           // is there an easy way to do this.  This seems like the hard way.
       case 0:
          if( f >= 3500 && f <= 4000 ){
             if( f < 3525 ) r = 'e';
             else if( f < 3600 ) r = 'g';
             else if( f < 3700 ) r = 'E';
             else if( f < 3800 ) r = 'A';
             else r = 'G';
          }
       break;
       case 1:
          if( (vfo_mode & VFO_USB) || (vfo_mode & VFO_DIGI) ){
             if( f == 53305 || f == 53465 || f == 53570 || f == 53715 || f == 54035 ) r = 'G';
          }
          if( vfo_mode & VFO_CW ){
             if( f == 53320 || f == 53480 || f == 53585 || f == 53730 || f == 54050 ) r = 'g'; 
          }
       break;
       case 2:
          if( f >= 7000 && f <= 7300 ){
             if( f < 7025 ) r = 'e';
             else if ( f < 7125 ) r = 'g';
             else if ( f < 7175 ) r = 'A';
             else r = 'G';
          }
       break;
       case 3:
          if( f >= 10100 && f <= 10150 ) r = 'g';
       break;
       case 4:
          if( f >= 14000 && f <= 14350 ){
             if( f < 14025 ) r = 'e';
             else if( f < 14150 ) r = 'g';
             else if( f < 14175 ) r = 'E';
             else if( f < 14225 ) r = 'A';
             else r = 'G';
          }
       break;
       case 5:
          if( f >= 18068 && f <= 18168 ){
             if( f < 18110 ) r = 'g';
             else r = 'G';
          }
       break;
       case 6:
          if( f >= 21000 && f <= 21450 ){
             if( f < 21025 ) r = 'e';
             else if( f < 21200 ) r = 'g';
             else if( f < 21225 ) r = 'E';
             else if( f < 21275 ) r = 'A';
             else r = 'G';
          }
       break;
       case 7:
         if( f >= 24890 && f <= 24990 ){
            if( f < 24930 ) r = 'g';
            else r = 'G';
         }
       break;
       case 8:
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
    case 'X':   stage_str("RADIO START"); stage('\r'); break; 
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
      stage_str("ER 1010-516");
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


void stage_str( String st ){      // !!! change this to use a char array
unsigned int i;
char c;

  for( i = 0; i < st.length(); ++i ){
     c = st.charAt( i );
     stage(c);
  }    
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
     case '0':  rx();  break;    // enter rx mode
     case '1':  tx();  break;    // TX
   }

}

// function stubs for cat calls
void cat_qsy( uint32_t freq ){       // test if a band change is needed

   if( vfo_mode & VFO_A ) vfo_a = freq;
   else vfo_b = freq;
   band_check( freq );
   vfo_freq_disp();
}

void band_check( uint32_t freq ){

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
   if( i != band ) band_change(i);
  
}

void mode_change( int i ){

  vfo_mode &= 7;                         // save A, B, SPLIT info
  if( i == 0 ) vfo_mode |= VFO_AM;
  if( i == 1 ) vfo_mode |= VFO_USB;
  if( i == 2 ) vfo_mode |= VFO_LSB;
  if( i == 3 ) vfo_mode |= VFO_CW;
  if( i == 4 ) vfo_mode |= VFO_DIGI;
  
}

void status_display(){

  vfo_mode_disp();
}

/********************* end Argo V CAT ******************************/

// what needs to happen to return to rx mode
void rx(){

  transmitting = 0;
  
}

// what needs to happen to enter tx mode
void tx(){

  transmitting = 1;
  
}
