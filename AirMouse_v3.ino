#include <SoftwareSerial.h>// import the serial library
#include<Wire.h>

SoftwareSerial mySerial(14, 15); // RX, TX
int LEFT=16; // Mouse LEFT click
int RIGHT=9; // Mouse RIGHT click

const int MPU_addr=0x68;// 12C address of th
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int X, Y, L, R;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.beginTransmission (MPU_addr);
  Wire.write (0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission (true);

  pinMode(LEFT,INPUT);
  pinMode(RIGHT,INPUT);
  
  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop() {

  char cX[4];
  char cY[4];
  char cL[4];
  char cR[4];
  char dest[10];
  
  Wire.beginTransmission (MPU_addr);
  Wire.write (0x3B); // starting with register Ox3B (ACCEL_XOUT_H)
  Wire.endTransmission (false);
  Wire.requestFrom (MPU_addr, 14, true); // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read(); // Ox 3B (ACCEL_XOUT_H) : Ox3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read(); // Ox3D (ACCEL_YOUT_H) s OX3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read(); // 0x3F (RCCEL_ZOUT_M) 0x40 (ACCEL_ZOUT_I)
  Tmp=Wire.read()<<8|Wire.read();// 0x41 (TEMP OUT) 50x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read(); //0x43 (GYRO XOUT # $ 0x44 (GYRO XOUT L)
  GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO YOUTH) 50x46 (GYRO YOUT L)
  GyZ=Wire.read()<<8|Wire.read(); // Ox 47 GYRO_20UT_$) 50x45 (GYRO_Z0UT_L)

  X = -(GyZ/100)+2;//+700; //-(GyZ-275)/10
  Y = (GyX/200)+2;//+400; //(GyX+475)/10
  if (X >= -2 && X <= 2)
    X = 0;
  if (Y >= -2 && Y <= 2)
    Y = 0;

  if(digitalRead(LEFT) == HIGH) {
    L = 1;
  } else {
    L = 0;
  }

  if(digitalRead(RIGHT) == HIGH) {
    R = 1;
  } else {
    R = 0;
  }
  
  itoa(X, cX, 10);
  itoa(Y, cY, 10);
  itoa(L, cL, 10);
  itoa(R, cR, 10);

  strcpy(dest, cX);
  strcat(dest, ",");
  strcat(dest, cY);
  strcat(dest, ",");
  strcat(dest, cL);
  strcat(dest, ",");
  strcat(dest, cR);

  mySerial.println(dest);
  Serial.print(dest);
  Serial.println();

delay(10);// prepare for next data ...
}
