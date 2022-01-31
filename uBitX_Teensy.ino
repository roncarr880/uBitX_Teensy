
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
 *      Maybe can use the R dividers to put the CW transmit signal where we want for each band.
 *        691.2/12/16 =  3.6 meg
 *        681.6/12/8  =  7.1 meg
 *          30 meters ?
 *        676.8/12/4  = 14.1 meg
 *          15 meters ?
 *        674.4/12/2  = 28.1 meg
 *      This doesn't seem to be a good idea.  So just change the divider to something other than 12 for CW transmit and reset the PLL. 
 *      
 *      ATU board.   An ATU-100 mini clone from uSDX SOTA radio.
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
 *      Use vfo A,B.  A is always the receive vfo, B is always the transmit vfo.  Update both when tuning in non-split mode.
 *        Auto split on transmit?  ( auto RIT ) Can't knock tx off freq and can tune rx at will.
 *      Touch could be modeled after my 40 meter QCX-Teensy.  Thinking will go with touch menu's instead of paddles as input.
 *        
 *      Display: use the LED fonts for main tuning display or play with an analog simulation.  
 *        Can display audio scope view or audio FFT
 *        Can decode CW, Hellschrieber, etc.
 *                
 *      DSP bandwidth filters for CW. Notch, de-noise possible.  Try AM decoder with Carrier on high side of the filter.
 *        Could use a high Q lowpass to enhance the carrier on the edge of the filter.
 *      
 *      Mount the finals differently, on some thin wood maybe?
 *        
 *      Construct a Teensy 4.1 version  ( make sure all pullups to 5 volts removed )
 *        Use the Teensy audio shield.  Ordered a Softrock with K2 IF, about 4.914 mhz.  Can receive using the IQ DSP and transmit using 
 *        the built in SSB filter at 11 mhz.
 *        Just swap the Teensy board for a different Radio.
 *          Or can route I Q audio to a jack and control the radio with a remote head via CAT control. ( usb host mode ? ).
 *          Bigger screen, speaker, audio amp, etc.  Guess would need audio cables with this idea. 
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
#define DBOUNCE 50     // 60



#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
//#include <Audio.h>
//#include <Wire.h>
#include <i2c_t3.h>            // non-blocking wire library
#include <SPI.h>
#include "led_fonts.h"

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
//  ? encoder switch  tap = enable split A selected (RIT), double tap enable split vfo B selected, long press B to A or A to B as above.

// radio defines for vfo_mode.
#define VFO_A  1
#define VFO_B  2
#define VFO_SPLIT  4
#define VFO_DIGI   8
#define VFO_CW    16
#define VFO_LSB   32
#define VFO_USB   64
#define VFO_AM   128

//  radio variables
int32_t  vfo_a = 7255000, vfo_b = 18100000;
uint8_t  vfo_mode = VFO_A + VFO_LSB;

uint8_t  fast_tune;



void setup() {

  pinMode( TR, OUTPUT );                 // set a default radio state, receive mode
  pinMode( LP_A, OUTPUT);
  pinMode( LP_B, OUTPUT);
  pinMode( LP_C, OUTPUT);
  pinMode( CW_KEY, OUTPUT);
  digitalWriteFast( TR, LOW );
  digitalWriteFast( LP_A, LOW );         // LP relays should be set up for the starting band
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
  ts.setRotation(1);
  
  tft.setTextSize(2);                       // sign on message, else see a blank screen until qcx boots.
  tft.setTextColor(ILI9341_WHITE);          // or foreground/background for non-transparent text
  tft.setCursor(10,200);
  tft.print("K1URC uBitX w/ Teensy 3.2");   // about 25 characters per line with text size 2

  vfo_freq_disp();
  vfo_mode_disp();

}

void loop() {
static uint32_t  tm;
uint32_t t;
int t2;


   t2 = encoder();
   freq_update( t2 );                    // will have encoder users eventually

   //  One ms processing
   t = millis() - tm;
   tm += t;
   if( t > 5 ) t = 5;                       // first time or out of processing power
   while( t-- ){                            // repeat for any missed time ticks
      
      t2 = button_state(0);
      if( t2 > DONE ) button_process(t2);

   }

  

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
     if( rpm > 20 /* || (vfo_mode & VFO_CW) == 0 */) count *= 10;
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
  vfo_freq_disp();
  
}


