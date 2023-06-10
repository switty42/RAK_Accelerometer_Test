/*  RAK Accelerometer test 
    Minor changes to RAK test code by Stephen Witty
    Using RAK4630 dev platform with Accelerometer module
    Example taken from RAK documentation Center
    https://docs.rakwireless.com/Product-Categories/WisBlock/RAK1904/Quickstart/#software-configuration-and-example

    Adding support to blink the onboard LED when MCU is moved and detected by accelerometer

    V1 - 6-9-23 - Initial development
*/

/**
   @file RAK1904_Accelerate_LIS3DH.ino
   @author rakwireless.com
   @brief Setup and read values from a lis3dh sensor
   @version 0.1
   @date 2020-12-28
   @copyright Copyright (c) 2020
**/

#include <Arduino.h>
#include "SparkFunLIS3DH.h" //http://librarymanager/All#SparkFun-LIS3DH
#include <Wire.h>

LIS3DH SensorTwo(I2C_MODE, 0x18);

void setup()
{
	time_t timeout = millis();
	Serial.begin(115200);
	while (!Serial)
	{
		if ((millis() - timeout) < 5000)
    {
      delay(100);
    }
    else
    {
      break;
    }
	}

	if (SensorTwo.begin() != 0)
	{
		Serial.println("Problem starting the sensor at 0x18.");
	}
	else
	{
		Serial.println("Sensor at 0x18 started.");
		// Set low power mode
		uint8_t data_to_write = 0;
		SensorTwo.readRegister(&data_to_write, LIS3DH_CTRL_REG1);
		data_to_write |= 0x08;
		SensorTwo.writeRegister(LIS3DH_CTRL_REG1, data_to_write);
		delay(100);

		data_to_write = 0;
		SensorTwo.readRegister(&data_to_write, 0x1E);
		data_to_write |= 0x90;
		SensorTwo.writeRegister(0x1E, data_to_write);
		delay(100);
	}
	Serial.println("enter !");

  pinMode(LED_BUILTIN,OUTPUT);
}

bool first_run = true;
float x=0,y=0,z=0,x_old=0,y_old=0,z_old=0;

void loop()
{  
  x=SensorTwo.readFloatAccelX();
  y=SensorTwo.readFloatAccelY();
  z=SensorTwo.readFloatAccelZ();

  Serial.print("X = "); Serial.print(x,4);
  Serial.print("   Y = "); Serial.print(y,4);
  Serial.print("   Z = "); Serial.println(z,4);

  if (!first_run)
  {
    if ( compare(x,x_old) || compare(y,y_old) || compare(z,z_old))
    {
      digitalWrite(LED_BUILTIN,HIGH);
      delay(20);
      digitalWrite(LED_BUILTIN,LOW);
    }
  }
  else first_run=false;

  x_old=x;
  y_old=y;
  z_old=z;

	delay(200);
}

bool compare(float a, float b)
{
  if (abs(a-b) > .04) return true;
    else return false;
}