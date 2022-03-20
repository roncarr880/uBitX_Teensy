; # include "p18f2220.h" 
; # asm 


        LIST

;==========================================================================
;  MPASM PIC18F2220 processor include
; 
;  (c) Copyright 1999-2013 Microchip Technology, All rights reserved
;==========================================================================

        NOLIST

;==========================================================================
;  This header file defines configurations, registers, and other useful
;  bits of information for the PIC18F2220 microcontroller.  These names
;  are taken to match the data sheets as closely as possible.
;
;  Note that the processor must be selected before this file is included.
;  The processor may be selected the following ways:
;
;       1. Command line switch:
;               C:\MPASM MYFILE.ASM /PIC18F2220
;       2. LIST directive in the source file
               LIST   P=PIC18F2220
;       3. Processor Type entry in the MPASM full-screen interface
;       4. Setting the processor in the MPLAB Project Dialog
        NOLIST
;==========================================================================
        
;==========================================================================
;
;       Verify Processor
;
;==========================================================================
        IFNDEF __18F2220
           MESSG "Processor-header file mismatch.  Verify selected processor."
        ENDIF

;==========================================================================
;       18xxxx Family        EQUates
;==========================================================================
FSR0            EQU 0
FSR1            EQU 1
FSR2            EQU 2

FAST            EQU 1

W               EQU 0
A               EQU 0
ACCESS          EQU 0
BANKED          EQU 1
;==========================================================================

;==========================================================================
;       16Cxxx/17Cxxx Substitutions
;==========================================================================
  #define DDRA  TRISA      ; PIC17Cxxx SFR substitution
  #define DDRB  TRISB      ; PIC17Cxxx SFR substitution
  #define DDRC  TRISC      ; PIC17Cxxx SFR substitution
  #define DDRD  TRISD      ; PIC17Cxxx SFR substitution
  #define DDRE  TRISE      ; PIC17Cxxx SFR substitution

;==========================================================================
;
;       Register Definitions
;
;==========================================================================

;----- Register Files -----------------------------------------------------
PORTA            EQU  H'0F80'
PORTB            EQU  H'0F81'
PORTC            EQU  H'0F82'
LATA             EQU  H'0F89'
LATB             EQU  H'0F8A'
LATC             EQU  H'0F8B'
DDRA             EQU  H'0F92'
TRISA            EQU  H'0F92'
DDRB             EQU  H'0F93'
TRISB            EQU  H'0F93'
DDRC             EQU  H'0F94'
TRISC            EQU  H'0F94'
OSCTUN2          EQU  H'0F9A'
OSCTUNE          EQU  H'0F9B'
PIE1             EQU  H'0F9D'
PIR1             EQU  H'0F9E'
IPR1             EQU  H'0F9F'
PIE2             EQU  H'0FA0'
PIR2             EQU  H'0FA1'
IPR2             EQU  H'0FA2'
EECON1           EQU  H'0FA6'
EECON2           EQU  H'0FA7'
EEDATA           EQU  H'0FA8'
EEADR            EQU  H'0FA9'
RCSTA            EQU  H'0FAB'
TXSTA            EQU  H'0FAC'
TXREG            EQU  H'0FAD'
RCREG            EQU  H'0FAE'
SPBRG            EQU  H'0FAF'
T3CON            EQU  H'0FB1'
TMR3             EQU  H'0FB2'
TMR3L            EQU  H'0FB2'
TMR3H            EQU  H'0FB3'
CMCON            EQU  H'0FB4'
CVRCON           EQU  H'0FB5'
CCP2CON          EQU  H'0FBA'
CCPR2            EQU  H'0FBB'
CCPR2L           EQU  H'0FBB'
CCPR2H           EQU  H'0FBC'
CCP1CON          EQU  H'0FBD'
CCPR1            EQU  H'0FBE'
CCPR1L           EQU  H'0FBE'
CCPR1H           EQU  H'0FBF'
ADCON2           EQU  H'0FC0'
ADCON1           EQU  H'0FC1'
ADCON0           EQU  H'0FC2'
ADRES            EQU  H'0FC3'
ADRESL           EQU  H'0FC3'
ADRESH           EQU  H'0FC4'
SSPCON2          EQU  H'0FC5'
SSPCON1          EQU  H'0FC6'
SSPSTAT          EQU  H'0FC7'
SSPADD           EQU  H'0FC8'
SSPBUF           EQU  H'0FC9'
T2CON            EQU  H'0FCA'
PR2              EQU  H'0FCB'
TMR2             EQU  H'0FCC'
T1CON            EQU  H'0FCD'
TMR1             EQU  H'0FCE'
TMR1L            EQU  H'0FCE'
TMR1H            EQU  H'0FCF'
RCON             EQU  H'0FD0'
WDTCON           EQU  H'0FD1'
LVDCON           EQU  H'0FD2'
OSCCON           EQU  H'0FD3'
T0CON            EQU  H'0FD5'
TMR0             EQU  H'0FD6'
TMR0L            EQU  H'0FD6'
TMR0H            EQU  H'0FD7'
STATUS           EQU  H'0FD8'
FSR2L            EQU  H'0FD9'
FSR2H            EQU  H'0FDA'
PLUSW2           EQU  H'0FDB'
PREINC2          EQU  H'0FDC'
POSTDEC2         EQU  H'0FDD'
POSTINC2         EQU  H'0FDE'
INDF2            EQU  H'0FDF'
BSR              EQU  H'0FE0'
FSR1L            EQU  H'0FE1'
FSR1H            EQU  H'0FE2'
PLUSW1           EQU  H'0FE3'
PREINC1          EQU  H'0FE4'
POSTDEC1         EQU  H'0FE5'
POSTINC1         EQU  H'0FE6'
INDF1            EQU  H'0FE7'
WREG             EQU  H'0FE8'
FSR0L            EQU  H'0FE9'
FSR            EQU  H'0FE9'
FSR0H            EQU  H'0FEA'
PLUSW0           EQU  H'0FEB'
PREINC0          EQU  H'0FEC'
POSTDEC0         EQU  H'0FED'
POSTINC0         EQU  H'0FEE'
INDF0            EQU  H'0FEF'
INDF            EQU  H'0FEF'
INTCON3          EQU  H'0FF0'
INTCON2          EQU  H'0FF1'
INTCON           EQU  H'0FF2'
PROD             EQU  H'0FF3'
PRODL            EQU  H'0FF3'
PRODH            EQU  H'0FF4'
TABLAT           EQU  H'0FF5'
TBLPTR           EQU  H'0FF6'
TBLPTRL          EQU  H'0FF6'
TBLPTRH          EQU  H'0FF7'
TBLPTRU          EQU  H'0FF8'
PC               EQU  H'0FF9'
PCL              EQU  H'0FF9'
PCLATH           EQU  H'0FFA'
PCLATU           EQU  H'0FFB'
STKPTR           EQU  H'0FFC'
TOS              EQU  H'0FFD'
TOSL             EQU  H'0FFD'
TOSH             EQU  H'0FFE'
TOSU             EQU  H'0FFF'

;----- PORTA Bits -----------------------------------------------------
RA0              EQU  H'0000'
RA1              EQU  H'0001'
RA2              EQU  H'0002'
RA3              EQU  H'0003'
RA4              EQU  H'0004'
RA5              EQU  H'0005'
RA6              EQU  H'0006'
RA7              EQU  H'0007'

AN0              EQU  H'0000'
AN1              EQU  H'0001'
AN2              EQU  H'0002'
AN3              EQU  H'0003'
T0CKI            EQU  H'0004'
AN4              EQU  H'0005'
CLKO             EQU  H'0006'
CLKI             EQU  H'0007'

VREFM            EQU  H'0002'
VREFP            EQU  H'0003'
SS               EQU  H'0005'
OSC2             EQU  H'0006'
OSC1             EQU  H'0007'

CVREF            EQU  H'0002'
NOT_SS           EQU  H'0005'

LVDIN            EQU  H'0005'

C1OUT_PORTA      EQU  H'0004'
C2OUT_PORTA      EQU  H'0005'


;----- PORTB Bits -----------------------------------------------------
RB0              EQU  H'0000'
RB1              EQU  H'0001'
RB2              EQU  H'0002'
RB3              EQU  H'0003'
RB4              EQU  H'0004'
RB5              EQU  H'0005'
RB6              EQU  H'0006'
RB7              EQU  H'0007'

INT0             EQU  H'0000'
INT1             EQU  H'0001'
INT2             EQU  H'0002'
CCP2_PORTB       EQU  H'0003'
KBI0             EQU  H'0004'
KBI1             EQU  H'0005'
KBI2             EQU  H'0006'
KBI3             EQU  H'0007'

AN12             EQU  H'0000'
AN10             EQU  H'0001'
AN8              EQU  H'0002'
AN9              EQU  H'0003'
AN11             EQU  H'0004'
PGM              EQU  H'0005'
PGC              EQU  H'0006'
PGD              EQU  H'0007'

CCP2A            EQU  H'0003'


;----- PORTC Bits -----------------------------------------------------
RC0              EQU  H'0000'
RC1              EQU  H'0001'
RC2              EQU  H'0002'
RC3              EQU  H'0003'
RC4              EQU  H'0004'
RC5              EQU  H'0005'
RC6              EQU  H'0006'
RC7              EQU  H'0007'

T1OSO            EQU  H'0000'
T1OSI            EQU  H'0001'
CCP1             EQU  H'0002'
SCK              EQU  H'0003'
SDI              EQU  H'0004'
SDO              EQU  H'0005'
TX               EQU  H'0006'
RX               EQU  H'0007'

T13CKI           EQU  H'0000'
CCP2_PORTC       EQU  H'0001'
P1A              EQU  H'0002'
SCL              EQU  H'0003'
SDA              EQU  H'0004'
CK               EQU  H'0006'
; DT is a reserved word
; DT               EQU  H'0007'

T1CKI            EQU  H'0000'


;----- LATA Bits -----------------------------------------------------
LATA0            EQU  H'0000'
LATA1            EQU  H'0001'
LATA2            EQU  H'0002'
LATA3            EQU  H'0003'
LATA4            EQU  H'0004'
LATA5            EQU  H'0005'
LATA6            EQU  H'0006'
LATA7            EQU  H'0007'


;----- LATB Bits -----------------------------------------------------
LATB0            EQU  H'0000'
LATB1            EQU  H'0001'
LATB2            EQU  H'0002'
LATB3            EQU  H'0003'
LATB4            EQU  H'0004'
LATB5            EQU  H'0005'
LATB6            EQU  H'0006'
LATB7            EQU  H'0007'


;----- LATC Bits -----------------------------------------------------
LATC0            EQU  H'0000'
LATC1            EQU  H'0001'
LATC2            EQU  H'0002'
LATC3            EQU  H'0003'
LATC4            EQU  H'0004'
LATC5            EQU  H'0005'
LATC6            EQU  H'0006'
LATC7            EQU  H'0007'


;----- DDRA Bits -----------------------------------------------------
TRISA0           EQU  H'0000'
TRISA1           EQU  H'0001'
TRISA2           EQU  H'0002'
TRISA3           EQU  H'0003'
TRISA4           EQU  H'0004'
TRISA5           EQU  H'0005'
TRISA6           EQU  H'0006'
TRISA7           EQU  H'0007'

RA0              EQU  H'0000'
RA1              EQU  H'0001'
RA2              EQU  H'0002'
RA3              EQU  H'0003'
RA4              EQU  H'0004'
RA5              EQU  H'0005'
RA6              EQU  H'0006'
RA7              EQU  H'0007'


;----- TRISA Bits -----------------------------------------------------
TRISA0           EQU  H'0000'
TRISA1           EQU  H'0001'
TRISA2           EQU  H'0002'
TRISA3           EQU  H'0003'
TRISA4           EQU  H'0004'
TRISA5           EQU  H'0005'
TRISA6           EQU  H'0006'
TRISA7           EQU  H'0007'

RA0              EQU  H'0000'
RA1              EQU  H'0001'
RA2              EQU  H'0002'
RA3              EQU  H'0003'
RA4              EQU  H'0004'
RA5              EQU  H'0005'
RA6              EQU  H'0006'
RA7              EQU  H'0007'


