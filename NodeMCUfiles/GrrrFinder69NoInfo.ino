/*
  Copyright June 2024 Ron Lyttle.
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Brain.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#define TFT_CS        -1
#define TFT_RST        D2 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         D1 //D5 to sck, D7 to sda

#ifndef STASSID
#define STASSID "your SSID"
#define STAPSK  "your Password"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;
int tone1;
int counter = 0;
int counter1 = 0;
int randNumber;
int randNumber1;
int randNumber2;
int sensorData[12];
int maxYValues[12]; // Array to store maximum Y values for each graph
int menuDial = 512;
int Fib = 0;
const int beepPin = D4;
const int rightButtonPin = D3;
const int leftButtonPin = D8;
bool statusright = HIGH;
bool statusleft = LOW;
String scFlag[12];

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Set up the brain reader, pass it the software serial object you want to listen on.
Brain brain(Serial);

void setup()   {
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  timeClient.begin();
  tft.init(240, 240, SPI_MODE3);           // Init ST7789 240x240
  tft.setRotation(2);
  tft.fillScreen(ST77XX_ORANGE);

  maxYValues[0] = 1000;
  maxYValues[1] = 200;
  maxYValues[2] = 100;
  maxYValues[3] = 200;
  maxYValues[4] = 900000;
  maxYValues[5] = 900000;
  maxYValues[6] = 50000;
  maxYValues[7] = 50000;
  maxYValues[8] = 20000;
  maxYValues[9] = 20000;
  maxYValues[10] = 20000;
  maxYValues[11] = 20000;
}

void loop() {
  menuDial = analogRead(A0);
  //Serial.print(menuDial);
  //Serial.print(" ");
  statusleft = digitalRead(leftButtonPin);
  //Serial.print(statusleft);
  //Serial.print(" ");
  statusright = digitalRead(rightButtonPin);
  //Serial.println(statusright);
  menuDial = analogRead(A0);
  menuDial = map(menuDial, 0, 1023, 1, 6);
  switch (menuDial) {
    case 1:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLUE);
      tft.print(" Grrr Finder       ");
      statusright = digitalRead(rightButtonPin);
      //Serial.println (statusright);
      if (statusright == LOW) {
        GrrrFinder();
      }
      break;
    case 2:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_MAGENTA, ST77XX_GREEN);
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
      tft.setTextColor(ST77XX_RED, ST77XX_MAGENTA);
      tft.print("    Clock          ");
      statusright = digitalRead(rightButtonPin);
      //Serial.println (statusright);
      if (statusright == LOW) {
        findTime();
      }
      break;
    case 4:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_RED, ST77XX_BLUE);
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
      tft.setTextColor(ST77XX_BLACK, ST77XX_YELLOW);
      tft.print("  Theremin       ");
      statusright = digitalRead(rightButtonPin);
      if (statusright == LOW) {
        Theremin();
      }
      break;
    /*    case 6:
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
        case 8:
          drawchar();
          tft.setCursor(0, 130);
          tft.setTextSize(3);
          tft.setTextWrap(false);
          tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
          tft.print("    Chess     ");
          statusright = digitalRead(rightButtonPin);
          if (statusright == LOW) {
            Chess();
          }
          break;*/
    case 6:
      drawchar();
      tft.setCursor(0, 130);
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.setTextColor(ST77XX_GREEN, ST77XX_BLUE);
      tft.print("     EEG      ");
      statusright = digitalRead(rightButtonPin);
      if (statusright == LOW) {
        EEG();
      }
      break;
  }
}

void GrrrFinder() {
  tft.fillScreen(ST77XX_BLUE);
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLUE);
  tft.setCursor(5, 65);
  tft.print("Grrr Location");
  tft.setCursor(0, 123);
  randNumber = random(360);
  tft.print(randNumber);
  tft.println(" bearing");
  randNumber1 = random(999);
  tft.print(randNumber1);
  tft.println(" distance");
  randNumber2 = random(-999, 999);
  tft.print(randNumber2);
  tft.println(" elevation");
  tone(beepPin, 432);
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
  tft.fillScreen(ST77XX_ORANGE);
}

