/*
  Musical Instrument
  - Action: Takes input from membrane keypad and outputs musical notes through Pulse Width Modulation (PWM) and a passive
  buzzer. The project works with 12 notes in octaves 1, 3, 5, and 7. The octave can be changed through the letters (A-D)
  on the membrane keypad. This prototype also displays frequency intensity in the respective octave through a matrix display.
  Additionally, there are colored LEDs displaying change between octaves.
  - Built by: Miguel Tapia
  - Boilerplate: Keypad Example, Matrix Example
  - Consulted Sources:
    * Frequency Chart: https://i.pinimg.com/originals/41/ee/a3/41eea38658a8db1e57e7a444dad10037.png
    * https://bgsu.instructure.com/courses/1157282/pages/tutorial-passive-buzzer
    * https://www.arduino.cc/reference/en/language/variables/utilities/sizeof/
  - NOTE: 
    * Included scroll and bounce effect on octave LED indicators at the start of the program.
*/

// -------------------- Libraries --------------------
#include <Keypad.h>
#include "LedControl.h"
#include <binary.h>

// -------------------- LED Matrix Inits --------------------
LedControl lc=LedControl(13,11,12,1);
unsigned long delayMatrix = 250;

// -------------------- Membrane Keyboard Inits --------------------
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// -------------------- Project Inits --------------------
// PWM Init Pin
const int PWM = 10;

// Octave Light Indicator Pins
const int indA = 38;
const int indB = 42;
const int indC = 46;
const int indD = 50;

// Int Array to Store Index
int iNote[2];

// Current Octave Global
char currentOctave = 'A';

// Octave 1
int notesA [4][3] = {
  {33, 35, 37}, 
  {39, 41, 44}, 
  {46, 49, 52}, 
  {55, 58, 62}
};

// Octave 3
int notesB [4][3] = {
  {131, 139, 147}, 
  {156, 165, 175}, 
  {185, 196, 208}, 
  {220, 233, 247}    
};

// Octave 5
int notesC [4][3] = {
  {523, 554, 587}, 
  {622, 659, 698}, 
  {740, 784, 831}, 
  {880, 932, 988}    
};

// Octave 7
int notesD [4][3] = {
  {2093, 2217, 2349}, 
  {2489, 2637, 2793}, 
  {2960, 3136, 3322}, 
  {3520, 3729, 3951}    
};

// GetIndex() - Gets Index To Map Against Current Octave
void getIndex (char key) {
  for (int i = 0; i < sizeof(hexaKeys) / sizeof(hexaKeys[0]); i++) {
    for (int j = 0; j < (sizeof(hexaKeys[i]) / sizeof(hexaKeys[0][0])); j++) {
      if (hexaKeys[i][j] == key) {
        iNote[0] = i;
        iNote[1] = j;
      }
    }
  }
}

// GetOctave() - Gets Octave If A, B, C, D Is Pressed
bool getOctave (char key) {
  // DigitalWrites Shine The Octave In Use
  if (key == 'A') {
    currentOctave = 'A';
    digitalWrite(indA, HIGH);
    delay(200);
    digitalWrite(indA, LOW);
    return true;
  } else if (key == 'B') {
    currentOctave = 'B';
    digitalWrite(indB, HIGH);
    delay(200);
    digitalWrite(indB, LOW);
    return true;
  } else if (key == 'C') {
    currentOctave = 'C';
    digitalWrite(indC, HIGH);
    delay(200);
    digitalWrite(indC, LOW);
    return true;
  } else if (key == 'D') {
    currentOctave = 'D';
    digitalWrite(indD, HIGH);
    delay(200);
    digitalWrite(indD, LOW);
    return true;
  }
  return false;
}

// LightSection() - Shines Portion Of Matrix
void lightSection (int r) {
  switch (r) {
    case 0:
      for (int i = 0; i < 2; i++) {
         lc.setColumn(0, i,  B11111111);
      }
      break;
    case 1:
      for (int i = 0; i < 4; i++) {
         lc.setColumn(0, i,  B11111111);
      }
      break;
    case 2:
      for (int i = 0; i < 6; i++) {
         lc.setColumn(0, i,  B11111111);
      }
      break;
    case 3:
      for (int i = 0; i < 8; i++) {
         lc.setColumn(0, i,  B11111111);
      }
      break;
  }
  delay(delayMatrix);
  lc.clearDisplay(0);
}

// RingTone() - Rings Tone According To Octave
void ringNote () {
  // LightSection Takes Index Mapped To Shine A Particular Portion
  if (currentOctave == 'A') {
    tone(PWM, notesA[iNote[0]][iNote[1]]);
    lightSection(iNote[0]);
  } else if (currentOctave == 'B') {
    tone(PWM, notesB[iNote[0]][iNote[1]]);
    lightSection(iNote[0]);
  } else if (currentOctave == 'C') {
    tone(PWM, notesC[iNote[0]][iNote[1]]);
    lightSection(iNote[0]);
  } else if (currentOctave == 'D') {
    tone(PWM, notesD[iNote[0]][iNote[1]]);
    lightSection(iNote[0]);
  }
  delay(200);
  noTone(PWM);
}

// FancySwayingLights() - Just A Cool Intro For Setup
void fancySwayingLights () {
  for (int i = 1; i <= 8; i++) {
    if (i % 2 == 0) {
      digitalWrite(indD, HIGH);
      delay(50);
      digitalWrite(indD, LOW);
      digitalWrite(indC, HIGH);
      delay(50);
      digitalWrite(indC, LOW);
      digitalWrite(indB, HIGH);
      delay(50);
      digitalWrite(indB, LOW);      
      digitalWrite(indA, HIGH);
      delay(50);
      digitalWrite(indA, LOW);
    } else {
      digitalWrite(indA, HIGH);
      delay(50);
      digitalWrite(indA, LOW);
      digitalWrite(indB, HIGH);
      delay(50);
      digitalWrite(indB, LOW);
      digitalWrite(indC, HIGH);
      delay(50);
      digitalWrite(indC, LOW);
      digitalWrite(indD, HIGH);
      delay(50);
      digitalWrite(indD, LOW);
    }
  }
}

// Setup() - Runs At Start
void setup(){
  // Start Serial Communication
  Serial.begin(9600);

  // Startup Matrix and Pins
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  pinMode(indA, OUTPUT);
  pinMode(indB, OUTPUT);
  pinMode(indC, OUTPUT);
  pinMode(indD, OUTPUT);

  // Make Lights Go Fancy...
  fancySwayingLights();
}

// Loop() - Runs Continually
void loop(){
  // Gets Key Pressed
  char customKey = customKeypad.getKey();

  // Runs If There Is A Key Pressed
  if (customKey){
    // If Octave Is Getting Updated, No Need To Beep
    bool updatingOctave = getOctave(customKey);
    if (!updatingOctave) {
      getIndex(customKey);
      ringNote();
    }
  }
}
