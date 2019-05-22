#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"

#define SD_ChipSelectPin 10
#define REED_PIN 8
#define PR_PIN A5
#define SPEAKER_PIN 9
#define DARK_BASELINE 30

TMRpcm music;

void setup() {
  pinMode(REED_PIN, INPUT_PULLUP);
  music.speakerPin = SPEAKER_PIN;

  // Initialize SD card
  if (!SD.begin(SD_ChipSelectPin)) {
    while (true) {
    }
  }

  // Count how many songs there are
  File root = SD.open("/");
  int numFiles = getNumFiles(root);
  root.close();

  // Initialize Variables
  bool doorOpen = true;
  int door = LOW;
  while (true) {
    // Make sure it's not night
    if (analogRead(A5) > DARK_BASELINE) {
      // Check if door is open or not
      door = digitalRead(REED_PIN);
      if (door == HIGH && !doorOpen) {
        // Reset random seed
        randomSeed(millis());

        // Picks random song
        int song = random(0, numFiles);
        String file = String(song);
        file += ".wav";

        // Plays song
        music.play(string2char(file));
        while (music.isPlaying() == 1 && digitalRead(REED_PIN) == HIGH) {
        }

        // When song is done, or door is shut, ends song
        music.disable();
        doorOpen = true;

      }
      // Door is shut, resets
      else if (door == LOW) {
        doorOpen = false;
        delay(100);
      }
    } else {
      // Only check light sensor once every 5 seconds
      delay(5000);
    }
  }
}

// Counts how many files are in a directory
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

// Turns a string into a char pointer array
char* string2char(String str) {
  if (str.length() != 0) {
    char* p = const_cast<char*>(str.c_str());
    return p;
  }
}

void loop() {
}
