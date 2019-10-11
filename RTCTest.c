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
	unsigned char alarmTrigger = 0b0000001;//This will set off alarm when seconds regester = this value
	
	wiringPiSetup();
	pinMode(RTCAlarm, INPUT);				//setup pin for alarm interupt
	RTC = wiringPiI2CSetup(RTCAddr);
	
	//random default values for RTC
	wiringPiI2CWriteReg8(RTC, HOUR, 0x13);
    wiringPiI2CWriteReg8(RTC, MIN, 0x54);
    wiringPiI2CWriteReg8(RTC, SEC, 0x00);
    wiringPiI2CWriteReg8(RTC, SEC, 0b10000000);

	//setup for the alarm
	rtcSetup =wiringPiI2CReadReg8(RTC, 0x0D);
	rtcSetup &= 0b10000111;
	rtcSetup |= 0b10000000;
	wiringPiI2CWriteReg8(RTC, 0x0D, rtcSetup);
	wiringPiI2CWriteReg8(RTC, 0x0A, alarmTrigger);
	rtcSetup = wiringPiI2CReadReg8(RTC, 0x07);
	rtcSetup |= 0b00010000;
	rtcSetup &= 0b10011111;
	wiringPiI2CWriteReg8(RTC, 0x07, rtcSetup);
	wiringPiISR (RTCAlarm, INT_EDGE_FALLING, interuptTest);
	
	while(1){}
}
	
void interuptTest()
{
	printf("Eh Victory!");
	
	int reset = wiringPiI2CReadReg8(RTC, 0x0D);	//reset alarm flag
	reset &= 0b11110111;					
	wiringPiI2CWriteReg8(RTC, 0x0D, reset);
}
	
