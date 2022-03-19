
/* usdx SOTA ATU in the bitX.  PIC18F2220 instead of 16f1938 due to part shortages */
/*
    Proposed commands on serial
  Z  -  Zero relays, keep current data.  Bypass mode.
  X  -  Set Quick solution relays.  Max 3 on.
  O  -  On, apply relays
  S  -  Set relay data Cval Lval ( apply or wait for O command? )
  R  -  Report relay values  Cval Lval
  P  -  Report Fwd and Rev Power. Fwd, Rev, high and low bytes.
  W  -  Report sWr
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

#define ADRIGHT 0x80           /* left or right justification of A/D result */
#define ADLEFT  0x00

#define MATCH 10               /* antenna matched swr * 10 */
#define QMATCH 14

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
char Hval;

char QC;            /* the quick solution, use for RX for minimum relay current */
char QL;
char QH;

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
char fwdh, fwdl;
char revh, revl;
char swr;
char tlow, thigh;   /* tune timeout trys counter */

/* integer math vars */
char accl;
char acch;
char argl;
char argh;
char overflow;
char carry;
char divi;
char divql;
char divqh;

/*  make these common variable names global in access page, should result in less ram bank changes */
char Ct,Lt,Ht;
char swrq;
char Cdv, Ldv;
char chg;



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
  X  -  Set Quick solution relays.  Max 3 on.
  O  -  On, apply relays
  S  -  Set relay data Cval Lval ( apply or wait for O command? )
  R  -  Report relay values  Cval Lval
  P  -  Report Fwd and Rev Power. Fwd, Rev, in 16 bit.
  W  -  Report sWr
  T  -  Tune ( return a done value ? )
  C D   Inc or Dec Cval
  L M   Inc or Dec Lval
  H  -  Toggle Cap to Low Z or High Z side of L
**********/


   if( rx_in != rx_out ){           /* commands */
      temp = get_rx();
      switch( temp ){
         case 'Z':  bypass();   break;      /* relays off, save current setting */
         case 'X':  write_quick();  break;  /* quick solution on */
         case 'O':  write_vals(); break;    /* relays on */
         case 'S':  set_vals(); /*write_vals();*/  break;    /* new relay data from host */
         case 'R':  report_vals(); break;   /* send relay settings to host */
         case 'P':  report_power(); break;  /* report new FWD, REV readings */
         case 'T':  tune();     break;
         case 'C':  ++Cvals;  /*write_vals();*/  break;
         case 'D':  --Cvals;  /*write_vals();*/  break;
         case 'L':  ++Lvals;  /*write_vals();*/  break;
         case 'M':  --Lvals;  /*write_vals();*/  break;
         case 'H':  Hval ^= B7; /*write_vals();*/  break;   /* toggle cap input or output side of L's */
         case 'W':
            put_tx('W');
            put_tx(swr);     /* last value from tune, for dynamic values use P - current fwd and rev power */
         break;
      }
   }

}

bypass(){               /* clear relays */
static char tc;
static char tl;
static char h;

    tc = Cvals;         /* but save current settings */
    tl = Lvals;
    h = Hval;
    Hval = Cvals = Lvals = 0;
    write_vals();
    Cvals = tc;
    Lvals = tl;
    Hval = h;
}

write_quick(){          /* set the quick solution value */
static char tc;
static char tl;
static char h;

    tc = Cvals;         /* save current settings */
    tl = Lvals;
    h = Hval;
    Hval = QH;
    Cvals = QC;
    Lvals = QL;
    write_vals();
    Cvals = tc;
    Lvals = tl;
    Hval = h;
}

set_vals(){            /* get new settings from host, wait to implement */

   while( rx_in == rx_out );
   Cvals = get_rx();
   while( rx_in == rx_out );
   Lvals = get_rx();
   Hval = Cvals & B7;
   Cvals &= 0x7F;
}

report_vals(){

   put_tx('R');
   put_tx( Cvals+Hval );
   put_tx( Lvals );
}

report_power(){
   
   ad_sample( ADRIGHT );
   put_tx('P');
   put_tx( fwdh );
   put_tx( fwdl );
   put_tx( revh );
   put_tx( revl );
}

tune(){

   tlow = thigh = 0;         /* timeout try counter */
   Qtune();
   if( thigh >= 4 ){         /* timeout, no signal probably */
      put_tx('T');
      put_tx( swr );
      return;
   }
   if( swr > MATCH ) Ftune();     /* fine tune from the best quick tune result */
   put_tx('T');
   put_tx(swr);

}

