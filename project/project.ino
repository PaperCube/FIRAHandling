#include <PS2X_lib.h>
#include "InitFIRA.h"
#include "ToolKit.h"

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT A6  //14
#define PS2_CMD A7  //15
#define PS2_SEL A13 //16
#define PS2_CLK A14 //17

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures true
//#define pressures   false
#define rumble true
//#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

// Reset func
void (*resetFunc)(void) = 0;

void setup()
{
  Serial.begin(115200);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  type = ps2x.readType();
  InitFIRA(0);
  //ScreenStart

  GLCD.clrScr();
  GLCD.setBackColor(0, 0, 0);
  GLCD.drawRoundRect(7, 5, 128 - 5, 160 - 5);
  GLCD.setFont(BigFont);
  GLCD.setColor(178, 16, 22);
  GLCD.print("FIRAxZB", 80, 25, 90);
  GLCD.setColor(255, 255, 255);
  GLCD.setFont(SmallFont);
  GLCD.print("Loading.....", 22, 40, 90);
  GLCD.setBackColor(70, 70, 70);
  GLCD.print("VisualDust Design", 60, 14, 90);
  GLCD.setBackColor(0, 0, 0);
  GLCD.setColor(255, 192, 0);
  for (int i = 5; i <= 155; i++)
    GLCD.drawLine(0, i, 3, i),
        delay(13);
  //ScreenStartEnd
  delay(500);
  GLCD.clrScr();
  GLCD.setColor(255, 255, 255);
  GLCD.drawRoundRect(5, 5, 128 - 12, 160 - 5);
  GLCD.print("Searching", 100, 23, 90);
  GLCD.print("Contorller", 86, 19, 90);
  //check if controller is availiable
  for (;;)
  {
    SetLedRGB(1, 0, 0);
    for (int r = 1; r <= 10; r++)
      GLCD.drawCircle(88, 130, r), delay(1);
    GLCD.setColor(0, 0, 0);
    SetLedRGB(0, 0, 1);
    for (int r = 1; r <= 10; r++)
      GLCD.drawCircle(88, 130, r), delay(1);
    GLCD.setColor(255, 255, 255);
    ps2x.read_gamepad();
    if (ps2x.ButtonPressed(PSB_SELECT))
    {
      for (int r = 1; r <= 10; r++)
        GLCD.drawCircle(88, 130, r), delay(1);
      break;
    }
  }
  GLCD.setFont(BigFont);
  GLCD.setColor(255, 255, 255);
  GLCD.setBackColor(0, 176, 80);
  GLCD.print("DONE", 40, 50, 90);
  //check finished
  GLCD.setFont(SmallFont);
  GLCD.setBackColor(0, 0, 0);
  GLCD.setColor(34, 177, 76);
  SetLedRGB(0, 1, 0);
  GLCD.print("Press\"Start\"ToRun", 60, 14, 90);
  for (;;)
  {
    ps2x.read_gamepad();
    if (ps2x.ButtonPressed(PSB_START) || GetKey(2))
      break;
    delay(30);
  }
  SetLedRGB(1, 1, 1);
  delay(100);
}

void loop()
{

  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */
  if (error == 1)
  { //skip loop if no controller found
    // resetFunc();
  }

  if (type == 2)
  {                      //Guitar Hero Controller
    ps2x.read_gamepad(); //read controller

    if (ps2x.ButtonPressed(GREEN_FRET))
      Serial.println("Green Fret Pressed");
    if (ps2x.ButtonPressed(RED_FRET))
      Serial.println("Red Fret Pressed");
    if (ps2x.ButtonPressed(YELLOW_FRET))
      Serial.println("Yellow Fret Pressed");
    if (ps2x.ButtonPressed(BLUE_FRET))
      Serial.println("Blue Fret Pressed");
    if (ps2x.ButtonPressed(ORANGE_FRET))
      Serial.println("Orange Fret Pressed");

    if (ps2x.ButtonPressed(STAR_POWER))
      Serial.println("Star Power Command");

    if (ps2x.Button(UP_STRUM)) //will be TRUE as long as button is pressed
      Serial.println("Up Strum");
    if (ps2x.Button(DOWN_STRUM))
      Serial.println("DOWN Strum");

    if (ps2x.Button(PSB_START)) //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if (ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");

    if (ps2x.Button(ORANGE_FRET))
    { // print stick value IF TRUE
      Serial.print("Wammy Bar Position:");
      Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);
    }
  }
  else
  {                                    //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    if (ps2x.Button(PSB_START))        //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if (ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");

    if (ps2x.Button(PSB_PAD_UP))
    { //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
    }
    if (ps2x.Button(PSB_PAD_RIGHT))
    {
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    }
    if (ps2x.Button(PSB_PAD_LEFT))
    {
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    }
    if (ps2x.Button(PSB_PAD_DOWN))
    {
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    }

    vibrate = ps2x.Analog(PSAB_CROSS); //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState())
    { //will be TRUE if any button changes state (on to off, or off to on)
      if (ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if (ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if (ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if (ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if (ps2x.Button(PSB_TRIANGLE))
        Serial.println("Triangle pressed");
    }

    if (ps2x.ButtonPressed(PSB_CIRCLE)) //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
    if (ps2x.NewButtonState(PSB_CROSS)) //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
    if (ps2x.ButtonReleased(PSB_SQUARE)) //will be TRUE if button was JUST released
      Serial.println("Square just released");

    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))
    { //print stick values if either is TRUE
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC);
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC);
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
    }
  }
  delay(50);
}
