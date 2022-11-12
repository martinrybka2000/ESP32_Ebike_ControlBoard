#ifndef OLED_H
#define OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define TEXT_SIZE 2 // OLED display text size 
#define ITEMS_TO_DISPLAY 2 // max number of items on the screen

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

class Oled {
    private:
        size_t bookmark=0; // iterator for countiong where we are in loop in the "show" method
        int y=35; // position on display
        unsigned long time; // time since last display
        
    public:
    Oled() {} // empty constructor

    enum valueUnit {TEMPERATURE, PROCENT, SPEED, VOLT, AMPER}; // enum to choose in what unit display value

    void setup(){ //initiazlization of oled display
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
            Serial.println(F("SSD1306 allocation failed")); // check if there is display
        }
        millis(); // get init time
        display.clearDisplay(); // all this is setup
    }

    void show(String elementName[], float elementValue[], valueUnit unit[], unsigned long displayTime)
    {
        if (millis() - time >= displayTime) // current time - last time code was done >= how often change display
        {
            if(bookmark >= (sizeof(elementValue) / sizeof(float))) bookmark = 0; // going back to the begining after showing all
            bookmark += 2; // increment the bookmark to change displayed items
            time = millis(); // wait time
        }
        else 
        {
            display.clearDisplay(); // clear display buffor
            dataLoop(elementName, elementValue, unit); // loop for prepering data for display
            display.display(); // send data to display to display
        }
        
    }
    //TODO add odd number display
    void dataLoop(String elementName[], float elementValue[], valueUnit unit[]) 
    {
        display.setTextColor(WHITE); 
        
        for (size_t i = 0; i < ITEMS_TO_DISPLAY; i++)
        {
            display.setTextSize(TEXT_SIZE * 0.5);
            display.setCursor(0, y*i);
            display.print(elementName[bookmark+i] + ": ");
            display.setTextSize(TEXT_SIZE);
            display.setCursor(0, (y*i) + 10);
            display.print(elementValue[bookmark+i]);
            display.print(" ");

            switch (unit[bookmark+i])
            {
            case PROCENT:
                display.print("%"); 
                break;

            case SPEED:
                display.print("km/h"); 
                break;

            case VOLT:
                display.print("V"); 
                break;

            case AMPER:
                display.print("A"); 
                break;

            case TEMPERATURE:
                display.setTextSize(TEXT_SIZE * 0.5);
                display.cp437(true); 
                display.write(167); // degres symbol
                display.setTextSize(TEXT_SIZE);
                display.print("C");
                break;
            
            default:
                display.print(" "); 
                break;
            }
        }
    }
};

#endif