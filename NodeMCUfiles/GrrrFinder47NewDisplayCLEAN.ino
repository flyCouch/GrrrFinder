
//#include <Adafruit_GFX.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#define TFT_CS        -1
#define TFT_RST        D2 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         D1 //D5 to sck, D7 to sda

#ifndef STASSID
#define STASSID "Put your SSID here"
#define STAPSK  "Put your Password here"
#endif

char *menu[7] = {"Grrr Finder", "Connect", "Clock", "DrawLines", "Theremin", "Calculator", "Abacus"};
const char* ssid     = STASSID;
const char* password = STAPSK;
int tone1;
int counter = 0;
int counter1 = 0;
int randNumber;
int randNumber1;
int randNumber2;
const int beepPin = D4;
const int rightButtonPin = D3;
const int leftButtonPin = D8;
int menuDial = 512;
bool statusright = HIGH;
bool statusleft = LOW;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup()   {
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  timeClient.begin();
  tft.init(240, 240, SPI_MODE3);           // Init ST7789 240x240
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
}

void loop() {
  menuDial = analogRead(A0);
  Serial.print(menuDial);
  Serial.print(" ");
  statusleft = digitalRead(leftButtonPin);
  Serial.print(statusleft);
  Serial.print(" ");
  statusright = digitalRead(rightButtonPin);
  Serial.println(statusright);
  menuDial = analogRead(A0);
  menuDial = map(menuDial, 0, 1024, 1, 7);
  switch (menuDial) {
    case 1:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      tft.print(" Grrr Finder       ");
      statusright = digitalRead(rightButtonPin);
      Serial.println (statusright);
      if (statusright == LOW) {
        GrrrFinder();
      }
      break;
    case 2:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
      tft.print("   Connect         ");
      statusright = digitalRead(rightButtonPin);
      if (statusright == LOW) {
        Connect();
      }
      break;
    case 3:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
      tft.print("    Clock          ");
      statusright = digitalRead(rightButtonPin);
      Serial.println (statusright);
      if (statusright == LOW) {
        findTime();
      }
      break;
    case 4:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
      tft.print("  DrawLines       ");
      // tft.tft();
      statusright = digitalRead(rightButtonPin);
      if (statusright == LOW) {
        drawline();
      }
      break;
    case 5:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
      tft.print("  Theremin       ");
      statusright = digitalRead(rightButtonPin);
      if (statusright == LOW) {
        Theremin();
      }
      break;
    case 6:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
      tft.print("  Calculator");
      break;
    case 7:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
      tft.print("   Abacus        ");
      break;
  }
}

void GrrrFinder() {

  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor(5, 65);
  tft.print("Grrr Location");
  tft.setCursor(0, 123);
  randNumber = random(359);
  tft.print(randNumber);
  tft.println(" bearing");
  randNumber1 = random(1000);
  tft.print(randNumber1);
  tft.println(" distance");
  randNumber2 = random(-10000, 10000);
  tft.print(randNumber2);
  tft.println(" elevation");
  analogWriteFreq(432);
  analogWrite(beepPin, 512);
  delay(107);
  noTone(beepPin);
  while (statusleft == LOW) {
    statusleft = digitalRead(leftButtonPin);
    statusright = digitalRead(rightButtonPin);
    yield();
    if (statusright == LOW) {
      GrrrFinder();
    }
  }
  tft.fillScreen(ST77XX_BLACK);
}

void Connect() {
  while (statusleft == LOW) {
    yield();
    statusleft = digitalRead(leftButtonPin);
    if ((WiFi.status() != WL_CONNECTED) && (counter == 0)) {
      counter = 1;
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.fillScreen(ST77XX_BLACK);
      tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
      tft.setCursor(0, 10);
      tft.print("Not Connected");
      Serial.println("Not Connected");
    }
    if ((WiFi.status() == WL_CONNECTED) && (counter1 == 0)) {
      counter1 = 1;
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.fillScreen(ST77XX_BLACK);
      //tft.setTextColor(ST77XX_WHITE);
      tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
      tft.setCursor(0, 10);
      tft.print("Connected");
      Serial.println("Connected");
    }
    while (WiFi.status() != WL_CONNECTED) {
      /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
        would try to act as both a client and an access-point and could cause
        network-issues with your other WiFi-devices on your WiFi-network. */
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println(".");
        tft.print(".");
      }
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      analogWriteFreq(432);
      analogWrite(beepPin, 500);
      delay(667);
      noTone(beepPin);
    }
  }
  counter = 0;
  counter1 = 0;
  tft.fillScreen(ST77XX_BLACK);
}