void Connect() {
  while (statusleft == LOW) {
    yield();
    statusleft = digitalRead(leftButtonPin);
    if ((WiFi.status() != WL_CONNECTED) && (counter == 0)) {
      counter = 1;
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.fillScreen(ST77XX_BLUE);
      tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLUE);
      tft.setCursor(0, 10);
      tft.print("Not Connected");
      //Serial.println("Not Connected");
    }
    statusleft = digitalRead(leftButtonPin);
    if ((WiFi.status() == WL_CONNECTED) && (counter1 == 0)) {
      counter1 = 1;
      tft.setTextSize(3);
      tft.setTextWrap(false);
      tft.fillScreen(ST77XX_BLUE);
      tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLUE);
      tft.setCursor(0, 10);
      tft.print("Connected");
      //Serial.println("Connected");
    }
    statusleft = digitalRead(leftButtonPin);
    while (WiFi.status() != WL_CONNECTED) {
      /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
        would try to act as both a client and an access-point and could cause
        network-issues with your other WiFi-devices on your WiFi-network. */
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        //Serial.println(".");
        tft.print(".");
      }
      statusleft = digitalRead(leftButtonPin);
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      tone(beepPin, 432);
      delay(109);
      noTone(beepPin);
    }
  }
  counter = 0;
  counter1 = 0;
  tft.fillScreen(ST77XX_ORANGE);
}

void findTime() {
  tft.fillScreen(ST77XX_MAGENTA);
  while (statusleft == LOW) {
    statusleft = digitalRead(leftButtonPin);
    Serial.println(statusleft);
    timeClient.update();
    timeClient.setTimeOffset(-5 * 60 * 60);
    //Serial.println(timeClient.getFormattedTime());

    tft.setCursor(0, 70);
    tft.setTextSize(5);
    tft.setTextColor(ST77XX_RED, ST77XX_MAGENTA);
    tft.println(timeClient.getFormattedTime());

    timeClient.setTimeOffset(-4 * 60 * 60);
    //Serial.println(timeClient.getFormattedTime());

    tft.setCursor(0, 135);
    tft.setTextSize(5);
    tft.setTextColor(ST77XX_RED, ST77XX_MAGENTA);
    tft.println(timeClient.getFormattedTime());
    statusleft = digitalRead(leftButtonPin);
  }
  tft.fillScreen(ST77XX_ORANGE);
}

void drawline() {
  while (statusleft == LOW) {
    for (int16_t i = 0; i < tft.width(); i += 4) {
      tft.drawLine(0, 0, i, tft.height() - 1, ST77XX_MAGENTA);
      //      tft.tft();
    }
    statusleft = digitalRead(leftButtonPin);
    for (int16_t i = 0; i < tft.height(); i += 4) {
      tft.drawLine(0, 0, tft.width() - 1, i, ST77XX_GREEN);
      //tft.tft();
    }
    statusleft = digitalRead(leftButtonPin);
    delay(250);
    for (int16_t i = 0; i < tft.width(); i += 4) {
      tft.drawLine(0, tft.height() - 1, i, 0, ST77XX_BLUE);
      //tft.tft();
    }
    statusleft = digitalRead(leftButtonPin);
    for (int8_t i = tft.height() - 1; i >= 0; i -= 4) {
      tft.drawLine(0, tft.height() - 1, tft.width() - 1, i, ST77XX_RED);
      // tft.tft();
    }
    statusleft = digitalRead(leftButtonPin);
    delay(250);
    for (int16_t i = tft.width() - 1; i >= 0; i -= 4) {
      tft.drawLine(tft.width() - 1, tft.height() - 1, i, 0, ST77XX_WHITE);
      // tft.tft();
    }
    statusleft = digitalRead(leftButtonPin);
    for (int16_t i = tft.height() - 1; i >= 0; i -= 4) {
      tft.drawLine(tft.width() - 1, tft.height() - 1, 0, i, ST77XX_BLACK);
      // tft.tft();
    }
    statusleft = digitalRead(leftButtonPin);
    delay(250);
    for (int16_t i = 0; i < tft.height(); i += 4) {
      tft.drawLine(tft.width() - 1, 0, 0, i, ST77XX_MAGENTA);
      // tft.tft();
    }
    statusleft = digitalRead(leftButtonPin);
    for (int16_t i = 0; i < tft.width(); i += 4) {
      tft.drawLine(tft.width() - 1, 0, i, tft.height() - 1, ST77XX_RED);
      // tft.tft();
    }
    statusleft = digitalRead(leftButtonPin);
    delay(250);
    tft.fillScreen(ST77XX_ORANGE);

  }
}

