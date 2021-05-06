/*  Arduino Library developed for a Degree Thesis at CEMDATIC, ETSIT, UPM
    Developed by Pablo de la Rosa. Directed by Morten A. Geday

    
    Aimed at controlling dynamic multilevel spiral phase liquid crystal lenses
    It is all based on generatic PWM signals sent to electrodes that align cells of Liquid Crystal
    These cells have characterized to generate a spiral phase delay with different topologies
    The duty cicle of each PWM generated is obtained from delta = A*e^(-B*dc) + C, where A, B and C are constants
    This Arduino Library allows you to generate an array of output PWM signals based on the parameters A, B and C, and the desired topology
*/

#ifndef Device_h
#define Device_h

#include <Arduino.h>
#include <math.h>

class Device
{
public:
    Device(uint8_t id, uint16_t num_electrodes, double A, double B, double C, uint8_t topo, uint8_t F1);
#ifdef faulty
    void faulty(uint16_t *faultyArray);
#endif
    void generate_profile();
    void setTopo(uint8_t topo);
    void setABC(double A, double B, double C);
    void setA(double A);
    void setB(double B);
    void setC(double C);
    void setInitFin(double dInit, double dFin);

    uint8_t _id;
    int _profile;
    uint16_t _numElectrodes;
    double _A;
    double _B;
    double _C;
    uint8_t _topo;
    uint8_t _diop;
    uint8_t _F1;
    uint16_t _numDrivers;
    double _pwmInit;
    double _pwmFin;
#ifdef faulty
    uint16_t _faultyArray[];
#endif
    uint16_t *_outputArray;
};

#endif