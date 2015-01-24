//***************************************************************************************
/*************************************************
Copyright:SWJTU_Photoelectric Engineering Institute & Nondestructive Testing Research Center
Author:Xiaoxiang Luo
Date:2015-01-12
Description:
使用MSP430F5438A产生两路控制LED环的信号，测试两个环进行通过一个暗的LED流动显示

**************************************************/
//***************************************************************************************

#include <msp430.h>

//管脚定义
#define SIN2 BIT0;
#define BLANK2 BIT1;
#define SCLK2 BIT2;
#define XLAT2 BIT3;

#define BLANK BIT4;  //P8.4         Serial data input for driver on/off control.
#define SIN BIT5;    //P8.5         Blank, all outputs. When BLANK = high level, all constant-current outputs (OUT0–OUT15) are forced off
#define SCLK BIT6;	 //P8.6         Serial data shift clock.
#define XLAT BIT7;   //P8.7         Edge triggered latch.

volatile unsigned int cnt=0; //counter for shift


volatile char GS[96] = {
                    0,1,1,1,1,1,1,1,  //1
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //2
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //3
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //4
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //5
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //6
                    1,1,1,1,1,1,1,1   //

              };
volatile char GS2[96] = {
                    1,0,1,1,1,1,1,1,  //1
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //2
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //3
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //4
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //5
                    1,1,1,1,1,1,1,1,  //

                    1,1,1,1,1,1,1,1,  //6
                    1,1,1,1,1,1,1,1   //

              };




void delay()
{
	volatile unsigned int i;
	i = 20000;
	do i--;
		while(i != 0);
}

void Write_bit_LED1(char Status)
{
	P8OUT &= ~SCLK;
	if(Status == 1)
	{
		P8OUT |= SIN;
	}
	else
	{
		P8OUT &= ~SIN;
	}
	P8OUT |= SCLK;
	P8OUT &= ~SCLK;
}

void updata1()
{
	volatile int j=0;
	P8OUT |= XLAT;
	P8OUT &= ~XLAT;
	P8OUT |= BLANK;
	for(j = 95; j>=0;j--)
		Write_bit_LED1(GS[j]);
	P8OUT &= ~BLANK;
	P8OUT |= XLAT;
	P8OUT &= ~XLAT;
}
void shift_LED1()
{
	updata1();
	delay();
	volatile unsigned char temp=0;
	if(cnt < 95)
	{
		temp = GS[cnt+1];
		GS[cnt+1] = GS[cnt];
		GS[cnt] = temp;
		cnt++;
	}
	else
	{
		 temp = GS[0];
		 GS[0] = GS[95];
		 GS[95] = temp;
		 cnt=0;
	}

}


void Init_Clk(void)
{
  WDTCTL = WDTPW + WDTHOLD;  //关闭看门狗

}

void main(void) {
	//WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	Init_Clk();
	P8DIR = 0xFF;
	P8OUT = 0x00;
	P9DIR = 0xFF;
	P9OUT = 0x00;
while(1)
{
	shift_LED1();
}
}



