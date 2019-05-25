#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"

#define SD_ChipSelectPin 10
#define REED_PIN 8
#define PR_PIN A5
#define SPEAKER_PIN 9
#define DARK_BASELINE 30
#define STATUS_LED 7
#define LIGHT_POT A3
#define SWITCH 5
#define AMP_SD 6

TMRpcm music;

void setup() {
  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(AMP_SD, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(AMP_SD, LOW);
  music.speakerPin = SPEAKER_PIN;
  Serial.begin(9600);

  // Initialize SD card
  if (!SD.begin(SD_ChipSelectPin)) {
      Serial.println("SD Fail");
    while (true) {
    }
  }
  Serial.println("SD Works");

  // Count how many songs there are
  File root = SD.open("/");
  int numFiles = getNumFiles(root);
  root.close();

  music.setVolume(4);

  // Initialize Variables
  bool doorOpen = true;
  int door = LOW;
  while (true) {
    // Make sure it's not night
    if (analogRead(PR_PIN) > analogRead(LIGHT_POT) && digitalRead(SWITCH) == HIGH) {
      digitalWrite(STATUS_LED, HIGH);

      // Check if door is open or not
      door = digitalRead(REED_PIN);

      if (door == HIGH && !doorOpen) {
        // Reset random seed
        randomSeed(millis());

        // Picks random song
        int song = random(0, numFiles);
        String file = String(song);
        file += ".wav";
        Serial.println(file);

        // Plays song
        music.play(string2char(file));
        digitalWrite(AMP_SD,HIGH);
        while (music.isPlaying() == 1 && digitalRead(REED_PIN) == HIGH) {
        }

        // When song is done, or door is shut, ends song
        digitalWrite(AMP_SD, LOW);
        music.disable();
        doorOpen = true;

      }
      // Door is shut, resets
      else if (door == LOW) {
        doorOpen = false;
        delay(100);
      }
    } else {
      digitalWrite(STATUS_LED, LOW);
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
    if (!entry.isDirectory()) {
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