;----- DDRB Bits -----------------------------------------------------
TRISB0           EQU  H'0000'
TRISB1           EQU  H'0001'
TRISB2           EQU  H'0002'
TRISB3           EQU  H'0003'
TRISB4           EQU  H'0004'
TRISB5           EQU  H'0005'
TRISB6           EQU  H'0006'
TRISB7           EQU  H'0007'

RB0              EQU  H'0000'
RB1              EQU  H'0001'
RB2              EQU  H'0002'
RB3              EQU  H'0003'
RB4              EQU  H'0004'
RB5              EQU  H'0005'
RB6              EQU  H'0006'
RB7              EQU  H'0007'


;----- TRISB Bits -----------------------------------------------------
TRISB0           EQU  H'0000'
TRISB1           EQU  H'0001'
TRISB2           EQU  H'0002'
TRISB3           EQU  H'0003'
TRISB4           EQU  H'0004'
TRISB5           EQU  H'0005'
TRISB6           EQU  H'0006'
TRISB7           EQU  H'0007'

RB0              EQU  H'0000'
RB1              EQU  H'0001'
RB2              EQU  H'0002'
RB3              EQU  H'0003'
RB4              EQU  H'0004'
RB5              EQU  H'0005'
RB6              EQU  H'0006'
RB7              EQU  H'0007'


;----- DDRC Bits -----------------------------------------------------
TRISC0           EQU  H'0000'
TRISC1           EQU  H'0001'
TRISC2           EQU  H'0002'
TRISC3           EQU  H'0003'
TRISC4           EQU  H'0004'
TRISC5           EQU  H'0005'
TRISC6           EQU  H'0006'
TRISC7           EQU  H'0007'

RC0              EQU  H'0000'
RC1              EQU  H'0001'
RC2              EQU  H'0002'
RC3              EQU  H'0003'
RC4              EQU  H'0004'
RC5              EQU  H'0005'
RC6              EQU  H'0006'
RC7              EQU  H'0007'


;----- TRISC Bits -----------------------------------------------------
TRISC0           EQU  H'0000'
TRISC1           EQU  H'0001'
TRISC2           EQU  H'0002'
TRISC3           EQU  H'0003'
TRISC4           EQU  H'0004'
TRISC5           EQU  H'0005'
TRISC6           EQU  H'0006'
TRISC7           EQU  H'0007'

RC0              EQU  H'0000'
RC1              EQU  H'0001'
RC2              EQU  H'0002'
RC3              EQU  H'0003'
RC4              EQU  H'0004'
RC5              EQU  H'0005'
RC6              EQU  H'0006'
RC7              EQU  H'0007'


;----- OSCTUN2 Bits -----------------------------------------------------
TUNSEL           EQU  H'0007'

TUN0             EQU  H'0000'
TUN1             EQU  H'0001'
TUN2             EQU  H'0002'
TUN3             EQU  H'0003'
TUN4             EQU  H'0004'
TUN5             EQU  H'0005'


;----- OSCTUNE Bits -----------------------------------------------------
TUN0             EQU  H'0000'
TUN1             EQU  H'0001'
TUN2             EQU  H'0002'
TUN3             EQU  H'0003'
TUN4             EQU  H'0004'
TUN5             EQU  H'0005'


;----- PIE1 Bits -----------------------------------------------------
TMR1IE           EQU  H'0000'
TMR2IE           EQU  H'0001'
CCP1IE           EQU  H'0002'
SSPIE            EQU  H'0003'
TXIE             EQU  H'0004'
RCIE             EQU  H'0005'
ADIE             EQU  H'0006'


;----- PIR1 Bits -----------------------------------------------------
TMR1IF           EQU  H'0000'
TMR2IF           EQU  H'0001'
CCP1IF           EQU  H'0002'
SSPIF            EQU  H'0003'
TXIF             EQU  H'0004'
RCIF             EQU  H'0005'
ADIF             EQU  H'0006'


;----- IPR1 Bits -----------------------------------------------------
TMR1IP           EQU  H'0000'
TMR2IP           EQU  H'0001'
CCP1IP           EQU  H'0002'
SSPIP            EQU  H'0003'
TXIP             EQU  H'0004'
RCIP             EQU  H'0005'
ADIP             EQU  H'0006'


;----- PIE2 Bits -----------------------------------------------------
CCP2IE           EQU  H'0000'
TMR3IE           EQU  H'0001'
LVDIE            EQU  H'0002'
BCLIE            EQU  H'0003'
EEIE             EQU  H'0004'
CMIE             EQU  H'0006'
OSCFIE           EQU  H'0007'


;----- PIR2 Bits -----------------------------------------------------
CCP2IF           EQU  H'0000'
TMR3IF           EQU  H'0001'
LVDIF            EQU  H'0002'
BCLIF            EQU  H'0003'
EEIF             EQU  H'0004'
CMIF             EQU  H'0006'
OSCFIF           EQU  H'0007'


;----- IPR2 Bits -----------------------------------------------------
CCP2IP           EQU  H'0000'
TMR3IP           EQU  H'0001'
LVDIP            EQU  H'0002'
BCLIP            EQU  H'0003'
EEIP             EQU  H'0004'
CMIP             EQU  H'0006'
OSCFIP           EQU  H'0007'


;----- EECON1 Bits -----------------------------------------------------
RD               EQU  H'0000'
WR               EQU  H'0001'
WREN             EQU  H'0002'
WRERR            EQU  H'0003'
FREE             EQU  H'0004'
CFGS             EQU  H'0006'
EEPGD            EQU  H'0007'


;----- RCSTA Bits -----------------------------------------------------
RX9D             EQU  H'0000'
OERR             EQU  H'0001'
FERR             EQU  H'0002'
ADDEN            EQU  H'0003'
CREN             EQU  H'0004'
SREN             EQU  H'0005'
RX9              EQU  H'0006'
SPEN             EQU  H'0007'

ADEN             EQU  H'0003'


;----- TXSTA Bits -----------------------------------------------------
TX9D             EQU  H'0000'
TRMT             EQU  H'0001'
BRGH             EQU  H'0002'
SYNC             EQU  H'0004'
TXEN             EQU  H'0005'
TX9              EQU  H'0006'
CSRC             EQU  H'0007'


;----- T3CON Bits -----------------------------------------------------
TMR3ON           EQU  H'0000'
TMR3CS           EQU  H'0001'
NOT_T3SYNC       EQU  H'0002'
T3CCP1           EQU  H'0003'
T3CCP2           EQU  H'0006'
RD16             EQU  H'0007'

T3SYNC           EQU  H'0002'
T3CKPS0          EQU  H'0004'
T3CKPS1          EQU  H'0005'


;----- CMCON Bits -----------------------------------------------------
CIS              EQU  H'0003'
C1INV            EQU  H'0004'
C2INV            EQU  H'0005'
C1OUT_CMCON      EQU  H'0006'
C2OUT_CMCON      EQU  H'0007'

CM0              EQU  H'0000'
CM1              EQU  H'0001'
CM2              EQU  H'0002'


;----- CVRCON Bits -----------------------------------------------------
CVRR             EQU  H'0005'
CVROE            EQU  H'0006'
CVREN            EQU  H'0007'

CVR0             EQU  H'0000'
CVR1             EQU  H'0001'
CVR2             EQU  H'0002'
CVR3             EQU  H'0003'


;----- CCP2CON Bits -----------------------------------------------------
CCP2M0           EQU  H'0000'
CCP2M1           EQU  H'0001'
CCP2M2           EQU  H'0002'
CCP2M3           EQU  H'0003'
DC2B0            EQU  H'0004'
DC2B1            EQU  H'0005'

CCP2Y            EQU  H'0004'
CCP2X            EQU  H'0005'

DCP2Y            EQU  H'0004'
DCP2X            EQU  H'0005'


;----- CCP1CON Bits -----------------------------------------------------
CCP1M0           EQU  H'0000'
CCP1M1           EQU  H'0001'
CCP1M2           EQU  H'0002'
CCP1M3           EQU  H'0003'
DC1B0            EQU  H'0004'
DC1B1            EQU  H'0005'

CCP1Y            EQU  H'0004'
CCP1X            EQU  H'0005'

DCP1Y            EQU  H'0004'
DCP1X            EQU  H'0005'


;----- ADCON2 Bits -----------------------------------------------------
ADFM             EQU  H'0007'

ADCS0            EQU  H'0000'
ADCS1            EQU  H'0001'
ADCS2            EQU  H'0002'
ACQT0            EQU  H'0003'
ACQT1            EQU  H'0004'
ACQT2            EQU  H'0005'


;----- ADCON1 Bits -----------------------------------------------------
PCFG0            EQU  H'0000'
PCFG1            EQU  H'0001'
PCFG2            EQU  H'0002'
PCFG3            EQU  H'0003'
VCFG0            EQU  H'0004'
VCFG1            EQU  H'0005'


;----- ADCON0 Bits -----------------------------------------------------
ADON             EQU  H'0000'
GO_NOT_DONE      EQU  H'0001'

GO               EQU  H'0001'
CHS0             EQU  H'0002'
CHS1             EQU  H'0003'
CHS2             EQU  H'0004'
CHS3             EQU  H'0005'

DONE             EQU  H'0001'

NOT_DONE         EQU  H'0001'

GO_DONE          EQU  H'0001'


;----- SSPCON2 Bits -----------------------------------------------------
SEN              EQU  H'0000'
RSEN             EQU  H'0001'
PEN              EQU  H'0002'
RCEN             EQU  H'0003'
ACKEN            EQU  H'0004'
ACKDT            EQU  H'0005'
ACKSTAT          EQU  H'0006'
GCEN             EQU  H'0007'


;----- SSPCON1 Bits -----------------------------------------------------
CKP              EQU  H'0004'
SSPEN            EQU  H'0005'
SSPOV            EQU  H'0006'
WCOL             EQU  H'0007'

SSPM0            EQU  H'0000'
SSPM1            EQU  H'0001'
SSPM2            EQU  H'0002'
SSPM3            EQU  H'0003'


;----- SSPSTAT Bits -----------------------------------------------------
BF               EQU  H'0000'
UA               EQU  H'0001'
R_NOT_W          EQU  H'0002'
S                EQU  H'0003'
P                EQU  H'0004'
D_NOT_A          EQU  H'0005'
CKE              EQU  H'0006'
SMP              EQU  H'0007'

I2C_READ         EQU  H'0002'
I2C_START        EQU  H'0003'
I2C_STOP         EQU  H'0004'
I2C_DATA         EQU  H'0005'

R                EQU  H'0002'
D                EQU  H'0005'

READ_WRITE       EQU  H'0002'
DATA_ADDRESS     EQU  H'0005'

NOT_WRITE        EQU  H'0002'
NOT_ADDRESS      EQU  H'0005'

NOT_W            EQU  H'0002'
NOT_A            EQU  H'0005'

R_W              EQU  H'0002'
D_A              EQU  H'0005'

I2C_DAT          EQU  H'0005'


;----- T2CON Bits -----------------------------------------------------
TMR2ON           EQU  H'0002'

T2CKPS0          EQU  H'0000'
T2CKPS1          EQU  H'0001'
TOUTPS0          EQU  H'0003'
TOUTPS1          EQU  H'0004'
TOUTPS2          EQU  H'0005'
TOUTPS3          EQU  H'0006'


;----- T1CON Bits -----------------------------------------------------
TMR1ON           EQU  H'0000'
TMR1CS           EQU  H'0001'
NOT_T1SYNC       EQU  H'0002'
T1OSCEN          EQU  H'0003'
T1RUN            EQU  H'0006'
RD16             EQU  H'0007'

T1SYNC           EQU  H'0002'
T1CKPS0          EQU  H'0004'
T1CKPS1          EQU  H'0005'


;----- RCON Bits -----------------------------------------------------
NOT_BOR          EQU  H'0000'
NOT_POR          EQU  H'0001'
NOT_PD           EQU  H'0002'
NOT_TO           EQU  H'0003'
NOT_RI           EQU  H'0004'
IPEN             EQU  H'0007'

