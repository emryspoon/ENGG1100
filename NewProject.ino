/*
  ENGG1100 Given program for demo.
  Board: Arduino Nano
  Version: 201808

Output, Servo1 : (D9)
Output, Servo2 : (D6)
Output,  7-segment (D10,D11)
Output, motors (D3,D2), (D5,D4)
Input: IR sensor long range (A5)
Input: IR sensor left (A4)
Input: IR sensor middle (A3)
Input: IR sensor right (A2)
*/
#include "ENGG1100.h"
#include <Servo.h>
#define BLK 0
#define WHT 1

/*
all speed up
set global gettime
  always change stage to add the global time
*/

#define RIGHT_POS 180
#define REAR_POS 90
#define LEFT_POS 0
#define CLAMP_OPEN 140
#define CLAMP_RELEASE 65
#define CLAMP_CLOSE 36
#define NORMAL_SPEED 0
#define TURN_SPEED_L 230
#define TURN_SPEED_R 230
#define LEFT_OFFSET 255
#define RIGHT_OFFSET 248

FSMClass FSM1;          //The given FSM is named 'FSM1'.
TM1637DisplayClass LEDDisplay(D10, D11); //7-segment LED Display is connected to D10,D11 for debug
ServoClass Servo1(D9);
ServoClass Servo2(D6);
SensorClass S1(A5);
SensorClass S2(A4);
SensorClass S3(A3);
SensorClass S4(A2);
//MotorClass MotorR(D3,D2);
//MotorClass MotorL(D5,D4);
MotorClass MotorL(3,2);
MotorClass MotorR(5,4);

int catchPos = 0; // 0 = L; 1 = R;
int goPos = 0; // 0 = L; 1 = M; 2 = R;
long rtime = 0; // run time;
long catched = 0; // not catched (0); catched (1);

//===== Basically, no need to modify setup() and loop() ====
void setup()
{
  Serial.begin(115200);          //optional, for debug
  LEDDisplay.setBrightness(15); //optional
  FSM1.init(S_999);           // must have this line, you can change the first state of the FSM
  Servo1.init();
  Servo2.init();
}

