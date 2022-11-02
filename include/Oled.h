#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define TEXT_SIZE 2 // OLED display text size 

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

class Oled {
    private:
        int y=0;
        
    public:
    Oled() {}

    enum valueUnit {TEMPERATURE, PROCENT, SPEED, VOLT, AMPER};

    void setup(){
        if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
            Serial.println(F("SSD1306 allocation failed"));
            for(;;);
        }
        delay(2000);
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.display(); 
        delay(2000);
    }

    // must be used two times to display
    void show(String elementName, float elementValue, valueUnit unit, int displayTime)
    {

        if(getY() == 0) display.clearDisplay();
        
        display.setTextSize(TEXT_SIZE * 0.5);
        display.setCursor(0, getY());
        display.print(elementName + ": ");
        display.setTextSize(TEXT_SIZE);
        display.setCursor(0, getY()+10);
        display.print(elementValue);
        display.print(" ");

        switch (unit)
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
                display.write(167);
                display.setTextSize(TEXT_SIZE);
                display.print("C");
                break;
        
            default:
                display.print("ERROR"); 
                break;
        }
        if(getY()==35){
            display.display();
            delay(displayTime);
            setY(0);
            }
        else setY(35);
    }

    int getY() { return y; }
    void setY(int set) { y = set; }
};