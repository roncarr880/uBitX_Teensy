
/* usdx SOTA ATU in the bitX.  PIC18F2220 instead of 16f1938 due to part shortages */
/*
    Proposed commands on serial, should all commands have a response ?
  Z  -  Zero relays, keep current data.  Bypass mode.
  O  -  On, apply relays
  S  -  Set relay data Cval Lval ( apply or wait for O command? )
  R  -  Report relay values  Cval Lval
  P  -  Report Fwd and Rev Power. Fwd, Rev, high and low bytes. 
  T  -  Tune ( return a done value ? )
  C D   Inc or Dec Cval
  L M   Inc or Dec Lval
  H  -  Toggle Cap to Low Z or High Z side of L
*/


#include "p18f2220.h"

/* probably would be better to put CONFIG commands in this file and leave the header generic */

/* globals */

#define B0  0x01
#define B1  0x02
#define B2  0x04
#define B3  0x08
#define B4  0x10
#define B5  0x20
#define B6  0x40
#define B7  0x80

/*   #pragma pic 504
     char stack[8];   not using this */
#pragma pic 0
char _temp;        /* some vars needed by the compiler */
char _temp2;
char _eedata;


                    /* access ram up to 127 */

                    /* there doesn't seem to be any method to the madness of the I/O port assignments */
char Cvals;         /* straight data 7 bits, will need to be dispersed to the correct port,bit */
char Lvals;
/*********
char Pa;            /*  Ports for relay bits 
char Pb;                changed function to inline asm
char Pc;
******/

                    /* serial tx and rx buffers */
char rx_buf[8];
char rx_in;
char rx_out;
char tx_buf[8];
char tx_in;
char tx_out;
char rstate;        /* uart vars that need to be initialized */
char tstate;
char rtimer;        /* relay time */



/*  statics and function args in page 1 */
/* perhaps could just use access ram, have 128 locations. */
#pragma pic 256

main(){
static char temp;

    /*  !!! testing, toggle an output 
    #asm
    BTG  LATA,RA7    ; pin 9
    #endasm

   /* !!! testing echo rs232 
   if( rx_in != rx_out ){
      temp = rx_buf[rx_out++];
      rx_out &= 7;
      tx_buf[tx_in++] = temp;
      tx_in &= 7;
   }  *******************/

/*********
  Z  -  Zero relays, keep current data.  Bypass mode.
  O  -  On, apply relays
  S  -  Set relay data Cval Lval ( apply or wait for O command? )
  R  -  Report relay values  Cval Lval
  P  -  Report Fwd and Rev Power. Fwd, Rev, upper or lower bits in 3rd value returned.
  T  -  Tune ( return a done value ? )
  C D   Inc or Dec Cval
  L M   Inc or Dec Lval
  H  -  Toggle Cap to Low Z or High Z side of L
**********/


   if( rx_in != rx_out ){           /* commands */
      temp = get_rx();
      switch( temp ){
         case 'Z':  bypass();   break;      /* relays off, save current setting */
         case 'O':  write_vals(); break;    /* relays on */
         case 'S':  set_vals(); /*write_vals();*/  break;    /* new relay data from host */
         case 'R':  report_vals(); break;   /* send relay settings to host */
         case 'P':  report_power(); break;  /* report FWD, REV readings */
         case 'T':  tune();     break;
         case 'C':  ++Cvals;  /*write_vals();*/  break;
         case 'D':  --Cvals;  /*write_vals();*/  break;
         case 'L':  ++Lvals;  /*write_vals();*/  break;
         case 'M':  --Lvals;  /*write_vals();*/  break;
         case 'H':  Cvals ^= B7; /*write_vals();*/  break;   /* toggle cap input or output side of L's */
      }
   }

}

bypass(){               /* clear relays */
static char tc;
static char tl;

    tc = Cvals;         /* but save current settings */
    tl = Lvals;
    Cvals = Lvals = 0;
    write_vals();
    Cvals = tc;
    Lvals = tl;
}

set_vals(){            /* get new settings from host, wait to implement */

   while( rx_in == rx_out );
   Cvals = get_rx();
   while( rx_in == rx_out );
   Lvals = get_rx();
}

report_vals(){

   put_tx('R');
   put_tx( Cvals );
   put_tx( Lvals );
}

report_power(){
   put_tx('P');
}

tune(){
}


put_tx( char val ){

   tx_buf[tx_in++] = val;
   tx_in &= 7;
}

char get_rx(){
static char temp;

   temp = rx_buf[rx_out++];
   rx_out &= 7;
   return temp;      
}