/***************      display freq of vfo's in a simulated 7 segment font   *****************************/
void vfo_freq_disp(){
//int val;
int32_t vfo;
//int pos;           // sceen x position
//int32_t mult;
int ca,cb;         // colors

   cb = 4;         // unused vfo color in EGA pallet
   ca = 6;         // was 2 or 4
//   if( screen_owner != DECODE ) return;
   if( vfo_mode & VFO_B ) vfo = vfo_b, cb = 10;
   else vfo = vfo_a, ca = 10;       // was 10

   if( vfo_mode & VFO_SPLIT ) cb = 12;    // vfo b is active transmit

   // qcx reports vfo 700 hz higher than actual as it is a CW receiver
   // remove that offset for AM, USB, LSB modes.  Add subtract BFO freq.
 //  if( (vfo_mode & VFO_AM ) == VFO_AM ) vfo += 12000;   // AM IF is 12.7 
 //  else if( ( vfo_mode & VFO_CW ) == 0 ) vfo -= 700;    // not CW, remove offset.
 //  if( (vfo_mode & VFO_USB ) ) vfo -= BFO_FREQ;
 //  if( (vfo_mode & VFO_LSB ) ) vfo += BFO_FREQ;
 //  if( (vfo_mode & VFO_CW ) &&  mode_menu_data.current == 1) vfo -= BFO_FREQ;   // SDR CW mode


   if( ca == 10 ) disp_vfo( vfo, 20, ca );
   else disp_vfo(vfo_a,20,ca);
   if( cb == 10 ) disp_vfo(vfo,170,cb);
   else disp_vfo(vfo_b,170,cb);
   

   
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

void disp_vfo( int32_t vfo, int pos, int col ){          // pos is column, col = color !!! fix


   int leading = 1; uint8_t c;
   int32_t mult = 10000000L;
   for( int i = 0; i < 8; ++i ){
       int digit = vfo / mult;
       if( leading && digit == 0 ) c = '/';
       else leading = 0, c = digit + '0';
       if( mult > 100 ) pos += disp_segments(pos,c,col,BigNumbers);
       else pos += disp_segments(pos,c,col,MediumNumbers);
       vfo -= digit*mult;
       mult /= 10;
       pos+= 3;                // wider spacing was 2
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

 //  if( screen_owner != DECODE ) return;
   a = b = s = r = 0;                       // set colors of the vfo mode
   c = u = l = m = 0;
   
   if( vfo_mode & VFO_B ) b = 10;
   if( vfo_mode & VFO_A ) a = 10;
   if( vfo_mode & VFO_SPLIT ) s = 10;
   if( vfo_mode & VFO_DIGI ) r = 10;
   if( vfo_mode & VFO_CW ) c = 10;
   if( vfo_mode & VFO_LSB ) l = 10;
   if( vfo_mode & VFO_USB ) u = 10;
   if( vfo_mode & VFO_AM )  m = 10;
   
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
    
   tft.drawLine(0,64,319,64,EGA[4]);      
   tft.drawLine(0,65,319,65,EGA[4]);
//   if( decode_menu_data.current != DHELL ){
//      tft.drawLine(0,128,319,128,EGA[4]);
//      tft.drawLine(0,129,319,129,EGA[4]);
//   }
  

 //  PhaseChange(0);           // just print current value
 //  tft.setCursor(262,78);    // print bandwidth ( spacing 12 pixel ? )
 //  tft.setTextSize(1);
 //  tft.setTextColor(EGA[14],0);
 //  tft.print("BW: ");
 //  tft.print(band_width_menu_data.menu_item[band_width_menu_data.current]);
 //  tft.setCursor(262,90);
 //  tft.print("Stp: ");
 //  if(stp <= 1000 ) p_leading(stp,4);
 //  tft.setCursor(262,102);
 //  tft.print("Atn: ");
 //  tft.print(peak_atn - 15);   tft.write(' ');
 //  tft.setCursor(262,114);
   //tft.print("Sig: ");
   //tft.print(rms1.read());  
}
