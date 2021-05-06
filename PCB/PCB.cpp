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

#include "PCB.h"

/*!
*   @brief Instantiates a new PCB class
*   
*   @param devices
*           Array with all Device devices to be controlled by the PCB
*
*/
PCB::PCB(Device *devices, int numDevices)
{
    memcpy(_devices, devices, sizeof(Device));
    _numDevices = numDevices;
    _numDrivers = 0;
    for (int i = 0; i < _numDevices; i++)
    {
        _numDrivers = _numDrivers + _devices[i]._numDrivers;
    }
    _pwmbuffer = (uint16_t *)malloc(2 * 24 * _numDrivers);
    memset(_pwmbuffer, 0, 2 * 24 * _numDrivers);
    _outputArray = (uint16_t *)malloc(24 * _numDrivers);
    memset(_outputArray, 0, 24 * _numDrivers);
}

/*!
*   @brief setups the pinouts and the display or the serial interface
*
*/
void PCB::begin()
{
    pinMode(_clk, OUTPUT);
    pinMode(_dat, OUTPUT);
    pinMode(_lat, OUTPUT);
    digitalWrite(_lat, LOW);
    Serial.begin(9600);
    Serial.println("Commands (the text between '' represents parameters that should be added)");
    Serial.println("!ShowInfo CR LF    -- Shows information of all the Devices connected");
    Serial.println("!SetTopo 'id' 'topo' CR LF  -- Change the topology of the selected device");
    Serial.println("!SetTopos 'topo1' 'topo2' ... 'topon'CR LF  -- Change the topology of all the devices");
    Serial.println("!SetABC 'id' 'A', 'B', 'C' CR LF  -- Changes the parameters A, B and C of the selected device");
    Serial.println("!SetLinear 'id' 'pwmInit', 'pwmFin' CR LF  -- Changes initial and final pwm values for a linear progression of the selected device");
    Serial.println("!SendPWM 'id' 'PWM1' 'PWM2' ... 'PWMn' CR LF  -- Manually set the PWM values [0-4095] for each electrode");
    Serial.println("!SendDevice 'id' CR LF -- This command resends the PWM duty cycle data to the selected device");
    Serial.println("CR meaning Carriage Return, and LF meaning Line Feed");
}

/*!
*   @brief writes pwm data to connected boards
*
*/
void PCB::write()
{
    digitalWrite(_lat, LOW);
    for (int16_t c = 24 * _numDrivers - 1; c >= 0; c--)
    {
        for (int8_t b = 11; b >= 0; b--)
        {
            digitalWrite(_clk, LOW);

            if (_pwmbuffer[c] & (1 << b))
                digitalWrite(_dat, HIGH);
            else
                digitalWrite(_dat, LOW);

            digitalWrite(_clk, HIGH);
        }
    }
    digitalWrite(_clk, LOW);

    digitalWrite(_lat, HIGH);
    digitalWrite(_lat, LOW);
}

/*!
*   @brief sets PWM value in channels
*
*   @param chan
*           channel, from [0-23]
*
*   @param pwm
*           pwm value from [0-4095]
*
*/
void PCB::setPWM(uint16_t chan, uint16_t pwm)
{
    if (pwm < 0)
        pwm = 0;
    if (pwm > 4095)
        pwm = 4095;
    if (chan > 24 * _numDrivers)
        return;
    _pwmbuffer[chan] = 4095 - pwm;
    Serial.print("PWM sent at channel ");
    Serial.print(chan);
    Serial.print(" = ");
    Serial.println(_pwmbuffer[chan]);
}