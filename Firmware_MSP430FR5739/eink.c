/*
 * eink.c
 *
 *  Created on: Mar 12, 2017
 *      Author: Gaurav
 */
#include <msp430.h>
#include <stdint.h>
#include "eink.h"
#include "arial40.h"

inline void eink_data(unsigned char data)
{
	DATA_PORT=0;
	DATA_PORT= (((data&0x01)&&0x01)<<D0) | (((data&0x02)&&0x01)<<D1) | (((data&0x04)&&0x01)<<D2) | (((data&0x08)&&0x01)<<D3) |(((data&0x10)&&0x01)<<D4) | (((data&0x20)&&0x1)<<D5) | (((data&0x40)&&0x01)<<D6) | (((data&0x80)&&0x01)<<D7);
}

inline void ioEinkWrite(uint8_t data) {
	eink_data(data);
	SCLOCK;
}

inline void ioEinkClk(int i) {
	while(i--) {
		SCLOCK
	}
}
void ioEinkVscanStop() {
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLEAR;
	__delay_cycles(375);
	CKV_SET;
	__delay_cycles(53);
}
void ioEinkVscanWrite(int len) {
	CL_SET;
	CKV_CLEAR;
	CL_CLEAR;
	//OE_CLEAR; //not happning
	__delay_cycles(34); //1.27 us
//	OE_SET; //not happning
	CKV_SET;
	__delay_cycles(24); //1.44 us
	LE_SET;
	LE_CLEAR;
	__delay_cycles(24);
}

void  ioEinkEna() {
	P2DIR |= SHDN |CL |OE|CE2;

		P1DIR |= LE | CKV| GMOD|SPH | SHR | SPV;
		P3DIR =0xFF;
		PJDIR |=CE1;
		PJOUT &=~CE1;
		P2OUT &=~CE2;
		__delay_cycles(1000);
			P2OUT |=SHDN;
			SHR_SET;
		//__delay_cycles(65530);

}

void ioEinkHscanStop() {
	SPH_SET;
}
void ioEinkHscanStart() {
	SCLOCK;
	SCLOCK;
	SPH_CLEAR;
}
void ioEinkVscanStart() {
	GMOD_SET;
	CKV_SET;
	OE_SET;
	LE_CLEAR;
	__delay_cycles(800); //100 us
	SPV_SET;
	__delay_cycles(800); //100 us
	SPV_CLEAR;
	__delay_cycles(24); //1 us //1us
	CKV_CLEAR;
	__delay_cycles(671); //25 us //28 us measured
	CKV_SET;
	__delay_cycles(24); //1 us //1us
	SPV_SET;
	__delay_cycles(671); //25 us //26 measured
	CKV_CLOCK;
	__delay_cycles(671); //25 us //26 measured
	CKV_CLOCK;
	__delay_cycles(671); //25 us //27.8
	CKV_CLOCK;
}

void cleanink(unsigned char bw)
{
	unsigned int j,i,data;

	data=bw?0xF0:0x0F;
	for(j=0;j<4;j++)
	{

		ioEinkVscanStart();

		for(i=0;i<600;i++){
		ioEinkHscanStart();
		//ioEinkWrite(data);
		DATA_PORT=data;
		ioEinkClk(800/4);
		ioEinkHscanStop();
		ioEinkVscanWrite(15);
		}
		ioEinkVscanStop();


	}
}
unsigned char transform[16]={  //GPIO port is little weirdly  mapped to eink IO port ,
							   //which means  D0 of MCU port is != D0 of eink , so need to transform is required look into schematic
		0b10101010,
		0b10101001,
		0b10100110,
		0b10100101,
		0b10011010,
		0b10011001,
		0b10010110,
		0b10010101,
		0b01101010,
		0b01101001,
		0b01100110,
		0b01100101,
		0b01011010,
		0b01011001,
		0b01010110,
		0b01010101
};
void showfont( unsigned char ch)
{

	unsigned char w,h;
	ioEinkVscanStart();

	for(h=0;h<ariald[ch][1];h++)
	{
		ioEinkHscanStart();
		for(w=0;w<(ariald[ch][0]>>3);w++){
		ioEinkWrite(transform[(arial40[ariald[ch][2]+w+(h*3)]&0xF0)>>4]);
		ioEinkWrite(transform[(arial40[ariald[ch][2]+w+(h*3)]&0x0F)]);
		}

		ioEinkWrite(0xAA);
		ioEinkClk((800/4)-3);
		ioEinkHscanStop();
		ioEinkVscanWrite(15);

	}

	ioEinkVscanStop();

}


void printstring( unsigned char *ch,uint16_t x, uint16_t y,uint8_t n)
{

	unsigned int w,h,test;
    unsigned int x_holder,n_holder;
    unsigned char *ho;
    ho=ch;
    x_holder=x>>2;
    n_holder=n;
	ioEinkVscanStart();
	for (h=0;h<y;h++)
	ioEinkVscanWrite(15);

	for(h=0;h<ariald[*ho-0x30][1];h++)
	{
		ioEinkHscanStart();
         x=x_holder;
         n=n_holder;
 		ioEinkWrite(0xFF);
 		ioEinkClk(x_holder);
        while(n--){

    	for(w=0;w<((ariald[*ch-0x30][0]+7)>>3);w++){

    		ioEinkWrite(transform[  	(arial40[		ariald[*ch-0x30][2]	+w+		(h*		((ariald[*ch-0x30][0]+7)	>>3	)	)		]&0xF0		)>>4  		]);
    		ioEinkWrite(transform[ 	 	(arial40[		ariald[*ch-0x30][2]	+w+		(h*		((ariald[*ch-0x30][0]+7)	>>3	)	)		]&0x0F		)     		]);
		}
        x=x+((ariald[*ch-0x30][0]+7)>>3);
        ch++;
        }

		ioEinkWrite(0xFF);
		ioEinkClk(200-(x<<1)-x_holder);
		ioEinkHscanStop();
		ioEinkVscanWrite(15);
             ch=ho;

	}
	for (;h<(600-y);h++)
	ioEinkVscanWrite(15);


	ioEinkVscanStop();

}
