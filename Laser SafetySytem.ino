/********************************
K40 Co2 Laser safety interlock and monitor

Author: Anthony Bolgar 
Based upon the code by SignaGFX

LCD is a common 20x4 lcd panel driven by an I2C board.

Pin Assignments for UNO board:
D2 - Coolant flow monitor
D3 - Door Switch 
D4 - Warning buzzer or status LED
D5 - Relay trigger to diasable laser fire
A0 - Coolant temperature minitor (Sensor - TMP36) 


You will need a small 5V relay to use as the interlock or you can try to invert the INTERLOCK states and trigger the safety on the power supply with the adruino itself.
I havent tried that method yet but it may be simpler, good luck.
************************************/
#include <Wire.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR 0x27 // Define I2C Address for LCD backpack
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
unsigned long duration; //used for flow meter timeer to watch if the flow stops
#define INTERLOCK 5 //pin that enables or disables laser (safety system)
#define ALARMPIN 4 //Connect + side of Alarm buzzer to this Digital pin ( - side to gnd )
#define FLOWSENSORPIN 2 //Connect flow sensor output to this Digital Pin
#define DOORSWITCH 3 //Connect safety interlock switch to this Digital pin other side to gnd(high=open - low=closed)
int TempSensorPin = A0;
//the analog pin the TMP36's Vout (sense) pin is connected to
//the resolution is 10 mV / degree centigrade with a
//500 mV offset to allow for negative temperatures
//-----------------------------------------------------------------------------
// set for space invader animation speed:
int y=125;

// drawing the pixels for each 8x5 part of the LCD (SPACE INVADER)
// --START SPACE INVADER--
// --EMPTY START--
byte charSP0[8] =
{
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
}
;
// --EMPTY END--
// --MOVE STEP A START--
byte charSP1a[8] =
{
    B00000,
    B00000,
    B00000,
    B10000,
    B10000,
    B00000,
    B00000,
    B10000
}
;
byte charSP2a[8] =
{
    B00000,
    B00000,
    B10000,
    B11000,
    B11000,
    B00000,
    B10000,
    B01000
}
;
byte charSP3a[8] =
{
    B00000,
    B10000,
    B11000,
    B01100,
    B11100,
    B10000,
    B01000,
    B10100
}
;
byte charSP4a[8] =
{
    B10000,
    B11000,
    B11100,
    B10110,
    B11110,
    B01000,
    B10100,
    B01010
}
;
byte charSP5a[8] =
{
    B11000,
    B11100,
    B11110,
    B11011,
    B11111,
    B00100,
    B11010,
    B00101
}
;
byte charSP6a[8] =
{
    B01100,
    B11110,
    B11111,
    B01101,
    B11111,
    B10010,
    B01101,
    B10010
}
;
byte charSP7a[8] =
{
    B00110,
    B01111,
    B11111,
    B10110,
    B11111,
    B01001,
    B10110,
    B01001
}
;
byte charSP8a[8] =
{
    B00011,
    B00111,
    B01111,
    B11011,
    B11111,
    B00100,
    B01011,
    B10100
}
;
byte charSP9a[8] =
{
    B00001,
    B00011,
    B00111,
    B01101,
    B01111,
    B00001,
    B00010,
    B00101
}
;
byte charSP10a[8] =
{
    B00000,
    B00001,
    B00011,
    B00110,
    B00111,
    B00001,
    B00010,
    B00101
}
;
byte charSP11a[8] =
{
    B00000,
    B00000,
    B00001,
    B00011,
    B00011,
    B00000,
    B00001,
    B00010
}
;
byte charSP12a[8] =
{
    B00000,
    B00000,
    B00000,
    B00001,
    B00001,
    B00000,
    B00000,
    B00001
}
;
// --MOVE STEP A END--
// --MOVE STEP B START--
byte charSP1b[8] =
{
    B00000,
    B00000,
    B00000,
    B10000,
    B10000,
    B00000,
    B10000,
    B00000
}
;
byte charSP2b[8] =
{
    B00000,
    B00000,
    B10000,
    B11000,
    B11000,
    B10000,
    B01000,
    B10000
}
;
byte charSP3b[8] =
{
    B00000,
    B10000,
    B11000,
    B01100,
    B11100,
    B01000,
    B00100,
    B01000
}
;
byte charSP4b[8] =
{
    B10000,
    B11000,
    B11100,
    B10110,
    B11110,
    B10100,
    B00010,
    B00100
}
;
byte charSP5b[8] =
{
    B11000,
    B11100,
    B11110,
    B11011,
    B11111,
    B11010,
    B00001,
    B00010
}
;
byte charSP6b[8] =
{
    B01100,
    B11110,
    B11111,
    B01101,
    B11111,
    B01101,
    B00000,
    B00001
}
;
byte charSP7b[8] =
{
    B00110,
    B01111,
    B11111,
    B10110,
    B11111,
    B10110,
    B00000,
    B10000
}
;
byte charSP8b[8] =
{
    B00011,
    B00111,
    B01111,
    B11011,
    B11111,
    B01011,
    B10000,
    B01000
}
;
byte charSP9b[8] =
{
    B00001,
    B00011,
    B00111,
    B01101,
    B01111,
    B00101,
    B01000,
    B00100
}
;
byte charSP10b[8] =
{
    B00000,
    B00001,
    B00011,
    B00110,
    B00111,
    B00010,
    B00100,
    B00010
}
;
byte charSP11b[8] =
{
    B00000,
    B00000,
    B00001,
    B00011,
    B00011,
    B00001,
    B00010,
    B00001
}
;
byte charSP12b[8] =
{
    B00000,
    B00000,
    B00000,
    B00001,
    B00001,
    B00000,
    B00001,
    B00000
}
;
// --MOVE STEP B END--
// --END SPACE INVADER--
//----------------------------------------------------------------------------

