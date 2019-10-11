#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RTCAlarm 7

const char RTCAddr = 0x6f;
const char SEC = 0x00;
const char MIN = 0x01;
const char HOUR = 0x02;
const int debounceTime = 200;

void interuptTest();

int RTC;
int checker = 0;
int main()
{
	int rtcSetup;
	unsigned char interval = 0b0000001;//1s default
	wiringPiSetup();
	pinMode(RTCAlarm, INPUT);
	RTC = wiringPiI2CSetup(RTCAddr);

	wiringPiI2CWriteReg8(RTC, HOUR, 0x13);
    wiringPiI2CWriteReg8(RTC, MIN, 0x54);
    wiringPiI2CWriteReg8(RTC, SEC, 0x00);
    wiringPiI2CWriteReg8(RTC, SEC, 0b10000000);

	rtcSetup =wiringPiI2CReadReg8(RTC, 0x0D);
	rtcSetup &= 0b10000111;
	rtcSetup |= 0b10000000;
	wiringPiI2CWriteReg8(RTC, 0x0D, rtcSetup);
	wiringPiI2CWriteReg8(RTC, 0x0A, interval);
	rtcSetup = wiringPiI2CReadReg8(RTC, 0x07);
	rtcSetup |= 0b00010000;
	rtcSetup &= 0b10011111;
	wiringPiI2CWriteReg8(RTC, 0x07, rtcSetup);
	//This will probably have a lot of issues
	wiringPiISR (RTCAlarm, INT_EDGE_FALLING, interuptTest);
	
	while(1)
	{
		checker = wiringPiI2CReadReg8(RTC, 0x0D);
		checker &= 0b00001000;
		if(checker){printf("Interupt Flag: %d\n",checker);
		rtcSetup = wiringPiI2CReadReg8(RTC, 0x0D);
    	rtcSetup &= 0b11110111;
    	wiringPiI2CWriteReg8(RTC, 0x0D, rtcSetup);}
	}
}
	
void interuptTest()
{
	int checker;
	printf("Eh Victory!");
	int rtcSetup = wiringPiI2CReadReg8(RTC, 0x0D);
	rtcSetup &= 0b11110111;
	wiringPiI2CWriteReg8(RTC, 0x0D, rtcSetup);
	checker = wiringPiI2CReadReg8(RTC, 0x00);
	checker +=2;
	wiringPiI2CWriteReg8(RTC, 0x0A, checker);
}
	