BOR              EQU  H'0000'
POR              EQU  H'0001'
PD               EQU  H'0002'
TO               EQU  H'0003'
RI               EQU  H'0004'


;----- WDTCON Bits -----------------------------------------------------
SWDTEN           EQU  H'0000'

SWDTE            EQU  H'0000'


;----- LVDCON Bits -----------------------------------------------------
LVDEN            EQU  H'0004'
IRVST            EQU  H'0005'

LVDL0            EQU  H'0000'
LVDL1            EQU  H'0001'
LVDL2            EQU  H'0002'
LVDL3            EQU  H'0003'
IVRST            EQU  H'0005'


;----- OSCCON Bits -----------------------------------------------------
SCS0             EQU  H'0000'
SCS1             EQU  H'0001'
IOFS             EQU  H'0002'
OSTS             EQU  H'0003'
IDLEN            EQU  H'0007'

SCS              EQU  H'0000'
FLTS             EQU  H'0002'
IRCF0            EQU  H'0004'
IRCF1            EQU  H'0005'
IRCF2            EQU  H'0006'


;----- T0CON Bits -----------------------------------------------------
PSA              EQU  H'0003'
T0SE             EQU  H'0004'
T0CS             EQU  H'0005'
T08BIT           EQU  H'0006'
TMR0ON           EQU  H'0007'

T0PS0            EQU  H'0000'
T0PS1            EQU  H'0001'
T0PS2            EQU  H'0002'
T0PS3            EQU  H'0003'
T016BIT          EQU  H'0006'


;----- STATUS Bits -----------------------------------------------------
C                EQU  H'0000'
DC               EQU  H'0001'
Z                EQU  H'0002'
OV               EQU  H'0003'
N                EQU  H'0004'


;----- INTCON3 Bits -----------------------------------------------------
INT1IF           EQU  H'0000'
INT2IF           EQU  H'0001'
INT1IE           EQU  H'0003'
INT2IE           EQU  H'0004'
INT1IP           EQU  H'0006'
INT2IP           EQU  H'0007'

INT1F            EQU  H'0000'
INT2F            EQU  H'0001'
INT1E            EQU  H'0003'
INT2E            EQU  H'0004'
INT1P            EQU  H'0006'
INT2P            EQU  H'0007'


;----- INTCON2 Bits -----------------------------------------------------
RBIP             EQU  H'0000'
TMR0IP           EQU  H'0002'
INTEDG2          EQU  H'0004'
INTEDG1          EQU  H'0005'
INTEDG0          EQU  H'0006'
NOT_RBPU         EQU  H'0007'

T0IP             EQU  H'0002'
RBPU             EQU  H'0007'


;----- INTCON Bits -----------------------------------------------------
RBIF             EQU  H'0000'
INT0IF           EQU  H'0001'
TMR0IF           EQU  H'0002'
RBIE             EQU  H'0003'
INT0IE           EQU  H'0004'
TMR0IE           EQU  H'0005'
PEIE_GIEL        EQU  H'0006'
GIE_GIEH         EQU  H'0007'

INT0F            EQU  H'0001'
T0IF             EQU  H'0002'
INT0E            EQU  H'0004'
T0IE             EQU  H'0005'
PEIE             EQU  H'0006'
GIE              EQU  H'0007'

INTF             EQU  H'0001'
INTE             EQU  H'0004'
GIEL             EQU  H'0006'
GIEH             EQU  H'0007'


;----- STKPTR Bits -----------------------------------------------------
STKUNF           EQU  H'0006'
STKFUL           EQU  H'0007'

STKPTR0          EQU  H'0000'
STKPTR1          EQU  H'0001'
STKPTR2          EQU  H'0002'
STKPTR3          EQU  H'0003'
STKPTR4          EQU  H'0004'
STKOVF           EQU  H'0007'

SP0              EQU  H'0000'
SP1              EQU  H'0001'
SP2              EQU  H'0002'
SP3              EQU  H'0003'
SP4              EQU  H'0004'



;==========================================================================
;
;       RAM Definitions
;
;==========================================================================
       __MAXRAM  H'0FFF'
       __BADRAM  H'0200'-H'0F7F'
       __BADRAM  H'0F83'-H'0F88'
       __BADRAM  H'0F8C'-H'0F91'
       __BADRAM  H'0F95'-H'0F99'
       __BADRAM  H'0F9C'
       __BADRAM  H'0FA3'-H'0FA5'
       __BADRAM  H'0FAA'
       __BADRAM  H'0FB0'
       __BADRAM  H'0FB6'-H'0FB9'
       __BADRAM  H'0FD4'

;==========================================================================
;
;   IMPORTANT: For the PIC18 devices, the __CONFIG directive has been
;              superseded by the CONFIG directive.  The following settings
;              are available for this device.
;
;   Oscillator Selection bits:
;     OSC = LP             LP Oscillator
;     OSC = XT             XT Oscillator
;     OSC = HS             HS Oscillator
;     OSC = EC             EC oscillator, CLKO function on RA6
;     OSC = ECIO           EC oscillator, port function on RA6
;     OSC = HSPLL          HS oscillator, PLL enabled (clock frequency = 4 x FOSC1)
;     OSC = RCIO           External RC oscillator, port function on RA6
      CONFIG     OSC = INTIO2        ; Internal RC oscillator, port function on RA6 and port function on RA7
;       CONFIG OSC = INTIO1        ; Internal RC oscillator, CLKO function on RA6 and port function on RA7
;     OSC = RC             External RC oscillator, CLKO function on RA6
;
;   Fail-Safe Clock Monitor Enable bit:
;     FSCM = OFF           Fail-Safe Clock Monitor disabled
;     FSCM = ON            Fail-Safe Clock Monitor enabled
;
;   Internal/External Switchover bit:
;     IESO = OFF           Internal/External Switchover mode disabled
;     IESO = ON            Internal/External Switchover mode enabled
;
;   Power-up Timer enable bit:
     CONFIG     PWRT = ON           ; PWRT enabled
;     PWRT = OFF           PWRT disabled
;
;   Brown-out Reset enable bit:
      CONFIG     BOR = OFF           ; Brown-out Reset disabled
;     BOR = ON             Brown-out Reset enabled
;
;   Brown-out Reset Voltage bits:
;     BORV = 45            VBOR set to 4.5V
;     BORV = 42            VBOR set to 4.2V
;     BORV = 27            VBOR set to 2.7V
;     BORV = 20            VBOR set to 2.0V
;
;   Watchdog Timer Enable bit:
      CONFIG     WDT = OFF           ; WDT disabled (control is placed on the SWDTEN bit)
;     WDT = ON             WDT enabled
;
;   Watchdog Timer Postscale Select bits:
;     WDTPS = 1            1:1
;     WDTPS = 2            1:2
;     WDTPS = 4            1:4
;     WDTPS = 8            1:8
;     WDTPS = 16           1:16
;     WDTPS = 32           1:32
;     WDTPS = 64           1:64
;     WDTPS = 128          1:128
;     WDTPS = 256          1:256
;     WDTPS = 512          1:512
;     WDTPS = 1024         1:1024
;     WDTPS = 2048         1:2048
;     WDTPS = 4096         1:4096
;     WDTPS = 8192         1:8192
;     WDTPS = 16384        1:16384
;     WDTPS = 32768        1:32768
;
;   CCP2 MUX bit:
;     CCP2MX = OFF         CCP2 input/output is multiplexed with RB3
;     CCP2MX = ON          CCP2 input/output is multiplexed with RC1
;
;   PORTB A/D Enable bit:
    CONFIG     PBAD = DIG          ; PORTB<4:0> pins are configured as digital I/O on Reset
;     PBAD = ANA           PORTB<4:0> pins are configured as analog input channels on Reset
;
;   MCLR Pin Enable bit:
;     MCLRE = OFF          MCLR disabled; RE3 input is enabled in 40-pin devices only (PIC18F4X20)
      CONFIG     MCLRE = ON          ; MCLR pin enabled; RE3 input pin disabled
;
;   Stack Full/Underflow Reset Enable bit:
;     STVR = OFF           Stack full/underflow will not cause Reset
;     STVR = ON            Stack full/underflow will cause Reset
;
;   Single-Supply ICSP Enable bit:
      CONFIG    LVP = OFF            ;Single-Supply ICSP disabled
;     LVP = ON             Single-Supply ICSP enabled
;
;   Background Debugger Enable bit:
;     DEBUG = ON           Background debugger enabled, RB6 and RB7 are dedicated to In-Circuit Debug
      CONFIG     DEBUG = OFF         ; Background debugger disabled, RB6 and RB7 configured as general purpose I/O pins
;
;   Code Protection bit:
;     CP0 = ON             Block 0 (000200-0007FFh) code-protected
      CONFIG     CP0 = OFF            ;Block 0 (000200-0007FFh) not code-protected
;
;   Code Protection bit:
;     CP1 = ON             Block 1 (000800-000FFFh) code-protected
      CONFIG     CP1 = OFF           ; Block 1 (000800-000FFFh) not code-protected
;
;   Boot Block Code Protection bit:
;     CPB = ON             Boot block (000000-0001FFh) is code-protected
      CONFIG     CPB = OFF           ; Boot block (000000-0001FFh) is not code-protected
;
;   Data EEPROM Code Protection bit:
;     CPD = ON             Data EEPROM is code-protected
      CONFIG     CPD = OFF           ; Data EEPROM is not code-protected
;
;   Write Protection bit:
;     WRT0 = ON            Block 0 (000200-0007FFh) write-protected
      CONFIG     WRT0 = OFF         ;  Block 0 (000200-0007FFh) not write-protected
;
;   Write Protection bit:
;     WRT1 = ON            Block 1 (000800-000FFFh) write-protected
      CONFIG     WRT1 = OFF         ;  Block 1 (000800-000FFFh) not write-protected
;
;   Configuration Register Write Protection bit:
;     WRTC = ON            Configuration registers (300000-3000FFh) are write-protected
      CONFIG     WRTC = OFF         ;  Configuration registers (300000-3000FFh) are not write-protected
;
;   Boot Block Write Protection bit:
;     WRTB = ON            Boot block (000000-0001FFh) is write-protected
      CONFIG     WRTB = OFF          ; Boot block (000000-0001FFh) is not write-protected
;
;   Data EEPROM Write Protection bit:
;     WRTD = ON            Data EEPROM is write-protected
      CONFIG     WRTD = OFF          ; Data EEPROM is not write-protected
;
;   Table Read Protection bit:
;     EBTR0 = ON           Block 0 (000200-0007FFh) protected from table reads executed in other blocks
      CONFIG     EBTR0 = OFF         ; Block 0 (000200-0007FFh) not protected from table reads executed in other blocks
;
;   Table Read Protection bit:
;     EBTR1 = ON           Block 1 (000800-000FFFh) protected from table reads executed in other blocks
      CONFIG    EBTR1 = OFF       ;   Block 1 (000800-000FFFh) not protected from table reads executed in other blocks
;
;   Boot Block Table Read Protection bit:
;     EBTRB = ON           Boot block (000000-0001FFh) is protected from table reads executed in other blocks
      CONFIG     EBTRB = OFF         ; Boot block (000000-0001FFh) is not protected from table reads executed in other blocks
;
;==========================================================================
;==========================================================================
;
;       Configuration Bits
;
;   NAME            Address
;   CONFIG1H        300001h
;   CONFIG2L        300002h
;   CONFIG2H        300003h
;   CONFIG3H        300005h
;   CONFIG4L        300006h
;   CONFIG5L        300008h
;   CONFIG5H        300009h
;   CONFIG6L        30000Ah
;   CONFIG6H        30000Bh
;   CONFIG7L        30000Ch
;   CONFIG7H        30000Dh
;
;==========================================================================

; The following is an assignment of address values for all of the
; configuration registers for the purpose of table reads
_CONFIG1H        EQU  H'300001'
_CONFIG2L        EQU  H'300002'
_CONFIG2H        EQU  H'300003'
_CONFIG3H        EQU  H'300005'
_CONFIG4L        EQU  H'300006'
_CONFIG5L        EQU  H'300008'
_CONFIG5H        EQU  H'300009'
_CONFIG6L        EQU  H'30000A'
_CONFIG6H        EQU  H'30000B'
_CONFIG7L        EQU  H'30000C'
_CONFIG7H        EQU  H'30000D'

