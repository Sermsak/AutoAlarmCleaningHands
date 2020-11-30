/*
 Hand Cleaning Alarm Project
 June 27, 2017
 Rev 2 On June 28,2017
   Issued: Prototype 
 Customer: 
  Western Digital ( Thailand) Co.,Ltd.
 Maker:
   ADEQNIC
   Code by Mr. Sermsak Puimom
           Mr. Pornthep Tubtim
           Mr. Pitthaya Boonluang
 */
/* Version 5
 *  MP3 VS1053
 *  Invers I/P Relay Active LOW
 *  Reorder From WD delivery on 30-Nov-2020 
 *  Master Frimware
 *  
 * /
// Written by Limor Fried/Ladyada for Adafruit Industries.  
//  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
// HW
//http://www.geeetech.com/wiki/index.php/Arduino_MP3_shield_board_with_TF_card
// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  8 //-1      // VS1053 reset pin (unused!)
#define SHIELD_CS     6 //7      // VS1053 chip select pin (output)
#define SHIELD_DCS    7 //6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 9 //4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ  2 //3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  
 // Pin Output Definition
#define G_LAMP A2
#define Y_LAMP A1
#define R_LAMP A0
#define BUZZER A3
//
#define TIME_MINUTE_MAX  600 // 600 sec or 10 minute
#define WAIT_LIMIT0 300   // 600 = 60 sec.
#define WAIT_LIMIT1 120   // 240 = 120 sec.
//  Sound
#define WAIT_SOUND 60
#define WAIT_SOUND2 12
//
//
#include <EEPROM.h>
// Pins Input Definition
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 4;    //Button Switch to press Ackknowlage Alarm 
// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
//
// set the LED:
 bool LEDstate;
 bool SWstatus;
 int reading;
 int count_wait=0;
 //
  int TimeInterval= 600; // 600 sec or 10 minute
 //
 int cnt_song=1;
 //
 const int CS_PA30W = 5; // Chip Select Power Amplifier 
 void fnFlashLED(){
  digitalWrite(G_LAMP, LOW);   
  digitalWrite(Y_LAMP, LOW);  
  digitalWrite(R_LAMP, LOW);  
  digitalWrite(BUZZER, LOW);
  delay(500);  
  digitalWrite(G_LAMP, HIGH);   
  digitalWrite(Y_LAMP, HIGH);  
  digitalWrite(R_LAMP, HIGH);  
  digitalWrite(BUZZER, HIGH);
  delay(500);
  digitalWrite(G_LAMP, LOW);   
  digitalWrite(Y_LAMP, LOW);  
  digitalWrite(R_LAMP, LOW);  
  digitalWrite(BUZZER, LOW);
  delay(200);  
  digitalWrite(G_LAMP, HIGH);   
  digitalWrite(Y_LAMP, HIGH);  
  digitalWrite(R_LAMP, HIGH);  
  digitalWrite(BUZZER, HIGH);
 }
 void fnDisplayLEDasMode(int value){
   if ( value == 0){
     digitalWrite(G_LAMP, HIGH);   
     digitalWrite(Y_LAMP, HIGH);  
     digitalWrite(R_LAMP, HIGH);
     TimeInterval=60; // 1 minute for Test 
      //mp3.play(ROOT,13);
     //while(!mp3.busy())delay(200);
       musicPlayer.playFullFile("/013.mp3");
   }
   else if(value ==1){
      digitalWrite(G_LAMP, LOW);   
     digitalWrite(Y_LAMP, HIGH);  
     digitalWrite(R_LAMP, HIGH);  
      TimeInterval=600;// 10 minute 
     //mp3.play(ROOT,14);
     //while(!mp3.busy())delay(200);
       musicPlayer.playFullFile("/014.mp3");
   }
    else if(value ==2){
       digitalWrite(G_LAMP, HIGH);   
     digitalWrite(Y_LAMP, LOW);  
     digitalWrite(R_LAMP, HIGH); 
     TimeInterval=1800; // 30 minute 
     //mp3.play(ROOT,15);
     //while(!mp3.busy())delay(200);
       musicPlayer.playFullFile("/015.mp3");
   }
    else if(value ==3){
       digitalWrite(G_LAMP,LOW);   
     digitalWrite(Y_LAMP, LOW);  
     digitalWrite(R_LAMP, HIGH);  
      TimeInterval=3600; // 60 minute // 1 hr.
     //mp3.play(ROOT,16);
     //while(!mp3.busy())delay(200);
       musicPlayer.playFullFile("/016.mp3");
   }
    else if(value ==4){
       digitalWrite(G_LAMP, HIGH);   
     digitalWrite(Y_LAMP, HIGH);  
     digitalWrite(R_LAMP, LOW); 
     TimeInterval=4200; // 70 minute  
     //mp3.play(ROOT,17);
     //while(!mp3.busy())delay(200);
     musicPlayer.playFullFile("/017.mp3");
   }
    else if(value ==5){
     digitalWrite(G_LAMP, LOW);   
     digitalWrite(Y_LAMP, HIGH);  
     digitalWrite(R_LAMP, LOW); 
     TimeInterval=5400; // 90 minute   // 1.30 hr
     //mp3.play(ROOT,18);
     //while(!mp3.busy())delay(200);
     musicPlayer.playFullFile("/018.mp3");
   }
   else if(value ==6){
     digitalWrite(G_LAMP, HIGH);   
     digitalWrite(Y_LAMP, LOW);  
     digitalWrite(R_LAMP, LOW);  
      TimeInterval=7200; // 120 minute // 2 hr.
      //mp3.play(ROOT,19);
      //while(!mp3.busy())delay(200);
      musicPlayer.playFullFile("/019.mp3");
   }
     else if(value ==7){
      digitalWrite(G_LAMP, LOW);   
     digitalWrite(Y_LAMP, LOW);  
     digitalWrite(R_LAMP, LOW); 
      TimeInterval=10800; // 180 minute // 3 hr.
     //mp3.play(ROOT,20);
     //while(!mp3.busy())delay(200);
     musicPlayer.playFullFile("/020.mp3");
   }
 }
 void fnSetTimeInterval()
 {
   int TimeOut = 0;
   int value =0;
   fnFlashLED();
   value = EEPROM.read(1);  // 0 = S/N ,1 = time Table
   fnDisplayLEDasMode(value);
   while(TimeOut < 500){
     TimeOut++;
     delay(10);
     reading = digitalRead(buttonPin);
     if(reading==LOW){
         delay(1000);
         if(reading==LOW){
            TimeOut=0;
            value++;
            if(value>7)value = 0;
            fnDisplayLEDasMode(value);
          }
     }
   }
   //mp3.play(ROOT,21);
   //while(!mp3.busy())delay(200);
   musicPlayer.playFullFile("/021.mp3");
   EEPROM.write(1, value);
   fnFlashLED();
 }

void fnTestHW()
{
  digitalWrite(G_LAMP, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(G_LAMP, HIGH);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  digitalWrite(Y_LAMP, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(Y_LAMP, HIGH);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  digitalWrite(R_LAMP, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(R_LAMP, HIGH);    // turn the LED off by making the voltage LOW
  delay(500);   
  digitalWrite(R_LAMP, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(R_LAMP, HIGH);    // turn the LED off by making the voltage LOW
  delay(500);      // wait for a second
  digitalWrite(R_LAMP, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(R_LAMP, HIGH);    // turn the LED off by making the voltage LOW
  delay(500);   
  digitalWrite(R_LAMP, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(R_LAMP, HIGH);    // turn the LED off by making the voltage LOW
  delay(500);      // wait for a second
  digitalWrite(BUZZER, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(BUZZER, HIGH);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
void fnScanButton()
{
    // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
  }
}
void fnTestButtonPinOnOffGreenLAmp()
{
    // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // set the LED:
  digitalWrite(G_LAMP, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
  
  //digitalWrite(G_LAMP, LOW);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);              // wait for a second
}
void fnSoundInform(int count){
    if ( count%WAIT_SOUND==0){
     //mp3.play(ROOT,3);
     musicPlayer.startPlayingFile("/003.mp3");
    }
}
void fnSoundInform2(int count){
    if ( count%WAIT_SOUND2==0){
     //mp3.play(ROOT,3);
     musicPlayer.startPlayingFile("/003.mp3");
    }
}
void fnHelpSound(){
  // mp3.stop();
  musicPlayer.stopPlaying();
  digitalWrite(G_LAMP, LOW);   
  digitalWrite(Y_LAMP, LOW);  
  digitalWrite(R_LAMP, LOW); 
  delay(500);
  digitalWrite(G_LAMP, HIGH);   
  digitalWrite(Y_LAMP, HIGH);  
  digitalWrite(R_LAMP, HIGH); 
  delay(500); 
  digitalWrite(G_LAMP, LOW);   
  digitalWrite(Y_LAMP, LOW);  
  digitalWrite(R_LAMP, LOW); 
  delay(500);
  digitalWrite(G_LAMP, HIGH);   
  digitalWrite(Y_LAMP, HIGH);  
  digitalWrite(R_LAMP, HIGH);    
   //mp3.play(ROOT,6);
   //while(!mp3.busy())delay(200);
   musicPlayer.playFullFile("/006.mp3");
   //mp3.play(ROOT,7);
   //while(!mp3.busy())delay(200);
   musicPlayer.playFullFile("/007.mp3");
   digitalWrite(G_LAMP, LOW);    // Turn Green One
   //mp3.play(ROOT,8);
   //while(!mp3.busy())delay(200);
   musicPlayer.playFullFile("/008.mp3");
   digitalWrite(G_LAMP, HIGH);  
   digitalWrite(Y_LAMP, LOW);  
   //mp3.play(ROOT,9);
   //while(!mp3.busy())delay(200);
   musicPlayer.playFullFile("/009.mp3");
   digitalWrite(Y_LAMP, HIGH);
   digitalWrite(R_LAMP, LOW);   
   // mp3.play(ROOT,10);
   //while(!mp3.busy())delay(200);
    musicPlayer.playFullFile("/010.mp3");
    digitalWrite(R_LAMP, LOW); 
    digitalWrite(BUZZER, LOW);
    delay(500);
    digitalWrite(R_LAMP, HIGH); 
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(R_LAMP, LOW); 
    digitalWrite(BUZZER, LOW);
    delay(500);
    digitalWrite(R_LAMP, HIGH); 
    digitalWrite(BUZZER, HIGH);
    delay(500);    
    //mp3.play(ROOT,11);
    //while(!mp3.busy())delay(200);
    musicPlayer.playFullFile("/011.mp3");
  digitalWrite(G_LAMP, HIGH);   
  digitalWrite(Y_LAMP, HIGH);  
  digitalWrite(R_LAMP, HIGH);  
}
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(G_LAMP, OUTPUT);
  pinMode(Y_LAMP, OUTPUT);
  pinMode(R_LAMP, OUTPUT);
  pinMode(CS_PA30W,OUTPUT);
  pinMode(BUZZER, OUTPUT);
  //digitalWrite(CS_PA30W, LOW);  
  // 
  pinMode(buttonPin ,  INPUT_PULLUP);
  delay(3000); 
  //
  Serial.begin(9600);
  //
  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  //printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  // 
   //digitalWrite(CS_PA30W, HIGH);   
   reading = digitalRead(buttonPin);
   if(reading==LOW){
     delay(2000);
     reading = digitalRead(buttonPin);
     if(reading==LOW){
        //mp3.play(ROOT,2);
        //while(!mp3.busy())delay(200);
         musicPlayer.playFullFile("/002.mp3");
       fnSetTimeInterval();
     }
   }
  // initialize digital pin 13 as an output.
  
  digitalWrite(G_LAMP, LOW);   
  digitalWrite(Y_LAMP, LOW);  
  digitalWrite(R_LAMP, LOW);  
  delay(2000);  
  digitalWrite(G_LAMP, HIGH);   
  digitalWrite(Y_LAMP, HIGH);  
  digitalWrite(R_LAMP, HIGH);  
  digitalWrite(BUZZER, LOW);
  delay(100);  
  digitalWrite(BUZZER, HIGH);
  //mp3.vol(30);
  //mp3.play(ROOT,1);
  //while(!mp3.busy())delay(100);
   musicPlayer.playFullFile("/001.mp3");
  reading = digitalRead(buttonPin);
   if(reading==LOW){
     delay(500);
     reading = digitalRead(buttonPin);
     if(reading==LOW){
       fnHelpSound();
     }
   } 
   fnDisplayLEDasMode(EEPROM.read(1)); 
}
// the loop function runs over and over again forever
void loop() {
   // fnTestButtonPinOnOffGreenLAmp();
   //
   digitalWrite(G_LAMP, LOW);
   digitalWrite(Y_LAMP, HIGH);  
   digitalWrite(R_LAMP, HIGH);  
    delay(1000);
    //mp3.play(ROOT,5);
    musicPlayer.playFullFile("/005.mp3");
   
    // mp3.pause();
   count_wait=0;
   while (count_wait<TimeInterval)
   {
   delay(1000);
   count_wait++;
   }
   digitalWrite(G_LAMP, HIGH);
   digitalWrite(Y_LAMP, LOW);
   digitalWrite(BUZZER, LOW);
   delay(250);
   digitalWrite(BUZZER, HIGH);
   delay(250);
   digitalWrite(BUZZER, LOW);
   delay(250);
   digitalWrite(BUZZER, HIGH);
   delay(250);
   digitalWrite(BUZZER, LOW);
   delay(250);
   digitalWrite(BUZZER, HIGH);
   fnSoundInform(WAIT_SOUND);
   reading = digitalRead(buttonPin);
   //SWstatus = 0;
   while((reading==HIGH)&&(count_wait<WAIT_LIMIT0)){
     reading = digitalRead(buttonPin);
     delay(100);
     count_wait++;
      fnSoundInform(count_wait);
   }
   digitalWrite(Y_LAMP, HIGH);
    //mp3.stop();
    musicPlayer.stopPlaying();
   bool LEDstate;
   count_wait=0;
   fnSoundInform2(WAIT_SOUND2);
   //reading = digitalRead(buttonPin);
   while((reading==HIGH)&&(count_wait<WAIT_LIMIT1)){
     digitalWrite(R_LAMP,LOW);
     reading = digitalRead(buttonPin);
     delay(500);
     count_wait++;
     fnSoundInform2(count_wait);
   }
   digitalWrite(R_LAMP,HIGH);
    //mp3.stop();
    musicPlayer.stopPlaying();
   LEDstate=LOW;
   //reading = digitalRead(buttonPin);
   while(reading==HIGH){
     LEDstate = !LEDstate;
     digitalWrite(R_LAMP, LEDstate);
     //digitalWrite(BUZZER, LEDstate);
     delay(500);
     reading = digitalRead(buttonPin);
   }
     digitalWrite(R_LAMP, HIGH);
     digitalWrite(BUZZER, HIGH);
     //mp3.play(ROOT,4); 
     //while(!mp3.busy())delay(200);
      musicPlayer.playFullFile("/004.mp3");
}
