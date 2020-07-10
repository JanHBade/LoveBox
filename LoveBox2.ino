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
#include <Fonts/FreeMono12pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <Servo.h>
#define SERVO_DATA_PIN 2
int pos = 90;
int increment = -2;
Servo myservo;

#define ANALOG_LIGHT_PIN 0
#define LIGHT_LEVEL 100
int light;

#define MAX_MIN 200
int Cnt_sec;
int Cnt_min;
int Trigger_min;
enum States
{
    start,
    wait,
    spin,
    wait_close
};
States Status = start;


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

void spinServo() {
    myservo.write(pos);

    if (pos <= 50 || pos >= 125) // Drehbereich zwischen 75°-105°
    { 
        increment *= -1;
    }
    pos += increment;
}

void setText(char* Zeile1, char* Zeile2)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);

    display.drawRoundRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 5, 1);

    display.setFont(&FreeMono12pt7b);
    display.setCursor(5, 20);
    display.println(Zeile1);
    display.setCursor(5, 50);
    display.println(Zeile2);
    display.display();
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

    scan();

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Lovebox von Sarah");
    display.display();
    delay(1000);

    display.clearDisplay();
    display.display();

    myservo.attach(SERVO_DATA_PIN);
    myservo.write(pos);

    randomSeed(RANDOM_REG32);

    Trigger_min = random(30, MAX_MIN);
    Status = wait;
}


void loop()
{
    light =  analogRead(ANALOG_LIGHT_PIN);

    //Serial.printf("%i\n", Status);
    switch (Status)
    {
    case wait:
        Serial.print("wait ");
        setText("Deckel", "zu!");
        if (Trigger_min == Cnt_min)
        {
            Status = spin;
        }
        break;
    case spin:
        Serial.print("spin ");
        spinServo();
        if (light > LIGHT_LEVEL)
        {
            Status = wait_close;
            setText("Lieb hab", "<3");
        }
        break;
    case wait_close:
        Serial.print("wait_close ");
        setText("Deckel", "zu!");
        if (light < LIGHT_LEVEL)
        {
            display.clearDisplay();
            display.display();
            Cnt_min = 0;
			Trigger_min = random(30, MAX_MIN);
            Status = wait;
        }
        break;
    default:
        break;
    }
    
    Serial.printf("%i:%i (%i) L:%i P:%i\n", Cnt_min, Cnt_sec, Trigger_min, light, pos);

    delay(1000);

    Cnt_sec++;
    if (60 == Cnt_sec)
    {
        Cnt_sec = 0;
        Cnt_min++;
    }
}
