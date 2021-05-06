#include "Device.h"

//#define faulty //uncomment and create a faulty array of 1s and 0s depending on which electrodes work and call to function faulty(faultyArray)
#define serial

#define id_1 1
#define n_e_1 24
#define topo_1 1
#define f1_1 0.3

#define id_2 2
#define n_e_2 72
#define topo_2 5
#define f1_2 0.45

double A_1 = 17;
double B_1 = -5.38;
double C_1 = 0.15;

double A_2 = 17;
double B_2 = -5.38;
double C_2 = 0.15;

Device Device1 = Device(id_1, n_e_1, A_1, B_1, C_1, topo_1, f1_1);
Device Device2 = Device(id_2, n_e_2, A_2, B_2, C_2, topo_2, f1_2);

void setup()
{
  Serial.begin(9600);
  Device1.generate_profile();
  Device2.generate_profile(); //generates profile with a topology of topo_2 (5), with n_e_2 (72) electrodes and with A, B and C parameters, then sends the information to each of the electrodes
  //Device1.setTopo(2);         //generates profile with a new topology (2) on Device1
  //Device2.setC(0.16);         //generates profile with a new C (0.16) parameter on Device2
}

void loop()
{
}