;----- CONFIG1H Options --------------------------------------------------
_LP_OSC              EQU  H'F0'    ; LP Oscillator
_LP_OSC_1H           EQU  H'F0'    ; LP Oscillator
_XT_OSC              EQU  H'F1'    ; XT Oscillator
_XT_OSC_1H           EQU  H'F1'    ; XT Oscillator
_HS_OSC              EQU  H'F2'    ; HS Oscillator
_HS_OSC_1H           EQU  H'F2'    ; HS Oscillator
_EC_OSC              EQU  H'F4'    ; EC oscillator, CLKO function on RA6
_EC_OSC_1H           EQU  H'F4'    ; EC oscillator, CLKO function on RA6
_ECIOP_OSC           EQU  H'F5'    ; EC oscillator, port function on RA6
_ECIOP_OSC_1H        EQU  H'F5'    ; EC oscillator, port function on RA6
_ECIO_OSC_1H         EQU  H'F5'    ; EC oscillator, port function on RA6
_HSPLL_OSC           EQU  H'F6'    ; HS oscillator, PLL enabled (clock frequency = 4 x FOSC1)
_HSPLL_OSC_1H        EQU  H'F6'    ; HS oscillator, PLL enabled (clock frequency = 4 x FOSC1)
_RCIO_OSC            EQU  H'F7'    ; External RC oscillator, port function on RA6
_RCIO_OSC_1H         EQU  H'F7'    ; External RC oscillator, port function on RA6
_INTIO2_OSC          EQU  H'F8'    ; Internal RC oscillator, port function on RA6 and port function on RA7
_INTIO2_OSC_1H       EQU  H'F8'    ; Internal RC oscillator, port function on RA6 and port function on RA7
_INTIO1_OSC          EQU  H'F9'    ; Internal RC oscillator, CLKO function on RA6 and port function on RA7
_INTIO1_OSC_1H       EQU  H'F9'    ; Internal RC oscillator, CLKO function on RA6 and port function on RA7
_RC_OSC              EQU  H'FC'    ; External RC oscillator, CLKO function on RA6
_RC_OSC_1H           EQU  H'FC'    ; External RC oscillator, CLKO function on RA6

_FSCM_OFF_1H         EQU  H'BF'    ; Fail-Safe Clock Monitor disabled
_FSCM_ON_1H          EQU  H'FF'    ; Fail-Safe Clock Monitor enabled

_IESO_OFF_1H         EQU  H'7F'    ; Internal/External Switchover mode disabled
_IESO_ON_1H          EQU  H'FF'    ; Internal/External Switchover mode enabled

;----- CONFIG2L Options --------------------------------------------------
_PWRT_ON_2L          EQU  H'FE'    ; PWRT enabled
_PWRT_OFF_2L         EQU  H'FF'    ; PWRT disabled

_BOR_OFF_2L          EQU  H'FD'    ; Brown-out Reset disabled
_BOR_ON_2L           EQU  H'FF'    ; Brown-out Reset enabled

_BORV_45_2L          EQU  H'F3'    ; VBOR set to 4.5V
_BORV_42_2L          EQU  H'F7'    ; VBOR set to 4.2V
_BORV_27_2L          EQU  H'FB'    ; VBOR set to 2.7V
_BORV_20_2L          EQU  H'FF'    ; VBOR set to 2.0V

;----- CONFIG2H Options --------------------------------------------------
_WDT_OFF_2H          EQU  H'FE'    ; WDT disabled (control is placed on the SWDTEN bit)
_WDT_ON_2H           EQU  H'FF'    ; WDT enabled

_WDTPS_1_2H          EQU  H'E1'    ; 1:1
_WDTPS_2_2H          EQU  H'E3'    ; 1:2
_WDTPS_4_2H          EQU  H'E5'    ; 1:4
_WDTPS_8_2H          EQU  H'E7'    ; 1:8
_WDTPS_16_2H         EQU  H'E9'    ; 1:16
_WDTPS_32_2H         EQU  H'EB'    ; 1:32
_WDTPS_64_2H         EQU  H'ED'    ; 1:64
_WDTPS_128_2H        EQU  H'EF'    ; 1:128
_WDTPS_256_2H        EQU  H'F1'    ; 1:256
_WDTPS_512_2H        EQU  H'F3'    ; 1:512
_1K                  EQU  H'F5'    ; 1:1024
_WDTPS_1K_2H         EQU  H'F5'    ; 1:1024
_2K                  EQU  H'F7'    ; 1:2048
_WDTPS_2K_2H         EQU  H'F7'    ; 1:2048
_4K                  EQU  H'F9'    ; 1:4096
_WDTPS_4K_2H         EQU  H'F9'    ; 1:4096
_8K                  EQU  H'FB'    ; 1:8192
_WDTPS_8K_2H         EQU  H'FB'    ; 1:8192
_16K                 EQU  H'FD'    ; 1:16384
_WDTPS_16K_2H        EQU  H'FD'    ; 1:16384
_32K                 EQU  H'FF'    ; 1:32768
_WDTPS_32K_2H        EQU  H'FF'    ; 1:32768

;----- CONFIG3H Options --------------------------------------------------
_CCP2MX_OFF          EQU  H'FE'    ; CCP2 input/output is multiplexed with RB3
_CCP2MX_OFF_3H       EQU  H'FE'    ; CCP2 input/output is multiplexed with RB3
_CCP2MX_B3           EQU  H'FE'    ; CCP2 input/output is multiplexed with RB3
_CCP2MX_B3_3H        EQU  H'FE'    ; CCP2 input/output is multiplexed with RB3
_CCP2MX_ON           EQU  H'FF'    ; CCP2 input/output is multiplexed with RC1
_CCP2MX_ON_3H        EQU  H'FF'    ; CCP2 input/output is multiplexed with RC1
_CCP2MX_C1           EQU  H'FF'    ; CCP2 input/output is multiplexed with RC1
_CCP2MX_C1_3H        EQU  H'FF'    ; CCP2 input/output is multiplexed with RC1

_PBAD_DIG_3H         EQU  H'FD'    ; PORTB<4:0> pins are configured as digital I/O on Reset
_PBAD_ANA_3H         EQU  H'FF'    ; PORTB<4:0> pins are configured as analog input channels on Reset

_MCLRE_OFF_3H        EQU  H'7F'    ; MCLR disabled; RE3 input is enabled in 40-pin devices only (PIC18F4X20)
_MCLRE_ON_3H         EQU  H'FF'    ; MCLR pin enabled; RE3 input pin disabled

;----- CONFIG4L Options --------------------------------------------------
_STVR_OFF_4L         EQU  H'FE'    ; Stack full/underflow will not cause Reset
_STVR_ON_4L          EQU  H'FF'    ; Stack full/underflow will cause Reset

_LVP_OFF_4L          EQU  H'FB'    ; Single-Supply ICSP disabled
_LVP_ON_4L           EQU  H'FF'    ; Single-Supply ICSP enabled

_DEBUG_ON_4L         EQU  H'7F'    ; Background debugger enabled, RB6 and RB7 are dedicated to In-Circuit Debug
_DEBUG_OFF_4L        EQU  H'FF'    ; Background debugger disabled, RB6 and RB7 configured as general purpose I/O pins

;----- CONFIG5L Options --------------------------------------------------
_CP0_ON_5L           EQU  H'FE'    ; Block 0 (000200-0007FFh) code-protected
_CP0_OFF_5L          EQU  H'FF'    ; Block 0 (000200-0007FFh) not code-protected

_CP1_ON_5L           EQU  H'FD'    ; Block 1 (000800-000FFFh) code-protected
_CP1_OFF_5L          EQU  H'FF'    ; Block 1 (000800-000FFFh) not code-protected

;----- CONFIG5H Options --------------------------------------------------
_CPB_ON_5H           EQU  H'BF'    ; Boot block (000000-0001FFh) is code-protected
_CPB_OFF_5H          EQU  H'FF'    ; Boot block (000000-0001FFh) is not code-protected

_CPD_ON_5H           EQU  H'7F'    ; Data EEPROM is code-protected
_CPD_OFF_5H          EQU  H'FF'    ; Data EEPROM is not code-protected

;----- CONFIG6L Options --------------------------------------------------
_WRT0_ON_6L          EQU  H'FE'    ; Block 0 (000200-0007FFh) write-protected
_WRT0_OFF_6L         EQU  H'FF'    ; Block 0 (000200-0007FFh) not write-protected

_WRT1_ON_6L          EQU  H'FD'    ; Block 1 (000800-000FFFh) write-protected
_WRT1_OFF_6L         EQU  H'FF'    ; Block 1 (000800-000FFFh) not write-protected

;----- CONFIG6H Options --------------------------------------------------
_WRTC_ON_6H          EQU  H'DF'    ; Configuration registers (300000-3000FFh) are write-protected
_WRTC_OFF_6H         EQU  H'FF'    ; Configuration registers (300000-3000FFh) are not write-protected

_WRTB_ON_6H          EQU  H'BF'    ; Boot block (000000-0001FFh) is write-protected
_WRTB_OFF_6H         EQU  H'FF'    ; Boot block (000000-0001FFh) is not write-protected

_WRTD_ON_6H          EQU  H'7F'    ; Data EEPROM is write-protected
_WRTD_OFF_6H         EQU  H'FF'    ; Data EEPROM is not write-protected

;----- CONFIG7L Options --------------------------------------------------
_EBTR0_ON_7L         EQU  H'FE'    ; Block 0 (000200-0007FFh) protected from table reads executed in other blocks
_EBTR0_OFF_7L        EQU  H'FF'    ; Block 0 (000200-0007FFh) not protected from table reads executed in other blocks

_EBTR1_ON_7L         EQU  H'FD'    ; Block 1 (000800-000FFFh) protected from table reads executed in other blocks
_EBTR1_OFF_7L        EQU  H'FF'    ; Block 1 (000800-000FFFh) not protected from table reads executed in other blocks

;----- CONFIG7H Options --------------------------------------------------
_EBTRB_ON_7H         EQU  H'BF'    ; Boot block (000000-0001FFh) is protected from table reads executed in other blocks
_EBTRB_OFF_7H        EQU  H'FF'    ; Boot block (000000-0001FFh) is not protected from table reads executed in other blocks


;----- DEVID Equates --------------------------------------------------
_DEVID1          EQU  H'3FFFFE'
_DEVID2          EQU  H'3FFFFF'

;----- IDLOC Equates --------------------------------------------------
_IDLOC0          EQU  H'200000'
_IDLOC1          EQU  H'200001'
_IDLOC2          EQU  H'200002'
_IDLOC3          EQU  H'200003'
_IDLOC4          EQU  H'200004'
_IDLOC5          EQU  H'200005'
_IDLOC6          EQU  H'200006'
_IDLOC7          EQU  H'200007'

        LIST


       ;declare constants used by the compiler
       ;list p = 18F2220
       radix  decimal
       
#define  rrf  rrcf
#define  rlf  rlcf

       ;set up vectors
       org   0          ; reset vector
       call  init
       goto _ml
       org   8
       goto  _interrupt  ; comment this out if you don't want interrupts
                        ; otherwise supply a _interrupt() C function
				; may want to use fast return
				; may want to use low priority vector at 0x18

_ml    call  main       ; loop
       goto  _ml


 ; -------------------- run time library  -------------------- */

        ; shifting, shift _temp with count in W
_rshift
        btfsc   STATUS,Z,A    
        return             ; ret if shift amount is zero
        bcf     STATUS,C,A   ; unsigned shift, mask bits
        rrcf     _temp,F
        addlw   255        ; sub one
        goto    _rshift

_lshift
        btfsc   STATUS,Z,A    
        return             ; ret if shift amount is zero
        bcf     STATUS,C,A   ; arithmetic shift
        rlcf     _temp,F
        addlw   255        ; sub one
        goto    _lshift

_eeadr                     ; set up EEDATA to contain desired data
        ;bcf     STATUS,RP0
        movwf   EEADR,A      ; correct address in W
        ;bsf     STATUS,RP0
        bsf     EECON1,RD,A
        ;bcf     STATUS,RP0
        return


