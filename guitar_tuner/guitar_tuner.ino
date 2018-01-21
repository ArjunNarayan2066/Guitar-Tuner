#include "analysis.h"
#include "notes.h"

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 11, 5, 4, 3, 2);

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

void bubbleSort(float arr[], int size);
float analysis (float freqList[], int avgCnt);
int findNote (float freq);
void printLCD (float freqDiffThreshold, float freq);

//clipping indicator variables
boolean clipping = 0;

//data storage variables
byte newData = 0;
byte prevData = 0;
unsigned int time = 0;//keeps time and sends vales to store in timer[] occasionally
int timer[10];//sstorage for timing of events
int slope[10];//storage for slope of events
unsigned int totalTimer;//used to calculate period
unsigned int period;//storage for period of wave
byte index = 0;//current storage index
float frequency;//storage for frequency calculations
int maxSlope = 0;//used to calculate max slope as trigger point
int newSlope;//storage for incoming slope data

//variables for decided whether you have a match
byte noMatch = 0;//counts how many non-matches you've received to reset variables if it's been too long
byte slopeTol = 3;//slope tolerance- adjust this if you need
int timerTol = 10;//timer tolerance- adjust this if you need

//variables for amp detection
unsigned int ampTimer = 0;
byte maxAmp = 0;
byte checkMaxAmp;
byte ampThreshold = 30;//raise if you have a very noisy signal

void setup(){
  
  lcd.createChar(0, block);
  lcd.begin (16, 2);
  
  Serial.begin(9600);
  
  pinMode(13,OUTPUT);//led indicator pin
  pinMode(12,OUTPUT);//output pin
  
  cli();//diable interrupts
  
  //set up continuous sampling of analog pin 0 at 38.5kHz
 
  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  
  ADMUX |= (1 << REFS0); //set reference voltage
  ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  
  sei();//enable interrupts
}

ISR(ADC_vect) {//when new ADC value ready
  
  PORTB &= B11101111;//set pin 12 low
  prevData = newData;//store previous value
  newData = ADCH;//get value from A0
  if (prevData < 127 && newData >=127){//if increasing and crossing midpoint
    newSlope = newData - prevData;//calculate slope
    if (abs(newSlope-maxSlope)<slopeTol){//if slopes are ==
      //record new data and reset time
      slope[index] = newSlope;
      timer[index] = time;
      time = 0;
      if (index == 0){//new max slope just reset
        PORTB |= B00010000;//set pin 12 high
        noMatch = 0;
        index++;//increment index
      }
      else if (abs(timer[0]-timer[index])<timerTol && abs(slope[0]-newSlope)<slopeTol){//if timer duration and slopes match
        //sum timer values
        totalTimer = 0;
        for (byte i=0;i<index;i++){
          totalTimer+=timer[i];
        }
        period = totalTimer;//set period
        //reset new zero index values to compare with
        timer[0] = timer[index];
        slope[0] = slope[index];
        index = 1;//set index to 1
        PORTB |= B00010000;//set pin 12 high
        noMatch = 0;
      }
      else{//crossing midpoint but not match
        index++;//increment index
        if (index > 9){
          reset();
        }
      }
    }
    else if (newSlope>maxSlope){//if new slope is much larger than max slope
      maxSlope = newSlope;
      time = 0;//reset clock
      noMatch = 0;
      index = 0;//reset index
    }
    else{//slope not steep enough
      noMatch++;//increment no match counter
      if (noMatch>9){
        reset();
      }
    }
  }
    
  if (newData == 0 || newData == 1023){//if clipping
    PORTB |= B00100000;//set pin 13 high- turn on clipping indicator led
    clipping = 1;//currently clipping
  }
  
  time++;//increment timer at rate of 38.5kHz
  
  ampTimer++;//increment amplitude timer
  if (abs(127-ADCH)>maxAmp){
    maxAmp = abs(127-ADCH);
  }
  if (ampTimer==1000){
    ampTimer = 0;
    checkMaxAmp = maxAmp;
    maxAmp = 0;
  }
  
}

void reset(){//clea out some variables
  index = 0;//reset index
  noMatch = 0;//reset match couner
  maxSlope = 0;//reset slope
}


void checkClipping(){//manage clipping indicator LED
  if (clipping){//if currently clipping
    PORTB &= B11011111;//turn off clipping indicator led
    clipping = 0;
  }
}

//Arjun Variables
int avgMax = 30;//Used as max # of values to avg
float * freqList = new float [avgMax];
//float refFreq[6] = {329.63, 246.94, 196.00, 146.83, 110.00, 82.41};

int avgCnt = 0;
float sum = 0;
float mean = 0;
int freqMax = 400;
int freqMin = 50;
float freqDiffThreshold = 0.35;


void loop()
{
 //Serial.println ("Going through loop");
 if ((checkMaxAmp>ampThreshold))//Only if loud enough to hear it 
 {
    frequency = 38462/float(period);//calculate frequency timer rate/period
    if (frequency > freqMin && frequency < freqMax && avgCnt < avgMax) //Within tolerable range & still counting
    {
      freqList[avgCnt] = frequency; //Push to current list
      avgCnt ++; //Keep track of # of values pushed
      //print results
      Serial.print(frequency);
      Serial.print(" hz");
      Serial.print ("   AvgCnt = ");
      Serial.println(avgCnt);
    }
    //else Serial.println ("Finished set of 30");
  }
  else if (avgCnt != 0)//Broke streak of tolerable ranges, process compounded frequencies
  {//Process freq data here
    mean = analysis (freqList, avgCnt); //Included in attached header file
    Serial.print("Final Mean = ");
    Serial.println(mean);
    avgCnt = 0;
    printLCD (freqDiffThreshold, mean);
  }
  delay(50);//delete this if you want
}

//Tuning Down = NEGATIVE
//Tuning Up = POSITIVE

void printS (float freq, int index, float freqDiffThreshold)
{
    Serial.print ("Freq = ");
    Serial.print (freq);
    Serial.print ("   Note = ");
    Serial.print (notes[index][1]);
    Serial.print ("   Diff = ");
    Serial.print (freq - atof(notes[index][0]));
    Serial.print ("   FreqDiffThreshold = ");
    Serial.print (freqDiffThreshold);
    Serial.print("\n");
}

void printLCD (float freqDiffThreshold, float freq)
{//Gather information about current situation
 lcd.clear();
 int index = findNote (freq);
 float refFreq = atof(notes[index][0]);
 float diff = freq - refFreq;
 lcd.setCursor (7, 0);
 lcd.print(notes[index][1]);
 int squares = 7 * (abs(diff) / 7.5); //Get # of squares to print
 printS (freq, index, freqDiffThreshold);
 if ((abs(diff) <= freqDiffThreshold) || (squares > 7)) //If Odd Cases
 {
    Serial.print ("\nIn Odd Cases\n");
    lcd.setCursor (0,1);
    lcd.write(byte(0));
    lcd.write(byte(0));
    if (squares > 7) lcd.print("Out of Range");
    else lcd.print("Note in tune");
    lcd.write(byte(0));
    lcd.write(byte(0));
    return;
 }
 else
 {
   //Set up prints
   lcd.setCursor (7,1);
   lcd.write(byte(0));
   lcd.write(byte(0));
   //Get Side Squares 
   if (diff < 0) lcd.setCursor (7 - squares, 1); //Note is flat, display on the left
   else lcd.setCursor (9, 1);//Note is Sharp, display squares on the right
   for (int i = 0; i < squares; i++) lcd.write(byte(0));
   return;
 }
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


