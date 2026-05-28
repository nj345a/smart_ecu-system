#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include <SPI.h>
#include <mcp_can.h>

// ================= WIFI =================

const char* ssid = "YOUR WIFI";
const char* password = "PASSWORD";

// ================= TELEGRAM =================

#define BOTtoken "BOTTOKEN"

#define CHAT_ID "6617071258"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// ================= CAN =================

#define SPI_CS_PIN 5

MCP_CAN CAN(SPI_CS_PIN);

void setup()
{
  Serial.begin(115200);

  // WiFi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("WiFi Connected");

  client.setInsecure();

  // CAN
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ))
  {
    Serial.println("CAN init failed");
    delay(100);
  }

  Serial.println("CAN init ok!");

  CAN.setMode(MCP_NORMAL);

  bot.sendMessage(CHAT_ID,
  "🚗 Smart Vehicle Master ECU Online",
  "");
}

void loop()
{
  if (CAN_MSGAVAIL == CAN.checkReceive())
  {
    unsigned long rxId;
    byte len = 0;
    byte rxBuf[8];

    CAN.readMsgBuf(&rxId, &len, rxBuf);

    Serial.print("Received Alert: ");

    for (int i = 0; i < len; i++)
    {
      Serial.print((char)rxBuf[i]);
    }

    Serial.println();

    // =================================
    // OBSTACLE ECU
    // =================================

    if(rxId == 0x01)
    {
      Serial.println("Obstacle Detected");

      bot.sendMessage(CHAT_ID,
      "⚠️ Obstacle detected near vehicle",
      "");
    }

    // =================================
    // TEMPERATURE ECU
    // =================================

    if(rxId == 0x02)
    {
      Serial.println("Engine Overheat");

      bot.sendMessage(CHAT_ID,
      "🔥 Engine overheating detected",
      "");
    }

    // =================================
    // CRASH ECU
    // =================================

    if(rxId == 0x03)
    {
      Serial.println("Crash Detected");

      bot.sendMessage(CHAT_ID,
      "🚨 CRASH DETECTED!\nAirbag deployed.\nGPS: 10.8505, 76.2711",
      "");
    }

    // =================================
    // SOS ECU
    // =================================

    if(rxId == 0x04)
    {
      Serial.println("Emergency SOS");

      bot.sendMessage(CHAT_ID,
      "🆘 Emergency SOS Button Pressed!\nLocation: 10.8505, 76.2711",
      "");
    }

    Serial.println("--------------------------------");
  }
}
