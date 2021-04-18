/* created by @DjamikTea */

#include <Adafruit_NeoPixel.h>
#define PIN 2   //GPIO pin
#define LED_COUNT 48  //led count
#define SSID ssid    //name wi-fi
#define PSK psk  // password
#include "ESP_Wahaj.h" // importing our library
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
int ledMode;
#define MAXSECTIONCOUNT 50
String sections[MAXSECTIONCOUNT];
String path = "nothing";
int startx;
int schet = 0;
int trig = 0;

void parseStringFromSerial() {
  int sectionIndex = 0;
  String section;

  while (Serial.available() > 0) {
    delay(1);
    char incomingChar = Serial.read();
    if (incomingChar == ' ' || incomingChar == '\n') {
      sections[sectionIndex] = section;
      section = "";
      ++sectionIndex;
    } else {
      section += incomingChar;
    }
  }
}

void clearSections() {
  for (int i = 0; i < MAXSECTIONCOUNT; ++i)
    sections[i] = "";
}

void printSections() {
  for (int i = 0; i < MAXSECTIONCOUNT; ++i) {
    Serial.print("sections[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(sections[i]);
  }
}

void colorSeter(String hexValue, int j)
{
  String hexstring = hexValue;
  long number = (long) strtol( &hexstring[0], NULL, 16);
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  //Serial.println(r);
  //Serial.println(g);
  //Serial.println(b);
  pixels.setPixelColor(j, pixels.Color(r, g, b));
}
void setup() {
  Serial.begin(115200);
  start("SSID", "PSK"); // Wifi details connect tox
  pixels.begin();
  pixels.show();
}

void loop() {
  if (CheckNewReq() == 1)
  {
    path = getPath();
    path.remove(0, 1);   //Remove slash /
    Serial.println(path);
    int sectionIndex = 0;
    int index = 0;
    String section;

    while (path[index]) {
      char incomingChar = path[index];
      if (incomingChar == '_' || incomingChar == '\n') {
        sections[sectionIndex] = section;
        section = "";
        ++sectionIndex;
        ++index;

      } else {
        section += incomingChar;
        ++index;
      }
    }
  }
  if (Serial.available() > 0) {
    startx = millis();
    parseStringFromSerial();
    //printSections();
  }
  if (sections[0] != NULL)
  {
    startx = millis();
    if (sections[0] == "clear")
    {
      pixels.clear();
      schet = 0;
    }
    else if (sections[0] == "set") {     //put a report point when you fill in
      schet = sections[1].toInt();
    }
    else if (sections[0] == "pixel") {   // set color pixel <num> <hex_color>
      colorSeter(sections[2], sections[1].toInt());
    }
    else if (sections[0] == "brigh") {
      pixels.setBrightness(sections[1].toInt());
    }
    else
    {
      while (sections[trig] != "") // seting colors
      {
        colorSeter(sections[trig], schet);
        schet++;
        trig++;
        //Serial.println(schet);
      }
      trig = 0;
      if (schet >= 48)
      {
        schet = 0;
      }
    }
    pixels.show();
    clearSections();
    Serial.println(millis() - startx);
  }
}
