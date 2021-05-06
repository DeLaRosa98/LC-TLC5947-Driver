/*  Arduino Library developed for a Degree Thesis at CEMDATIC, ETSIT, UPM
    Developed by Pablo de la Rosa. Directed by Morten A. Geday
    Aimed at using the different peripherials from a PCB (also designed for the Degree Thesis)
    Controls from an Arduino Nano:
        3 LEDS
        5 Buttons
        Touch Display
        AD9833 (wave generator)
        Device
    All the pin configuration has been set according to the PCB design and routing
*/

#ifndef PCB_h
#define PCB_h

#include <Arduino.h>
#include <math.h>
#include <SPI.h>
#include <Device.h>
#include <AD9833.h>
#include <MegunoLink.h>
#include <CommandHandler.h>

#define p_SDATA 6
#define p_SCLK 5
#define p_XLAT 4
#define p_FSYNC 2

class PCB
{
public:
    PCB(Device *devices, int numDevices);
    void begin();

    int _numDevices;
    uint16_t _numDrivers;
    uint16_t *_pwmbuffer;
    uint8_t _lat = p_XLAT;
    uint8_t _clk = p_SCLK;
    uint8_t _dat = p_SDATA;
    uint16_t *_outputArray;
    Device _devices[];

    void write();
    void setPWM(uint16_t chan, uint16_t pwm);
};
#endif