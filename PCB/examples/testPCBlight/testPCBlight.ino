#include "PCB.h"
#include <SPI.h>
#include <stdio.h>
#include <stdlib.h>

#define LIST

#define id_1 1
#define n_e_1 24
#define topo_1 1
#define f1_1 0.3

#define id_2 2
#define n_e_2 48
#define topo_2 5
#define f1_2 0.45

double A_1 = 17;
double B_1 = -5.38;
double C_1 = 0.15;

double A_2 = 17;
double B_2 = -5.38;
double C_2 = 0.15;

uint16_t numDigits = 6;
uint32_t freqSGLo[] = {0, 5, 0, 0, 0, 0};

Device Device1 = Device(id_1, n_e_1, A_1, B_1, C_1, topo_1, f1_1);
Device Device2 = Device(id_2, n_e_2, A_2, B_2, C_2, topo_2, f1_2);
const int numDevices = 2;

Device devices[numDevices] = {Device1, Device2};

PCB board1 = PCB(devices, numDevices);
CommandHandler<10, 250, 10> SerialCommandHandler;

//AD9833 ad = AD9833(p_SDATA, p_SCLK, p_FSYNC, numDigits);

void setup()
{
  //ad.setFreq(freqSGLo);
  //ad.begin();
  SerialCommandHandler.AddCommand(F("ShowInfo"), Cmd_ShowInfo);
  SerialCommandHandler.AddCommand(F("SetTopo"), Cmd_SetTopo);
  SerialCommandHandler.AddCommand(F("SetTopos"), Cmd_SetTopos);
  SerialCommandHandler.AddCommand(F("SetABC"), Cmd_SetABC);
  SerialCommandHandler.AddCommand(F("SetLinear"), Cmd_SetLinear);
  SerialCommandHandler.AddCommand(F("SendPWM"), Cmd_SendPWM);
  SerialCommandHandler.AddCommand(F("SendDevice"), Cmd_SendDevice);
  SerialCommandHandler.SetDefaultHandler(Cmd_Unknown);
  board1.begin();
}

void loop()
{
  SerialCommandHandler.Process();
}

void Cmd_ShowInfo(CommandParameter &parameter)
{
  for (int i = 0; i < board1._numDevices; i++)
  {
    Serial.print("Device nº: ");
    Serial.println(board1._devices[i]._id);
    Serial.print(board1._devices[i]._numElectrodes);
    Serial.println(" electrodes");
    Serial.print("Parameters: ");
    Serial.print("A = ");
    Serial.print(board1._devices[i]._A);
    Serial.print(" B = ");
    Serial.print(board1._devices[i]._B);
    Serial.print(" C = ");
    Serial.println(board1._devices[i]._C);
    Serial.print("Topology: ");
    Serial.println(board1._devices[i]._topo);
  }
}

void Cmd_SetTopo(CommandParameter &parameter)
{
  double id = parameter.NextParameterAsDouble();
  double topo = parameter.NextParameterAsDouble();
  int j;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
    }
  }
  board1._devices[j].setTopo(topo);
  Serial.print("Device nº: ");
  Serial.println(board1._devices[j]._id);
  Serial.print("Topology: ");
  Serial.println(board1._devices[j]._topo);
}

void Cmd_SetTopos(CommandParameter &parameter)
{
  double topo;
  for (int i = 0; i < board1._numDevices; i++)
  {
    topo = parameter.NextParameterAsDouble();
    board1._devices[i].setTopo(topo);
    Serial.print("Device nº: ");
    Serial.println(board1._devices[i]._id);
    Serial.print("Topology: ");
    Serial.println(board1._devices[i]._topo);
  }
}

void Cmd_SetABC(CommandParameter &parameter)
{
  double id = parameter.NextParameterAsDouble();
  double A = parameter.NextParameterAsDouble();
  double B = parameter.NextParameterAsDouble();
  double C = parameter.NextParameterAsDouble();
  int j;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
    }
  }
  board1._devices[j].setABC(A, B, C);
  Serial.print("Device nº: ");
  Serial.println(board1._devices[j]._id);
  Serial.print("A: ");
  Serial.println(board1._devices[j]._A);
  Serial.print("B: ");
  Serial.println(board1._devices[j]._B);
  Serial.print("C: ");
  Serial.println(board1._devices[j]._C);
}

void Cmd_SetLinear(CommandParameter &parameter)
{
  double id = parameter.NextParameterAsDouble();
  double pwmInit = parameter.NextParameterAsDouble();
  double pwmFin = parameter.NextParameterAsDouble();
  int j;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
    }
  }
  board1._devices[j].setInitFin(pwmInit, pwmFin);
  Serial.print("Device nº: ");
  Serial.println(board1._devices[j]._id);
  Serial.print("pwmInit: ");
  Serial.println(board1._devices[j]._pwmInit);
  Serial.print("pwmFin: ");
  Serial.println(board1._devices[j]._pwmFin);
}

void Cmd_SendPWM(CommandParameter &parameter)
{
  double id = parameter.NextParameterAsDouble();
  uint16_t pwm;
  int j;
  int prevElectrodes = 0;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
      break;
    }
    prevElectrodes += board1._devices[i]._numElectrodes;
  }
  for (int k = 0; k < board1._devices[j]._numDrivers * 24; k++)
  {
    pwm = parameter.NextParameterAsInteger();
    if (pwm > 50000)
    {
      pwm = 0;
    }
    board1.setPWM(k + prevElectrodes, pwm);
  }
  board1.write();
}

void Cmd_SendDevice(CommandParameter &parameter)
{
  double id = parameter.NextParameterAsDouble();
  int j;
  int prevElectrodes = 0;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
      break;
    }
    prevElectrodes += board1._devices[i]._numElectrodes;
  }

  board1._devices[j].generate_profile();
  Serial.print("ID: ");
  Serial.println(board1._devices[j]._id);
  Serial.print("numDrivers: ");
  Serial.println(board1._devices[j]._numDrivers);
  Serial.print("numElectrodes: ");
  Serial.println(board1._devices[j]._numElectrodes);
  for (int k = 0; k < board1._devices[j]._numElectrodes; k++)
  {
    board1.setPWM(k + prevElectrodes, board1._devices[j]._outputArray[k]);
  }
  board1.write();
}

void Cmd_Unknown()
{
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