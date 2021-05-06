/* Arduino Library adapted from example:
https://www.instructables.com/Signal-Generator-AD9833/
Will be used for a Degree Thesis at CEMDATIC, ETSIT, UPM
Developed by Pablo de la Rosa. Directed by Morten A. Geday

The library creates a Square Wave from an AD9833 chip
*/

#include "AD9833.h"

/*!
*   @brief Instantiates a new AD9833 class
*   
*   @param data
*           SPI data pin
*
*   @param clk
*           SPI clk pin
*
*   @param fsync
*           SPI fsync pin
*
*   @param numberOfDigits
*           number of digits in freqSGLo array
*
*/
AD9833::AD9833(uint16_t data, uint16_t clk, uint16_t fsync, uint8_t numberOfDigits)
{
    _data = data;
    _clk = clk;
    _fsync = fsync;
    _waveType = wSquare;
    _numberOfDigits = numberOfDigits;
}

/*!
*   @brief Sets an array with the frequency values with LSB
*   
*   @param freqSGLo[]
*           input array of frequency with LSB
*           50Hz would be (with numberOfDigits = 6) freqSGLo[] = {0, 5, 0, 0, 0, 0}
*
*/
void AD9833::setFreq(uint32_t *freqSGLo)
{
    memcpy(_freqSGLo, freqSGLo, _numberOfDigits * sizeof(uint32_t));
}

/*!
*   @brief 10^y
*   
*   @param y
*           power
*
*/
uint32_t AD9833::power(uint16_t y)
{
    uint32_t t = 1;
    for (uint8_t i = 0; i < y; i++)
    {
        t = t * 10;
    }
    return t;
}

/*!
*   @brief calculates the frequency from the array freqSGLo
*   
*   @param freqSG
*           values in array of frequencies
*
*/
uint32_t AD9833::calcFreq(uint32_t *freqSG)
{
    uint32_t i = 0;
    for (uint32_t x = 0; x < _numberOfDigits; x++)
    {
        i = i + freqSG[x] * power(x);
    }
    return i;
}

/*!
*   @brief writes data
*   
*   @param data
*           data to be written
*
*/
void AD9833::write(uint16_t data)
{
    digitalWrite(_clk, LOW);
    digitalWrite(_clk, HIGH);
    digitalWrite(_fsync, LOW);
    for (uint8_t i = 0; i < 16; i++)
    {
        if (data & 0x8000)
        {
            digitalWrite(_data, HIGH);
        }
        else
        {
            digitalWrite(_data, LOW);
        }
        data = data << 1;
        digitalWrite(_clk, HIGH);
        digitalWrite(_clk, LOW);
    }
    digitalWrite(_clk, HIGH);
    digitalWrite(_fsync, HIGH);
}

/*!
*   @brief writes data to 0
*   
*/
void AD9833::reset()
{
    delay(100);
    write(0x100);
    delay(100);
}

/*!
*   @brief resets frequency and creates the square wave
*   
*   @param freq
*           frequency of the wave
*
*   @param wave
*           wave type, square in our case
*
*/
void AD9833::freqReset(uint32_t freq, uint16_t wave)
{
    uint32_t fl = freq * (0x10000000 / 25000000.0);
    write(0x2100);
    write((uint16_t)(fl & 0x3FFF) | 0x4000);
    write((uint16_t)((fl & 0xFFFC000) >> 14) | 0x4000);
    write(0xC000);
    write(wave);
    _waveType = wave;
}

/*!
*   @brief setups the square wave generator
*
*/
void AD9833::begin()
{
    pinMode(_data, OUTPUT);
    pinMode(_clk, OUTPUT);
    pinMode(_fsync, OUTPUT);
    digitalWrite(_fsync, HIGH);
    digitalWrite(_clk, HIGH);
    reset();
    freqReset(calcFreq(_freqSGLo), _waveType);
}