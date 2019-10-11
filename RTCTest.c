#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>

const char RTCAddr = 0x6f;
const char SEC = 0x00;
const char MIN = 0x01;
const char HOUR = 0x02;
const int debounceTime = 200;

int main()
{
	int RTC;
	
	int rtcSetup;
	unsigned char interval = 0b0000001;//1s default
	wiringPiSetup();
	pinMode(RTCAlarm, INPUT);
	RTC = wiringPiI2CSetup(RTCAddr);
	rtcSetup = ReadReg8(RTC, 0x0D);
	rtcSetup &= 0b10000111;
	rtcSetup |= 0b10000000;
	wiringPiI2CWriteReg8(RTC, 0x0D, rtcSetup);
	wiringPiI2CWriteReg8(RTC, 0x0A, interval);
	rtcSetup = ReadReg8(RTC, 0x07);
	rtcSetup |= 0b00010000;
	rtcSetup &= 0b10111111;
	wiringPiI2CWriteReg8(RTC, 0x07, rtcSetup);
	//This will probably have a lot of issues
	wiringPiISR (RTCAlarm, INT_EDGE_FALLING, interuptTest);
	
}
	
void interuptTest()
{
	printf("Eh Victory!");
}
	
