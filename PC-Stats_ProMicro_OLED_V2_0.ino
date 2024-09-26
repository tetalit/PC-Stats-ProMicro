#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>

#define CODE_VERS  "2.0.0"  
//  ProMicro:    SDA: D2, SCL: D3
//----------------------------------- OLED Setup ----------------------------------------
#define OLED_SSD1306

#define i2c_Address 0x3c //инициализация по I2C 

/*поворот экрана:  0 или 2  (0, 180)*/
#define rotateScreen 0

#define LED_PIN 5 // пин подключения rgb ленты
#define LED_NUM 9 // колличество светодиодов
CRGB leds[LED_NUM];// масив цветов

//-----------------------
/*SSD1306 OLED setup*/
#ifdef OLED_SSD1306
#include <Adafruit_SSD1306.h> 
#define SSD1306_128_64
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET 4     
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

//----------------------
int oledDraw = 0;
int oledOverride = 0;
//----------------------
long invertDelay = 60000; // 60 sec  delay
long lastInvertTime = 0;
int invertedStatus = 1;
//----------------------
long lastDebounceTime = 0;
long debounceDelay = 3000;
//----------------------
int Serial_eventDelay = 0;
boolean activeConn = false;
long lastActiveConn = 0;
boolean bootMode = true;
/*переменные для считывания параметров с com порта (serial input)*/
String inputString = "";
boolean stringComplete = false;
//----------------------
/* начальный стиль дисплея */
int displayChangeMode = 1;
//----------------------

void setup() {
#ifdef OLED_SSD1306
  display.begin(SSD1306_SWITCHCAPVCC, i2c_Address); 
#endif

  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setRotation(rotateScreen);// Rotate the display at the start:  0, 1, 2 or 3 = (0, 90, 180 or 270 degrees)
  display.clearDisplay();

  /* остановка разрыва текста*/
  display.setTextWrap(false); 

  Serial.begin(115200);
  inputString.reserve(200);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM); // определение ленты
  FastLED.setBrightness(250); // яркость ленты 0-255

  /*экран загрузки*/
  splashScreen();

}

byte counter; // для rgb ленты
int gpuDegree;
//---------------------------------------LOOP-------------------------------------------
void loop() {
rgbTape(gpuDegree); // функция для ргб ленты
  serialEvent();

  /* OLED вывод характеристи */
  if (stringComplete) {

    if (bootMode) {
      display.clearDisplay();
      display.display();
      display.stopscroll();
      bootMode = false;
    }

    //lastActiveConn = millis();

    if (displayChangeMode == 1) {
      DisplayStyle1B();
    }

    inputString = "";
    stringComplete = false;
//    CustomTriggerGPU_temp(gpuDegree);
    /* функция защиты выгорания экрана */
    if ((millis() - lastInvertTime) > invertDelay && oledDraw == 1) {
      lastInvertTime = millis();

#ifdef enableInvertscreen
      inverter();
#endif
    }
  }
}

void serialEvent() {

  while (Serial.available()) {          //  проверка доступности serial порта
    char inChar = (char)Serial.read();  // считывание по usb
    //Serial.print(inChar); // Debug 

    inputString += inChar;
    if (inChar == '|') {
      stringComplete = true;

      //delay(Serial_eventDelay);   //задержка, чтобы избежать повреждения экранных данных

      //display.drawRect(82, 0, 44, 10, WHITE); // Position Test
      //display.fillRect(115, 0, 42, 10, BLACK); // мигание индикатора смены стиля(D1, D2, D3)
      display.display();

    }
  }
}
/*
void activityChecker() {
  if (millis() - lastActiveConn > lastActiveDelay)
    activeConn = false;
  else
    activeConn = true;
  if (!activeConn) {
    if (invertedStatus)

      //Выключает дисплей, когда нет никакой активности
      display.invertDisplay(0);
    display.clearDisplay();
    display.display();

    display.fillRect(0, 0, 128, 64, BLACK);
    display.display();
    oledDraw = 0;

  }
}
*/
//-------------------------------------------- Anti Screen Burn inverter ------------------------------------------------

void antiBurn() {
  display.invertDisplay(0);
  display.fillRect(0, 0, 128, 64, BLACK);
  display.display();
  oledDraw = 0;
}

void inverter() {
  if ( invertedStatus == 1 ) {
    invertedStatus = 0;
  } else {
    invertedStatus = 1;
  };
  display.invertDisplay(invertedStatus);
  display.display();
}

//--------------------------------------------- Load Screen --------------------------------------------------------
void splashScreen() {
  display.setTextSize(3);
  display.setCursor(50, 5);
  display.println("PC");
  display.setTextSize(2);
  display.setCursor(36, 30);
  display.println("STATS");

  //Set version to USB Serial
  display.setTextSize(1);
  display.setCursor(32, 55);
  display.print("USB:v");
  display.print (CODE_VERS);

  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();

  
  display.setTextSize(3); 
  display.setCursor(1, 1);
  display.println(">");
  display.display();
  display.setTextSize(2);
  display.setCursor(20, 30);
  display.display();
  display.println("(>_<)");
  
  display.display();
  display.setTextSize(1);
  display.setCursor(1, 54);
  display.println("waiting for data.....");
  display.display();
  //delay(5000);
  display.clearDisplay();
  //display.display(); //note... not needed "main loop" will initiate this, when serial coms establish
}

void rgbTape(int gpuDegree){
  float GPUtempfactor = gpuDegree ;
    // цикл для радуги на ленте(пока-что так)
  for (int i = 0; i < LED_NUM; i++) {
    if (GPUtempfactor >= (10) ) {
    //leds[i].setHue(counter + i * 255 / LED_NUM); //радуга
    leds[i] = CRGB::Green; 
    }
    if (GPUtempfactor >= (50) ) {
      leds[i] = CRGB::Orange;
      }
    if (GPUtempfactor >= (65) ) {
      leds[i] = CRGB::Red;
      }
  }
  counter++;        // counter меняется от 0 до 255 (тип данных byte)
  FastLED.show();  // вывод массива на ленту, есть команда clear отчищающая ленту
  delay(10);        // скорость движения радуги(меньше - быстрее)
}
