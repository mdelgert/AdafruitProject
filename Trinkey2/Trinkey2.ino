//https://learn.adafruit.com/adafruit-trinkey-qt2040/blink-2
//Board -> Raspberry Pi RP2040 Boards -> Adafruit Trinkey RP2040 QT
//https://forum.arduino.cc/t/change-the-name-of-arduino-pro-micro-shown-in-the-devices-and-printers/660942/4
//C:\Users\mdelgert\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\2.6.4

#include <Adafruit_NeoPixel.h>
#include <Mouse.h>

// How many internal neopixels do we have? some boards have more than one!
#define NUMPIXELS        1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

unsigned long timer = 0;
unsigned long blinkDelay = 500;
unsigned long timerInterval = 60000;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);

#if defined(NEOPIXEL_POWER)
  // If this board has a power control pin, we must set it to output and high
  // in order to enable the NeoPixels. We put this in an #if defined so it can
  // be reused for other boards without compilation errors
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(20); // not so bright
}

// the loop routine runs over and over again forever:
void loop() {
  MoveCycle();
}

void Blink()
{
  // set color
  pixels.fill(0x00FF00);
  pixels.show();
  delay(blinkDelay); // wait half a second

  // turn off
  pixels.fill(0x000000);
  pixels.show();
  delay(blinkDelay);
}

void MoveCycle()
{
	if (millis() >= timer)
	{
		timer += timerInterval;
		Mouse.move(1, 1, 0);
    Blink();
	}
}