Qtune(){                 /* shift a bit across L and  C */
/* char swrq;*/
/* char Lt,Ct,Ht;  */


   /* swrq = 249; */
   QC = Cvals = Ct = 0;
   QL = Lvals = Lt = 0;
   QH = Hval  = Ht = 0;
   write_vals();              /* get a base reading of passthrough */
   get_swr();
   swrq = swr;
   if( thigh >= 4 || swr <= MATCH ) return;     /* timeout or antenna matched without tuner */


   for( Cvals = 1; Cvals < 128;  Cvals <<= 1 ){
      for( Lvals = 1; Lvals < 128; Lvals <<= 1 ){
         Hval = 0;               /* test low Z */
         Qtest();
         Hval = 128;             /* test high Z */
         Qtest();
      }
      if( swrq <= QMATCH && Ct > 1 ) break;      /* avoid early exit for low Ct, Ht may be incorrect, HiZ LoZ */
      if( thigh >= 4 || swrq <= MATCH ) break;   /* timeout or matched */
   }      

   /* apply best, save quick solution */
   QC = Cvals = Ct;
   QL = Lvals = Lt;
   QH = Hval = Ht;
   write_vals();
   swr = swrq;  
}


Qtest(){

   write_vals();
   get_swr();
   if( swr < swrq ){       /* save best results so far */
      Lt = Lvals;
      Ct = Cvals;
      Ht = Hval;
      swrq = swr;
   }
}

  /* a binary tuning algorithm, try above and below quick tune result with decreasing offset */
  /* high Z, low Z not changed */
Ftune(){
static char dv;                 /* delta value */

   Lt = Lvals;  Ct = Cvals;
   Cdv = Ct >> 1;               /* start search next power of two down */
   Ldv = Lt >> 1;
   dv = ( Ldv > Cdv ) ? Ldv : Cdv;
   if( Cdv == 0 ) Cdv = 1;      /* set min adjustment values or nothing to do here */
   if( Ldv == 0 ) Ldv = 1;
   if( dv == 0 ) dv = 1;

   write_vals();                /* starting swr is known but get it again in case preceeding algorithm is changed */
   get_swr();
   swrq = swr;  

   while( dv ){
      chg = 0;

      /* try up L */
      Lvals += Ldv;
      if( Lvals < 128 ) FCLtest();
      Lvals = Lt;

      /* try down C */
      Cvals -= Cdv;
      if( Cvals < 128 ) FCLtest();           /* unsigned math */
      Cvals = Ct;

      /* try down L */
      Lvals -= Ldv;
      if( Lvals < 128 ) FCLtest();           /* unsigned math */
      Lvals = Lt;

      /* try up C */
      Cvals += Cdv;
      if( Cvals < 128 ) FCLtest();
      Cvals = Ct;

      if( chg == 0 ){
         dv >>= 1;
         if( dv < Cdv ) Cdv = dv;
         if( dv < Ldv ) Ldv = dv;
      }
      if( thigh >= 4 || swrq <= MATCH ) break;
   }

   swr = swrq;                /* write best */
   Cvals = Ct;
   Lvals = Lt;
   write_vals(); 

}


FCLtest(){

   write_vals();
   get_swr();
   if( swr < swrq ){
      Ct = Cvals;
      Lt = Lvals;
      ++chg;
      swrq = swr;
   }
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
   Hval = Cvals = Lvals = 0;
   rstate = tstate = 0;


   /* start timer 2 and enable interrupt.   1200 baud at 8 mhz clock */
   /* sample at 3x baud rate */
   TMR2 = 0;
   PR2 = 139;                /* 139 * 4 * 3 / 2  == bit time in us.  834 ( 833.333333 ) */
   PIE1 = B1;                /* interrupts at 139 * 4 / 2 ==  278 us, can do 556 instructions at 8 mhz clock */
   T2CON = B0 + B2;          /* prescale by 4, on bit  B0 + B2 */
   interrupts();
   write_vals();             /* clear ports */

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
   ;if( Hvals & 0x80 ) Pc |= B0;    /* caps on input side or output side */
      btfsc Hval,7
      bsf   LATC,0
      btfss Hval,7
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

   /* wait here for relay settling time before processing any more commands */
   delay(10);

}

