#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"

#define SD_ChipSelectPin 4
#define REED_PIN 2

TMRpcm music;

void setup() {
  pinMode(REED_PIN, INPUT_PULLUP);
  music.speakerPin = 9;
  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD Fail");
    while (true) {
    }
  }

  File root = SD.open("/");
  int numFiles = getNumFiles(root);
  String files[numFiles];
  root = SD.open("/");
  for (int i = 0; i < numFiles; i++) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    if (entry.isDirectory()) {
      i--;
    } else {
      files[i] = entry.name();
    }
    entry.close();
  }

  randomSeed(analogRead(A0));

  music.volume(7);

  bool doorOpen = true;
  int proximity = LOW;
  while (true) {
    proximity = digitalRead(REED_PIN);
    if (proximity == HIGH && !doorOpen) {
      randomSeed(millis());
      int song = random(0, numFiles);
      music.play(string2char(files[song]));
      while (music.isPlaying() == 1 && digitalRead(REED_PIN) == HIGH) {
      }
      music.disable();
      doorOpen = true;
    } else if (proximity == LOW) {
      doorOpen = false;
      delay(100);
    }
  }
}

int getNumFiles(File dir) {
  int count = 0;
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }

    if (entry.isDirectory()) {
    } else {
      count++;
    }
    entry.close();
  }
  return count;
}

char* string2char(String str) {
  if (str.length() != 0) {
    char* p = const_cast<char*>(str.c_str());
    return p;
  }
}

void loop() {
}