init(){

  /* while( (OSCCON & B2) == 0 ); this hangs maybe wrong spot ? /* wait for clock stable */
   OSCCON = ( B4+B5+B6+B1 );    /* 8 mhz internal clock */

   FSR0H = 0;                   /* FSR will only work in page zero as MCC is not updating FSR0H */
  /* FSR1H = 1;                    use stack temps in page 1 ? 
   FSR1L = &stack[7];  **************/

   TRISC = 0;                   /* all outputs */
   TRISA = 3;                   /* RA0 and RA1 are the FWD and REV analog inputs */
   ADCON1 =  0x0D;                     /* two analog channels, 1101 */
   LATB = B7;                          /* uart idles high */
   TRISB = ( 0xff ^ (B3+B4+B5+B7 ));   /* careful, some of the B pins are shorted to ground or +5 */
                                       /* !!! will want the floating B pin as an output, which one is that */
                                       /* B7 as output, B6 as an input for serial tx */
                                       /* have a timer interrupt and sample B6 */

   /* init variables that need init values */
   rx_in = rx_out = tx_in = tx_out = 0;
   Cvals = Lvals = 0;
   rstate = tstate = 0;


   /* start timer 2 and enable interrupt.   1200 baud at 8 mhz clock */
   /* sample at 3x baud rate */
   TMR2 = 0;
   PR2 = 139;                /* 139 * 4 * 3 / 2  == bit time in us.  834 ( 833.333333 ) */
   PIE1 = B1;                /* interrupts at 139 * 4 / 2 ==  278 us, can do 556 instructions at 8 mhz clock */
   T2CON = B0 + B2;          /* prescale by 4, on bit  B0 + B2 */
   interrupts();

}

           /* implement a software uart on RB6 and RB7, interrupt at 3x baud rate.  data is LSB first */
_interrupt(){
static char rdat;
static char rmod;
static char rbit;
static char fsr0;
static char tdat;
static char tmod;
static char tbit;
static char temp;
/* static char tstate, rstate global for init purposes */

    /*  testing, toggle an output 
    #asm
    BTG  LATA,RA6    ; pin 10
    #endasm  ****************/
   
    
    fsr0 = FSR0L;         /* save indirect address */

    if( rtimer ) --rtimer;              /* relay settling timer */
    
    if( ++rmod > 2 ) rmod = 0;
    
    switch( rstate ){
       case 0:                          /* looking for start bit */
          if( (PORTB & B6) == 0 ){
             rbit = rdat = rmod = 0;
             ++rstate;
          }
       break;
       case 1:
          if( rmod == 1 ){                   /* clock bits, start bit will fall off the end of 8 bits */
             rdat >>= 1;                     /* rlcf vs rlf, #define in header file to fix */
             if( PORTB & B6 ) rdat |= 0x80;
             if( ++rbit == 9 ) ++rstate; 
                                             /* could check start bit was zero here */
          }
       break;
       case 2:                          /* could check for framing error, save data */
          rx_buf[rx_in++] = rdat;
          rx_in &= 7;
          rstate = 0;
       break;
    }

    /* transmitter */

    if( ++tmod > 2 ) tmod = 0;

    switch( tstate ){
       case 0:                          /* something to send ? */
          if( tx_in != tx_out ){
             tdat = tx_buf[tx_out++];
             tx_out &= 7;
             tmod = tbit = 0;
             LATB ^= B7;                /* start bit low, assume it was high */
             ++tstate;
          }
       break;
       case 1:
          if( tmod == 0 ){
             temp = ( tdat & 1 ) ? B7 : 0;
             temp ^= LATB;
             temp &= B7;
             LATB ^= temp;
             tdat >>= 1;
             if( ++tbit == 8 ) ++tstate;
          }
       break;
       case 2:                           /* stop bit */
          if( tmod == 0 ){
             LATB |= B7;
             ++tstate;
          }
       break;
       case 3:                          /* done with stop bit */
          if( tmod == 0 ) tstate = 0;
       break;
    }

       

    FSR0L = fsr0;

    /* clear the interrupt bit */
    #asm
    BCF  PIR1,TMR2IF
    retfie FAST           ; WREG, STATUS, BSR restored
    #endasm
}