void loop()
{
    FSM1.run(); //important, don't modify this line
}
//================= Students add STATES below this line ====================
/*
void S_000() {
  if (FSM1.doTask()) {
    LEDDisplay.setValue(000);
  }
}
//------------------------------------
*/
//----------start of state S_999 -----
void S_999() {
  if (FSM1.doTask()) {
    Servo1.setValue(CLAMP_OPEN);
    Servo2.setValue(REAR_POS);
    LEDDisplay.setValue(999);
  }
  // contorl catch Position
  // 1 go to left 2 go to right
  if (S1.getHiLow() == BLK && FSM1.getTime() < 1000) catchPos++;
  if (FSM1.getTime() > 1000) {
    // trun catcher
    // catcher position: 0 = L ; 1 = R;
    if (!catchPos) {
      Servo2.setValue(LEFT_POS);
    } else {
      Servo2.setValue(RIGHT_POS);
    }

    // go to select section
    FSM1.transit(S_000);
  }
}
//------------------------------------
// select section
void S_000() {
  if (FSM1.doTask()) {
    LEDDisplay.setValue(000);
  }

  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) {
    // W B B go to left
    goPos = 0;
  } else if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) {
    // W B W go to middle
    goPos = 1;
  } else if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) {
    // B B W go to right
    goPos = 2;
  }

  if (catched) goPos = 0;

  if (FSM1.getTime() > 500) FSM1.transit(S_101);
}
//------------------------------------
void S_101() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(101);
  }

  if (catched) {
    if ((S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT)
     || (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK)) {
      if (rtime > 7000) {
        FSM1.transit(S_201);
      } else {
        if (catched) {
          MotorR.setSpeed(0);
          MotorL.setSpeed(0);
          delay(2000);
        }
        FSM1.transit(S_103);
      }
    }
  } else if (S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_201);

  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_102);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_103);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_104);
}
//------------------------------------
void S_102() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);


    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }

  // call gettime() and read to the rtime
  if (FSM1.getTime() && (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT))) {
    LEDDisplay.setValue(2999);
    rtime += FSM1.getTime();
/* for debugs
    if (catched) {
      MotorR.setSpeed(0);
      MotorL.setSpeed(0);
      delay(2000);
    }
*/
    FSM1.transit(S_101);
  }
}
//------------------------------------
void S_103() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);


    MotorR.setSpeed(0);
    MotorL.setSpeed(NORMAL_SPEED - TURN_SPEED_L);
  }

  // call gettime() and read to the rtime
  if ((FSM1.getTime()) && (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK))) {
    LEDDisplay.setValue(3999);
    rtime += FSM1.getTime();
/* for debugs
    if (catched) {
      MotorR.setSpeed(0);
      MotorL.setSpeed(0);
      delay(2000);
    }
*/
    FSM1.transit(S_101);
  }
}
//------------------------------------
void S_104() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);


    MotorR.setSpeed(NORMAL_SPEED - TURN_SPEED_R);
    MotorL.setSpeed(0);
  }

  // call gettime() and read to the rtime
  if ((FSM1.getTime()) && (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT))) {
    LEDDisplay.setValue(4999);
    rtime += FSM1.getTime();
/* for debugs
    if (catched) {
      MotorR.setSpeed(0);
      MotorL.setSpeed(0);
      delay(2000);
    }
*/
    FSM1.transit(S_101);
  }
}
//------------------------------------
void S_201() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(201);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  /* is it a must to record the time in this few minutes?
  if (FSM1.getTime() > 100) {
    if (goPos == 0) {
      rtime += FSM1.getTime();
      FSM1.transit(S_100);
    } else if (goPos == 1) {
      rtime += FSM1.getTime();
      FSM1.transit(S_010);
    } else if (goPos == 2) {
      rtime += FSM1.getTime();
      FSM1.transit(S_001);
    }
  }
  */

  if (FSM1.getTime() > 100) {
    if (goPos == 0) {
      FSM1.transit(S_100);
    } else if (goPos == 1) {
      FSM1.transit(S_010);
    } else if (goPos == 2) {
      FSM1.transit(S_001);
    }
  }
}
//------------------------------------
// go to left
void S_100() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(100);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(0);
  }
  if (FSM1.getTime() > 500) {
    if(S3.getHiLow() == BLK && S4.getHiLow() == WHT) {
      rtime += FSM1.getTime();
      FSM1.transit(S_301);
    }
  }
}
//------------------------------------
// go to middle
void S_010() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(10);
    MotorR.setSpeed(NORMAL_SPEED - RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 200) {
    rtime += FSM1.getTime();
    FSM1.transit(S_301);
  }
}
//------------------------------------
// go to right
void S_001() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(1);
    MotorR.setSpeed(0);
    MotorL.setSpeed(NORMAL_SPEED - LEFT_OFFSET);
  }
  if (FSM1.getTime() > 500) {
    if(S3.getHiLow()== BLK && S2.getHiLow()== WHT) {
      rtime += FSM1.getTime();
      FSM1.transit(S_301);
    }
  }
}
//------------------------------------
void S_301() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(301);
  }
  if (!catched){
    if (S1.getHiLow() == BLK)  FSM1.transit(S_401);
  } else {
    if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_556);
  }

  if (rtime > 8000){
    if ((S2.getHiLow() == BLK && S3.getHiLow() == BLK && S4.getHiLow() == BLK)
     || (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT)) {
       FSM1.transit(S_303);
     }
  }
  if (S2.getHiLow() == BLK && S4.getHiLow() == BLK) FSM1.transit(S_302);
  if (S2.getHiLow() == WHT && S4.getHiLow() == WHT) FSM1.transit(S_302);
  if (S2.getHiLow() == WHT && S4.getHiLow() == BLK) FSM1.transit(S_303);
  if (S2.getHiLow() == BLK && S4.getHiLow() == WHT) FSM1.transit(S_304);
}
//------------------------------------
void S_302() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(302);
    MotorR.setSpeed(NORMAL_SPEED-RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED-LEFT_OFFSET);
  }

  if (FSM1.getTime()) {
    if (!catched) {
      if (S1.getHiLow() == BLK)  {
        rtime += FSM1.getTime();
        FSM1.transit(S_401);
      }
    }

    if (catched) {
      if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT) {
        FSM1.transit(S_556);
      }
    }

    if (!(S2.getHiLow() == WHT && S4.getHiLow() == WHT)) {
      rtime += FSM1.getTime();
      FSM1.transit(S_301);
    }
  }
}
//------------------------------------
void S_303() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(303);
    MotorR.setSpeed(0);
    MotorL.setSpeed(NORMAL_SPEED - TURN_SPEED_L);
  }

  if (FSM1.getTime()) {
    if (!catched) {
      if (S1.getHiLow() == BLK)  {
        rtime += FSM1.getTime();
        FSM1.transit(S_401);
      }
    }

    if (catched) {
      if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT) {
        FSM1.transit(S_556);
      }
    }

    if (!(S2.getHiLow() == WHT && S4.getHiLow() == BLK)) {
      rtime += FSM1.getTime();
      FSM1.transit(S_301);
    }
  }

}
//------------------------------------
void S_304() {
  if(FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(304);
    MotorR.setSpeed(NORMAL_SPEED - TURN_SPEED_R);
    MotorL.setSpeed(0);
  }

  if (FSM1.getTime()) {
    if (!catched) {
      if (S1.getHiLow() == BLK)  {
        rtime += FSM1.getTime();
        FSM1.transit(S_401);
      }
    }

    if (catched) {
      if (S2.getHiLow() == WHT && S3.getHiLow() == WHT && S4.getHiLow() == WHT) {
        FSM1.transit(S_556);
      }
    }

    if (!(S2.getHiLow() == BLK && S4.getHiLow() == WHT)) {
      rtime += FSM1.getTime();
      FSM1.transit(S_301);
    }
  }

}
//------------------------------------
// stop the car and ready to catch the cup
void S_401() {
  if(FSM1.doTask()) {
    // for checking the rtime
    LEDDisplay.setValue(rtime);

    //LEDDisplay.setValue(401);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
  }

  if (FSM1.getTime() > 100) FSM1.transit(S_555);
}
//------------------------------------
// catch the cup and start to return
void S_555() {
  if (FSM1.doTask()) {
    // for checking the rtime
    LEDDisplay.setValue(rtime);

    //LEDDisplay.setValue(555);
    Servo1.setValue(CLAMP_CLOSE);
    delay(500);
    Servo2.setValue(REAR_POS);
    delay(500);

    catched = 1;
  }
  if (FSM1.getTime() > 100) {
    if (goPos == 0) {
      if (rtime <= 8000) {
        FSM1.transit(S_1100);
      } else {
        rtime = 0;
        FSM1.transit(S_000);
      }
    }
    if (goPos == 1) FSM1.transit(S_1010);
    if (goPos == 2) {
      FSM1.transit(S_1001);
    }
  }
}
//------------------------------------
void S_556() {
  if (FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(556);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
    Servo2.setValue(LEFT_POS);
    delay(500);
    Servo1.setValue(CLAMP_RELEASE);
  }
}
//------------------------------------
// left back right
void S_1100() {
  if (FSM1.doTask()) {
    // for checking the rtime
    LEDDisplay.setValue(rtime);

    //LEDDisplay.setValue(1100);

    rtime -= 2000;

    MotorR.setSpeed(TURN_SPEED_R);
    MotorL.setSpeed(LEFT_OFFSET);
  }

  if (FSM1.getTime()) {
    // debug: rtime < getTime() --> rtime < btime
    if (rtime < FSM1.getTime()) {
      FSM1.transit(S_556);
    }
    /*else {
    // for debugs
    btime += FSM1.getTime();
    FSM1.transit(S_1100);
    }*/
  }
}
//------------------------------------
// mid back mid
void S_1010() {
  if (FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(1010);

    if (catchPos) {
      //MotorR.setSpeed(RIGHT_OFFSET);
      MotorR.setSpeed(LEFT_OFFSET);
      MotorL.setSpeed(LEFT_OFFSET);
    } else {
      MotorR.setSpeed(LEFT_OFFSET);
      MotorL.setSpeed(LEFT_OFFSET);
    }
  }

  if (FSM1.getTime()) {
    // debug: rtime < getTime() --> rtime < btime
    if (rtime < FSM1.getTime()) {
      FSM1.transit(S_556);
    }
    /* else {
    // for debugs
    btime += getTime();
    FSM1.transit(S_1010);
    }
    */
  }
}
//------------------------------------
// right back left
void S_1001() {
  if (FSM1.doTask()) {
    // for checking the rtime
    //LEDDisplay.setValue(rtime);

    LEDDisplay.setValue(1001);

    rtime -= 2000;

    MotorR.setSpeed(RIGHT_OFFSET);
    MotorL.setSpeed(TURN_SPEED_L);
  }

  if (FSM1.getTime()) {
    // debug: rtime < getTime() --> rtime < btime
    if (rtime < FSM1.getTime()) {
      FSM1.transit(S_556);
    }
    /* else {
    // for debugs
    btime += getTime();
    FSM1.transit(S_1001);
    }
    */
  }
}
//------------------------------------



