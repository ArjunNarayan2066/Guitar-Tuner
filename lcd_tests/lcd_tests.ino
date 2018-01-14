#include <LiquidCrystal.h>
#include "notes.h"
LiquidCrystal lcd (12, 11, 5, 4, 3, 2);
//SetCursor (Column, Row)

byte block[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

void setup() {
  Serial.begin(9600);
  lcd.createChar(0, block);
  lcd.begin (16, 2);

}

void loop() {
  // put your main code here, to run repeatedly:
  float freq = 320;
  int freqDiffThreshold = 4;
  
  printLCD (freqDiffThreshold, freq);
}

int findNote (float freq)
{//Find Note being played
  int index = 0;
  for (int i = 0; i < 31; i++)//Stop right before 32 so as to never test notes[32][0], beyond bounds
  {
    float noteLow = atof(notes[i][0]);
    float noteHigh = atof(notes[i+1][0]);
    if ((noteLow < freq || noteLow == freq) && (noteHigh > freq || noteHigh == freq))
    {//We know the note is included in the range      
      if (freq - noteLow < noteHigh - freq) index = i;
      else index = i+1;
    }
    if (i == 0 && freq < noteLow) index = 0;
    else if (i == 30 && freq > noteHigh) index = 31;
  }
  return index;
}

void printS (float freq, int index)//Print info to Serial monitor
{
  Serial.print("\nFreq: ");
  Serial.print (freq);
  Serial.print (" RefFreq: ");
  Serial.print (atof(notes[index][0]));
  Serial.print (" Diff: ");
  Serial.print (freq - atof(notes[index][0]));
  Serial.print (" Note: ");
  Serial.print (notes[index][1]);
  Serial.print ("\n");
}

//Tuning Down = NEGATIVE
//Tuning Up = POSITIVE

void printLCD (int freqDiffThreshold, float freq)
{//Gather information about current situation
 int index = findNote (freq);
 float refFreq = atof(notes[index][0]);
 float diff = freq - refFreq;
 lcd.setCursor (7, 0);
 lcd.print(notes[index][1]);
 int squares = 7 * (abs(diff) / 9.0); //Get # of squares to print
 if ((diff <= freqDiffThreshold) || (squares > 7)) //If Odd Cases
 {
    lcd.setCursor (0,1);
    lcd.write(byte(0));
    lcd.write(byte(0));
    if (squares > 7) lcd.print("Out of Range");
    else lcd.print("Note in tune");
    lcd.write(byte(0));
    lcd.write(byte(0));
    return;
 }
 //Set up prints
 //printS (freq, index);
 lcd.setCursor (7,1);
 lcd.write(byte(0));
 lcd.write(byte(0));
 //Get Side Squares 
 if (diff < 0) lcd.setCursor (7 - squares, 1); //Note is flat, display on the left
 else lcd.setCursor (9, 1);//Note is Sharp, display squares on the right
 for (int i = 0; i < squares; i++) lcd.write(byte(0));   
}
