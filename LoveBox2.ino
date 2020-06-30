/*
 Name:		LoveBox.ino
 Created:	30.06.2020 18:21:38
 Author:	jhb
 
*/

#include <Wire.h>
#include <WiFi.h>

#define NO_ADAFRUIT_SSD1306_COLOR_COMPATIBILITY

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void scan()
{
    Serial.println("Scanning I2C Addresses Channel 1");
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < 128; i++) {
        Wire.beginTransmission(i);
        uint8_t ec = Wire.endTransmission(true);
        if (ec == 0) {
            if (i < 16)Serial.print('0');
            Serial.print(i, HEX);
            cnt++;
        }
        else Serial.print("..");
        Serial.print(' ');
        if ((i & 0x0f) == 0x0f)Serial.println();
    }
    Serial.print("Scan Completed, ");
    Serial.print(cnt);
    Serial.println(" I2C Devices found.");
}

void setup()
{
    Serial.begin(115200);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
                                                // init done

    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.display();
    delay(500);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Lovebox von Sarah");
    display.display();
    delay(1000);
}


void loop()
{
    int light =  analogRead(0);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);

    display.drawRoundRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 4, 1);

    display.setFont(&FreeMono9pt7b);
    display.setCursor(10, 20);
    display.println(light);
    display.setCursor(10, 40);
    display.println("Lieb hab");
    display.display();

    delay(1000);
}