; junk for compiler, define RP0
RP0     EQU     7  ; unused bit in status reg 

QMD1
; # pragma pic 3968 
; char PORTA ; 
	;data
PORTA	equ 3968
; char PORTB ; 
PORTB	equ 3969
; char PORTC ; 
PORTC	equ 3970
; char PORTD ; 
PORTD	equ 3971
; char PORTE ; 
PORTE	equ 3972
; # pragma pic 3977 
; char LATA ; 
LATA	equ 3977
; char LATB ; 
LATB	equ 3978
; char LATC ; 
LATC	equ 3979
; char LATD ; 
LATD	equ 3980
; char LATE ; 
LATE	equ 3981
; # pragma pic 3986 
; char TRISA ; 
TRISA	equ 3986
; char TRISB ; 
TRISB	equ 3987
; char TRISC ; 
TRISC	equ 3988
; char TRISD ; 
TRISD	equ 3989
; char TRISE ; 
TRISE	equ 3990
; # pragma pic 3995 
; char OSCTUNE ; 
OSCTUNE	equ 3995
; # pragma pic 3997 
; char PIE1 ; 
PIE1	equ 3997
; char PIR1 ; 
PIR1	equ 3998
; char IPR1 ; 
IPR1	equ 3999
; char PIE2 ; 
PIE2	equ 4000
; char PIR2 ; 
PIR2	equ 4001
; char IPR2 ; 
IPR2	equ 4002
; # pragma pic 4006 
; char EECON1 ; 
EECON1	equ 4006
; char EECON2 ; 
EECON2	equ 4007
; char EEDATA ; 
EEDATA	equ 4008
; char EEADR ; 
EEADR	equ 4009
; char BAUDCTL ; 
BAUDCTL	equ 4010
; char RCSTA ; 
RCSTA	equ 4011
; char TXSTA ; 
TXSTA	equ 4012
; char TXREG ; 
TXREG	equ 4013
; char RCREG ; 
RCREG	equ 4014
; char SPBRG ; 
SPBRG	equ 4015
; char SPBRGH ; 
SPBRGH	equ 4016
; char T3CON ; 
T3CON	equ 4017
; char TMR3L ; 
TMR3L	equ 4018
; char TMR3H ; 
TMR3H	equ 4019
; char CMCON ; 
CMCON	equ 4020
; char CVRCON ; 
CVRCON	equ 4021
; # pragma pic 4022 
; char ECCPAS ; 
ECCPAS	equ 4022
; char PWM1CON ; 
PWM1CON	equ 4023
; # pragma pic 4026 
; char CCP2CON ; 
CCP2CON	equ 4026
; char CCPR2L ; 
CCPR2L	equ 4027
; char CCPR2H ; 
CCPR2H	equ 4028
; char CCP1CON ; 
CCP1CON	equ 4029
; char CCPR1L ; 
CCPR1L	equ 4030
; char CCPR1H ; 
CCPR1H	equ 4031
; char ADCON2 ; 
ADCON2	equ 4032
; char ADCON1 ; 
ADCON1	equ 4033
; char ADCON0 ; 
ADCON0	equ 4034
; char ADRESL ; 
ADRESL	equ 4035
; char ADRESH ; 
ADRESH	equ 4036
; char SSPCON2 ; 
SSPCON2	equ 4037
; char SSPCON1 ; 
SSPCON1	equ 4038
; char SSPSTAT ; 
SSPSTAT	equ 4039
; char SSPADD ; 
SSPADD	equ 4040
; char SSPBUF ; 
SSPBUF	equ 4041
; # pragma pic 4042 
; char T2CON ; 
T2CON	equ 4042
; char PR2 ; 
PR2	equ 4043
; char TMR2 ; 
TMR2	equ 4044
; char T1CON ; 
T1CON	equ 4045
; char TMR1L ; 
TMR1L	equ 4046
; char TMR1H ; 
TMR1H	equ 4047
; char RCON ; 
RCON	equ 4048
; char WDTCON ; 
WDTCON	equ 4049
; char LVDCON ; 
LVDCON	equ 4050
; char OSCCON ; 
OSCCON	equ 4051
; # pragma pic 4053 
; char T0CON ; 
T0CON	equ 4053
; char TMR0L ; 
TMR0L	equ 4054
; char TMR0H ; 
TMR0H	equ 4055
; char STATUS ; 
STATUS	equ 4056
; char FSR2L ; 
FSR2L	equ 4057
; char FSR2H ; 
FSR2H	equ 4058
; char PLUSW2 ; 
PLUSW2	equ 4059
; char PREINC2 ; 
PREINC2	equ 4060
; char POSTDEC2 ; 
POSTDEC2	equ 4061
; char POSTINC2 ; 
POSTINC2	equ 4062
; char INDF2 ; 
INDF2	equ 4063
; char BSR ; 
BSR	equ 4064
; char FSR1L ; 
FSR1L	equ 4065
; char FSR1H ; 
FSR1H	equ 4066
; char PLUSW1 ; 
PLUSW1	equ 4067
; char PREINC1 ; 
PREINC1	equ 4068
; char POSTDEC1 ; 
POSTDEC1	equ 4069
; char POSTINC1 ; 
POSTINC1	equ 4070
; char INDF1 ; 
INDF1	equ 4071
; char WREG ; 
WREG	equ 4072
; char FSR0L ; 
FSR0L	equ 4073
; char FSR0H ; 
FSR0H	equ 4074
; char PLUSW0 ; 
PLUSW0	equ 4075
; char PREINC0 ; 
PREINC0	equ 4076
; char POSTDEC0 ; 
POSTDEC0	equ 4077
; char POSTINC0 ; 
POSTINC0	equ 4078
; char INDF0 ; 
INDF0	equ 4079
; char INTCON3 ; 
INTCON3	equ 4080
; char INTCON2 ; 
INTCON2	equ 4081
; char INTCON ; 
INTCON	equ 4082
; char PRODL ; 
PRODL	equ 4083
; char PRODH ; 
PRODH	equ 4084
; char TABLAT ; 
TABLAT	equ 4085
; char TBLPTRL ; 
TBLPTRL	equ 4086
; char TBLPTRH ; 
TBLPTRH	equ 4087
; char TBLPTRU ; 
TBLPTRU	equ 4088
; char PCL ; 
PCL	equ 4089
; char PCLATH ; 
PCLATH	equ 4090
; char PCLATU ; 
PCLATU	equ 4091
; char STKPTR ; 
STKPTR	equ 4092
; char TOSL ; 
TOSL	equ 4093
; char TOSH ; 
TOSH	equ 4094
; char TOSU ; 
TOSU	equ 4095
; # pragma pic 0 
; # pragma bsr 
; # pragma eesize 256 
; # pragma banksize 256 
; # pragma access 0 127 
; # pragma access 3968 4095 
; # define B0 0x01 
; # define B1 0x02 
; # define B2 0x04 
; # define B3 0x08 
; # define B4 0x10 
; # define B5 0x20 
; # define B6 0x40 
; # define B7 0x80 
; # define ADRIGHT 0x80 
; # define ADLEFT 0x00 
; # define MATCH 10 
; # define QMATCH 13 
; # pragma pic 0 
; char _temp ; 
_temp	equ 0
; char _temp2 ; 
_temp2	equ 1
; char _eedata ; 
_eedata	equ 2
; char Cvals ; 
Cvals	equ 3
; char Lvals ; 
Lvals	equ 4
; char Hval ; 
Hval	equ 5
; char QC ; 
QC	equ 6
; char QL ; 
QL	equ 7
; char QH ; 
QH	equ 8
; char rx_buf [ 8 ] ; 
rx_buf	equ 9
; char rx_in ; 
rx_in	equ 17
; char rx_out ; 
rx_out	equ 18
; char tx_buf [ 8 ] ; 
tx_buf	equ 19
; char tx_in ; 
tx_in	equ 27
; char tx_out ; 
tx_out	equ 28
; char rstate ; 
rstate	equ 29
; char tstate ; 
tstate	equ 30
; char rtimer ; 
rtimer	equ 31
; char fwdh , fwdl ; 
fwdh	equ 32
fwdl	equ 33
; char revh , revl ; 
revh	equ 34
revl	equ 35
; char swr ; 
swr	equ 36
; char tlow , thigh ; 
tlow	equ 37
thigh	equ 38
; char accl ; 
accl	equ 39
; char acch ; 
acch	equ 40
; char argl ; 
argl	equ 41
; char argh ; 
argh	equ 42
; char overflow ; 
overflow	equ 43
; char carry ; 
carry	equ 44
; char divi ; 
divi	equ 45
; char divql ; 
divql	equ 46
; char divqh ; 
divqh	equ 47
; char Ct , Lt , Ht ; 
Ct	equ 48
Lt	equ 49
Ht	equ 50
; char swrq ; 
swrq	equ 51
; char Cdv , Ldv ; 
Cdv	equ 52
Ldv	equ 53
; char chg ; 
chg	equ 54
; # pragma pic 256 
; main ( ) { 
	;code
main
; static char temp ; 
	;data
QMD2	equ 256
; if ( rx_in != rx_out ) { 
	;code
	movf	rx_in,W
	movwf	_temp
	movf	rx_out,W
	subwf	_temp,W
	btfsc	STATUS,Z
	goto	QMD3
; temp = get_rx ( ) ; 
	call	get_rx
	BANKSEL	QMD2
	movwf	QMD2
; switch ( temp ) { 
	movf	QMD2,W
; case 'Z' : bypass ( ) ; break ; 
	addlw	166
	btfss	STATUS,Z
	goto	QMD6
QMD5
	call	bypass
	goto	QMD4
; case 'X' : write_quick ( ) ; break ; 
QMD6
	addlw	2
	btfss	STATUS,Z
	goto	QMD9
QMD8
	call	write_quick
	goto	QMD4
; case 'O' : write_vals ( ) ; break ; 
QMD9
	addlw	9
	btfss	STATUS,Z
	goto	QMD12
QMD11
	call	write_vals
	goto	QMD4
; case 'S' : set_vals ( ) ; break ; 
QMD12
	addlw	252
	btfss	STATUS,Z
	goto	QMD15
QMD14
	call	set_vals
	goto	QMD4
; case 'R' : report_vals ( ) ; break ; 
QMD15
	addlw	1
	btfss	STATUS,Z
	goto	QMD18
QMD17
	call	report_vals
	goto	QMD4
; case 'P' : report_power ( ) ; break ; 
QMD18
	addlw	2
	btfss	STATUS,Z
	goto	QMD21
QMD20
	call	report_power
	goto	QMD4
; case 'T' : tune ( ) ; break ; 
QMD21
	addlw	252
	btfss	STATUS,Z
	goto	QMD24
QMD23
	call	tune
	goto	QMD4
; case 'C' : ++ Cvals ; break ; 
QMD24
	addlw	17
	btfss	STATUS,Z
	goto	QMD27
QMD26
	incf	Cvals,F
	goto	QMD4
; case 'D' : -- Cvals ; break ; 
QMD27
	addlw	255
	btfss	STATUS,Z
	goto	QMD30
QMD29
	decf	Cvals,F
	goto	QMD4
; case 'L' : ++ Lvals ; break ; 
QMD30
	addlw	248
	btfss	STATUS,Z
	goto	QMD33
QMD32
	incf	Lvals,F
	goto	QMD4
; case 'M' : -- Lvals ; break ; 
QMD33
	addlw	255
	btfss	STATUS,Z
	goto	QMD36
QMD35
	decf	Lvals,F
	goto	QMD4
; case 'H' : Hval ^= 0x80  ; break ; 
QMD36
	addlw	5
	btfss	STATUS,Z
	goto	QMD39
QMD38
	movlw	128
	xorwf	Hval,F
	goto	QMD4
; case 'W' : 
QMD39
; : put_tx ( 'W' ) ; 
	addlw	241
	btfss	STATUS,Z
	goto	QMD42
QMD41
	movlw	87
	call	put_tx
; put_tx ( swr ) ; 
	movf	swr,W
	call	put_tx
; break ; 
	goto	QMD4
; } 
QMD42
QMD4
; } 
; } 
QMD3
	return