void drawchar() {
  tft.setTextSize(2);
  tft.setTextWrap(false);
  tft.setTextColor(ST77XX_GREEN, ST77XX_ORANGE);
  tft.setCursor(30, 10);
  tft.println("Lyttle Research");
  tft.println("");
  tft.println("     Grrr Finder");
}

void Theremin() {
  statusleft = digitalRead(leftButtonPin);
  statusright = HIGH;
  tft.fillScreen(ST77XX_YELLOW);
  while (statusleft == LOW) {
    tone1 = analogRead(A0);
    Fib = 1.618 * tone1;
    tone1 = map(Fib, 0, 1655.24, 0, 7007);
    tft.setTextSize(6);
    tft.setTextColor(ST77XX_BLACK, ST77XX_YELLOW);;
    tft.setCursor(50, 20);
    tft.print("Tone");
    tft.setCursor(50, 130);
    tft.print(tone1);
    tft.print("   ");
    statusright = digitalRead(rightButtonPin);
    while (statusright == LOW) {
      tone1 = analogRead(A0);
      tone1 = map(tone1, 0, 1023, 1, 7000);
      //tft.setTextSize(5);
      //tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);;
      tft.setCursor(50, 20);
      tft.print("Tone");
      tft.setCursor(50, 130);
      tft.print(tone1);
      tft.print("   ");
      tone(beepPin, tone1);
      statusright = digitalRead(rightButtonPin);
    }
    noTone(beepPin);
    statusleft = digitalRead(leftButtonPin);
  }
  tft.fillScreen(ST77XX_ORANGE);
}

/*
  void Chess() {
  tft.setTextSize(3);
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLUE);
  tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
  tft.setCursor(0, 10);
  tft.print("Chess");
  //Serial.println("Chess");
  delay (4321);
  tft.fillScreen(ST77XX_BLUE);
  /*
    Chess Notes

    For the Purpose of Writing

    An Alogrithm that Can Be

    Coded In A Computer Language

    CopyRight © Ron J. Lyttle

    December 27/2012



    void DeterminationOfProcedure () {
    }

    void InputOpponentsMove () {
    }

    void StartGame () {
    W/B?
    }

    void ResetProcedure () {
    }

    void OpeningProcedures () {
    }

    void MiddleGameProcedures () {
    Will a move capture a piece?
    Will a move risk a piece (1-3moves)?
    }

    void EndGameProcedures () {
    It is often useful to have the enemy King in check every move.
    }

    void CheckMateSearchProcedures() {
    }


    Overall Game Procedures

    void WhereCanPieceGoAndRate() {
    }

    void MistakeRejection () {
    }

    void ScanForHighValueAttacks () {
    }



    Use a pawn or lower value piece as point with lots of high level backup.

    Moves that also place the enemy in check may be beneficial.

    Attacking two pieces with one piece MBB.

    Having the rooks linked MBB.

    Make sure your pieces have freedom to move in the future if possible.
    Blocking the opponents freedom to move or choosing a move
    which leaves remaining a block that is already there.

    Squares in the center are more valuable with a decreasing value outwards.

    Defensive and offensive positional qualities will have to be observed, such as a line of pawns,
    and that they need to be taken out at the base,
    changing the value of that particular square.

    Pieces should go out as teams of two or more. Guarding each other as they travel,
    initially forward, then to chase.

    Controlling as many squares as possible, even ones not in use.
    The squares will have to be rated for value each turn.

    Does it link pawns?


    Other Notes

    This algorithim will not use memorized or rote procedures, more than i do,
    because that is what I want.
    Every move is to be reasoned out,
    using a imitation of my thinking procedures for chess.

    I wish for this to be fast. I will do this by not searching every move combination,
    which is time-consuming
    but by searching rationally for the best move.

    I will limit the algorithim to searching only 3 deep (?) for moves.
    This will limit the amount of moves the AI has to rate.

    Each factor in the decision making process will be given a statisical rate of value. Some of these
    values might vary because of links to other factors.
    Likely, the more factors and the
    better I can rate them, that will improve the performance of the AI.

    The goal is not necessarily to make the best chess engine
    but one that most accurately reflects my mind.
    Although with a mistake rejection system.
    And see how it does.

  }*/
