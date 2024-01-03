#include <MaxMatrix.h>

int DIN = 4;
int CS = 3;
int CLK = 2;
int maxuse = 1;

int p1pin = A0;
int p2pin = A1;
int count = 0;

int game[8][8] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};
int p1 = 0;
int p2 = 0;

int ball[] = { 4, 5 };  // col, row aka H, V

bool ballDirV = true;  // true == up
bool ballDirH = true;  // true == left

MaxMatrix m(DIN, CS, CLK, maxuse);

void setup() {
  // put your setup code here, to run once:
  m.init();
  m.setIntensity(8);
  Serial.begin(9600);
}

void loop() {
  int sensor1 = analogRead(p1pin);
  p1 = map(sensor1, 0, 1023, 0, 59);
  p1 /= 10;
  int sensor2 = analogRead(p2pin);
  p2 = map(sensor2, 0, 1023, 0, 59);
  p2 /= 10;
  createPaddles();
  delay(50);
  count++;
  if (count > 10) {
    ballMovement();
    count = 0;
  }
}

void createPaddles() {
  if ((game[0][p1] != 1 || game[7][p2] != 1) || (game[0][p1 + 2] != 1 || game[7][p2 + 2] != 1)) {
    for (int i = 0; i < 8; i++) {
      m.setDot(0, i, false);
      m.setDot(7, i, false);
      game[i][7] = 0;
      game[i][0] = 0;
    }
    for (int i = p1; i < p1 + 3; i++) {
      m.setDot(0, i, true);
      game[i][0] = 1;
    }
    for (int i = p2; i < p2 + 3; i++) {
      m.setDot(7, i, true);
      game[i][7] = 1;
    }
  }
}

void ballMovement() {
  while (ballDirV) {
    if (ball[1] >= 7) {
      ballDirV = !ballDirV;
      break;
    }

    while (ballDirH) {
      if (ball[1] >= 7) {
        ballDirV = !ballDirV;
        break;
      }
      if (ball[0] >= 6) {
        printGame();
        if (game[ball[1]][7] == 1) {
          ballDirH = !ballDirH;
          break;
        } else {
          p2miss();
          resetBall();
          delay(500);
        }
      }
      m.setDot(ball[0], ball[1], false);
      ball[0] = ball[0] + 1;
      ball[1] = ball[1] + 1;
      m.setDot(ball[0], ball[1], true);
      break;
    }
    while (!ballDirH) {
      if (ball[1] >= 7) {
        ballDirV = !ballDirV;
        break;
      }
      if (ball[0] <= 1) {
        printGame();
        if (game[ball[1]][0] == 1) {
          ballDirH = !ballDirH;
          break;
        } else {
          p1miss();
          resetBall();
          delay(500);
        }
      }
      m.setDot(ball[0], ball[1], false);
      ball[0] = ball[0] - 1;
      ball[1] = ball[1] + 1;
      m.setDot(ball[0], ball[1], true);
      break;
    }
    break;
  }
  while (!ballDirV) {
    if (ball[1] <= 0) {
      ballDirV = !ballDirV;
      break;
    }

    while (ballDirH) {
      if (ball[1] <= 0) {
        ballDirV = !ballDirV;
        break;
      }
      if (ball[0] >= 6) {
        printGame();
        if (game[ball[1]][7] == 1) {
          ballDirH = !ballDirH;
          break;
        } else {
          p2miss();
          resetBall();
          delay(500);
        }
      }
      m.setDot(ball[0], ball[1], false);
      ball[0] = ball[0] + 1;
      ball[1] = ball[1] - 1;
      m.setDot(ball[0], ball[1], true);
      break;
    }
    while (!ballDirH) {
      if (ball[1] <= 0) {
        ballDirV = !ballDirV;
        break;
      }
      if (ball[0] <= 1) {
        printGame();
        if (game[ball[1]][0] == 1) {
          ballDirH = !ballDirH;
          break;
        } else {
          p1miss();
          resetBall();
          delay(500);
        }
      }
      m.setDot(ball[0], ball[1], false);
      ball[0] = ball[0] - 1;
      ball[1] = ball[1] - 1;
      m.setDot(ball[0], ball[1], true);
      break;
    }
    break;
  }
}

void p1miss() {
  //m.clear();
  for (int i = 0; i < 4; i++) {
    // turn on
    for (int j = i; j > -1; j--) {
      m.setDot(i - j, 3 - j, true);
      m.setDot(i - j, 4 + j, true);
    }
    delay(200);
    // turn off old instruction
    for (int j = i; j > -1; j--) {
      m.setDot(i - j, 3 - j, false);
      m.setDot(i - j, 4 + j, false);
    }
    // for(int col = 0; col < 8; col++){
    //   m.setDot(col, 0, false);
    // }
  }
  m.clear();
}

void p2miss() {
  //m.clear();
  for (int i = 7; i > 3; i--) {
    // turn on
    for (int j = i; j > -1; j--) {
      m.setDot(i + j, 3 - j, true);
      m.setDot(i + j, 4 + j, true);
    }
    delay(200);
    // turn off old instruction
    for (int j = i; j > -1; j--) {
      m.setDot(i + j, 3 - j, false);
      m.setDot(i + j, 4 + j, false);
    }
    // for(int col = 0; col < 8; col++){
    //   m.setDot(col, 0, false);
    // }
  }
  m.clear();
}

void resetBall() {
  ballDirV = !ballDirV;
  ballDirH = !ballDirH;
  ball[0] = 3;
  ball[1] = 4;
}

void printGame(){
  Serial.println("Current Field:");
  for (int row = 0; row < 8; row++){
    for(int col = 0; col < 8; col++){
      Serial.print(game[row][col]);
    }
    Serial.println("");
  }
}