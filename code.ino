#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"
#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1

DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
RTC_DS3231 rtc;

SoftwareSerial BT(2, 3); // RX, TX

char Text[100] = "WELCOME TO SISODIYA FAMILY";

// ----------------------------------------------------
// DMD REFRESH
// ----------------------------------------------------

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

// ----------------------------------------------------
// UPDATE RTC
// ----------------------------------------------------

void updateRTC(const char* timePart, const char* datePart) {

  DateTime now = rtc.now();

  int h = now.hour();
  int m = now.minute();
  int s = now.second();

  int d = now.day();
  int mo = now.month();
  int y = now.year();

  // ---------- TIME ----------

  if (strcmp(timePart, "HH/MM/SS") != 0) {

    int hh, mm, ss;

    if (sscanf(timePart,
               "%d/%d/%d",
               &hh,
               &mm,
               &ss) == 3) {

      h = hh;
      m = mm;
      s = ss;
    }
  }

  // ---------- DATE ----------

  if (strcmp(datePart, "DD/MM/YY") != 0) {

    int dd, month, yy;

    if (sscanf(datePart,
               "%d/%d/%d",
               &dd,
               &month,
               &yy) == 3) {

      d = dd;
      mo = month;
      y = 2000 + yy;
    }
  }

  Serial.print("RTC Updated -> ");

Serial.print(d);
Serial.print("/");

Serial.print(mo);
Serial.print("/");

Serial.print(y);

Serial.print(" ");

Serial.print(h);
Serial.print(":");

Serial.print(m);
Serial.print(":");

Serial.println(s);

  rtc.adjust(DateTime(y, mo, d, h, m, s));
}

// ----------------------------------------------------
// PROCESS COMMAND
// ----------------------------------------------------

void processCommand(char *cmd) {

  Serial.println();
  Serial.println("---------------------");

  Serial.print("Command: ");
  Serial.println(cmd);

  // MESSAGE

  if (cmd[0] == '*') {

    char *endHash = strchr(cmd, '#');

    if (endHash) {

      *endHash = '\0';

      strncpy(
        Text,
        cmd + 1,
        sizeof(Text) - 1
      );

      Text[sizeof(Text) - 1] = '\0';

      Serial.print("Message Updated: ");
      Serial.println(Text);
    }

    return;
  }

  // DATETIME

  if (strncmp(cmd, "#@", 2) == 0) {

    char temp[60];

    strncpy(
      temp,
      cmd + 2,
      sizeof(temp) - 1
    );

    temp[sizeof(temp) - 1] = '\0';

    char *timePart = strtok(temp, ",");
    char *datePart = strtok(NULL, ",");

    if (timePart && datePart) {

      Serial.print("Time Part: ");
      Serial.println(timePart);

      Serial.print("Date Part: ");
      Serial.println(datePart);

      updateRTC(
        timePart,
        datePart
      );
    }
  }
}


// ----------------------------------------------------
// BLUETOOTH
// ----------------------------------------------------

void checkBluetooth() {

  static char buffer[120];
  static byte index = 0;

  while (BT.available()) {

    char c = BT.read();

    Serial.print(c);

    if (c == '\r')
      continue;

    if (c == '\n') {

      buffer[index] = '\0';

      if (index > 0) {

        processCommand(buffer);
      }

      index = 0;
    }

    else {

      if (index < sizeof(buffer) - 1) {

        buffer[index++] = c;
      }
    }
  }
}

// ----------------------------------------------------
// SETUP
// ----------------------------------------------------

void setup() {

  Serial.begin(9600);

  Serial.println("================================");
  Serial.println("Real-Time LED Display Board");
  Serial.println("System Starting...");
  Serial.println("================================");

  Wire.begin();

  BT.begin(9600);

  if (!rtc.begin()) {

    Serial.println("RTC NOT FOUND!");

    while (1);
  }

  Serial.println("RTC Connected");
  Serial.println("Bluetooth Ready");

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Timer1.initialize(1000);
  Timer1.attachInterrupt(ScanDMD);

  dmd.clearScreen(true);
  dmd.selectFont(SystemFont5x7);

  Serial.println("Display Ready");
}

// ----------------------------------------------------
// MESSAGE SCREEN
// ----------------------------------------------------

void showMessageScreen() {

  int textWidth = strlen(Text) * 6;

  for (int x = 64;
       x > -textWidth - 10;
       x--) {

    checkBluetooth();

    dmd.clearScreen(true);

    dmd.drawLine(
      x - 2,
      0,
      x + textWidth + 1,
      0,
      GRAPHICS_NORMAL
    );

    dmd.drawLine(
      x - 2,
      15,
      x + textWidth + 1,
      15,
      GRAPHICS_NORMAL
    );

    dmd.drawString(
      x,
      4,
      Text,
      strlen(Text),
      GRAPHICS_NORMAL
    );

    delay(45);
  }
}

// ----------------------------------------------------
// TIME SCREEN
// ----------------------------------------------------

void showTimeScreen() {

  unsigned long startTime = millis();

  while (millis() - startTime < 8000) {

    checkBluetooth();

    DateTime now = rtc.now();

    char timeBuffer[12];

    sprintf(
      timeBuffer,
      "%02d:%02d:%02d",
      now.hour(),
      now.minute(),
      now.second()
    );

    dmd.clearScreen(true);

    dmd.drawLine(
      0, 0,
      63, 0,
      GRAPHICS_NORMAL
    );

    dmd.drawLine(
      0, 15,
      63, 15,
      GRAPHICS_NORMAL
    );

    dmd.drawString(
      8,
      4,
      timeBuffer,
      strlen(timeBuffer),
      GRAPHICS_NORMAL
    );

    delay(200);
  }
}

// ----------------------------------------------------
// DATE SCREEN
// ----------------------------------------------------

void showDateScreen() {

  unsigned long startTime = millis();

  while (millis() - startTime < 8000) {

    checkBluetooth();

    DateTime now = rtc.now();

    char dateBuffer[16];

    sprintf(
      dateBuffer,
      "%02d/%02d/%04d",
      now.day(),
      now.month(),
      now.year()
    );

    dmd.clearScreen(true);

    dmd.drawLine(
      0, 0,
      63, 0,
      GRAPHICS_NORMAL
    );

    dmd.drawLine(
      0, 15,
      63, 15,
      GRAPHICS_NORMAL
    );

    dmd.drawString(
      2,
      4,
      dateBuffer,
      strlen(dateBuffer),
      GRAPHICS_NORMAL
    );

    delay(200);
  }
}

// ----------------------------------------------------
// LOOP
// ----------------------------------------------------

void loop() {

  showMessageScreen();

  showTimeScreen();

  showDateScreen();
}
