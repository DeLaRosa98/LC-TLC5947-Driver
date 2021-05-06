#include "AD9833.h"

const uint16_t sdata = 9;
const uint16_t sclk = 8;
const uint16_t fsync = 5;
const uint16_t numDigits = 6;
const uint32_t freqSGLo[numDigits] = {0, 5, 0, 0, 0, 0}; // 50 Hz

AD9833 ad = AD9833(sdata, sclk, fsync, numDigits);

void setup()
{
  ad.setFreq(freqSGLo);
  ad.begin();
}

void loop()
{
}
