#include "Oled.h"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Oled::Oled()
{

}

void Oled::setup(size_t paramCnt)                       
{
    if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {   // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed")); // check if there is display
    }
    lastSwithTime = millis();       // get init time
    lastrefreshTime = millis();     // get init time
    _display.clearDisplay();        // all this is setup

    itemsToDisplay = paramCnt;      // u have to pass how many parametrs are in the data arrayes
}

void Oled::show(String elementName[], float elementValue[], valueUnit unit[], unsigned long swithTime, unsigned long refreshTime)
{
    if(millis() - lastrefreshTime >= refreshTime)       // current time - last time code was done >= how often refresh display
    {
         
        if (millis() - lastSwithTime >= swithTime)      // current time - last time code was done >= how often swith display
        {
            bookmark += 2;                         // increment the bookmark to change displayed items 
            if(bookmark >= itemsToDisplay) bookmark = 0; // going back to the begining after showing all
            lastSwithTime = millis();                   // update wait time
        }
        else 
        {
            _display.clearDisplay();                     // clear display buffor
            dataLoop(elementName, elementValue, unit);  // loop for prepering data for display
            _display.display();                          // send data to display to display
            delay(1);                                   // czas na ogarniecie
        }

        lastrefreshTime = millis();                     // update wait time
    }
}

void Oled::dataLoop(String * elementName, float * elementValue, valueUnit * unit) 
{
    _display.setTextColor(WHITE); // clear display and the buffor
        
    for (size_t i = 0; i < ITEMS_TO_DISPLAY; i++) //goes tru loop 2 times for 2 items on display
    {
        
        if(bookmark+i >= itemsToDisplay && i == 1) break; // displaying odd number of parametrs

        _display.setTextSize(TEXT_SIZE * 0.5); // size of name of the data
        _display.setCursor(0, y*i); // placing cursor on the start of each of the two lines
        _display.print(elementName[bookmark+i] + ": "); // printing one of the two element names
        _display.setTextSize(TEXT_SIZE); // bigger text size for the data
        _display.setCursor(0, (y*i) + 10); // 10 pixels below name data will be printed
        _display.print(elementValue[bookmark+i]); // printing data
        _display.print(" "); // space betwen data and data unit

        switch (unit[bookmark+i]) // switch statment to determine what unit should be used
        {
            case PROCENT:
                _display.print("%");  // procent unit
                break;

            case SPEED:
                _display.print("km/h"); // speed unit
                break;

            case VOLT:
                _display.print("V");  // volts
                break;

            case AMPER:
                _display.print("A"); // amperes
                break;
            
            case AMPER_H:
                _display.print("Ah"); // amperes hours
                break;

            case TEMPERATURE:
                _display.setTextSize(TEXT_SIZE * 0.5); // smaller text for the "degree" symbol
                _display.cp437(true); // code page 437 (special characters)
                _display.write(167); // degres symbol
                _display.setTextSize(TEXT_SIZE); // bigger text size for celcius char
                _display.print("C"); // celcius char
                break;

            case WATT:
                _display.print("W");  // watts
                break;

            case WATT_H:
                _display.print("Wh");  // watts hours
                break;
            
            case RPM:
                _display.print("rpm"); // amperes hours
                break;

            default:
                _display.print(" "); // NULL handler
                break;
        }
    }
    
}