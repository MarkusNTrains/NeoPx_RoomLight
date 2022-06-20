#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 22

#define NUMPIXELS      16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int taster1Pin = 12;                 //Taster 1 an Pin 12 angeschlossen
int taster2Pin = 11;
int taster1LED = 10;
int taster2LED = 9;

long T_Led1High = 20000;              //LED 1 bleibt 1000ms an
long HT_Led1High = T_Led1High;       //Hilfstimer der runter Zählt
long T_Led1Low = 20000;               //LED 1 bleibt 2000ms aus
long HT_Led1Low = T_Led1Low;         //Hilfstimer der runter 
long T_Fade = 20;                    //Zeit zwischen den einzelnen Statusänderungen beim Faden
long HT_Fade = T_Fade;
long T_Gewitter = 8000;
long HT_Gewitter = T_Gewitter;

unsigned long currentMillis;
unsigned long previousMillis;
unsigned long CycleMillis;

int Step = 1;                        //aktueller Step der abgearbeitet wird.
int brightness = 255;                  //Helligkeits Wert der LED
int counter = 0;                     //Zähler für die Fade-Funktionen

int HRot = 0;
int HGruen = 0;
int HBlau = 0;
int HWeis = 0;
int HRot2;
int HGruen2;
int HBlau2;
int HWeis2;
float GewitterFaktor = 1;
bool Gewitter = false;

void setup() {
  pixels.begin(); 
  pinMode(taster1Pin, INPUT_PULLUP); //Taster ist immer HIGH und muss beim Drücken gegen GND gebrückt werden
  pinMode(taster2Pin, INPUT_PULLUP);
}

void loop() {
  //millis() ist die interne Uhr welche bei 0 startet und jede Millisekunde eins nach oben Zählt
  currentMillis = millis();                      //hier sichern wir die aktuelle "Zeit"                    
  CycleMillis = currentMillis - previousMillis;  //berechnen der Zykluszeit
  previousMillis = currentMillis;                //Duplizieren der aktuellen "Zeit" für die Berechnung im nächsten Zyklus
//-----------------Step 1-----------------------  
  if(Step == 1)
  {
    HT_Fade = HT_Fade - CycleMillis;
    if(HT_Fade <= 0)
    {
      HT_Fade = T_Fade;
      if(HRot == brightness)
      {
        HGruen = HGruen +1;
        HWeis = HWeis +1;
      }
      else
      {
        HRot = HRot +1;
        HBlau = HBlau +1;
      }
    }
    if (HGruen == brightness)
    {
      Step = 2;
    }
  }
//-----------------Step 2-----------------------  
  if(Step == 2)
  {
    HT_Led1High = HT_Led1High - CycleMillis;     //Restzeit wird verkürzt
    if(HT_Led1High <= 0) 
    {
      HT_Led1High = T_Led1High;                  //Restzeit zurücksetzten 
      brightness = 10;
      Step = 3;   
    }
  }
//-----------------Step 3-----------------------  
  if(Step == 3)
  {
    HT_Fade = HT_Fade - CycleMillis;
    if(HT_Fade <= 0)
    {
      HT_Fade = T_Fade;
      if(HGruen == 0)
      {
        HRot = HRot -1;
        HBlau = HBlau -1;
      }
      else
      {
        HGruen = HGruen -1;
        HWeis = HWeis -1;
      }
    }
    if (HRot == brightness)
    {
      Step = 4;     
    }
  }
//-----------------Step 4-----------------------  
  if(Step == 4)
  {
    HT_Led1Low = HT_Led1Low - CycleMillis;
    if(HT_Led1Low <= 0)
    {
      HT_Led1Low = T_Led1Low;
      brightness = 200;
      Step = 1;
    }    
  }

//-----------------Taster 1 - Gewitter start---------------
  if(digitalRead(taster1Pin) == LOW)              //Wenn die Taste gedrückt wird...
  {
    Gewitter = true;                                
  }
  if(Gewitter == true)
  {
    digitalWrite(taster1LED, HIGH);
    HT_Gewitter = HT_Gewitter - CycleMillis;
    if(HT_Gewitter <= 0)
    {
      HT_Gewitter = T_Gewitter;
      Gewitter = false;
    }      
  }
  else
  {
    digitalWrite(taster1LED, LOW);
  }
//-----------------Bei gewitter abdunkeln---------------------
  if(Gewitter == true)
  {
    if(GewitterFaktor > 0.3)
    {
      GewitterFaktor = GewitterFaktor - 0.01;
    }
  }
  else
  {
    if(GewitterFaktor < 1)
    {
      GewitterFaktor = GewitterFaktor + 0.01;
    }
  }
  HRot2 = HRot * GewitterFaktor;
  HGruen2 = HGruen * GewitterFaktor;
  HBlau2 = HBlau * GewitterFaktor;
  HWeis2 = HWeis * GewitterFaktor; 
//------------------------Alle an-------------------------------
  if(digitalRead(taster2Pin) == LOW)
  {
    digitalWrite(taster2LED, HIGH);
    HRot2 = 255;
    HGruen2 = 255; 
    HBlau2 = 255;
    HWeis2 = 255; 
  }
  else{
    digitalWrite(taster2LED, LOW);
  }

//----------------Helligkeit an LEDs übertragen-----------------      
  for(int LED=0; LED<NUMPIXELS; LED++)
  {
    pixels.setPixelColor(LED, pixels.Color(HRot2,HGruen2,HBlau2,HWeis2));
  }


//---------------zufällige Blitze erzeugen---------------------  
  if(Gewitter == true)
  {
    randomSeed(millis());
    int Blitz = random(1, 100);
    if (Blitz == 1)
    {
      int Ort = random(1, NUMPIXELS);
      pixels.setPixelColor(Ort, pixels.Color(255,255,255,255));
      Ort = Ort+1;
      pixels.setPixelColor(Ort, pixels.Color(255,255,255,255));
      Ort = Ort-1;
      pixels.setPixelColor(Ort, pixels.Color(255,255,255,255));
    }
  }  
//------------------Werte an Stripe senden----------------------     
  pixels.show();
}
