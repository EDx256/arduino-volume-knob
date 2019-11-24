// KY-040 volume controller
// Wiring map
// CLK <-> D11
// DT <-> D9
// SW <-> D10
// + <-> 5v
// GND <-> GND

// https://github.com/NicoHood/HID
// needed to control media keys
#include "HID-Project.h"

int CLK_PIN = 11; // CLK pin
int DT_PIN = 9;   // DT pin
int SW_PIN = 10;  // SW pin (click)

int clkPinLastValue; 
bool wasKnobDown;

void setup() 
{ 
  // init pins, use pullup for internal resistors, seems to stabilize it a bit
  pinMode (CLK_PIN, INPUT_PULLUP);
  pinMode (DT_PIN,  INPUT_PULLUP);
  pinMode (SW_PIN,  INPUT_PULLUP);

  // init pin last values
  clkPinLastValue = digitalRead(CLK_PIN);   
  wasKnobDown = digitalRead(SW_PIN) == 0;

  Consumer.begin();
  Serial.begin (9600);
} 

void loop() 
{ 
   handleRotation();
   handleKnobPress();  
} 

void handleRotation()
{
  int clkPinCurrValue = digitalRead(CLK_PIN);
  
  if (clkPinCurrValue == clkPinLastValue) // no rotation happening, nothing to do
    return;
  
  if(digitalRead(DT_PIN) == clkPinCurrValue) // if DT and CLK are the same, rotation is CCW
    onRotateCCW();
  else
    onRotateCW();

  clkPinLastValue = clkPinCurrValue;
}

void handleKnobPress()
{
  bool isKnobDown = digitalRead(SW_PIN) == 0; // LOW when knob pressed
  
  if(isKnobDown && !wasKnobDown)
    onKnobDown();
  else if(!isKnobDown && wasKnobDown)
    onKnobUp();
  else if(isKnobDown && wasKnobDown)
    onKnobHeld();
  //else if (!isKnobDown && !wasKnobDown)
  //  onKnobReleased();
  wasKnobDown = isKnobDown;
}

// ############# CALLBACKS #############

void onRotateCW()
{
  Consumer.write(MEDIA_VOLUME_UP);
}

void onRotateCCW()
{
  Consumer.write(MEDIA_VOLUME_DOWN);
}

void onKnobDown() // called one time on press
{
  Consumer.write(MEDIA_VOLUME_MUTE);
}

void onKnobHeld() // called each tick knob is held after initial down
{
  //Serial.println("held");
}

void onKnobUp() // called one time on release
{
  //Serial.println("up");
}

// off by default
void onKnobReleased () // called each tick knob is not held after initial up
{
  //Serial.println("released");
}
 