void findTime() {
  tft.fillScreen(ST77XX_BLACK);
  while (statusleft == LOW) {
    statusleft = digitalRead(leftButtonPin);
    Serial.println(statusleft);
    timeClient.update();
    timeClient.setTimeOffset(-4 * 60 * 60);
    Serial.println(timeClient.getFormattedTime());

    tft.setCursor(0, 70);
    tft.setTextSize(5);
    tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
    tft.println(timeClient.getFormattedTime());
    if (statusleft == LOW) {
      delay(1300);
    }
  }
  tft.fillScreen(ST77XX_BLACK);
}

void drawline() {
  do {

    for (int16_t i = 0; i < tft.width(); i += 4) {
      tft.drawLine(0, 0, i, tft.height() - 1, ST77XX_MAGENTA);
      //      tft.tft();
    }
    for (int16_t i = 0; i < tft.height(); i += 4) {
      tft.drawLine(0, 0, tft.width() - 1, i, ST77XX_GREEN);
      //tft.tft();
    }
    delay(250);


    for (int16_t i = 0; i < tft.width(); i += 4) {
      tft.drawLine(0, tft.height() - 1, i, 0, ST77XX_BLUE);
      //tft.tft();
    }
    for (int8_t i = tft.height() - 1; i >= 0; i -= 4) {
      tft.drawLine(0, tft.height() - 1, tft.width() - 1, i, ST77XX_RED);
      // tft.tft();
    }
    delay(250);


    for (int16_t i = tft.width() - 1; i >= 0; i -= 4) {
      tft.drawLine(tft.width() - 1, tft.height() - 1, i, 0, ST77XX_WHITE);
      // tft.tft();
    }
    for (int16_t i = tft.height() - 1; i >= 0; i -= 4) {
      tft.drawLine(tft.width() - 1, tft.height() - 1, 0, i, ST77XX_BLACK);
      // tft.tft();
    }
    delay(250);


    for (int16_t i = 0; i < tft.height(); i += 4) {
      tft.drawLine(tft.width() - 1, 0, 0, i, ST77XX_MAGENTA);
      // tft.tft();
    }
    for (int16_t i = 0; i < tft.width(); i += 4) {
      tft.drawLine(tft.width() - 1, 0, i, tft.height() - 1, ST77XX_RED);
      // tft.tft();
    }
    delay(250);
    statusleft = digitalRead(leftButtonPin);
    Serial.println(statusleft);
  } while (statusleft == LOW);
}

void drawchar() {
  tft.setTextSize(2);
  tft.setTextWrap(false);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor(30, 10);
  tft.println("Lyttle Research");
  tft.println("");
  tft.println("     Grrr Finder");
}

void Theremin() {
  statusleft = digitalRead(leftButtonPin);
  statusright = HIGH;
  tft.fillScreen(ST77XX_BLACK);
  while (statusleft == LOW) {
    tone1 = analogRead(A0);
    tone1 = map(tone1, 1, 1024, 100, 4000);
    tft.setTextSize(5);
    tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);;
    tft.setCursor(50, 20);
    tft.print("Tone");
    tft.setCursor(50, 130);
    tft.print(tone1);
    tft.print("   ");
    if (statusright == LOW) {
      analogWriteFreq(tone1);
      analogWrite(beepPin, 512);
      delay(107);
      noTone(beepPin);
    }
    statusright = digitalRead(rightButtonPin);
    statusleft = digitalRead(leftButtonPin);
    Serial.print (tone1);
    Serial.print(" ");
    Serial.print(statusleft);
    Serial.print(" ");
    Serial.println(statusright);
  }
  tft.fillScreen(ST77XX_BLACK);
}