delay( char tm ){      /* approx delay using .278 ms steps, use .25 in calculation */
                       /* max delay for call about 63 ms, or shift overflows */
   tm <<= 2;           /* mult by 4 for ms */
   rtimer = tm;
   while( rtimer );    /* interrupt service decrements the timer */
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

ad_sample( char just ){
static char i;

  ADCON2 = just + 0x25;       /* 8 tad sample, tad is 16 divider of 8 meg clock, 2us */
  ADCON0 = 5;                 /* on sample chan 1.  Fwd Rev swapped with my 2nd try at a bridge */
  for( i = 0; i < 5; ++i );   /* delay */
  #asm
    bsf ADCON0,1
  #endasm
  while( ADCON0 & B1 );    /* wait done */
  acch = ADRESH;
  accl = ADRESL;

  for( i = 0; i < 5; ++i );   /* average 2 samples */
  #asm
    bsf ADCON0,1
  #endasm
  while( ADCON0 & B1 );    /* wait done */
  argh = ADRESH;
  argl = ADRESL;

  dadd();
  right_shift(1);
  fwdh = acch;   fwdl = accl;


  ADCON0 = 1;                 /* chan 0 */
  for( i = 0; i < 5; ++i );   /* delay */
  #asm
    bsf ADCON0,1
  #endasm
  while( ADCON0 & B1 );    /* wait done */
  acch = ADRESH;
  accl = ADRESL;

  for( i = 0; i < 5; ++i );   /* delay */
  #asm
    bsf ADCON0,1
  #endasm
  while( ADCON0 & B1 );    /* wait done */
  argh = ADRESH;
  argl = ADRESL;

  dadd();
  right_shift(1);
  revh = acch;   revl = accl;
  
}

get_swr(){
static char fwd;

   fwd = 0;
   while( fwd < 5 ){                 /* noise always less than Number ? */
     if( ++tlow == 0 ) ++thigh;
     ad_sample( ADRIGHT );
     if( fwdh == 0 ) fwd = fwdl;     /* see if have a reading */
     else fwd = 99;                  /* just any value > Number to exit this loop */
     if( fwd < 5 ) delay( 10 );
     if( thigh >= 4 ){
        swr = 253;
        return;
     }
   }

   /* calc swr in 16 bit */

   acch = fwdh;   accl = fwdl;
   argh = revh;   argl = revl;
   dadd();
   multiply( 10 );                     /* scale up for factional part */
   divqh = acch;  divql = accl;

   acch = fwdh;   accl = fwdl;
   argh = revh;   argl = revl;
   if( dsub() ){                       /* borrow, rev is higher than fwd */
      swr = 250;
      return;
   }
   argh = acch;   argl = accl;
   if( argh == 0 && argl == 0 ){       /* test divide by zero */
      swr = 251;
      return;
   }
   divide();
   if( divqh ){                        /* result more than 8 bits */
      swr = 252;
      return;
   }
   swr = divql;

}


/****************   16bit  math ********************/

zacc(){          /* zero accumulator */

   accl = acch = 0;
}

char dadd(){     /* double add, return carry */

   #asm
      BANKSEL overflow
      clrf    overflow
      clrf    carry

      movf	argl,W
      addwf	accl,F          ; add low bytes
      btfsc   STATUS,C
      incf    carry,F          ; save carry

      movf	carry,W        ; add the carry if any to high byte
      addwf	acch,F

      btfsc   STATUS,C
      incf    overflow,F        ; capture and save if adding one caused overflow 

      movf	argh,W          ; add the high bytes
      addwf	acch,F

      btfsc   STATUS,C
      incf    overflow,F        ; merge in if had an overflow here

   #endasm

   return overflow;     /* return 0, 1, or 2 */
   
}

right_shift( char count ){   /* unsigned shift double (or divide by 2) the accumulator */

   while( count-- ){
     #asm
        banksel  acch
        bcf   STATUS,C      ; logical shift right double
        rrf   acch,F
        rrf   accl,F
     #endasm
   }
}

char dsub(){    /* double subb, return borrow as a true value */


   #asm

      BANKSEL overflow
      clrf    overflow
      clrf    carry

      movf	argl,W
      subwf	accl,F          ; sub low bytes
      btfss   STATUS,C         ; carry set means no borrow, skip next
      incf    carry,F          ; save borrow

      movf	carry,W        ; sub the borrow if any from high byte
      subwf	acch,F

      btfss   STATUS,C
      incf    overflow,F        ; capture and save if subbing one caused a borrow 

      movf	argh,W          ; sub the high bytes
      subwf	acch,F

      btfss   STATUS,C
      incf    overflow,F        ; merge in if had a borrow here

   #endasm
   return overflow;     /* return borrow as true */

}

/* a divide algorithm */
/* dividend quotient has the dividend, argh,argl has the divisor, remainder in acch,accl */
/* shift upper bits of the dividend into the remainder, test a subtraction of the divisor */
/* restore the remainder on borrow, or set a bit in quotient if no borrow */
/* divisor remains unchanged */

divide(){
 
   zacc();
   for( divi = 0; divi < 16; ++divi ){
      #asm
        bcf   STATUS,C
        rlf   divql,F        ; banksel ok here, same as divi
        rlf   divqh,F
        rlf   accl,F
        rlf   acch,F
      #endasm
      if( dsub() ) dadd();     /* borrow, add back */
      else divql |= 1;         /* no borrow, so set a 1 in quotient */
   }
}

divide_k( char constant ){      /* divide accumulator by a constant or char */
                                /* put result in the accum overwriting remainder */
   
   divql = accl;   divqh = acch;
   argh  = 0;      argl = constant;
   divide();
   accl = divql;   acch = divqh;
}


/* untested multiply routine */
/* 8 by 16 bit multiply, unsigned, assume result fits in 16 bits, return overflow if not */
/* 
   acch,accl has multiplicand, moved to argh,argl.
   8 bit multiplier as function argument,   product in acch,accl
   add increasing powers of 2 of the multiplicand to the product depending upon bits set in the multiplier
*/
char multiply( char multi ){
static char over;

    over = 0;
    divi = multi;  /* use a same bank variable as the multiplier variable */
    argh = acch;   /* get the multiplicand out of the accumulator */
    argl = accl;
    zacc();

    while( divi ){
       if( divi & 1 ) over |= dadd();
       divi >>= 1;
       #asm
          ; banksel argl     ; multi and argl in different banks, fixed by using divi
          bcf  STATUS,C
          rlf  argl,F
          rlf  argh,F
       #endasm
    }

    return over;
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

/*

Qtune_oldcode(){                 /* shift a bit across L and C, should take 1 second ( 7*7*2*10ms ) 
static char swrq;        /* this algorithm misses trying zero C with L values and zero L with C values 
static char L,C,H;

   swrq = 249;
   L = C = H = 0;
   QC = Cvals = C;
   QL = Lvals = L;
   QH = Hval = H;
   write_vals();      /* get a base reading of passthrough 
   get_swr();
   swrq = swr;
   if( thigh >= 4 || swr <= 13 ) return;

   for( Lvals = 1; Lvals < 128; Lvals <<= 1 ){
      for( Cvals = 1; Cvals < 128; Cvals <<= 1 ){
         Hval = 0;
         write_vals();
         get_swr();
         if( swr < swrq ){
            L = Lvals;
            C = Cvals;
            H = 0;
            swrq = swr;
         }
         Hval = 128;            /* caps other side of L 
         write_vals();
         get_swr();
         if( swr < swrq ){
            L = Lvals;
            C = Cvals;
            H = 128;
            swrq = swr;
         }
      }
      if( thigh >= 4 || swrq <= 13 ) break;      /* how low a swr expected from quick algorithm? 
   }
   /* apply best so far, save quick solution 
   QC = Cvals = C;
   QL = Lvals = L;
   QH = Hval = H;
   write_vals();
   swr = swrq;  
}


Ftune_old(){                   /* fine tune near the quick tune result, power 2 below to power 2 above 
static char swrq;
static char L,C,H;
static char EL, EC;
static char BC;

   L = Lvals;  C = Cvals;  H = Hval;
   Lvals >>= 1;
   Cvals >>= 1;
   BC = Cvals;
   EL = L << 1;
   EC = C << 1;
   write_vals();                /* starting swr 
   get_swr();
   swrq = swr;  
   if( thigh >= 4 ){            /* timeout, use best known so far 
     Cvals = C;
     Lvals = L;
    /* Hval = H;                not changing H in fine tune 
     write_vals();     
     return;
   }

   for( ; Lvals < EL;  ++Lvals ){
      for( Cvals = BC; Cvals < EC; ++Cvals ){
         write_vals();
         get_swr();
         if( swr < swrq ){
            C = Cvals;
            L = Lvals;
            swrq = swr;
         }
         if( thigh >= 4 || swrq <= 12 ) break;
      }
      if( thigh >= 4 || swrq <= 12 ) break;
   }
   swr = swrq;                /* write best 
   Cvals = C;
   Lvals = L;
   /* Hval = H;    
   write_vals(); 
}


***********/