//Setup alarm flags
int doorstate = 1;
//default to pin HIGH (door open)
int displayalarm = 1; //Toggle when alarm messages are on the LCD

//Flow sensor junk
// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect)
{
    uint8_t x = digitalRead(FLOWSENSORPIN);
    if (x == lastflowpinstate)
    {
        lastflowratetimer++;
        return;
        // nothing changed!
    }
    if (x == HIGH)
    {
        //low to high transition!
        pulses++;
    }
    lastflowpinstate = x;
    flowrate = 1000.0;
    flowrate /= lastflowratetimer;
    // in hertz
    lastflowratetimer = 0;
}
void useInterrupt(boolean v)
{
    if (v)
    {
        OCR0A = 0xAF;
        TIMSK0 |= _BV(OCIE0A);
    }
    else
    {
       TIMSK0 &= ~_BV(OCIE0A);
    }
}
void setup()
{
    lcd.begin (20, 4);
    // Switch on the backlight
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    // Setup the pins
    pinMode(INTERLOCK, OUTPUT);
    digitalWrite(INTERLOCK, LOW);
    pinMode(FLOWSENSORPIN, INPUT);
    digitalWrite(FLOWSENSORPIN, HIGH);
    pinMode(ALARMPIN, OUTPUT);
    digitalWrite(ALARMPIN,LOW);
    pinMode(DOORSWITCH, INPUT);
    digitalWrite(DOORSWITCH, HIGH);
    lastflowpinstate = digitalRead(FLOWSENSORPIN);
    useInterrupt(true);
    space01();
    lcd.clear();
}
void loop()
{
    Interlock();
}
void Interlock()
{
    DoorSafety();    //loop that keeps an eye on the door switch state

   //Watchdog for the water flow
    duration = pulseIn(FLOWSENSORPIN, LOW,3000000);
    if (duration <= 0)
    {
        lcd.clear();
        lcd.setCursor(2,1);
        lcd.print("NO COOLANT FLOW!");
        lcd.setCursor(5,2);
        lcd.print("Check Pump");
        displayalarm=1;
        digitalWrite(ALARMPIN,HIGH); //pin for buzzer or light. Your choice.
        delay(1000);
    }
    FlowSensor();    //loop that runs the coolant flow animation
    TempSensor();    //loop that measures the TIP36 temp output.

    //this section shuts down the laser enable ... Switch Interlock Pin states to meet your needs. (i.e. High for relay activation, Low to gnd pin directly from arduino)
    if (displayalarm==1)  //1=ALARM! 0=all good
    {
        digitalWrite(INTERLOCK, LOW);
    }
    else
    {
        digitalWrite(INTERLOCK,HIGH);
    }
}
void DoorSafety()//watch the door switch
{
    doorstate = digitalRead(DOORSWITCH);
    if (doorstate==1)
    {
        lcd.clear();
        lcd.setCursor(5,1);
        lcd.print("Door Open");
        displayalarm=1;
        delay(500);
        lcd.clear();
    }
    if (doorstate==0)
    {
        displayalarm=0;
    }
}
void FlowSensor() //coolant flow animation
{
    if (displayalarm == 0)
    {
        lcd.setCursor(4, 0);
        lcd.print("Coolant Flow");
        lcd.setCursor(0,2);
        lcd.print(" ");
        lcd.setCursor(2,1);
        lcd.print("[");
        lcd.setCursor(17,1);
        lcd.print("]");
        if (pulses >48)
        {
            pulses=0;
            lcd.setCursor(3,1);
            lcd.print("  ");
        }
        else
        {
            lcd.setCursor((pulses)/4 +3 ,1);
            lcd.print("->");
        }
    }
}
void TempSensor()  //Temp sensor loop
{
    int reading = analogRead(TempSensorPin);
    // converting that reading to voltage, for 3.3v arduino use 3.3
    float voltage = reading * 5.0;
    voltage /= 1024.0;
    float temperatureC = (voltage - 0.5) * 100 ;
    //converting from 10 mv per degree wit 500 mV offset
    //to degrees ((voltage - 500mV) times 100)
    lcd.setCursor(2, 2);
    float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
    if (temperatureF > 70) //Set this to the upper temperature limit . for the K40 it's been suggested to never exceed 70F or 21C
    {
        lcd.setCursor(0,0);
        lcd.print(" TEMP WARNING!!!!");
        lcd.setCursor(2,1);
        lcd.print(temperatureF);
        lcd.print(" degrees F");
        lcd.setCursor(2,2);
        lcd.print("IMMEDIATE ACTION");
        lcd.setCursor(6,3);
        lcd.print("REQUIRED");
        digitalWrite(ALARMPIN,HIGH);
        digitalWrite(INTERLOCK, LOW);
        displayalarm = 1;
        delay(700);
        lcd.clear();
    }
    else
    {
        if (displayalarm == 0)
        {
            lcd.setCursor(2,3);
            lcd.print(temperatureF);
            lcd.print(" degrees F");
            digitalWrite(ALARMPIN,LOW);
        }
        delay(50);
    }
}