; } bypass ( ) { 
bypass
; static char tc ; 
	;data
QMD44	equ 257
; static char tl ; 
QMD45	equ 258
; static char h ; 
QMD46	equ 259
; tc = Cvals ; 
	;code
	movf	Cvals,W
	BANKSEL	QMD44
	movwf	QMD44
; tl = Lvals ; 
	movf	Lvals,W
	movwf	QMD45
; h = Hval ; 
	movf	Hval,W
	movwf	QMD46
; Hval = Cvals = Lvals = 0 ; 
	movlw	0
	movwf	Lvals
	movwf	Cvals
	movwf	Hval
; write_vals ( ) ; 
	call	write_vals
; Cvals = tc ; 
	BANKSEL	QMD44
	movf	QMD44,W
	movwf	Cvals
; Lvals = tl ; 
	movf	QMD45,W
	movwf	Lvals
; Hval = h ; 
	movf	QMD46,W
	movwf	Hval
	return
; } 
; write_quick ( ) { 
write_quick
; static char tc ; 
	;data
QMD47	equ 260
; static char tl ; 
QMD48	equ 261
; static char h ; 
QMD49	equ 262
; tc = Cvals ; 
	;code
	movf	Cvals,W
	BANKSEL	QMD47
	movwf	QMD47
; tl = Lvals ; 
	movf	Lvals,W
	movwf	QMD48
; h = Hval ; 
	movf	Hval,W
	movwf	QMD49
; Hval = QH ; 
	movf	QH,W
	movwf	Hval
; Cvals = QC ; 
	movf	QC,W
	movwf	Cvals
; Lvals = QL ; 
	movf	QL,W
	movwf	Lvals
; write_vals ( ) ; 
	call	write_vals
; Cvals = tc ; 
	BANKSEL	QMD47
	movf	QMD47,W
	movwf	Cvals
; Lvals = tl ; 
	movf	QMD48,W
	movwf	Lvals
; Hval = h ; 
	movf	QMD49,W
	movwf	Hval
	return
; } 
; set_vals ( ) { 
set_vals
; while ( rx_in == rx_out ) ; 
QMD50
	movf	rx_in,W
	movwf	_temp
	movf	rx_out,W
	subwf	_temp,W
	btfss	STATUS,Z
	goto	QMD51
	goto	QMD50
QMD51
; Cvals = get_rx ( ) ; 
	call	get_rx
	movwf	Cvals
; while ( rx_in == rx_out ) ; 
QMD52
	movf	rx_in,W
	movwf	_temp
	movf	rx_out,W
	subwf	_temp,W
	btfss	STATUS,Z
	goto	QMD53
	goto	QMD52
QMD53
; Lvals = get_rx ( ) ; 
	call	get_rx
	movwf	Lvals
; Hval = Cvals & 0x80  ; 
	movf	Cvals,W
	andlw	128
	movwf	Hval
; Cvals &= 0x7F ; 
	movlw	127
	andwf	Cvals,F
	return
; } 
; report_vals ( ) { 
report_vals
; put_tx ( 'R' ) ; 
	movlw	82
	call	put_tx
; put_tx ( Cvals + Hval ) ; 
	movf	Cvals,W
	addwf	Hval,W
	call	put_tx
; put_tx ( Lvals ) ; 
	movf	Lvals,W
	call	put_tx
	return
; } 
; report_power ( ) { 
report_power
; ad_sample ( 0x80  ) ; 
	movlw	128
	call	ad_sample
; put_tx ( 'P' ) ; 
	movlw	80
	call	put_tx
; put_tx ( fwdh ) ; 
	movf	fwdh,W
	call	put_tx
; put_tx ( fwdl ) ; 
	movf	fwdl,W
	call	put_tx
; put_tx ( revh ) ; 
	movf	revh,W
	call	put_tx
; put_tx ( revl ) ; 
	movf	revl,W
	call	put_tx
	return
; } 
; tune ( ) { 
tune
; tlow = thigh = 0 ; 
	movlw	0
	movwf	thigh
	movwf	tlow
; Qtune ( ) ; 
	call	Qtune
; if ( thigh >= 4 ) { 
	movf	thigh,W
	addlw	252
	btfss	STATUS,C
	goto	QMD54
; put_tx ( 'T' ) ; 
	movlw	84
	call	put_tx
; put_tx ( swr ) ; 
	movf	swr,W
	call	put_tx
; return ; 
	return
; } 
; if ( swr > 10  ) Ftune ( ) ; 
QMD54
	movf	swr,W
	addlw	246
	btfss	STATUS,Z	;_JLE
	btfss	STATUS,C
	goto	QMD55
	call	Ftune
; put_tx ( 'T' ) ; 
QMD55
	movlw	84
	call	put_tx
; put_tx ( swr ) ; 
	movf	swr,W
	call	put_tx
	return
; } 
; Qtune ( ) { 
Qtune
; QC = Cvals = Ct = 0 ; 
	movlw	0
	movwf	Ct
	movwf	Cvals
	movwf	QC
; QL = Lvals = Lt = 0 ; 
	movlw	0
	movwf	Lt
	movwf	Lvals
	movwf	QL
; QH = Hval = Ht = 0 ; 
	movlw	0
	movwf	Ht
	movwf	Hval
	movwf	QH
; write_vals ( ) ; 
	call	write_vals
; get_swr ( ) ; 
	call	get_swr
; swrq = swr ; 
	movf	swr,W
	movwf	swrq
; if ( thigh >= 4 || swr <= 10  ) return ; 
	movf	thigh,W
	addlw	252
	btfsc	STATUS,C
	goto	QMD56
	movf	swr,W
	addlw	246
	btfss	STATUS,Z	;_JG
	btfss	STATUS,C
	goto	_JG1
	goto	QMD57
_JG1
QMD56
	return
; for ( Cvals = 1 ; Cvals < 128 ; Cvals <<= 1 ) { 
QMD57
	movlw	1
	movwf	Cvals
QMD58
	movf	Cvals,W
	addlw	128
	btfsc	STATUS,C
	goto	QMD60
; for ( Lvals = 1 ; Lvals < 128 ; Lvals <<= 1 ) { 
	movlw	1
	movwf	Lvals
QMD61
	movf	Lvals,W
	addlw	128
	btfsc	STATUS,C
	goto	QMD63
; Hval = 0 ; 
;movlw	0
	clrf 	Hval
; Qtest ( ) ; 
	call	Qtest
; Hval = 128 ; 
	movlw	128
	movwf	Hval
; Qtest ( ) ; 
	call	Qtest
; } 
QMD62
; Lvals <<= 1 )  if ( swrq <= 13  && Ct > 1 ) break ; 
	bcf	STATUS,C
	rlf	Lvals,F
	goto	QMD61
QMD63
	movf	swrq,W
	addlw	243
	btfss	STATUS,Z	;_JG
	btfss	STATUS,C
	goto	_JG2
	goto	QMD64
_JG2
	movf	Ct,W
	addlw	255
	btfss	STATUS,Z	;_JLE
	btfss	STATUS,C
	goto	QMD64
	goto	QMD60
; if ( thigh >= 4 || swrq <= 10  ) break ; 
QMD64
	movf	thigh,W
	addlw	252
	btfsc	STATUS,C
	goto	QMD65
	movf	swrq,W
	addlw	246
	btfss	STATUS,Z	;_JG
	btfss	STATUS,C
	goto	_JG3
	goto	QMD66
_JG3
QMD65
	goto	QMD60
; } 
QMD66
; } QC = Cvals = Ct ; 
QMD59
	bcf	STATUS,C
	rlf	Cvals,F
	goto	QMD58
QMD60
	movf	Ct,W
	movwf	Cvals
	movwf	QC
; QL = Lvals = Lt ; 
	movf	Lt,W
	movwf	Lvals
	movwf	QL
; QH = Hval = Ht ; 
	movf	Ht,W
	movwf	Hval
	movwf	QH
; write_vals ( ) ; 
	call	write_vals
; swr = swrq ; 
	movf	swrq,W
	movwf	swr
	return
; } 
; Qtest ( ) { 
Qtest
; write_vals ( ) ; 
	call	write_vals
; get_swr ( ) ; 
	call	get_swr
; if ( swr < swrq ) { 
	movf	swr,W
	movwf	_temp
	movf	swrq,W
	subwf	_temp,W
	btfsc	STATUS,C
	goto	QMD67
; Lt = Lvals ; 
	movf	Lvals,W
	movwf	Lt
; Ct = Cvals ; 
	movf	Cvals,W
	movwf	Ct
; Ht = Hval ; 
	movf	Hval,W
	movwf	Ht
; swrq = swr ; 
	movf	swr,W
	movwf	swrq
; } 
; } 
QMD67
	return
; } Ftune ( ) { 
Ftune
; static char dv ; 
	;data
QMD68	equ 263
; Lt = Lvals ; Ct = Cvals ; 
	;code
	movf	Lvals,W
	movwf	Lt
	movf	Cvals,W
	movwf	Ct
; Cdv = Ct >> 1 ; 
	movf	Ct,W
	movwf	_temp
	movlw	1
	iorlw	0
	call	_rshift
	movf	_temp,W
	movwf	Cdv
; Ldv = Lt >> 1 ; 
	movf	Lt,W
	movwf	_temp
	movlw	1
	iorlw	0
	call	_rshift
	movf	_temp,W
	movwf	Ldv
; dv = ( Ldv > Cdv ) ? Ldv : Cdv ; 
	movf	Ldv,W
	movwf	_temp
	movf	Cdv,W
	subwf	_temp,W
	btfss	STATUS,Z	;_JLE
	btfss	STATUS,C
	goto	QMD69
	movf	Ldv,W
	goto	QMD70
QMD69
	movf	Cdv,W
QMD70
	BANKSEL	QMD68
	movwf	QMD68
; if ( Cdv == 0 ) Cdv = 1 ; 
	movf	Cdv,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD71
	movlw	1
	movwf	Cdv
; if ( Ldv == 0 ) Ldv = 1 ; 
QMD71
	movf	Ldv,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD72
	movlw	1
	movwf	Ldv
; if ( dv == 0 ) dv = 1 ; 
QMD72
	BANKSEL	QMD68
	movf	QMD68,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD73
	movlw	1
	movwf	QMD68
; write_vals ( ) ; 
QMD73
	call	write_vals
; get_swr ( ) ; 
	call	get_swr
; swrq = swr ; 
	movf	swr,W
	movwf	swrq
; while ( dv ) { 
QMD74
	BANKSEL	QMD68
	movf	QMD68,W
	btfsc	STATUS,Z
	goto	QMD75
; chg = 0 ; 
;movlw	0
	clrf 	chg
; Lvals += Ldv ; 
	movf	Ldv,W
	addwf	Lvals,F
; if ( Lvals < 128 ) FCLtest ( ) ; 
	movf	Lvals,W
	addlw	128
	btfsc	STATUS,C
	goto	QMD76
	call	FCLtest
; Lvals = Lt ; 
QMD76
	movf	Lt,W
	movwf	Lvals
; Cvals -= Cdv ; 
	movf	Cdv,W
	subwf	Cvals,F
; if ( Cvals < 128 ) FCLtest ( ) ; 
	movf	Cvals,W
	addlw	128
	btfsc	STATUS,C
	goto	QMD77
	call	FCLtest
; Cvals = Ct ; 
QMD77
	movf	Ct,W
	movwf	Cvals
; Lvals -= Ldv ; 
	movf	Ldv,W
	subwf	Lvals,F
; if ( Lvals < 128 ) FCLtest ( ) ; 
	movf	Lvals,W
	addlw	128
	btfsc	STATUS,C
	goto	QMD78
	call	FCLtest
; Lvals = Lt ; 
QMD78
	movf	Lt,W
	movwf	Lvals
; Cvals += Cdv ; 
	movf	Cdv,W
	addwf	Cvals,F
; if ( Cvals < 128 ) FCLtest ( ) ; 
	movf	Cvals,W
	addlw	128
	btfsc	STATUS,C
	goto	QMD79
	call	FCLtest
; Cvals = Ct ; 
QMD79
	movf	Ct,W
	movwf	Cvals
; if ( chg == 0 ) { 
	movf	chg,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD80
; dv >>= 1 ; 
	BANKSEL	QMD68
	bcf	STATUS,C
	rrf	QMD68,F
; if ( dv < Cdv ) Cdv = dv ; 
	movf	QMD68,W
	movwf	_temp
	movf	Cdv,W
	subwf	_temp,W
	btfsc	STATUS,C
	goto	QMD81
	movf	QMD68,W
	movwf	Cdv
; if ( dv < Ldv ) Ldv = dv ; 
QMD81
	BANKSEL	QMD68
	movf	QMD68,W
	movwf	_temp
	movf	Ldv,W
	subwf	_temp,W
	btfsc	STATUS,C
	goto	QMD82
	movf	QMD68,W
	movwf	Ldv
; } 
QMD82
; } if ( thigh >= 4 || swrq <= 10  ) break ; 
QMD80
	movf	thigh,W
	addlw	252
	btfsc	STATUS,C
	goto	QMD83
	movf	swrq,W
	addlw	246
	btfss	STATUS,Z	;_JG
	btfss	STATUS,C
	goto	_JG4
	goto	QMD84
_JG4
QMD83
	goto	QMD75
; } 
QMD84
; } swr = swrq ; 
	goto	QMD74
