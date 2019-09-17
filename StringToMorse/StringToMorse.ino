/*
  StringToMorse Serial Converter
  - Action: Takes serial input and flashes LED taking spaces and pauses into account.
  - Built by: Miguel Tapia
  - Boilerplate:  Example blink last modified by Scott Fitzgerald
  - Consulted Sources:
    * Morse Chart  :  https://morsecode.scphillips.com/morse2.html
    * Morse Timings:  https://morsecode.scphillips.com/timing.html
    * Text to Morse:  http://www.unit-conversion.info/texttools/morse-code/
  - NOTE:
    * I intentionally left Serial.print in multiple locations. This is to
    see the morse "." or "-" in screen as well as through the led. Furthermore,
    the spaces between characters and words can be seen on the serial monitor.
*/

// Needed Variable Declarations
int outPin;
int dotSpeed;
int dashSpeed;
int wordSpeed;
String input;

char letters[51] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 
                    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
                    'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                    '&', '\'', '@', ')', '(', ':', ',', '=', '!', '.', '-', '+',
                    '"', '?', '/'
                    };

String morse[51] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", 
                    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", 
                    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----",
                    ".----", "..---", "...--", "....-", ".....", "-....", "--...", 
                    "---..", "----.", ".-...", ".----.", ".--.-.", "-.--.-", "-.--.", 
                    "---...", "--..--", "-...-", "-.-.--", ".-.-.-", "-....-", ".-.-.", 
                    ".-..-.", "..--..", "-..-."
                    };


// START: setup()                     
void setup() {
  // * Ardino Setup Before Running Loop *
  /*
    - Intra-character space and Inter-character space not
    declared or initialized, for these spaces are equal to
    1 unit and 3 unit respectively which are the same as 
    dotSpeed and dashSpeed. Thus, I decided to use dotSpeed 
    and dashSpeed to represent these spaces. 
  */
  
  outPin = LED_BUILTIN;
  dotSpeed = 250;
  dashSpeed = 750;
  wordSpeed = 1750;
  
  input = "";
  pinMode(outPin, OUTPUT);

  // Opens Serial Connection
  Serial.begin(9600);
} // END: setup() 


// START: toMorse() 
String toMorse(String standardIn) {
  // * Reads each character in "standardIn" and output the equivalent morse code.*
  int sLength = standardIn.length(); 
  String output = "";
  
  for (int i = 0; i < sLength; i++) {
    char thisChar = standardIn.charAt(i);
    if (thisChar == ' ') {
      output += "~";
      continue; 
    }
    
    for (int j = 0; j < 51; j++) {
      if (letters[j] == thisChar) {
        output += morse[j] + "|";
        exit; // Takes you out into the parent loop
      }
    }
  }

  Serial.print(output);
  return output;
} // END: toMorse() 


// START: flashTheLed() 
void flashTheLed(String morseIn) {
  // * Flashes the LED accordingly *
  int lenMorseIn = morseIn.length();
  String charSec = "";
  char currentChar;
  int j = 0; // "J" declared here so that inner for knows its last position in charSec
  
  for (int i = 0; i < lenMorseIn; i++) {
    currentChar = morseIn.charAt(i);
    if (currentChar != '|') {
      // Creates String With One Character
      charSec += currentChar;
    } else {
      /* Flash */
      // A pipe was reached. Flash the light.
      int lenCharSec = charSec.length();
      for (;j < lenCharSec; j++) {
        if (charSec.charAt(j) == '-') {
          Serial.print(charSec.charAt(j));
          digitalWrite(outPin, HIGH);
          delay(dashSpeed);
          digitalWrite(outPin, LOW);
          
          /* Intra-Character Space 1 Unit */
          delay(dotSpeed);
        } else if (charSec.charAt(j) == '.') {
          Serial.print(charSec.charAt(j));
          digitalWrite(outPin, HIGH);
          delay(dotSpeed);
          digitalWrite(outPin, LOW);
          
          /* Intra-Character Space 1 Unit */
          delay(dotSpeed);
        } else {
          Serial.print(charSec.charAt(j));
          
          /* Word Space 7 Units */
          delay(wordSpeed);
        }
      }
      /* Inter-Character Space 3 Units */
      Serial.print(" "); // Shows inter char spaces 
      delay(dashSpeed);
    }
  }

  // Newline For Multiple Entries
  if (lenMorseIn != 0) {
    Serial.println('\n'); 
  }
} // END: flashTheLed() 


// START: loop() 
void loop() {
  // * Reads data incoming from serial line *
  while (Serial.available()) {
    input += Serial.readString();
  }
  /* Change Input to Uppercase Characters */
  input.toUpperCase();

  /* Get Morse String */
  String morsed = toMorse(input);

  Serial.print(input);
  flashTheLed(morsed);

  // Finished with the output; reset variables for next input
  input = "";
} // END: loop() 