/*
//----------start of state S_999 -----
void S_999()
{
  if(FSM1.doTask())
  {
    Servo1.setValue(CLAMP_OPEN);
    Servo2.setValue(LEFT_POS);
    LEDDisplay.setValue(999);
  }
  if (FSM1.getTime() >3000) FSM1.transit(S_101);
}
//------------------------------------
void S_101()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(101);
  }
  if (S2.getHiLow()== WHT && S4.getHiLow()== WHT) FSM1.transit(S_102);
  if (S2.getHiLow()== WHT && S4.getHiLow()== BLK) FSM1.transit(S_103);
  if (S2.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_104);
  if (S2.getHiLow()== BLK && S3.getHiLow()== BLK && S4.getHiLow()== BLK) FSM1.transit(S_201);
}
//------------------------------------
void S_102()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(104);
    MotorR.setSpeed(NORMAL_SPEED-RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED-LEFT_OFFSET);
  }
  if (!(S2.getHiLow()== WHT && S4.getHiLow()== WHT)) FSM1.transit(S_101);
}
//------------------------------------
void S_103()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(103);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (!(S2.getHiLow()== WHT && S4.getHiLow()== BLK)) FSM1.transit(S_101);
}
//------------------------------------
void S_104()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(102);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow()== BLK && S4.getHiLow()== WHT)) FSM1.transit(S_101);
}
//------------------------------------
void S_201()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(201);
    MotorR.setSpeed(NORMAL_SPEED-RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED-LEFT_OFFSET);
  }
  if (FSM1.getTime() >100) FSM1.transit(S_202);
}
//------------------------------------
void S_202()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(202);
    MotorR.setSpeed(TURN_SPEED-RIGHT_OFFSET);
    MotorL.setSpeed(0);
  }
  if (S3.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_301);
}
//------------------------------------
void S_301()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(301);
  }
  if (S1.getHiLow()== BLK)  FSM1.transit(S_401);
  if (S2.getHiLow()== WHT && S4.getHiLow()== WHT) FSM1.transit(S_302);
  if (S2.getHiLow()== WHT && S4.getHiLow()== BLK) FSM1.transit(S_303);
  if (S2.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_304);
}
//------------------------------------
void S_302()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(302);
    MotorR.setSpeed(NORMAL_SPEED-RIGHT_OFFSET);
    MotorL.setSpeed(NORMAL_SPEED-LEFT_OFFSET);
  }
  if (S1.getHiLow()== BLK)  FSM1.transit(S_401);
  if (!(S2.getHiLow()== WHT && S4.getHiLow()== WHT)) FSM1.transit(S_301);
}
//------------------------------------
void S_303()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(303);
    MotorR.setSpeed(0);
    MotorL.setSpeed(TURN_SPEED);
  }
  if (S1.getHiLow()== BLK) FSM1.transit(S_401);
  if (!(S2.getHiLow()== WHT && S4.getHiLow()== BLK)) FSM1.transit(S_301);
}
//------------------------------------
void S_304()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(304);
    MotorR.setSpeed(TURN_SPEED);
    MotorL.setSpeed(0);
  }
  if (S1.getHiLow()== BLK)  FSM1.transit(S_401);
  if (!(S2.getHiLow()== BLK && S4.getHiLow()== WHT)) FSM1.transit(S_301);
}
//------------------------------------
void S_401()
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(401);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);
  }
}
//------------------------------------
*/