write_vals( ){     /* apply the relay settings */
   
   #asm
   ;if( Cvals & 0x01 ) Pc |= B7;
      btfsc Cvals,0
      bsf   LATC,7
      btfss Cvals,0
      bcf   LATC,7
   ;if( Cvals & 0x02 ) Pc |= B3;
      btfsc Cvals,1
      bsf   LATC,3
      btfss Cvals,1
      bcf   LATC,3
   ;if( Cvals & 0x04 ) Pc |= B6;
      btfsc Cvals,2
      bsf   LATC,6
      btfss Cvals,2
      bcf   LATC,6
   ;if( Cvals & 0x08 ) Pc |= B2;
      btfsc Cvals,3
      bsf   LATC,2
      btfss Cvals,3
      bcf   LATC,2
   ;if( Cvals & 0x10 ) Pc |= B5;
      btfsc Cvals,4
      bsf   LATC,5
      btfss Cvals,4
      bcf   LATC,5
   ;if( Cvals & 0x20 ) Pc |= B1;
      btfsc Cvals,5
      bsf   LATC,1
      btfss Cvals,5
      bcf   LATC,1
   ;if( Cvals & 0x40 ) Pc |= B4;
      btfsc Cvals,6
      bsf   LATC,4
      btfss Cvals,6
      bcf   LATC,4
   ;if( Cvals & 0x80 ) Pc |= B0;    /* caps on input side or output side */
      btfsc Cvals,7
      bsf   LATC,0
      btfss Cvals,7
      bcf   LATC,0

   ;if( Lvals & 0x01 ) Pb |= B3;
      btfsc Lvals,0
      bsf   LATB,3
      btfss Lvals,0
      bcf   LATB,3
   ;if( Lvals & 0x02 ) Pa |= B2;
      btfsc Lvals,1
      bsf   LATA,2
      btfss Lvals,1
      bcf   LATA,2
   ;if( Lvals & 0x04 ) Pb |= B4;
      btfsc Lvals,2
      bsf   LATB,4
      btfss Lvals,2
      bcf   LATB,4
   ;if( Lvals & 0x08 ) Pa |= B3;
      btfsc Lvals,3
      bsf   LATA,3
      btfss Lvals,3
      bcf   LATA,3
   ;if( Lvals & 0x10 ) Pb |= B5;
      btfsc Lvals,4
      bsf   LATB,5
      btfss Lvals,4
      bcf   LATB,5
   ;if( Lvals & 0x20 ) Pa |= B5;
      btfsc Lvals,5
      bsf   LATA,5
      btfss Lvals,5
      bcf   LATA,5
   ;if( Lvals & 0x40 ) Pa |= B4; 
      btfsc Lvals,6
      bsf   LATA,4
      btfss Lvals,6
      bcf   LATA,4
   #endasm

   /* !!! should we wait here for relay settling time before processing any more commands? */
   delay(10);
   /*  should we kick off a FWD REV measurement after waiting */
   /* I think report power should do the measurement and report the fresh value */
}

delay( char tm ){      /* approx delay using .278 ms steps, use .25 in calculation */
                       /* max delay for call about 63 ms, or shift overflows */
   tm <<= 2;           /* mult by 4 for ms */
   rtimer = tm;
   while( rtimer );
}

interrupts(){

   #asm
      bsf   INTCON,GIE
      bsf   INTCON,PEIE   ;  need for tmr2 ? : yes  
   #endasm
}

no_interrupts(){

   #asm
      bcf   INTCON,GIE
      btfsc INTCON,GIE    ;see AN576.  What devices have this issue?
      goto $-2
   #endasm

}


/*****************************************  save older code
write_vals( ){
static char temp;                 /* set up INDF1 as a stack instead of statics ? 
   
   /*--FSR1L;

   Pa = Pb = Pc = 0;
   if( Cvals & 0x01 ) Pc |= B7;
   if( Cvals & 0x02 ) Pc |= B3;
   if( Cvals & 0x04 ) Pc |= B6;
   if( Cvals & 0x08 ) Pc |= B2;
   if( Cvals & 0x10 ) Pc |= B5;
   if( Cvals & 0x20 ) Pc |= B1;
   if( Cvals & 0x40 ) Pc |= B4;
   if( Cvals & 0x80 ) Pc |= B0;    /* caps on input side or output side 

   if( Lvals & 0x01 ) Pb |= B3;
   if( Lvals & 0x02 ) Pa |= B2;
   if( Lvals & 0x04 ) Pb |= B4;
   if( Lvals & 0x08 ) Pa |= B3;
   if( Lvals & 0x10 ) Pb |= B5;
   if( Lvals & 0x20 ) Pa |= B5;
   if( Lvals & 0x40 ) Pa |= B4; 

   LATC = Pc;

   /***********   
   INDF1 = LATA ^ Pa;              using stack temp 
   INDF1 &= ( B2+B3+B4+B5 );
   LATA ^= INDF1;

   INDF1 = LATB ^ Pb;
   INDF1 &= ( B3+B4+B5 );
   LATB ^= INDF1;
   ***************

   temp = LATA ^ Pa;               /* toggle change bits 
   temp &= ( B2+B3+B4+B5 );
   LATA ^= temp;

   temp = LATB ^ Pb;
   temp &= ( B3+B4+B5 );
   LATB ^= temp;                   /* atomic or need to disable interrupts ? 


   /*++FSR1L;
}

******************************/


