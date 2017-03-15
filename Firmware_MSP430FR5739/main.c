#include <msp430.h> 
#include <stdint.h>
#include "eink.h"

/*
 * main.c
 */
#define RAM_START 	0x1F00 //up to 0x1FFF
#define FRAM_START 	0xFE80


static void initClock()
{
   	PM5CTL0=0;
    CSCTL0 =0xA500;
   // FRCTL0 = 0xA500 | NWAITS0; //keine warte weil 8Mhz clk
   CSCTL1 = DCOFSEL_3|DCORSEL;  // on reset set to 24MHz
   CSCTL2 = SELA__DCOCLK |  SELS__DCOCLK | SELM__DCOCLK;
   CSCTL3 &= ~(DIVA0 | DIVA1 | DIVA2 | DIVS0 | DIVS1 | DIVS2 | DIVM0| DIVM1 | DIVM2  ); // MCLK and SMCLK to be 8Mhz
   CSCTL3 |= DIVA1|DIVA0;  // ACLK = CLK/8 ==  1Mhz
   // CSCTL4 = HFXTDRIVE0|HFXTDRIVE1 | HFFREQ1 |LFXTOFF  ;  //drive HFXT to be max power ( kein crytal bei stromsucher V1.0 deshab nicht benötigt)
  // _delay_cycles(5000);
  // CSCTL0 =0x0000;
   SFRIFG1 &=~( OFIFG);
}


int main(void) {
	volatile unsigned int i=0;
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer for a while

	 ioEinkEna();
	 initClock();

	 cleanink(1);
	 cleanink(0);
	 cleanink(1);
	 cleanink(0);
	 cleanink(0);
	 cleanink(1);
	 cleanink(1); //its really important that you do as minimum of as possible clean_white operation right before writing actual content to get correct contrast
	 	 	 	 // if you do too many you will get faded content
	 	 	 	 // if you do too less you will get bad contrast



 printstring("0123456789ABCDEFGHIJKLMNOPQ",0,150,26);
 printstring("0123456789ABCDEFGHIJKLMNOPQ",0,150,26);


	 OE_CLEAR;
	 GMOD_CLEAR;




	while(1)
    {



    }


}
