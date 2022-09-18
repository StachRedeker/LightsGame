//// ARCADE LIGHT GAME ////
//// BY STACH REDEKER ////
////      V1.0.      ////


// USED SHIFT REGISTER PINS LEDS //
const int latchPin1 = 11; // LEDS
const int clockPin1 = 9;  // LEDS 
const int dataPin1 = 12;  // LEDS

// USED SHIFT REGISTER PINS DISPLAY PLAYER 1 //
const int latchPin2 = 6;
const int clockPin2 = 7;
const int dataPin2 = 5;

// USED SHIFT REGISTER PINS DISPLAY PLAYER 2 //
const int latchPin3 = 3;
const int clockPin3 = 4;
const int dataPin3 = 2;

// LEDS NOT CONNECTED TO SHIFT REGISTER //
#define LED8 A2
#define LED9 A3
#define LED10 A4

// BUTTONS //
#define button1 A0
#define button2 A1

// INITIAL LED SPEED
int LED_speed_init = 500;


/// ^^^^ EDIT ABOVE THIS LINE ^^^^ ////

// PLAYER SCORES & PLAYER VARIABLES //
int score1 = 0;
int score2 = 0;
int autoPlay1 = 0;
int autoPlay2 = 0;

// SEVEN SEGMENT DISPLAY VALUES //
byte seven_seg_digits[21] = { B11111100,  // = 0
                              B01100000,  // = 1
                              B11011010,  // = 2
                              B11110010,  // = 3
                              B01100110,  // = 4
                              B10110110,  // = 5
                              B10111110,  // = 6
                              B11100000,  // = 7
                              B11111110,  // = 8
                              B11100110,  // = 9
                              B11111101,  // = 0. 10
                              B01100001,  // = 1. 11
                              B11011011,  // = 2. 12
                              B11110011,  // = 3. 13
                              B01100111,  // = 4. 14
                              B10110111,  // = 5. 15
                              B10111111,  // = 6. 16
                              B11100001,  // = 7. 17
                              B11111111,  // = 8. 18
                              B11100111,  // = 9. 19
                              B00011100   // = L  20
                             };

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
  pinMode(latchPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(latchPin3, OUTPUT);
  pinMode(clockPin3, OUTPUT);
  pinMode(dataPin3, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(LED9, OUTPUT);
  pinMode(LED10, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  resetGame(); // reset all game values before we start

  // CHECK IF AUTOPLAY IS ACTIVATED //
  if (digitalRead(button1) == HIGH) {
    autoPlay1 = 1;
    display1(score1);
  }
  
  if (digitalRead(button2) == HIGH) {
    autoPlay2 = 1;
    display2(score2);
  }
  
  
  Serial.begin(9600);
  delay(2000); // and wait two seconds before the start of the game. 
  // This line is also here to solve a bug that is caused if player 1 activates the autoplay: if we ignore the wait time, 
  // the Arduino will register the button press also as a game button press, yielding a 1 point loss for player 1

  
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

  // PRINT PLAYER 2 SCORE
  if (score2 < 10) { 
    display2(score2);
  }
  else { // check if player 1 has lost (that is the case when player 2 has 10 points)
    display1(20); // show L on the losing player's display
    winAnimation();
    resetGame();
  }
  
  startFrom2(); // continue with player 2

  // DIAGNOSITICS //
  Serial.println("Player 2's turn is over.");
  Serial.print("Score Player 1: ");
  Serial.println(score1);
  Serial.print("Score Player 2: ");
  Serial.println(score2);

  // PRINT PLAYER 1 SCORE
  if (score1 < 10) { 
    display1(score1);
  }
  else { // check if player 2 has lost (that is the case when player 1 has 10 points)
    display2(20); // show L on the losing player's display
    winAnimation();
    resetGame();
  }

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

// FUNCTION FOR UPDATING THE SHIFT REGISTER CONTROLLING THE LEDS//
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
      
      for (int i = 0; i < 11; i++) { // loop through the 11 LEDs
      LEDcontrol(i, 1); // and turn LED n on
      Serial.print("LED");
      Serial.println(i);

      while (true) {
        unsigned long currentTime = millis(); // a solve for not using delay. Using millis(), we are able to detect a button press at all times

        if (currentTime - previousTime >= LED_speed) { // if the LED_speed time has passed, move on, if not, start looking for button presses
         previousTime = currentTime;
         break;
        }

        if ((digitalRead(button1) == HIGH) && i < 8) { // too early press
          delay(1000);
          LEDcontrol(i, 0);
          score2++;
          Serial.println("Too early!");
          resetSpeed();
          return 0;
        }
         else if ((digitalRead(button1) == HIGH) || (autoPlay1 == 1) && i >= 8 && i < 10) { // press just right
           delay(1000);
           LEDcontrol(i, 0);
           Serial.println("Correct press!");
           return 0;
        }
         else if (i == 10) { // too late press
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
         else if ((digitalRead(button2) == HIGH) || (autoPlay2 == 1) && i <= 2 && i > 0) {
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

// DISPLAY A NUMBER FOR PLAYER 1
void display1(byte digit) {
  
  digitalWrite(latchPin2, LOW);

  if (autoPlay1 == 0) { 
    shiftOut(dataPin2, clockPin2, LSBFIRST, seven_seg_digits[digit]);  
  }
  else { // if the autoplay is turned on, show a dot next to every number requested (and given the way we structured our array, we need to add 10 to the actual number)
    shiftOut(dataPin2, clockPin2, LSBFIRST, seven_seg_digits[digit + 10]); 
  }
  
  digitalWrite(latchPin2, HIGH);
  
}

// DISPLAY A NUMBER FOR PLAYER 2
void display2(byte digit) {
  
  digitalWrite(latchPin3, LOW);

  if (autoPlay2 == 0) {
    shiftOut(dataPin3, clockPin3, LSBFIRST, seven_seg_digits[digit]); 
  }
  else {
     shiftOut(dataPin3, clockPin3, LSBFIRST, seven_seg_digits[digit + 10]); 
  }
  
  digitalWrite(latchPin3, HIGH);
  
}

// RESETTING LEDS & SPEED & DISPLAYS//
void resetGame() { 

  updateLEDs();
  resetSpeed();
  score1 = 0;
  score2 = 0;
  autoPlay1 = 0;
  autoPlay2 = 0;
  digitalWrite(LED8, LOW);
  digitalWrite(LED9, LOW);
  digitalWrite(LED10, LOW);
  display1(score1);
  display2(score2);
  
}

// A FUNNY WIN ANIMATION //
void winAnimation() {

  for (int i = 0; i < 11; i++) { // loop through all the LEDs
    LEDcontrol(i, 1);
    delay(100);
    LEDcontrol(i,0);
  }
  for (int i = 10; i >= 0; i--) { // loop back
    LEDcontrol(i, 1);
    delay(100);
    LEDcontrol(i,0);
  }

  delay(5000); // wait 5 seconds before restarting
  
}
