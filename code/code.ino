#include <Wire.h>
#include <splash.h>
#include <Adafruit_SSD1306.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>

#define S1 7
#define S2 8
#define S3 9 
#define fmon 12
#define ampon 11
#define mp3on 10
#define torch 5
#define torchs 2
#define battery A6
#define chrg 4

#define OLED_ADDR   0x3C


Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int mode;
int modet;
int freq;
float freqd;
int freqB;
int bat;
float batv;
byte freqH, freqL;

void setBatv(){
  bat = analogRead(battery);
  Serial.print(bat);
  batv = map(bat,736,1023,0,100);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(batv, "%");
  display.display();
}

void setFrequency(){
  
  freqB = freq - 870; // chip needs to have freq offset from lowest freq (870)
  freqH = freqB>>2; // you need to break the offset freq into 2 parts (hi/low)
  freqL = (freqB&3)<<6; // Shift channel selection for matching register 0x03
   
  Wire.beginTransmission(0x11);
  Wire.write(0x03);
  Wire.write(freqH); // write High freq byte
  Wire.write(freqL + 0x10); // write Low freq byte
  Wire.endTransmission();
}

void setInitialize(){

  // Initialize the RDA5807M chip //
 
  Wire.beginTransmission(0x20); // Device address is 0x11
  Wire.write(0x02); // Register address 0x02
  Wire.write(0xC0); 
  Wire.write(0x03); // Initialize the settings
  Wire.endTransmission(); // stop condition
  delay(500); // wait 500ms
   
  Wire.beginTransmission(0x11); // Device address is 0x11
  Wire.write(0x02); // Register address 0x02
  Wire.write(0xC0); 
  Wire.write(0x0D); // Setup the radio for communications
  Wire.endTransmission();
  delay(500);

  // The volume is from 0-F and its the first bytes, leave all the bytes (0x84D0 - 0x84DF)
 
  Wire.beginTransmission(0x11);
  Wire.write(0x05);
  Wire.write(0x84U);
  Wire.write(0xD5); // set volume to 1
  Wire.endTransmission();
}
void setDisplayfm(){
 display.clearDisplay();
 display.setTextSize(2);
 display.setTextColor(WHITE);
 display.setCursor(20,30);
 display.print("FM:");
 display.print(freqd);
 display.display();
} 

void setDisplaymp3(){

 display.clearDisplay();
 display.setTextSize(3);
 display.setTextColor(WHITE);
 display.setCursor(20,30);
 display.print("MP3");
 display.display();
}
 
void setup(){
  
  
  Wire.begin();
  Serial.begin(9600);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S1, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(torchs, INPUT_PULLUP);
  pinMode(torch, OUTPUT);
  pinMode(fmon, OUTPUT);
  pinMode(mp3on, OUTPUT);
  pinMode(ampon, OUTPUT);
  digitalWrite(ampon, LOW);
  digitalWrite(fmon, LOW);
  digitalWrite(mp3on, LOW);
  digitalWrite(torch, LOW);
  mode = 2;
  modet = 0;
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
  display.clearDisplay();
  setBatv();
}
 
void loop()

{
  
 if(digitalRead(S1) == LOW){
  Serial.println("1");
  mode = 0;
 }
 
 if(mode == 0){

 digitalWrite(mp3on, LOW);
 digitalWrite(fmon, HIGH);
 digitalWrite(ampon, HIGH);
 delay(1000);
 setInitialize();
 delay(1000);
 freq =  1023;
 freqd = freq/10.0;
 setFrequency();
 setDisplayfm();
 setBatv();

 }
 
 while(mode == 0){
 
 if(digitalRead(S2) == LOW){

  freq =  freq+1;
  freqd = freq/10.0;
  setFrequency();
  setDisplayfm();
  Serial.println("1");
  
 }

 if(digitalRead(S3) == LOW){

  freq =  freq-1;
  freqd = freq/10.0;
  setFrequency();
  setDisplayfm();
  
 }

 if(digitalRead(S1) == LOW){

  mode = 1;
  digitalWrite(ampon, LOW);
  digitalWrite(fmon, LOW);
  delay(500);
 }
 if(digitalRead(torchs) == LOW){
  if(modet == 0){
    analogWrite(torch, 150);
    modet = 1;
  }
  if(modet == 1){
    digitalWrite(torch, LOW);
    modet = 0;
  }
 }
  setBatv();
 
}

if(mode == 1){

  digitalWrite(fmon, LOW);
  digitalWrite(mp3on, HIGH);
  digitalWrite(ampon, HIGH);
  setDisplaymp3();
  delay(200);
  setBatv();
}
while(mode == 1){
  setBatv();
  if(digitalRead(S1) == LOW){
  Serial.println("1");
  mode = 0;
  if(digitalRead(torchs) == LOW){
  if(modet == 0){
    analogWrite(torch, 150);
    modet = 1;
  }
  if(modet == 1){
    digitalWrite(torch, LOW);
    modet = 0;
  }
 }
 }
}
setBatv();
if(digitalRead(torchs) == LOW){
  if(modet == 0){
    analogWrite(torch, 150);
    modet = 1;
  }
  if(modet == 1){
    digitalWrite(torch, LOW);
    modet = 0;
  }
 }
}