QMD75
	movf	swrq,W
	movwf	swr
; Cvals = Ct ; 
	movf	Ct,W
	movwf	Cvals
; Lvals = Lt ; 
	movf	Lt,W
	movwf	Lvals
; write_vals ( ) ; 
	call	write_vals
	return
; } 
; FCLtest ( ) { 
FCLtest
; write_vals ( ) ; 
	call	write_vals
; get_swr ( ) ; 
	call	get_swr
; if ( swr < swrq ) { 
	movf	swr,W
	movwf	_temp
	movf	swrq,W
	subwf	_temp,W
	btfsc	STATUS,C
	goto	QMD85
; Ct = Cvals ; 
	movf	Cvals,W
	movwf	Ct
; Lt = Lvals ; 
	movf	Lvals,W
	movwf	Lt
; ++ chg ; 
	incf	chg,F
; swrq = swr ; 
	movf	swr,W
	movwf	swrq
; } 
; } 
QMD85
	return
; } put_tx ( char val ) { 
	;data
_put_tx	equ	264
	;code
put_tx
	banksel	_put_tx
	movwf	_put_tx
; tx_buf [ tx_in ++ ] = val ; 
	movf	tx_in,W
	incf	tx_in,F
	addlw	tx_buf
	movwf	FSR
	movf	_put_tx,W
	movwf	INDF
; tx_in &= 7 ; 
	movlw	7
	andwf	tx_in,F
	return
; } 
; char get_rx ( ) { 
get_rx
; static char temp ; 
	;data
QMD86	equ 265
; temp = rx_buf [ rx_out ++ ] ; 
	;code
	movf	rx_out,W
	incf	rx_out,F
	addlw	rx_buf
	movwf	FSR
	movf	INDF,W
	BANKSEL	QMD86
	movwf	QMD86
; rx_out &= 7 ; 
	movlw	7
	andwf	rx_out,F
; return temp ; 
	movf	QMD86,W
	return
; } 
; init ( ) { 
init
; OSCCON = ( 0x10  + 0x20  + 0x40  + 0x02  ) ; 
	movlw	114
	movwf	OSCCON
; FSR0H = 0 ; 
;movlw	0
	clrf 	FSR0H
; TRISC = 0 ; 
;movlw	0
	clrf 	TRISC
; TRISA = 3 ; 
	movlw	3
	movwf	TRISA
; ADCON1 = 0x0D ; 
	movlw	13
	movwf	ADCON1
; LATB = 0x80  ; 
	movlw	128
	movwf	LATB
; TRISB = ( 0xff ^ ( 0x08  + 0x10  + 0x20  + 0x80  ) ) ; 
	movlw	71
	movwf	TRISB
; rx_in = rx_out = tx_in = tx_out = 0 ; 
	movlw	0
	movwf	tx_out
	movwf	tx_in
	movwf	rx_out
	movwf	rx_in
; Hval = Cvals = Lvals = 0 ; 
	movlw	0
	movwf	Lvals
	movwf	Cvals
	movwf	Hval
; rstate = tstate = 0 ; 
	movlw	0
	movwf	tstate
	movwf	rstate
; TMR2 = 0 ; 
;movlw	0
	clrf 	TMR2
; PR2 = 139 ; 
	movlw	139
	movwf	PR2
; PIE1 = 0x02  ; 
	movlw	2
	movwf	PIE1
; T2CON = 0x01  + 0x04  ; 
	movlw	5
	movwf	T2CON
; interrupts ( ) ; 
	call	interrupts
; write_vals ( ) ; 
	call	write_vals
	return
; } 
; _interrupt ( ) { 
_interrupt
; static char rdat ; 
	;data
QMD87	equ 266
; static char rmod ; 
QMD88	equ 267
; static char rbit ; 
QMD89	equ 268
; static char fsr0 ; 
QMD90	equ 269
; static char tdat ; 
QMD91	equ 270
; static char tmod ; 
QMD92	equ 271
; static char tbit ; 
QMD93	equ 272
; static char temp ; 
QMD94	equ 273
; fsr0 = FSR0L ; 
	;code
	movf	FSR0L,W
	BANKSEL	QMD90
	movwf	QMD90
; if ( rtimer ) -- rtimer ; 
	movf	rtimer,W
	btfsc	STATUS,Z
	goto	QMD95
	decf	rtimer,F
; if ( ++ rmod > 2 ) rmod = 0 ; 
QMD95
	BANKSEL	QMD88
	incf	QMD88,F
	movf	QMD88,W
	addlw	254
	btfss	STATUS,Z	;_JLE
	btfss	STATUS,C
	goto	QMD96
;movlw	0
	clrf 	QMD88
; switch ( rstate ) { 
QMD96
	movf	rstate,W
; case 0 : 
; : if ( ( PORTB & 0x40  ) == 0 ) { 
	addlw	0
	btfss	STATUS,Z
	goto	QMD99
QMD98
	movf	PORTB,W
	andlw	64
;addlw	256
	btfss	STATUS,Z
	goto	QMD101
; rbit = rdat = rmod = 0 ; 
	movlw	0
	BANKSEL	QMD88
	movwf	QMD88
	movwf	QMD87
	movwf	QMD89
; ++ rstate ; 
	incf	rstate,F
; } 
; break ; 
QMD101
	goto	QMD97
; case 1 : 
QMD99
; : if ( rmod == 1 ) { 
	addlw	255
	btfss	STATUS,Z
	goto	QMD103
QMD102
	BANKSEL	QMD88
	movf	QMD88,W
	addlw	255
	btfss	STATUS,Z
	goto	QMD105
; rdat >>= 1 ; 
	bcf	STATUS,C
	rrf	QMD87,F
; if ( PORTB & 0x40  ) rdat |= 0x80 ; 
	movf	PORTB,W
	andlw	64
	btfsc	STATUS,Z
	goto	QMD106
	movlw	128
	iorwf	QMD87,F
; if ( ++ rbit == 9 ) ++ rstate ; 
QMD106
	BANKSEL	QMD89
	incf	QMD89,F
	movf	QMD89,W
	addlw	247
	btfss	STATUS,Z
	goto	QMD107
	incf	rstate,F
; } 
QMD107
; } break ; 
QMD105
	goto	QMD97
; case 2 : 
QMD103
; : rx_buf [ rx_in ++ ] = rdat ; 
	addlw	255
	btfss	STATUS,Z
	goto	QMD109
QMD108
	movf	rx_in,W
	incf	rx_in,F
	addlw	rx_buf
	movwf	FSR
	BANKSEL	QMD87
	movf	QMD87,W
	movwf	INDF
; rx_in &= 7 ; 
	movlw	7
	andwf	rx_in,F
; rstate = 0 ; 
;movlw	0
	clrf 	rstate
; break ; 
	goto	QMD97
; } 
QMD109
QMD97
; if ( ++ tmod > 2 ) tmod = 0 ; 
	BANKSEL	QMD92
	incf	QMD92,F
	movf	QMD92,W
	addlw	254
	btfss	STATUS,Z	;_JLE
	btfss	STATUS,C
	goto	QMD111
;movlw	0
	clrf 	QMD92
; switch ( tstate ) { 
QMD111
	movf	tstate,W
; case 0 : 
; : if ( tx_in != tx_out ) { 
	addlw	0
	btfss	STATUS,Z
	goto	QMD114
QMD113
	movf	tx_in,W
	movwf	_temp2
	movf	tx_out,W
	subwf	_temp2,W
	btfsc	STATUS,Z
	goto	QMD116
; tdat = tx_buf [ tx_out ++ ] ; 
	movf	tx_out,W
	incf	tx_out,F
	addlw	tx_buf
	movwf	FSR
	movf	INDF,W
	BANKSEL	QMD91
	movwf	QMD91
; tx_out &= 7 ; 
	movlw	7
	andwf	tx_out,F
; tmod = tbit = 0 ; 
	movlw	0
	movwf	QMD93
	movwf	QMD92
; LATB ^= 0x80  ; 
	movlw	128
	xorwf	LATB,F
; ++ tstate ; 
	incf	tstate,F
; } 
; break ; 
QMD116
	goto	QMD112
; case 1 : 
QMD114
; : if ( tmod == 0 ) { 
	addlw	255
	btfss	STATUS,Z
	goto	QMD118
QMD117
	BANKSEL	QMD92
	movf	QMD92,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD120
; temp = ( tdat & 1 ) ? 0x80  : 0 ; 
	movf	QMD91,W
	andlw	1
	btfsc	STATUS,Z
	goto	QMD121
	movlw	128
	iorlw	0
	goto	QMD122
QMD121
	movlw	0
	iorlw	0
QMD122
	BANKSEL	QMD94
	movwf	QMD94
; temp ^= LATB ; 
	movf	LATB,W
	xorwf	QMD94,F
; temp &= 0x80  ; 
	movlw	128
	andwf	QMD94,F
; LATB ^= temp ; 
	movf	QMD94,W
	xorwf	LATB,F
; tdat >>= 1 ; 
	bcf	STATUS,C
	rrf	QMD91,F
; if ( ++ tbit == 8 ) ++ tstate ; 
	incf	QMD93,F
	movf	QMD93,W
	addlw	248
	btfss	STATUS,Z
	goto	QMD123
	incf	tstate,F
; } 
QMD123
; } break ; 
QMD120
	goto	QMD112
; case 2 : 
QMD118
; : if ( tmod == 0 ) { 
	addlw	255
	btfss	STATUS,Z
	goto	QMD125
QMD124
	BANKSEL	QMD92
	movf	QMD92,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD127
; LATB |= 0x80  ; 
	movlw	128
	iorwf	LATB,F
; ++ tstate ; 
	incf	tstate,F
; } 
; break ; 
QMD127
	goto	QMD112
; case 3 : 
QMD125
; : if ( tmod == 0 ) tstate = 0 ; 
	addlw	255
	btfss	STATUS,Z
	goto	QMD129
QMD128
	BANKSEL	QMD92
	movf	QMD92,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD131
;movlw	0
	clrf 	tstate
; break ; 
QMD131
	goto	QMD112
; } 
QMD129
QMD112
; FSR0L = fsr0 ; 
	BANKSEL	QMD90
	movf	QMD90,W
	movwf	FSR0L
; # asm 
    BCF  PIR1,TMR2IF
    retfie FAST           ; WREG, STATUS, BSR restored
QMD132
	retfie
