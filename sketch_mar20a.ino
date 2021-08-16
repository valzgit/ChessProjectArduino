#include <LedControl.h>

int DIN = 12;
int CS =  11;
int CLK = 10;

LedControl lc=LedControl(DIN,CLK,CS,0);
void setup(){
 lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
 lc.setIntensity(0,4);      // Set the brightness to maximum value 15 je maksimum meni je na 10
 lc.clearDisplay(0);         // and clear the display
 Serial.begin(9600);
}

byte smile[] = {B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B11111111,B11111111};

void printByte(byte character [])
{
    int i = 0;
    for(i=0;i<8;i++)
    {
       lc.setRow(0,i,character[i]);
    }
}

void loop(){ 
    if(Serial.available()>0){
      String recenica = Serial.readStringUntil('\n'); 
      int rcerec = recenica.toInt();
//      Serial.print(rcerec/1000);
//      Serial.print(((rcerec/100)% 10));
//      Serial.print((rcerec%100)/10);
//      Serial.print(rcerec%10);
      if(rcerec<10000){
        smile[rcerec/1000] &= ~ (1 << (7-(rcerec/100)% 10));
        smile[(rcerec%100)/10] |= ((1 << (7-rcerec%10)));
      }
      else{
        rcerec-=10000;
        byte oldrow1 = smile[rcerec/1000];
        byte oldrow2 = smile[(rcerec%100)/10];
        for(int i=0;i<5;++i){
          smile[rcerec/1000] |= (1 << (7-(rcerec/100)% 10));
          smile[(rcerec%100)/10] |= 1 << (7-rcerec%10);
          printByte(smile);
          delay(250);
          smile[rcerec/1000] &= ~ (1 << (7-(rcerec/100)% 10));
          smile[(rcerec%100)/10] &= ~(1 << (7-rcerec%10));
          printByte(smile);
          delay(250);
        }
        smile[rcerec/1000] = oldrow1;
        smile[(rcerec%100)/10] =  oldrow2;
        printByte(smile);
        
        
      }
      //print("blabla");
    }

    printByte(smile);
    delay(10);
}
