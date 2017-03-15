/*
 * eink.h
 *
 *  Created on: Mar 12, 2017
 *      Author: Gaurav
 */

#ifndef EINK_H_
#define EINK_H_

#include <msp430.h>


#define SHDN BIT7

#define DATA_PORT P3OUT
#define D0 0 //BIT0 of eink data port
#define D1 7 //BIT7 of eink data port
#define D2 1 //BIT1 of eink data port
#define D3 6 //BIT6 of eink data port
#define D4 2 //BIT2 of eink data port
#define D5 5 //BIT5 of eink data port
#define D6 3 //BIT3 of eink data port
#define D7 4 //BIT4 of eink data port

#define CL 			BIT4 	//port2
#define CL_SET 		P2OUT|=CL
#define CL_CLEAR 	P2OUT&=~CL

#define LE 			BIT1 	//port1
#define LE_SET 		P1OUT|=LE
#define LE_CLEAR	P1OUT&=~LE

#define OE 			BIT3 	//port2
#define OE_SET		P2OUT|=OE
#define OE_CLEAR	P2OUT&=~OE

#define CKV 		BIT5 	//port1
#define CKV_SET		P1OUT|=CKV
#define CKV_CLEAR	P1OUT&=~CKV

#define GMOD 		BIT3 	//port1
#define GMOD_SET	P1OUT|=GMOD
#define	GMOD_CLEAR	P1OUT&=~GMOD

#define SPH 		BIT0 	//port1
#define SPH_SET		P1OUT|=SPH
#define SPH_CLEAR	P1OUT&=~SPH

#define SHR 		BIT2 	//port1
#define SHR_SET		P1OUT|=SHR
#define SHR_CLEAR	P1OUT&=~SHR

#define SPV 		BIT4    //port 1
#define SPV_SET		P1OUT|=SPV
#define SPV_CLEAR	P1OUT&=~SPV

#define CE2 		BIT5    //port 2
#define CE2_SET		P2OUT|=CE2
#define CE2_CLEAR	P2OUT&=~CE2

#define CE1 		BIT4    //port J
#define CE1_SET		PJOUT|=CE1
#define CE1_CLEAR	PJOUT&=~CE1

#define REG_SHDN P2OUT&=~SHDN

#define GDISP_SCREEN_HEIGHT 600
#define CKV_CLOCK 		    { CKV_CLEAR;	__delay_cycles(24);	CKV_SET;  __delay_cycles(24);   }
#define SCLOCK        {  CL_SET;       CL_CLEAR; }


void printstring( unsigned char *ch,uint16_t x, uint16_t y,uint8_t n);


#endif /* EINK_H_ */