; } write_vals ( ) { 
write_vals
; # asm 
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
QMD133
; delay ( 10 ) ; 
	movlw	10
	call	delay
	return
; } 
; delay ( char tm ) { 
	;data
_delay	equ	274
	;code
delay
	banksel	_delay
	movwf	_delay
; tm <<= 2 ; 
	bcf	STATUS,C
	rlf	_delay,F
	bcf	STATUS,C
	rlf	_delay,F
; rtimer = tm ; 
	movf	_delay,W
	movwf	rtimer
; while ( rtimer ) ; 
QMD134
	movf	rtimer,W
	btfsc	STATUS,Z
	goto	QMD135
	goto	QMD134
QMD135
	return
; } 
; interrupts ( ) { 
interrupts
; # asm 
      bsf   INTCON,GIE
      bsf   INTCON,PEIE   ;  need for tmr2 ? : yes  
QMD136
	return
; } no_interrupts ( ) { 
no_interrupts
; # asm 
      bcf   INTCON,GIE
      btfsc INTCON,GIE    ;see AN576.  What devices have this issue?
      goto $-2
QMD137
	return
; } ad_sample ( char just ) { 
	;data
_ad_sample	equ	275
	;code
ad_sample
	banksel	_ad_sample
	movwf	_ad_sample
; static char i ; 
	;data
QMD138	equ 276
; ADCON2 = just + 0x25 ; 
	;code
	BANKSEL	_ad_sample
	movf	_ad_sample,W
	addlw	37
	movwf	ADCON2
; ADCON0 = 5 ; 
	movlw	5
	movwf	ADCON0
; for ( i = 0 ; i < 5 ; ++ i ) ; 
	movlw	0
	movwf	QMD138
QMD139
	BANKSEL	QMD138
	movf	QMD138,W
	addlw	251
	btfsc	STATUS,C
	goto	QMD141
QMD140
; ++ i )  # asm 
	BANKSEL	QMD138
	incf	QMD138,F
	goto	QMD139
QMD141
    bsf ADCON0,1
QMD142
; while ( ADCON0 & 0x02  ) ; 
QMD143
	movf	ADCON0,W
	andlw	2
	btfsc	STATUS,Z
	goto	QMD144
	goto	QMD143
QMD144
; acch = ADRESH ; 
	movf	ADRESH,W
	movwf	acch
; accl = ADRESL ; 
	movf	ADRESL,W
	movwf	accl
; for ( i = 0 ; i < 5 ; ++ i ) ; 
	movlw	0
	BANKSEL	QMD138
	movwf	QMD138
QMD145
	BANKSEL	QMD138
	movf	QMD138,W
	addlw	251
	btfsc	STATUS,C
	goto	QMD147
QMD146
; ++ i )  # asm 
	BANKSEL	QMD138
	incf	QMD138,F
	goto	QMD145
QMD147
    bsf ADCON0,1
QMD148
; while ( ADCON0 & 0x02  ) ; 
QMD149
	movf	ADCON0,W
	andlw	2
	btfsc	STATUS,Z
	goto	QMD150
	goto	QMD149
QMD150
; argh = ADRESH ; 
	movf	ADRESH,W
	movwf	argh
; argl = ADRESL ; 
	movf	ADRESL,W
	movwf	argl
; dadd ( ) ; 
	call	dadd
; right_shift ( 1 ) ; 
	movlw	1
	call	right_shift
; fwdh = acch ; fwdl = accl ; 
	movf	acch,W
	movwf	fwdh
	movf	accl,W
	movwf	fwdl
; ADCON0 = 1 ; 
	movlw	1
	movwf	ADCON0
; for ( i = 0 ; i < 5 ; ++ i ) ; 
	movlw	0
	BANKSEL	QMD138
	movwf	QMD138
QMD151
	BANKSEL	QMD138
	movf	QMD138,W
	addlw	251
	btfsc	STATUS,C
	goto	QMD153
QMD152
; ++ i )  # asm 
	BANKSEL	QMD138
	incf	QMD138,F
	goto	QMD151
QMD153
    bsf ADCON0,1
QMD154
; while ( ADCON0 & 0x02  ) ; 
QMD155
	movf	ADCON0,W
	andlw	2
	btfsc	STATUS,Z
	goto	QMD156
	goto	QMD155
QMD156
; acch = ADRESH ; 
	movf	ADRESH,W
	movwf	acch
; accl = ADRESL ; 
	movf	ADRESL,W
	movwf	accl
; for ( i = 0 ; i < 5 ; ++ i ) ; 
	movlw	0
	BANKSEL	QMD138
	movwf	QMD138
QMD157
	BANKSEL	QMD138
	movf	QMD138,W
	addlw	251
	btfsc	STATUS,C
	goto	QMD159
QMD158
; ++ i )  # asm 
	BANKSEL	QMD138
	incf	QMD138,F
	goto	QMD157
QMD159
    bsf ADCON0,1
QMD160
; while ( ADCON0 & 0x02  ) ; 
QMD161
	movf	ADCON0,W
	andlw	2
	btfsc	STATUS,Z
	goto	QMD162
	goto	QMD161
QMD162
; argh = ADRESH ; 
	movf	ADRESH,W
	movwf	argh
; argl = ADRESL ; 
	movf	ADRESL,W
	movwf	argl
; dadd ( ) ; 
	call	dadd
; right_shift ( 1 ) ; 
	movlw	1
	call	right_shift
; revh = acch ; revl = accl ; 
	movf	acch,W
	movwf	revh
	movf	accl,W
	movwf	revl
	return
; } 
; get_swr ( ) { 
get_swr
; static char fwd ; 
	;data
QMD163	equ 277
; fwd = 0 ; 
	;code
	movlw	0
	BANKSEL	QMD163
	movwf	QMD163
; while ( fwd < 5 ) { 
QMD164
	BANKSEL	QMD163
	movf	QMD163,W
	addlw	251
	btfsc	STATUS,C
	goto	QMD165
; if ( ++ tlow == 0 ) ++ thigh ; 
	incf	tlow,F
	movf	tlow,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD166
	incf	thigh,F
; ad_sample ( 0x80  ) ; 
QMD166
	movlw	128
	call	ad_sample
; if ( fwdh == 0 ) fwd = fwdl ; 
	movf	fwdh,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD167
	movf	fwdl,W
	BANKSEL	QMD163
	movwf	QMD163
; else fwd = 99 ; 
	goto	QMD168
QMD167
	movlw	99
	BANKSEL	QMD163
	movwf	QMD163
QMD168
; if ( fwd < 5 ) delay ( 10 ) ; 
	BANKSEL	QMD163
	movf	QMD163,W
	addlw	251
	btfsc	STATUS,C
	goto	QMD169
	movlw	10
	call	delay
; if ( thigh >= 4 ) { 
QMD169
	movf	thigh,W
	addlw	252
	btfss	STATUS,C
	goto	QMD170
; swr = 253 ; 
	movlw	253
	movwf	swr
; return ; 
	return
; } 
; } 
QMD170
; } acch = fwdh ; accl = fwdl ; 
	goto	QMD164
QMD165
	movf	fwdh,W
	movwf	acch
	movf	fwdl,W
	movwf	accl
; argh = revh ; argl = revl ; 
	movf	revh,W
	movwf	argh
	movf	revl,W
	movwf	argl
; dadd ( ) ; 
	call	dadd
; multiply ( 10 ) ; 
	movlw	10
	call	multiply
; divqh = acch ; divql = accl ; 
	movf	acch,W
	movwf	divqh
	movf	accl,W
	movwf	divql
; acch = fwdh ; accl = fwdl ; 
	movf	fwdh,W
	movwf	acch
	movf	fwdl,W
	movwf	accl
; argh = revh ; argl = revl ; 
	movf	revh,W
	movwf	argh
	movf	revl,W
	movwf	argl
; if ( dsub ( ) ) { 
	call	dsub
	iorlw	0
	btfsc	STATUS,Z
	goto	QMD171
; swr = 250 ; 
	movlw	250
	movwf	swr
; return ; 
	return
; } 
; argh = acch ; argl = accl ; 
QMD171
	movf	acch,W
	movwf	argh
	movf	accl,W
	movwf	argl
; if ( argh == 0 && argl == 0 ) { 
	movf	argh,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD172
	movf	argl,W
;addlw	256
	btfss	STATUS,Z
	goto	QMD172
; swr = 251 ; 
	movlw	251
	movwf	swr
; return ; 
	return
; } 
; divide ( ) ; 
QMD172
	call	divide
; if ( divqh ) { 
	movf	divqh,W
	btfsc	STATUS,Z
	goto	QMD173
; swr = 252 ; 
	movlw	252
	movwf	swr
; return ; 
	return
; } 
; swr = divql ; 
QMD173
	movf	divql,W
	movwf	swr
	return
; } 
; zacc ( ) { 
zacc
; accl = acch = 0 ; 
	movlw	0
	movwf	acch
	movwf	accl
	return
; } 
; char dadd ( ) { 
dadd
; # asm 
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

QMD174
; return overflow ; 
	movf	overflow,W
	return
; } 
; right_shift ( char count ) { 
	;data
_right_shift	equ	278
	;code
right_shift
	banksel	_right_shift
	movwf	_right_shift
; while ( count -- ) { 
QMD175
	BANKSEL	_right_shift
	movf	_right_shift,W
	decf	_right_shift,F
	iorlw	0
	btfsc	STATUS,Z
	goto	QMD176
; # asm 
        banksel  acch
        bcf   STATUS,C      ; logical shift right double
        rrf   acch,F
        rrf   accl,F
QMD177
; } } 
	goto	QMD175
QMD176
	return
; char dsub ( ) { 
dsub
; # asm 

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

QMD178
; return overflow ; 
	movf	overflow,W
	return
; } 
; divide ( ) { 
divide
; zacc ( ) ; 
	call	zacc
; for ( divi = 0 ; divi < 16 ; ++ divi ) { 
	movlw	0
	movwf	divi
QMD179
	movf	divi,W
	addlw	240
	btfsc	STATUS,C
	goto	QMD181
; # asm 
        bcf   STATUS,C
        rlf   divql,F        ; banksel ok here, same as divi
        rlf   divqh,F
        rlf   accl,F
        rlf   acch,F
QMD182
; if ( dsub ( ) ) dadd ( ) ; 
	call	dsub
	iorlw	0
	btfsc	STATUS,Z
	goto	QMD183
	call	dadd
; else divql |= 1 ; 
	goto	QMD184
QMD183
	movlw	1
	iorwf	divql,F
QMD184
; } 
QMD180
; ++ divi )  } 
	incf	divi,F
	goto	QMD179
QMD181
	return
; divide_k ( char constant ) { 
	;data
_divide_k	equ	279
	;code
divide_k
	banksel	_divide_k
	movwf	_divide_k
; divql = accl ; divqh = acch ; 
	movf	accl,W
	movwf	divql
	movf	acch,W
	movwf	divqh
; argh = 0 ; argl = constant ; 
	movlw	0
	movwf	argh
	movf	_divide_k,W
	movwf	argl
; divide ( ) ; 
	call	divide
; accl = divql ; acch = divqh ; 
	movf	divql,W
	movwf	accl
	movf	divqh,W
	movwf	acch
	return
; } 
; char multiply ( char multi ) { 
	;data
_multiply	equ	280
	;code
multiply
	banksel	_multiply
	movwf	_multiply
; static char over ; 
	;data
QMD185	equ 281
; over = 0 ; 
	;code
	movlw	0
	BANKSEL	QMD185
	movwf	QMD185
; divi = multi ; 
	movf	_multiply,W
	movwf	divi
; argh = acch ; 
	movf	acch,W
	movwf	argh
; argl = accl ; 
	movf	accl,W
	movwf	argl
; zacc ( ) ; 
	call	zacc
; while ( divi ) { 
QMD186
	movf	divi,W
	btfsc	STATUS,Z
	goto	QMD187
; if ( divi & 1 ) over |= dadd ( ) ; 
	movf	divi,W
	andlw	1
	btfsc	STATUS,Z
	goto	QMD188
	call	dadd
	BANKSEL	QMD185
	iorwf	QMD185,F
; divi >>= 1 ; 
QMD188
	bcf	STATUS,C
	rrf	divi,F
; # asm 
          ; banksel argl     ; multi and argl in different banks, fixed by using divi
          bcf  STATUS,C
          rlf  argl,F
          rlf  argh,F
QMD189
; } return over ; 
	goto	QMD186
QMD187
	BANKSEL	QMD185
	movf	QMD185,W
	return
; } 
	end
