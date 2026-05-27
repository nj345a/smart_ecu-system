#include <SPI.h>
#include <mcp_can.h>
#include <DHT.h>
#include <Wire.h>
#include <Servo.h>
#include <math.h>

// ---------------- Ultrasonic ----------------
#define TRIG 3
#define ECHO 4

// ---------------- DHT11 ----------------
#define DHTPIN 6
#define DHTTYPE DHT11

// ---------------- Servo ----------------
#define SERVO_PIN 9

// ---------------- Emergency Button ----------------
#define BUTTON 7

// ---------------- MPU6050 ----------------
#define MPU_ADDR 0x68

DHT dht(DHTPIN, DHTTYPE);

Servo airbag;

const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);

bool airbagDeployed = false;

int16_t rawX, rawY, rawZ;
float accelX, accelY, accelZ;

void setup()
{
  Serial.begin(115200);

  // Ultrasonic
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // DHT11
  dht.begin();

  // Servo
  airbag.attach(SERVO_PIN);
  airbag.write(0);

  // Button
  pinMode(BUTTON, INPUT_PULLUP);

  // MPU6050 Init
  Wire.begin();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);

  if (Wire.endTransmission() == 0)
  {
    Serial.println("MPU6050 READY");
  }
  else
  {
    Serial.println("MPU6050 ERROR");

    while(1);
  }

  // CAN Init
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ))
  {
    Serial.println("CAN init failed");
    delay(100);
  }

  Serial.println("CAN init ok!");

  CAN.setMode(MCP_NORMAL);
}

void loop()
{
  // =================================================
  // OBSTACLE ECU
  // =================================================

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);

  int distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  if(distance < 20)
  {
    byte data1[8] = {'O','B','S','T','A','C','L','E'};

    CAN.sendMsgBuf(0x01, 0, 8, data1);

    Serial.println("Obstacle Alert Sent");
  }

  // =================================================
  // TEMPERATURE ECU
  // =================================================

  float temp = dht.readTemperature();

  if(!isnan(temp))
  {
    Serial.print("Temperature: ");
    Serial.println(temp);

    // Increased threshold
    if(temp > 40)
    {
      byte data2[3] = {'H','O','T'};

      CAN.sendMsgBuf(0x02, 0, 3, data2);

      Serial.println("Temperature Alert Sent");
    }
  }
  else
  {
    Serial.println("DHT11 Error");
  }

  // =================================================
  // CRASH ECU
  // =================================================

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_ADDR, 6, true);

  rawX = Wire.read() << 8 | Wire.read();
  rawY = Wire.read() << 8 | Wire.read();
  rawZ = Wire.read() << 8 | Wire.read();

  accelX = (rawX / 16384.0) * 9.81;
  accelY = (rawY / 16384.0) * 9.81;
  accelZ = (rawZ / 16384.0) * 9.81;

  float totalAccel =
  sqrt(
    accelX * accelX +
    accelY * accelY +
    accelZ * accelZ
  );

  Serial.print("Total Acceleration: ");
  Serial.println(totalAccel);

  // More sensitive threshold
  if(totalAccel > 15 && !airbagDeployed)
  {
    Serial.println("CRASH DETECTED");

    // Deploy airbag
    airbag.write(180);

    airbagDeployed = true;

    byte data3[5] = {'C','R','A','S','H'};

    CAN.sendMsgBuf(0x03, 0, 5, data3);

    Serial.println("Crash Alert Sent");
  }

  // =================================================
  // EMERGENCY BUTTON ECU
  // =================================================

  if(digitalRead(BUTTON) == LOW)
  {
    Serial.println("EMERGENCY BUTTON PRESSED");

    byte data4[3] = {'S','O','S'};

    CAN.sendMsgBuf(0x04, 0, 3, data4);

    Serial.println("SOS Alert Sent");

    delay(1000);
  }

  delay(1000);
}