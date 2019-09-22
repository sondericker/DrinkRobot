
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <time.h>
#include <math.h>
#include <iostream>

#include "PWMDriver.h"


using namespace std;


int pwmHatFD = -1;

PWMDriver::PWMDriver() {
	
	
}

void PWMDriver::initPWM(int address) 			// 0x40 by default
{
    pwmHatFD = wiringPiI2CSetup(address);

    // zero all PWM ports
    resetAllPWM(0,0);

    wiringPiI2CWriteReg8(pwmHatFD, __MODE2, __OUTDRV);
    wiringPiI2CWriteReg8(pwmHatFD, __MODE1, __ALLCALL);

    int mode1 = wiringPiI2CReadReg8(pwmHatFD, __MODE1);
    mode1 = mode1 & ~__SLEEP;
    wiringPiI2CWriteReg8(pwmHatFD, __MODE1, mode1);

    setPWMFreq(60);
}

void PWMDriver::setPWMFreq(int freq)
{
    float prescaleval = 25000000;
    prescaleval /= 4096.0;
    prescaleval /= (float)freq;
    prescaleval -= 1.0;
    int prescale = floor(prescaleval + 0.5);

    int oldmode = wiringPiI2CReadReg8(pwmHatFD, __MODE1);
    int newmode = (oldmode & 0x7F) | 0x10;
    wiringPiI2CWriteReg8(pwmHatFD, __MODE1, newmode);
    wiringPiI2CWriteReg8(pwmHatFD, __PRESCALE, floor(prescale));
    wiringPiI2CWriteReg8(pwmHatFD, __MODE1, oldmode);

    wiringPiI2CWriteReg8(pwmHatFD, __MODE1, oldmode | 0x80);
}

void PWMDriver::setPWM(int channel, int on, int off)
{
    wiringPiI2CWriteReg8(pwmHatFD, __LED0_ON_L+4*channel, on & 0xFF);
    wiringPiI2CWriteReg8(pwmHatFD, __LED0_ON_H+4*channel, on >> 8);
    wiringPiI2CWriteReg8(pwmHatFD, __LED0_OFF_L+4*channel, off & 0xFF);
    wiringPiI2CWriteReg8(pwmHatFD, __LED0_OFF_H+4*channel, off >> 8);
}

void PWMDriver::resetAllPWM(int on, int off)
{
    wiringPiI2CWriteReg8(pwmHatFD, __ALL_LED_ON_L, on & 0xFF);
    wiringPiI2CWriteReg8(pwmHatFD, __ALL_LED_ON_H, on >> 8);
    wiringPiI2CWriteReg8(pwmHatFD, __ALL_LED_OFF_L, off & 0xFF);
    wiringPiI2CWriteReg8(pwmHatFD, __ALL_LED_OFF_H, off >> 8);
}

