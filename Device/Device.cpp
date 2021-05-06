/*  Arduino Library developed for a Degree Thesis at CEMDATIC, ETSIT, UPM
    Developed by Pablo de la Rosa. Directed by Morten A. Geday


    Aimed at controlling dynamic multilevel sM_piral phase liquid crystal lenses
    It is all based on generatic PWM signals sent to electrodes that align cells of Liquid Crystal
    These cells have characterized to generate a sM_piral phase delay with different topologies
    The duty cicle of each PWM generated is obtained from delta = A*e^(-B*dc) + C, where A, B and C are constants
    This Arduino Library allows you to generate an array of output PWM signals based on the parameters A, B and C, and the desired topology


    Some parts of it are adapted from Adafruit's Adafruit_Device5947 library:
    https://learn.adafruit.com/Device5947-Device59711-pwm-led-driver-breakout/programming?embeds=allow

*/

#include "Device.h"

/*!
*   @brief Instantiates a new Device class
*   
*   @param id
*           identifier for using several Devices
*
*   @param numElectrodes
*           number of electrodes controlled by the device
*
*   @param A
*           parameter A from formula delta = A*e^(-B*dc) + C
*
*   @param B
*           parameter B from formula delta = A*e^(-B*dc) + C
*
*   @param C
*           parameter C from formula delta = A*e^(-B*dc) + C
*
*   @param topo
*           topology of the lense
*
*   @param F1
*           focal distance in meters
*
*   @param clk
*           SM_PI clk M_pin 
*
*   @param dat
*           SM_PI data M_pin
*
*   @param lat
*           SM_PI latch M_pin
*
*/
Device::Device(uint8_t id, uint16_t numElectrodes, double A, double B, double C, uint8_t topo, uint8_t F1)
{
    _id = id;
    _numElectrodes = numElectrodes;
    _A = A;
    _B = B;
    _C = C;
    _topo = topo;
    _diop = _topo / _F1;
    _numDrivers = _numElectrodes / 24;
    _outputArray = (uint16_t *)malloc(_numElectrodes);
    _profile = 1;
    memset(_outputArray, 0, _numElectrodes);
}

#ifdef faulty
void Device::faulty(uint16_t *faultyArray[])
{
    memcpy(_faultyArray, faultyArray, _numElectrodes * sizeof(uint16_t));
}
#endif

/*!
*   @brief generates outputArray with duty cicles for PWM based on delta from [pi, 3pi]
*/
void Device::generate_profile()
{
    double div = (_numElectrodes / _topo);
    double d_i = 2* M_PI;
    double di = d_i / div; //delta_i divides the [PI, 3PI] into equidistant values
    double d = 0;
    double d_t = 0;
    double dc = 0;
    double t1 = 0;
    double t2 = 0;
    double t3 = 0;

    switch (_profile)
    {
    case 1:
        for (int i = 0; i < _numElectrodes; i++)
        {
            d_t = fmod(d, 2 * M_PI) + M_PI;
            if (_B >= 0)
            {
                t1 = -1 / _B;
                t2 = (d_t - _C) / _A;
                t3 = log(t2);
                dc = (t1)*t3;
            }
            else
            {
                t1 = 1 / _B;
                t2 = (d_t - _C) / _A;
                t3 = log(t2);
                dc = (t1)*t3;
            }
            d = d + di;
            _outputArray[i] = dc * 4095;
        }
        break;
    case 2:
        dc = _pwmInit;
        double pwm_i = (_pwmInit - _pwmFin)/div;
        for (int i = 0; i < _numElectrodes; i++)
        {   
            _outputArray[i] = dc;
            dc = dc + pwm_i;
        }
        break;
    default:
        break;
    }
#ifdef faulty
    for (int j = 0; j < _numElectrodes; j++)
    {
        _outputArray[j] = _outputArray[j] * _faultyArray[j];
    }
#endif
}

/*!
*   @brief updates the topology and generates a new profile
*
*   @param topo
*           topology of the lense
*
*/
void Device::setTopo(uint8_t topo)
{
    _topo = topo;
    _diop = _topo / _F1;
    generate_profile();
}

/*!
*   @brief updates the parameters A, B and C and generates a new profile
*
*   @param A
*           parameter A from formula delta = A*e^(-B*dc) + C
*
*   @param B
*           parameter B from formula delta = A*e^(-B*dc) + C
*
*   @param C
*           parameter C from formula delta = A*e^(-B*dc) + C
*
*/
void Device::setABC(double A, double B, double C)
{
    _A = A;
    _B = B;
    _C = C;
    _profile = 1;
    generate_profile();
}

void Device::setInitFin(double pwmInit, double pwmFin)
{
    _profile = 2;
    _pwmInit = pwmInit;
    _pwmFin = pwmFin;
    generate_profile();
}