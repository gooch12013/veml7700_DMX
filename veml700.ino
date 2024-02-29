#include <TimerOne.h>
#include <SoftwareSerial.h>
#include <Adafruit_VEML7700.h>
#include <DmxMaster.h>

#define DMX_MAX 512 // Maximum number of DMX channels (adjust this value based on your setup)


Adafruit_VEML7700 veml = Adafruit_VEML7700();

int time = 200;
String command;

// Function prototypes
void sendLux();
void setGain(int gain);
void setIntegrationTime(uint16_t integrationTime);
void getGain();
void getIntegrationTime();
void processDmxMessage(String message);

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  Serial.println("Adafruit VEML7700 Auto Lux Test");

  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");
}

void loop() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    
    if (command.startsWith("dmxao")) {
      allDmxChannelsOff();
    }
    else if (command.startsWith("gain:")) {
      int gainValue = command.substring(5).toInt();
      setGain(gainValue);
    } else if (command.startsWith("it:")) {
      int itValue = command.substring(3).toInt();
      setIntegrationTime(itValue);
    } else if (command == "lux") {
      sendLux();
    } else if (command == "getg") {
      getGain();
    } else if (command == "getit") {
      getIntegrationTime();
    } else if (command.startsWith("dmx:")) {
      processDmxMessage(command);
    }
  }
}

void sendLux() {
  float lux = veml.readLux();
  Serial.println("Lux:" + String(lux));
}

void setGain(int gain) {
  switch(gain) {
    case 1:
      veml.setGain(VEML7700_GAIN_1);
      Serial.println("gain:VEML7700_GAIN_1");
      break;
    case 2:
      veml.setGain(VEML7700_GAIN_2);
      Serial.println("gain:VEML7700_GAIN_2");
      break;
    case 18:
      veml.setGain(VEML7700_GAIN_1_8);
      Serial.println("gain:VEML7700_GAIN_1_8");
      break;
    case 14:
      veml.setGain(VEML7700_GAIN_1_4);
      Serial.println("gain:VEML7700_GAIN_1_4");
      break;
    default:
      Serial.println("gain:0");
  }
}

void setIntegrationTime(uint16_t integrationTime) {
  switch(integrationTime) {
    case 25:
      veml.setIntegrationTime(VEML7700_IT_25MS);
      Serial.println("it:25");
      break;
    case 50:
      veml.setIntegrationTime(VEML7700_IT_50MS);
      Serial.println("it:50");
      break;
    case 100:
      veml.setIntegrationTime(VEML7700_IT_100MS);
      Serial.println("it:100");
      break;
    case 200:
      veml.setIntegrationTime(VEML7700_IT_200MS);
      Serial.println("it:200");
      break;
    case 400:
      veml.setIntegrationTime(VEML7700_IT_400MS);
      Serial.println("it:400");
      break;
    case 800:
      veml.setIntegrationTime(VEML7700_IT_800MS);
      Serial.println("it:800");
      break;
    default:
      Serial.println("it:0");
  }
}

void getGain() {
  Serial.print(F("gain:"));
  switch (veml.getGain()) {
    case VEML7700_GAIN_1: Serial.println("1"); break;
    case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }
}

void getIntegrationTime() {
  Serial.print(F("it:"));
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS: Serial.println("25"); break;
    case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    case VEML7700_IT_200MS: Serial.println("200"); break;
    case VEML7700_IT_400MS: Serial.println("400"); break;
    case VEML7700_IT_800MS: Serial.println("800"); break;
  }
}

void processDmxMessage(String message) {
  // Example: dmx:1:25
  int colonIndex1 = message.indexOf(':');
  int colonIndex2 = message.indexOf(':', colonIndex1 + 1);
  
  if (colonIndex1 != -1 && colonIndex2 != -1 && message.length() >= colonIndex2 + 1) {
    String channelStr = message.substring(colonIndex1 + 1, colonIndex2);
    String valueStr = message.substring(colonIndex2 + 1);
    
    // Remove leading and trailing whitespaces
    channelStr.trim();
    valueStr.trim();

    // Convert string representations to integers
    int channel = channelStr.toInt();
    int value = valueStr.toInt();
    
    DmxMaster.write(channel, value);
    Serial.println("DMX message processed: Channel " + String(channel) + ", Value " + String(value));
  } else {
    Serial.println("Invalid DMX message format");
  }
}

void allDmxChannelsOff() {
  for (int channel = 1; channel <= DMX_MAX; ++channel) {
    DmxMaster.write(channel, 0);  // Set channel value to 0
  }
  Serial.println("All DMX channels turned off");
}
