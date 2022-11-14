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
        int y=35; // const value how far is next data display on oled
        unsigned long time; // time since last display
        
    public:
    Oled() {} // empty constructor
    
    float bug; // ONLY FOR DEBUGGING

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
            if(bookmark >= (sizeof(*elementValue) / sizeof(float))) bookmark = 0; // going back to the begining after showing all
            else bookmark += 2; // increment the bookmark to change displayed items 
            time = millis(); // update wait time
            
        }
        else 
        {
            display.clearDisplay(); // clear display buffor
            dataLoop(elementName, elementValue, unit); // loop for prepering data for display
            display.display(); // send data to display to display
            delay(1); // czas na ogarniecie
        }
        
    }

    void dataLoop(String * elementName, float * elementValue, valueUnit * unit) 
    {
        display.setTextColor(WHITE); // clear display and the buffor
        
        for (size_t i = 0; i < ITEMS_TO_DISPLAY; i++) //goes tru loop 2 times for 2 items on display
        {
            if(bookmark >= (sizeof(*elementValue) / sizeof(float)) && i == 1) break; // displaying odd number of parametrs

            display.setTextSize(TEXT_SIZE * 0.5); // size of name of the data
            display.setCursor(0, y*i); // placing cursor on the start of each of the two lines
            display.print(elementName[bookmark+i] + ": "); // printing one of the two element names
            display.setTextSize(TEXT_SIZE); // bigger text size for the data
            display.setCursor(0, (y*i) + 10); // 10 pixels below name data will be printed
            display.print(elementValue[bookmark+i]); // printing data
            display.print(" "); // space betwen data and data unit

            switch (unit[bookmark+i]) // switch statment to determine what unit should be used
            {
            case PROCENT:
                display.print("%");  // procent unit
                break;

            case SPEED:
                display.print("km/h"); // speed unit
                break;

            case VOLT:
                display.print("V");  // volts
                break;

            case AMPER:
                display.print("A"); // amperes
                break;

            case TEMPERATURE:
                display.setTextSize(TEXT_SIZE * 0.5); // smaller text for the "degree" symbol
                display.cp437(true); // code page 437 (special characters)
                display.write(167); // degres symbol
                display.setTextSize(TEXT_SIZE); // bigger text size for celcius char
                display.print("C"); // celcius char
                break;
            
            default:
                display.print(" "); // NULL handler
                break;
            }
        }
    }
};

#endif