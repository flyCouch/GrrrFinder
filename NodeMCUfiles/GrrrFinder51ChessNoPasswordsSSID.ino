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
#define STASSID "Put Your SSID here"
#define STAPSK  "Put Your Password Here"
#endif

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
  menuDial = map(menuDial, 0, 1023, 1, 8);
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
      tone(beepPin, 432);
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
    tft.fillScreen(ST77XX_BLACK);
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
    tone1 = map(tone1, 1, 1024, 1, 7000);
    tft.setTextSize(5);
    tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);;
    tft.setCursor(50, 20);
    tft.print("Tone");
    tft.setCursor(50, 130);
    tft.print(tone1);
    tft.print("   ");
    statusright = digitalRead(rightButtonPin);
    while (statusright == LOW) {
      tone1 = analogRead(A0);
      tone1 = map(tone1, 1, 1024, 1, 7000);
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
  tft.fillScreen(ST77XX_BLACK);
}

void Chess() {
  tft.setTextSize(3);
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
  tft.setCursor(0, 10);
  tft.print("Chess");
  Serial.println("Chess");
  delay (4321);
  tft.fillScreen(ST77XX_BLACK);
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
  */
}
