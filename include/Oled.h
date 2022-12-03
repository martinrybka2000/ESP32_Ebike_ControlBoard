#ifndef OLED_H
#define OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels
#define TEXT_SIZE 2             // OLED display text size 
#define ITEMS_TO_DISPLAY 2      // max number of items on the screen

class Oled {
    private:
        size_t bookmark = 0;            // iterator for countiong where we are in loop in the "show" method
        int y = 35;                     // const value how far is next data display on oled
        unsigned long lastSwithTime;    // time since last swith
        unsigned long lastrefreshTime;  // time since last refresh
        size_t itemsToDisplay;          // Becasue C is so low somehow I have to pass size of passed arrays size there it is
        
    public:
        Oled();                     // empty constructor

        float bug;                  // ONLY FOR DEBUGGING

        enum valueUnit {TEMPERATURE, PROCENT, SPEED, VOLT, AMPER}; // enum to choose in what unit display value

        void setup(size_t paramCnt);// initiazlization of oled display

        void show(String elementName[], float elementValue[], valueUnit unit[], unsigned long swithTime, unsigned long refreshTime);

    private:
        void dataLoop(String *elementName, float *elementValue, valueUnit *unit);
};

#endif