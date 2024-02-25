#include <Keypad.h>
#include <Wire.h>
#include <Encoder.h>
#include <Bounce2.h>
#include "HID-Project.h"

//Keypad buttons
int R1 = 6;
int R2 = 5;
int R3 = 21;
int R4 = 20;
int C1 = 7;
int C2 = 8;
int C3 = 9;
const byte ROWS = 4;
const byte COLS = 3;
char keys[COLS][ROWS] = {
  {'X','7','4','1'},
  {'*','8','5','2'},
  {'-','9','6','3'}
};
byte rowPins[ROWS] = {R1, R2, R3, R4};
byte colPins[COLS] = {C1, C2, C3};
Keypad kpd = Keypad( makeKeymap(keys), colPins, rowPins, COLS, ROWS);


const int numStates = 4;
int currentState = 0;

int lastDebounceTime = 0;
const int debounceTime = 50;

//Encoder
int SW = 19;
int DT = 0;
int CLK = 1;
Encoder volumeKnob(DT,CLK);
Bounce encoderButton = Bounce(SW,10);
int timeLimit = 500;
long oldPosition = -999;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(CLK, INPUT_PULLUP);

  Keyboard.begin();
  //Consumer.begin();

  Serial.print("Ready");

}


void changeStateUp(){
  currentState++;
  if (currentState == numStates){
    currentState = 0;
  }
  //Serial.print("State Changed. Current State: "); 
  //Serial.println(currentState);
  delay(100);
  return;
}

void changeStateDown(){
  currentState--;
  if (currentState == 0){
    currentState = numStates;
  }
 
  //Serial.print("State Changed. Current State: "); 
  //Serial.println(currentState);
  delay(100);
  return;
}

void Layout1(char button){
  switch(button){
    case '1':
      Keyboard.print('1');
      break;
    case '2':
      Keyboard.print('2');
      break;
    case '3':
      Keyboard.print('3');
      break;
    case '4':
      Keyboard.print('4');
      break;
    case '5':
      Keyboard.print('5');
      break;
    case '6':
      Keyboard.print('6');
      break;
    case '7':
      Keyboard.print('7');
      break;
    case '8':
      Keyboard.print('8');
      break;
    case '9':
      Keyboard.print('9');
      break;
  };
}//
void Layout2(char button){
  switch(button){
    case '1'://
      break;
    case '2'://
      break;
    case '3'://
      break;
    case '4'://
      break;
    case '5'://
      break;
    case '6'://Return
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      break;
    case '7'://Escape
      Keyboard.press(KEY_ESC);
      Keyboard.releaseAll();
      break;
    case '8'://
      break;
    case '9'://
      break;
  };
}

void Layout3(char button){
  switch(button){
    case '1':
      Keyboard.print('7');
      break;
    case '2':
      Keyboard.print('8');
      break;
    case '3':
      Keyboard.print('9');
      break;
    case '4':
      Keyboard.print('4');
      break;
    case '5':
      Keyboard.print('5');
      break;
    case '6':
      Keyboard.print('6');
      break;
    case '7':
      Keyboard.print('1');
      break;
    case '8':
      Keyboard.print('2');
      break;
    case '9':
      Keyboard.print('3');
      break;
  };
}

void Layout4(char button){
  switch(button){
    case '1':
      Keyboard.print('1');
      break;
    case '2':
      Keyboard.print('2');
      break;
    case '3':
      Keyboard.print('3');
      break;
    case '4':
      Keyboard.print('4');
      break;
    case '5':
      Keyboard.print('5');
      break;
    case '6':
      Keyboard.print('6');
      break;
    case '7':
      Keyboard.print('7');
      break;
    case '8':
      Keyboard.print('8');
      break;
    case '9':
      Keyboard.print('9');
      break;
  };
}

void loop() {
  //check the key matrix first
  char key = kpd.getKey();
  if(key) {
    switch(key){
      case '*':
        changeStateUp();
        break;
      case '-':
        changeStateDown();
        break;
      default:
        switch(currentState){
          case 0:
            Layout1(key);
            break;
          case 1:
            Layout2(key);
            break;
          case 2:
            Layout3(key);
            break;
          case 3: 
            Layout4(key);
            break;
        }
    }
  }

  //check the encoder button
  if(encoderButton.update()) {
    if(encoderButton.fallingEdge()) {
      int fall = millis();
      while(!encoderButton.update()){}
      if(encoderButton.risingEdge()){
        int rise = millis();
        //Serial.println(rise - fall);
        if(rise - fall > timeLimit){
          Consumer.write(MEDIA_NEXT);
          Serial.print("Next");
        } else {
          Consumer.write(MEDIA_PLAY_PAUSE);
          Serial.print("Play/Pause");
        }
      }
      Keyboard.releaseAll();
    }
  }

  //check encoder rotation
  long newPosition = volumeKnob.read();
  if(newPosition != oldPosition){
    Serial.print(newPosition);

    if((newPosition - oldPosition) > 0) {
      //volumeup
      Serial.println("volume up");
      Consumer.write(MEDIA_VOLUME_UP);
    } else {
      //volumedown
      Serial.println("volume down");
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
    oldPosition = newPosition;
    Keyboard.releaseAll();
    delay(200); //a delay of 200 seems to be the sweet spot for this encoder.
  }  
}