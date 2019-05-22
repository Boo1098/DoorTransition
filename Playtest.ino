#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"

#define SD_ChipSelectPin 4
#define REED_PIN 2
#define PR_PIN A5
#define DARK_BASELINE 30

TMRpcm music;

void setup() {
  pinMode(REED_PIN, INPUT_PULLUP);
  music.speakerPin = 9;
  if (!SD.begin(SD_ChipSelectPin)) {
    while (true) {
    }
  }

  File root = SD.open("/");
  int numFiles = getNumFiles(root);
  root.close();

  bool doorOpen = true;
  int proximity = LOW;
  while (true) {
    if (analogRead(A5) > DARK_BASELINE) {
      proximity = digitalRead(REED_PIN);
      if (proximity == HIGH && !doorOpen) {
        randomSeed(millis());
        int song = random(0, numFiles);
        String file = String(song);
        file += ".wav";
        music.play(string2char(file));
        while (music.isPlaying() == 1 && digitalRead(REED_PIN) == HIGH && analogRead(A5) > DARK_BASELINE) {
        }
        music.disable();
        doorOpen = true;
      } else if (proximity == LOW) {
        doorOpen = false;
        delay(100);
      }
    } else {
      delay(5000);
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
      Serial.println(entry.name());
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
