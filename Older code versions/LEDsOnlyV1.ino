//// ARCADE LIGHT GAME ////
//// BY STACH REDEKER ////


// USED SHIFT REGISTER PINS //
int latchPin1 = 11; // LEDS
int clockPin1 = 9;  // LEDS 
int dataPin1 = 12;  // LEDS

// LEDs not connected to SHIFT REGISTER //
int LED8 = 2;
int LED9 = 3;
int LED10 = 4;

// BUTTONS //
#define button1 A0
#define button2 A1

// INITIAL LED SPEED
int LED_speed_init = 500;


/// ^^^^ EDIT ABOVE THIS LINE ^^^^ ////

// PLAYER SCORES //
int score1 = 0;
int score2 = 0;

// SHIFT REGISTER VARIABLES //
byte leds = 0; // a byte, thus in this case 00000000, needed for setting the shift register

// OTHER GLOBAL VARIABLES //
unsigned long previousTime = 0;
unsigned long LED_speed;

void setup() 
{
  
  // SETTING PIN MODES //
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);  
  pinMode(clockPin1, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(LED9, OUTPUT);
  pinMode(LED10, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  // RESETTING LEDS & SPEED //
  updateLEDs();
  resetSpeed();
  digitalWrite(LED8, LOW);
  digitalWrite(LED9, LOW);
  digitalWrite(LED10, LOW);

  Serial.begin(9600);

  
}

void loop() 
{

  startFrom1(); // start with player 1

  // DIAGNOSITICS //
  Serial.println("Player 1's turn is over.");
  Serial.print("Score Player 1: ");
  Serial.println(score1);
  Serial.print("Score Player 2: ");
  Serial.println(score2);
  
  startFrom2(); // continue with player 2

  // DIAGNOSITICS //
  Serial.println("Player 2's turn is over.");
  Serial.print("Score Player 1: ");
  Serial.println(score1);
  Serial.print("Score Player 2: ");
  Serial.println(score2);

  // INCREASE SPEED AFTER SUCCESFUL ROUND //
  if (LED_speed > 50) {
    LED_speed = LED_speed - 50;
  }
  else if (LED_speed > 10) {
    LED_speed = LED_speed - 2;
  }
  

}


// FUNCTION FOR TURING ON/OFF A SPECIFIC LED //
void LEDcontrol (int LED_n, int LED_status) {
  if (LED_n >= 0 && LED_n <= 7) { // if a LED is part of the shift register
    if (LED_status == 1) { // and the wanted status is on (1)
      bitSet(leds, LED_n); // set the n-th bit in the led byte to 1
      updateLEDs(); // and reset the shift register via the function updateLEDs()
    }
    if (LED_status == 0) { // and the wanted status is off (0)
      bitClear(leds, LED_n); // set the n-th bit in the led byte to 0
      updateLEDs(); // , , ,
    }
  }
  // LEDs that are not being controlled by the shift register are being controlled manually via digitalWrite()
  else if (LED_n == 8) {
    digitalWrite(LED8, LED_status);
  }
  else if (LED_n == 9) {
    digitalWrite(LED9, LED_status);
  }
  else if (LED_n == 10) {
    digitalWrite(LED10, LED_status);
  }
}

// FUNCTION FOR UPDATING THE SHIFT REGISTER //
void updateLEDs() {
   digitalWrite(latchPin1, LOW);
   shiftOut(dataPin1, clockPin1, LSBFIRST, leds);
   digitalWrite(latchPin1, HIGH);
}

// FUNCTION FOR RESETTING THE LED SPEED //
void resetSpeed() {
  LED_speed = LED_speed_init;
}

// FUNCTION START PLAYER 1 //
void startFrom1() {
      
      for (int i = 0; i < 11; i++) {
      LEDcontrol(i, 1);
      Serial.print("LED");
      Serial.println(i);

      while (true) {
        unsigned long currentTime = millis();

        if (currentTime - previousTime >= LED_speed) {
         previousTime = currentTime;
         break;
        }

        if ((digitalRead(button1) == HIGH) && i < 8) {
          delay(1000);
          LEDcontrol(i, 0);
          score2++;
          Serial.println("Too early!");
          resetSpeed();
          return 0;
        }
         else if ((digitalRead(button1) == HIGH) && i >= 8 && i < 10) {
           delay(1000);
           LEDcontrol(i, 0);
           Serial.println("Correct press!");
           return 0;
        }
         else if (i == 10) {
           delay(1000);
           LEDcontrol(i, 0);
           score2++;
           Serial.println("Too late!");
           resetSpeed();
           return 0;
        }
        
      }
      
      LEDcontrol(i, 0);
     
    }
    
}

// FUNCTION START PLAYER 2 //
void startFrom2() {
      
      for (int i = 10; i >= 0; i--) {
      LEDcontrol(i, 1);
      Serial.print("LED");
      Serial.println(i);

      while (true) {
        unsigned long currentTime = millis();

        if (currentTime - previousTime >= LED_speed) {
         previousTime = currentTime;
         break;
        }

        if ((digitalRead(button2) == HIGH) && i > 2) {
          delay(1000);
          LEDcontrol(i, 0);
          score1++;
          Serial.println("Too early!");
          resetSpeed();
          return 0;
        }
         else if ((digitalRead(button2) == HIGH) && i <= 2 && i > 0) {
           delay(1000);
           LEDcontrol(i, 0);
           Serial.println("Correct press!");
           return 0;
        }
         else if (i == 0) {
           delay(1000);
           LEDcontrol(i, 0);
           score1++;
           Serial.println("Too late!");
           resetSpeed();
           return 0;
        }
        
      }
      
      LEDcontrol(i, 0);
     
    }
}
