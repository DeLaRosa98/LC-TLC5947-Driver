#include "PCB.h"
#include <SPI.h>

#define LIST

#define p_XLAT 7
#define p_SCLK 8
#define p_SDATA 9
#define p_FSYNC 5

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

uint16_t numDigits = 6;
uint32_t freqSGLo[] = {0, 5, 0, 0, 0, 0};

TLC tlc1 = TLC(id_1, n_e_1, A_1, B_1, C_1, topo_1, f1_1);

TLC devices[1] = {tlc1};

PCB board1 = PCB(devices);
CommandHandler<10, 30, 216> SerialCommandHandler;

//AD9833 ad = AD9833(p_SDATA, p_SCLK, p_FSYNC, numDigits);

void setup()
{
  //ad.setFreq(freqSGLo);
  //ad.begin();
  SerialCommandHandler.AddCommand(F("ShowInfo"), Cmd_ShowInfo);
  SerialCommandHandler.AddCommand(F("SetTopo"), Cmd_SetTopo);
  SerialCommandHandler.AddCommand(F("SetTopos"), Cmd_SetTopos);
  SerialCommandHandler.AddCommand(F("SetABC"), Cmd_SetABC);
  SerialCommandHandler.AddCommand(F("SetA"), Cmd_SetA);
  SerialCommandHandler.AddCommand(F("SetB"), Cmd_SetB);
  SerialCommandHandler.AddCommand(F("SetC"), Cmd_SetC);
  SerialCommandHandler.AddCommand(F("SendPWM"), Cmd_SendPWM);
  SerialCommandHandler.SetDefaultHandler(Cmd_Unknown);
  board1.begin();
  board1.blink();
}

void loop()
{
  SerialCommandHandler.Process();
}

void Cmd_ShowInfo(CommandParameter &parameter)
{
  for (int i = 0; i < board1._numDevices; i++)
  {
    Serial.println("------");
    Serial.print("Device nº: ");
    Serial.println(board1._devices[i]._id);
    Serial.print(board1._devices[i]._numElectrodes);
    Serial.println(" electrodes");
    Serial.print("Parameters: ");
    Serial.print("A = ");
    Serial.print(board1._devices[i]._A);
    Serial.print("B = ");
    Serial.print(board1._devices[i]._B);
    Serial.print("C = ");
    Serial.println(board1._devices[i]._C);
    Serial.print("Topology: ");
    Serial.println(board1._devices[i]._topo);
  }
  Serial.println("------");
  board1.blink();
}

void Cmd_SetTopo(CommandParameter &parameter)
{
  uint8_t id = parameter.NextParameterAsInteger();
  uint16_t topo = parameter.NextParameterAsInteger();
  int j;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
    }
  }
  board1._devices[j].setTopo(topo);
  Serial.println("------");
  Serial.print("Device nº: ");
  Serial.println(board1._devices[j]._id);
  Serial.print("Topology: ");
  Serial.println(board1._devices[j]._topo);
  Serial.println("------");
  board1.blink();
}

void Cmd_SetTopos(CommandParameter &parameter)
{
  uint16_t topo;
  for (int i = 0; i < board1._numDevices; i++)
  {
    topo = parameter.NextParameterAsInteger();
    board1._devices[i].setTopo(topo);
    Serial.println("------");
    Serial.print("Device nº: ");
    Serial.println(board1._devices[i]._id);
    Serial.print("Topology: ");
    Serial.println(board1._devices[i]._topo);
  }
  Serial.println("------");
  board1.blink();
}

void Cmd_SetABC(CommandParameter &parameter)
{
  uint8_t id = parameter.NextParameterAsInteger();
  uint16_t A = parameter.NextParameterAsInteger();
  uint16_t B = parameter.NextParameterAsInteger();
  uint16_t C = parameter.NextParameterAsInteger();
  int j;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
    }
  }
  board1._devices[j].setABC(A, B, C);
  Serial.println("------");
  Serial.print("Device nº: ");
  Serial.println(board1._devices[j]._id);
  Serial.print("A: ");
  Serial.println(board1._devices[j]._A);
  Serial.print("B: ");
  Serial.println(board1._devices[j]._B);
  Serial.print("C: ");
  Serial.println(board1._devices[j]._C);
  Serial.println("------");
  board1.blink();
}

void Cmd_SetA(CommandParameter &parameter)
{
  uint8_t id = parameter.NextParameterAsInteger();
  uint16_t A = parameter.NextParameterAsInteger();
  int j;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
    }
  }
  board1._devices[j].setA(A);
  Serial.println("------");
  Serial.print("Device nº: ");
  Serial.println(board1._devices[j]._id);
  Serial.print("A: ");
  Serial.println(board1._devices[j]._A);
  Serial.println("------");
  board1.blink();
}

void Cmd_SetB(CommandParameter &parameter)
{
  uint8_t id = parameter.NextParameterAsInteger();
  uint16_t B = parameter.NextParameterAsInteger();
  int j;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
    }
  }
  board1._devices[j].setB(B);
  Serial.println("------");
  Serial.print("Device nº: ");
  Serial.println(board1._devices[j]._id);
  Serial.print("B: ");
  Serial.println(board1._devices[j]._B);
  Serial.println("------");
  board1.blink();
}

void Cmd_SetC(CommandParameter &parameter)
{
  uint8_t id = parameter.NextParameterAsInteger();
  uint16_t C = parameter.NextParameterAsInteger();
  int j;
  for (int i = 0; i < board1._numDevices; i++)
  {
    if (board1._devices[i]._id == id)
    {
      j = i;
    }
  }
  board1._devices[j].setC(C);
  Serial.println("------");
  Serial.print("Device nº: ");
  Serial.println(board1._devices[j]._id);
  Serial.print("C: ");
  Serial.println(board1._devices[j]._C);
  Serial.println("------");
  board1.blink();
}

void Cmd_SendPWM(CommandParameter &parameter)
{
  uint16_t pwm;
  Serial.println("------");
  for (int i = 0; i < board1._numDrivers * 24; i++)
  {
    pwm = parameter.NextParameterAsInteger();
    board1.setPWM(i, pwm);
    Serial.print("PWM ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(pwm);
  }

  Serial.println("------");
  board1.blink();
  board1.write();
}

void Cmd_Unknown()
{
  Serial.println("The command sent was not recognized");
  Serial.println("Here is a list of the commands (the text between '' represents parameters that should be added)");
  Serial.println("Additionally all commands should end with CR LF");
  Serial.println("!ShowInfo CR LF    -- This command shows the information of all the TLC Devices connected");
  Serial.println("!SetTopo 'id' 'topo' CR LF  -- This command lets you change the topology of the device with its id");
  Serial.println("!SetTopos 'topo1' 'topo2' ... 'topon'CR LF  -- This command lets you change the topology of all the devices");
  Serial.println("!SetABC 'id' 'A', 'B', 'C' CR LF  -- This command lets you changes the parameters A, B and C of the device with its id");
  Serial.println("!SetA 'id' 'A' CR LF  -- This command lets you change the parameter A of the device with its id");
  Serial.println("!SetB 'id' 'B' CR LF  -- This command lets you change the parameter B of the device with its id");
  Serial.println("!SetC 'id' 'C' CR LF  -- This command lets you change the parameter C of the device with its id");
  Serial.println("!SendPWM 'PWM1' 'PWM2' ... 'PWMn' CR LF  -- This command lets you set manually the PWM values [0-4095] for each electrode");
  Serial.println("CR meaning Carriage Return, and LF meaning Line Feed");
  board1.blink();
}