void EEG() {
  tft.fillScreen(ST77XX_BLUE);
  while (statusleft == LOW) {
    menuDial = analogRead(A0);
    menuDial = map(menuDial, 0, 1023, 0, 11);
    for (int i = 0; i < 12; i++) {
      scFlag[i] = "ST77XX_GREEN";
    }
    switch (menuDial) {
      case 0:
        scFlag[0] = "ST77XX_RED";
        noTone(beepPin);
        break;
      case 1:
        scFlag[1] = "ST77XX_RED";
        break;
      case 2:
        scFlag[2] = "ST77XX_RED";
        break;
      case 3:
        scFlag[3] = "ST77XX_RED";
        break;
      case 4:
        scFlag[4] = "ST77XX_RED";
        break;
      case 5:
        scFlag[5] = "ST77XX_RED";
        break;
      case 6:
        scFlag[6] = "ST77XX_RED";
        break;
      case 7:
        scFlag[7] = "ST77XX_RED";
        break;
      case 8:
        scFlag[8] = "ST77XX_RED";
        break;
      case 9:
        scFlag[9] = "ST77XX_RED";
        break;
      case 10:
        scFlag[10] = "ST77XX_RED";
        break;
      case 11:
        scFlag[11] = "ST77XX_RED";
        break;
    }
    //Serial.println("20,22,24,26,28,29,30,32,31,11,2");

    // Expect packets about once per second.
    if (brain.update()) {
      Serial.println(brain.readCSV());
      // }
      // Read sensor data into the array (replace with your sensor reading code)
      sensorData[0] = 0;
      sensorData[1] = int(brain.readSignalQuality());
      sensorData[2] = int(brain.readAttention());
      sensorData[3] = int(brain.readMeditation());
      sensorData[4] = int(brain.readDelta());
      sensorData[5] = int(brain.readTheta());
      sensorData[6] = int(brain.readLowAlpha());
      sensorData[7] = int(brain.readHighAlpha());
      sensorData[8] = int(brain.readLowBeta());
      sensorData[9] = int(brain.readHighBeta());
      sensorData[10] = int(brain.readLowGamma());
      sensorData[11] = int(brain.readMidGamma());

      statusleft = digitalRead(leftButtonPin);
      //}
      /*
        sensorData[0] = 0;
        sensorData[1] = 30;
        sensorData[2] = 40;
        sensorData[3] = 10;
        sensorData[4] = 25;
        sensorData[5] = 55;
        sensorData[6] = 70;
        sensorData[7] = 60;
        sensorData[8] = 75;
        sensorData[9] = 65;
        sensorData[10] = 35;
        sensorData[11] = 47;
      */

      int graphWidth = 40;
      int halfHeight = 120;
      tft.fillScreen(ST77XX_BLUE);

      for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) { // Draw 2 graphs per column (top and bottom)
          int graphIndex = i * 2 + j; // Calculate index for data and max Y value
          int barHeight = (sensorData[graphIndex] * halfHeight) / maxYValues[graphIndex];
          if (barHeight > 119){
            barHeight = 119
;          }
          if (scFlag[graphIndex] == "ST77XX_RED" && graphIndex != 0) {
            Fib = 1.618 * barHeight;
            tone1 = map(Fib, 0, 192.542, 0, 7007);
            tone(beepPin, tone1);
          }
          if (j == 0) {
            // Draw top half of the graph
            if (scFlag[graphIndex] == "ST77XX_GREEN") {
              tft.fillRect(i * graphWidth + 2, 0, 38, barHeight, ST77XX_GREEN);
            }
            else {
              tft.fillRect(i * graphWidth + 2, 0, 38, barHeight, ST77XX_RED);
            }
          }
          // Draw bottom half of the graph (mirrored for bottom graph)
          if (j == 1) {
            if (scFlag[graphIndex] == "ST77XX_GREEN") {
              tft.fillRect(i * graphWidth + 2, 240 - barHeight, 38, barHeight, ST77XX_GREEN);
            }
            else {
              tft.fillRect(i * graphWidth + 2, 240 - barHeight, 38, barHeight, ST77XX_RED);
            }
          }
        }
        statusleft = digitalRead(leftButtonPin);
      }
      statusleft = digitalRead(leftButtonPin);
    }
  }
  tft.fillScreen(ST77XX_ORANGE);
  noTone(beepPin);
}
