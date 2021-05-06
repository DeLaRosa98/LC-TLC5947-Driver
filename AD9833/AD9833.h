/* Arduino Library adapted from example:
https://www.instructables.com/Signal-Generator-AD9833/
Will be used for a Degree Thesis at CEMDATIC, ETSIT, UPM
Developed by Pablo de la Rosa. Directed by Morten A. Geday

The library creates a Square Wave from an AD9833 chip
*/

#ifndef AD9833_h
#define AD9833_h

#include <Arduino.h>
#include <math.h>

#define wSquare 0b0000000000101000
#define p_SDATA 6
#define p_SCLK 5
#define p_XLAT 4
#define p_FSYNC 2

class AD9833
{
public:
    AD9833(uint16_t data, uint16_t clk, uint16_t fsync, uint8_t numberOfDigits);
    void setFreq(uint32_t *freqSGLo);
    void begin();

    uint16_t _data = p_SDATA;
    uint16_t _clk = p_SCLK;
    uint16_t _fsync = p_FSYNC;
    uint16_t _waveType = wSquare;
    uint16_t _numberOfDigits;
    uint32_t _freqSGLo[];

private:
    uint32_t power(uint16_t y);
    uint32_t calcFreq(uint32_t *freqSG);
    void write(uint16_t data);
    void reset();
    void freqReset(uint32_t freq, uint16_t wave);
};

#endif