//creds to J03Ibe for the invaders
void space01()
{
    for (int a=0; a<10;a+=2)
    {
        int b=a+1;
        int c=a+2;
        int d=a+3;
        int e=a+4;
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Loading....");
        lcd.setCursor(0, 2);
        lcd.print("Laser Safety System");
        lcd.setCursor(13,0);
        lcd.print("K40");
        lcd.createChar(0, charSP12a);
        lcd.createChar(1, charSP7a);
        lcd.createChar(2, charSP2a);
        lcd.createChar(3, charSP0);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)1);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)3);
        lcd.setCursor(e,0);
        lcd.write((byte)3);
        delay(y);
        lcd.createChar(0, charSP0);
        lcd.createChar(1, charSP8b);
        lcd.createChar(2, charSP3b);
        lcd.createChar(3, charSP0);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)1);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)3);
        lcd.setCursor(e,0);
        lcd.write((byte)3);
        delay(y);
        lcd.createChar(0, charSP0);
        lcd.createChar(1, charSP9a);
        lcd.createChar(2, charSP4a);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)1);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)0);
        lcd.setCursor(e,0);
        lcd.write((byte)0);
        delay(y);
        lcd.createChar(0, charSP0);
        lcd.createChar(1, charSP10b);
        lcd.createChar(2, charSP5b);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)1);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)0);
        lcd.setCursor(e,0);
        lcd.write((byte)0);
        delay(y);
        lcd.createChar(0, charSP0);
        lcd.createChar(1, charSP11a);
        lcd.createChar(2, charSP6a);
        lcd.createChar(3, charSP1a);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)1);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)3);
        lcd.setCursor(e,0);
        lcd.write((byte)0);
        delay(y);
        lcd.createChar(0, charSP0);
        lcd.createChar(1, charSP12b);
        lcd.createChar(2, charSP7b);
        lcd.createChar(3, charSP2b);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)1);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)3);
        lcd.setCursor(e,0);
        lcd.write((byte)0);
        delay(y);
        lcd.createChar(0, charSP0);
        lcd.createChar(1, charSP0);
        lcd.createChar(2, charSP8a);
        lcd.createChar(3, charSP3a);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)1);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)3);
        lcd.setCursor(e,0);
        lcd.write((byte)0);
        delay(y);
        lcd.createChar(0, charSP0);
        lcd.createChar(2, charSP9b);
        lcd.createChar(3, charSP4b);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)0);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)3);
        lcd.setCursor(e,0);
        lcd.write((byte)0);
        delay(y);
        lcd.createChar(0, charSP0);
        lcd.createChar(2, charSP10a);
        lcd.createChar(3, charSP5a);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)0);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)3);
        lcd.setCursor(e,0);
        lcd.write((byte)0);
        delay(y);
        lcd.createChar(0, charSP0);
        lcd.createChar(2, charSP11b);
        lcd.createChar(3, charSP6b);
        lcd.createChar(4, charSP1b);
        lcd.setCursor(a,0);
        lcd.write((byte)0);
        lcd.setCursor(b,0);
        lcd.write((byte)0);
        lcd.setCursor(c,0);
        lcd.write((byte)2);
        lcd.setCursor(d,0);
        lcd.write((byte)3);
        lcd.setCursor(e,0);
        lcd.write((byte)4);
        delay(y);
    }
   }
