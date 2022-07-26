
// Define i/o pin constants
const int REDBUTTON = 13;
const int GREENBUTTON = 12;
const int YELLOWBUTTON = 11;
const int BLUEBUTTON = 10;
const int RESETBUTTON = 5;
const int REDDIODE = 9;
const int GREENDIODE = 8;
const int YELLOWDIODE = 7;
const int BLUEDIODE = 6;
const int BUZZER = 3;

const int SIZE = 250;
int sequence[SIZE];
int clickSequence[SIZE];

const int BLINK = 100;
const int DIODEF = 250;
int BLINKLENGTH = 100;

int level = 0;
const int LENGTH = 5;
int difficultylength = 0;
int difficulty = 1;
bool lost = false;


void setup() {

  // Set i/o pinmode accordingly
  pinMode(REDBUTTON, INPUT_PULLUP);
  pinMode(GREENBUTTON, INPUT_PULLUP);
  pinMode(YELLOWBUTTON, INPUT_PULLUP);
  pinMode(BLUEBUTTON, INPUT_PULLUP);
  pinMode(RESETBUTTON, INPUT_PULLUP);

  pinMode(REDDIODE, OUTPUT);
  pinMode(GREENDIODE, OUTPUT);
  pinMode(YELLOWDIODE, OUTPUT);
  pinMode(BLUEDIODE, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  doIntroBlinking();
  difficultylength = LENGTH * difficulty;
  generateSequence();
  
  while (!lost) {
    delay(BLINKLENGTH * 10);

    for  (int i = 0; i < level; i++) {
      delay(BLINKLENGTH * 3);
      doBlink(sequence[i]);
    }

    for (int i = 0; i < level; i++) {
      if (!checkClick(sequence[i])) {
        lost = true;
        break;
      }
      delay(333);
    }

    if (level == difficultylength && lost == false) {
      doWinBlinking();
      return;
    }

    level++;
  }

  doLostBlinking();
}

void generateSequence() {
  const int minimum = BLUEDIODE;
  const int maximum = REDDIODE;

  randomSeed(millis());
  for (int i = 0; i < SIZE; i ++) {
    sequence[i] = random(4) + BLUEDIODE;
  }
}

void doWinBlinking() {
  while (true) {
      digitalWrite(REDDIODE, HIGH);
      digitalWrite(GREENDIODE, HIGH);
      digitalWrite(YELLOWDIODE, HIGH);
      digitalWrite(BLUEDIODE, HIGH);
      delay(BLINKLENGTH);
      if (isPressed(RESETBUTTON)) {
        resetState();
        return;
      }

      tone(BUZZER, DIODEF * 4);
      delay(BLINKLENGTH * 1.5);
      noTone(BUZZER);

      tone(BUZZER, DIODEF * 5);
      delay(BLINKLENGTH * 1.5);
      noTone(BUZZER);

      tone(BUZZER, DIODEF * 6);
      delay(BLINKLENGTH * 1.5);
      noTone(BUZZER);
      
      digitalWrite(REDDIODE, LOW);
      digitalWrite(GREENDIODE, LOW);
      digitalWrite(YELLOWDIODE, LOW);
      digitalWrite(BLUEDIODE, LOW);
      delay(BLINKLENGTH);
      if (isPressed(RESETBUTTON)) {
        resetState();
        return;
      }
  }
}

void doIntroBlinking() {
  while (true) {
    doBlink(REDDIODE);
    delay(BLINKLENGTH * 3);
    if (chooseDifficulty() != 0) {
      resetState();
      return;
    }

    doBlink(YELLOWDIODE);
    delay(BLINKLENGTH * 3);
    if (chooseDifficulty() != 0) {
      resetState();
      return;
    }

    doBlink(GREENDIODE);
    delay(BLINKLENGTH * 3);
    if (chooseDifficulty() != 0) {
      resetState();
      return;
    }

    doBlink(BLUEDIODE);
    delay(BLINKLENGTH * 3);
    if (chooseDifficulty() != 0) {
      resetState();
      return;
    }
  }
}

int chooseDifficulty() {
  if (isPressed(REDBUTTON)) {
    doBlink(REDDIODE);
    difficulty = 1;
    return difficulty;
  }

  if (isPressed(GREENBUTTON)) {
    doBlink(GREENDIODE);
    difficulty = 2;
    return difficulty;
  }

  if (isPressed(YELLOWBUTTON)) {
    doBlink(YELLOWDIODE);
    difficulty = 3;
    return difficulty;
  }

  if (isPressed(BLUEBUTTON)) {
    doBlink(BLUEDIODE);
    difficulty = 4;
    return difficulty;
  }

  return 0;
}

void doLostBlinking() {
  while (true) {

    doBlink(REDDIODE);
    delay(BLINKLENGTH);
    if (isPressed(RESETBUTTON)) {
      resetState();
      return;
    }

    doBlink(GREENDIODE);
    delay(BLINKLENGTH);
    if (isPressed(RESETBUTTON)) {
      resetState();
      return;
    }

    doBlink(YELLOWDIODE);
    delay(BLINKLENGTH);
    if (isPressed(RESETBUTTON)) {
      resetState();
      return;
    }

    doBlink(BLUEDIODE);
    delay(BLINKLENGTH);
    if (isPressed(RESETBUTTON)) {
      resetState();
      return;
    }
  }
}

void resetState() {
  lost = false;
  level = 0;
}

bool isPressed(int button) {
  return digitalRead(button) == HIGH;
}

void doBlink(int diode) {
  digitalWrite(diode, HIGH);
  tone(BUZZER, (diode - BLUEDIODE)*DIODEF + 250);
  delay(BLINKLENGTH);
  digitalWrite(diode, LOW);
  noTone(BUZZER);
}

bool checkClick(int sequence) {

  while (true) {
    if (isPressed(REDBUTTON) && !(isPressed(GREENBUTTON) ||  isPressed(YELLOWBUTTON) || isPressed(BLUEBUTTON)) && sequence == REDDIODE) {
      doBlink(REDDIODE);
      return true;
    } else if ((isPressed(GREENBUTTON) ||  isPressed(YELLOWBUTTON) || isPressed(BLUEBUTTON)) && sequence == REDDIODE) {
      return false;
    }

    if (isPressed(GREENBUTTON) && !(isPressed(REDBUTTON) ||  isPressed(YELLOWBUTTON) || isPressed(BLUEBUTTON)) && sequence == GREENDIODE) {
      doBlink(GREENDIODE);
      return true;
    } else if ((isPressed(REDBUTTON) ||  isPressed(YELLOWBUTTON) || isPressed(BLUEBUTTON)) && sequence == GREENDIODE) {
      return false;
    }

    if (isPressed(YELLOWBUTTON) && !(isPressed(GREENBUTTON) ||  isPressed(REDBUTTON) || isPressed(BLUEBUTTON)) && sequence == YELLOWDIODE) {
      doBlink(YELLOWDIODE);
      return true;
    } else if ((isPressed(GREENBUTTON) ||  isPressed(REDBUTTON) || isPressed(BLUEBUTTON)) && sequence == YELLOWDIODE) {
      return false;
    }

    if (isPressed(BLUEBUTTON) && !(isPressed(GREENBUTTON) ||  isPressed(YELLOWBUTTON) || isPressed(REDBUTTON)) && sequence == BLUEDIODE) {
      doBlink(BLUEDIODE);
      return true;
    } else if ((isPressed(GREENBUTTON) ||  isPressed(YELLOWBUTTON) || isPressed(REDBUTTON)) && sequence == BLUEDIODE) {
      return false;
    }

    if (isPressed(RESETBUTTON)) {
      lost = true;
      return false;
    }
  }

}
