# DoorTransition
A small handy device that plays transition music anytime you walk through the door.
# Features
* Uses reed switch to detect door opening
* Photoresistor to stop playing if it is too dark
* Potentiometer to adjust light sensitivity
* Potentiometer to adjust volume
* Switch to hard turn off
# Wiring
![Wiring Diagram](https://i.imgur.com/tuCnKeq.png)
Uses a PAM8302 from adafruit to amplify the audio.
Micro SD card reader holds all the song files.
# Setup
After wiring, just load onto the micro sd card as many wav files as you like. They must be 8-bit, less than 32000Hz, and in PCM 8-bit unsigned file type.
There is also a potentiometer to adjust the light sensitivity and another to adjust the volume.
# Future
This could be condensed more if I weren't using an arduino Uno and instead used just the Atmel chip.
