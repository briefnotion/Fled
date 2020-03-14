// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2857 Core Dynamics
// ***************************************************************************************
// *
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.03A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: gi6$b*E>*q%;
// *
// ***************************************************************************************
// *  Programmer Notes:
// *
// *            Coder:  Robert Lebowski
// *    Support Coder:  None
// *  --- Apnd:
// *    Other Sources:  None
// *
// *  Description:
// *    LED is an acronym for Light Emitting Diode. My goal for this little project is
// *    to create a control module multi colored light strips. The individual modules,
// *    running on an Arduino board, will be able to run as stand alone and or, while
// *    working together, as a single device. The module will be able to be
// *    preprogrammed to display certain effects when triggered by different
// *    preprogrammed events.
// *
// *    The first step in the project is to create a single working module.  It will
// *    need to be easily configured. It will need to be able to handle timed events,
// *    triggers events, calculated events, and possibly a combination of events triggers.
// *
// *    The second goal will be too create a communication system between the modules
// *    so that Multiple modules can be triggered by different events all module will
// *    be able to function independently or in parallel while working together.
// *
// *    The third goal will be to create a control module within each device. This
// *    control module device will just be added to each device as just another type of
// *    triggered event and will not be external code. It will just be another way to
// *    configure the device.
// *
// *    The fourth goal will be to determine a way to easily configure all devices in its
// *    system without the need to re flash each device independently. This goal is
// *    basically a central and easy way to setup all triggered events and reactions on
// *    all devices on its system by reading a simple program.
// *
// *    The F in FLED stands of Failed, as in I failed to receive backing for this
// *    project, and I failed to assemble a team to assist me with this. Because of
// *    this, the F can also stand for fiction, because at the time of writing this
// *    description, and having neither of those 2 things, that may be all this amounts
// *    to being.
// *
// *  Background:
// *    https://github.com/briefnotion/Fled/blob/master/Description%20and%20Background.txt
// *
// ***************************************************************************************
// *
// *  V 0.03 _200314
// *    - Corrected a few bugs.  Added a few capabilities.
// *    - Added a simple switch monitor and animation routines.
// *    - Set Up some fake triggers and events for testing.
// *
// *  V 0.02b _200313
// *    - Dirty Birdy update:
// *        The code is a big hot mess.  Nearly completely undocumented.  Crap is
// *        everywhere.  Stuff is scattered and unorganized.  Varibles are huge
// *        and nothing is unorganized.  The thing is ...
// *    - It works.
// *    - Next step: clean this bird up to and get it ready for a stew.
// *    0.02c
// *    - Quickly added comments and started cleaning things up.
// *    - Tested to see if it still runs as before.  Yep.  All good.
// *
// *  V 0.02  _200311
// *    - Creating main working system.
// *
// *  V 0.01  _200309
// *    - Project Start.
// *
// ***************************************************************************************

#include <FastLED.h>

// HARDWARE SETUP

#define DATA_PIN    3
//#define CLK_PIN     4       // If your LED_TYPE requires one.
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    60

#define BRIGHTNESS        96  // Using Example Code.  Max unknown
#define FRAMES_PER_SECOND 120 // Will not be necessary, but keeping, for now, just in case.


#define NUM_TIMED_EVENTS  10  // Untill I can remember how LL, this is being
// hardcoded.

// ***************************************************************************************
// STRUCTURES
// ***************************************************************************************

// ---------------------------------------------------------------------------------------

struct light_path
//  Create a light_path:
//    Controls the animation of a single LED over time.  Path referes to path in time.
//    If a light path is over written the previous animation will be lost.
//    Maybe, if I ever need it, I will enable the ability to stack light paths.
{
  unsigned long tmeSTARTTIME;
  unsigned long intDURATION;
  byte bytANIMATION;
  CRGB crgbCOLORDEST;
  CRGB crgbCOLORSTART;
  boolean booExpired = true;
  boolean booFirstRun = false;

  void set(unsigned long tmeCurrentTime, unsigned long tmeStartInTime,
           unsigned long intDuration, byte bytAnimation, CRGB crgbDestinationColor)
  //  Setup proces to prepare an animation to start at a future time.
  //
  //  Parameters:
  //
  //  CurrentTime   - For the cycle or actual real time of when the event is said to be
  //                  created.
  //  Start In Time - The delay or scheduled wait time after the event was created.
  //  Duration      - How fast, or how long the event will last.
  //  Routine       - 0 to 255.  Determines the event routine.
  //  sRGB Color    - Determines the destination Color.

  //  Events:
  //  0 - Clear Timed Event
  //  1 - Fill LEDs with color over duration.
  //  2 - Pulse Leds with color over duration.

  {
    tmeSTARTTIME = tmeCurrentTime + tmeStartInTime;
    intDURATION = intDuration;
    bytANIMATION = bytAnimation;
    crgbCOLORDEST = crgbDestinationColor;
    booExpired = false;
    booFirstRun = true;
  }

  boolean is_ready(unsigned long tmeCurrentTime, CRGB crgbCurrentColor)
  //  Checks to see if the light animation is ready to be executed.
  //  Returns true if ready.
  {
    if ((booExpired == false) && (tmeCurrentTime >= tmeSTARTTIME))
    {
      if (booFirstRun == true)
      {
        crgbCOLORSTART = crgbCurrentColor;
        booFirstRun = false;
      }
      return true;
    }
    else
    {
      return false;
    }
  }

  CRGB execute(unsigned long tmeCurrentTime)
  //  Returns a CRGB value of the single LED respective to what its value should
  //    be as it is in its time path.

  //  LED Animations:
  //    0 - Clear:  Clears future animation and sets to no future animation.
  //    1 - Fade:   Fades single LED from its initial value to its destination value from
  //                  start of animation, and completed and end of its duration.
  //    2 - Pulse:  Pulses the LED to desired value.  The LED will have its requested
  //                  full value at half duration time, and will return to its origional
  //                  value at the end of the duration.
  {
    switch (bytANIMATION)
    {
      case 0:
        // Clear Animation Path
        {
          booExpired = false;
          booFirstRun = true;
        }
      case 1:
        // Fade Animation Path
        {
          if (tmeCurrentTime >= tmeSTARTTIME + intDURATION)
          {
            booExpired = true;
            return crgbCOLORDEST;
          }
          else
          {
            float fltPower = (float)(tmeCurrentTime - tmeSTARTTIME) / (float)intDURATION;

            byte R = (fltPower * crgbCOLORDEST.r) + ((1 - fltPower) * crgbCOLORSTART.r);
            byte G = (fltPower * crgbCOLORDEST.g) + ((1 - fltPower) * crgbCOLORSTART.g);
            byte B = (fltPower * crgbCOLORDEST.b) + ((1 - fltPower) * crgbCOLORSTART.b);

            return CRGB(R, G, B);
          } // END CASE 1
        case 2:
          // Pulse Animation Path
          // This can be done beter with a simple ABS function.
          {
            if (tmeCurrentTime >= tmeSTARTTIME + intDURATION)
            {
              booExpired = true;
              return crgbCOLORSTART;
            }
            else
            {
              unsigned long tmeElapsed = tmeCurrentTime - tmeSTARTTIME;
              float fltPower;

              if ((tmeElapsed * 2) <= (intDURATION))
              {
                fltPower = (float)(tmeElapsed) * 2 / ((float)intDURATION);
              }
              else
              {
                fltPower = 1 - (((float)((tmeElapsed * 2) - intDURATION) / (float)intDURATION));
              }

              byte R = (fltPower * crgbCOLORDEST.r) + ((1 - fltPower) * crgbCOLORSTART.r);
              byte G = (fltPower * crgbCOLORDEST.g) + ((1 - fltPower) * crgbCOLORSTART.g);
              byte B = (fltPower * crgbCOLORDEST.b) + ((1 - fltPower) * crgbCOLORSTART.b);

              return CRGB(R, G, B);
            }
          } // END CASE 2
        } // END CASE STATEMENT
    }
  }

};

// ---------------------------------------------------------------------------------------

struct timed_event
//  Create a Timed LED Event:
//    Controls when a group of LEDs are to begin their individual light path animation.
//    If an event is over written the previous animation will be lost.
{
  unsigned long tmeSTARTTIME;
  unsigned long intDURATION;
  unsigned int intSPEED;
  byte bytANIMATION;
  byte bytLEDANIMATION;
  CRGB crgbCOLORDEST;;
  int intSTARTPOS;
  int intENDPOS;
  boolean booREPEAT;

  boolean booExpired = true;

  void set(light_path lpLightPaths[], unsigned long tmeCurrentTime,
           unsigned long tmeStartInTime, unsigned long intDuration, unsigned int intSpeed,
           byte bytAnimation, byte bytLEDAnimation, CRGB crgbColor, int intStartPos,
           int intEndPos, boolean booRepeat)
  //  Setup proces to prepare multiple LEDs to start at future time.

  //  Create a light event:
  //  Parameters:

  //  LightPaths    - LightPath Array to be set.
  //  CurrentTime   - For the cycle or actual real time of when the event is said to be
  //                  created.
  //  Start In Time - The delay or scheduled wait time after the event was created.
  //  Duration      - How fast, or how long the event will last.
  //  Routine       - 0 to 255.  Determines the event routine.
  //  sRGB Color    - Determines the destination Color.
  //  StartPos      - First LED to start event at.
  //  EndPos        - Last LED of the Event.

  //  Events:
  //  0 - Clear Timed Event
  //  1 - Fill LEDs from StartPos to Endpos with color at Speed.  Passes Duration.

  {
    tmeSTARTTIME = tmeCurrentTime + tmeStartInTime;
    intDURATION = intDuration;
    intSPEED = intSpeed;
    bytANIMATION = bytAnimation;
    bytLEDANIMATION = bytLEDAnimation;
    crgbCOLORDEST = crgbColor;
    intSTARTPOS = intStartPos;
    intENDPOS = intEndPos;
    booREPEAT = booRepeat;

    booExpired = false;
  }

  boolean is_ready(unsigned long tmeCurrentTime)
  //  Checks to see if the event animation is ready to be executed.
  //  Returns true if ready.
  {
    if ((booExpired == false) && (tmeCurrentTime >= tmeSTARTTIME))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  void clear()
  {
    booExpired = true;
  }

  boolean is_expired()
  //  Checks to see if the event has ran and can be over written.
  //  Returns true if it can be recycled.
  {
    return booExpired;
  }

  void execute(light_path lpLedArray[], unsigned long tmeCurrentTime)
  //  Sets all requested light paths, start to end position, to begin their animation
  //    at a future time.

  //  Event Animations:
  //    0 - Clear:  Clears future event and sets to no future future.
  //    1 - Sweep:  Sets light paths for all LEDs from start to end posion.
  //                  Speed is the time difference in miliseconds between each adjacent
  //                  light path.
  //                  Duration, destination color, and LED animation will also be passed
  //                  to its light path.
  {
    switch (bytANIMATION)
    {
      case 0:
        // Clear Event
        {
          booExpired = false;
        }
      case 1:
        // Sweep Event - Can handle a reverse sweep.  But, is the a simpler way?
        {
          unsigned long tmeStart;
          int pos;
          int num;

          if (intSTARTPOS <= intENDPOS)
          {
            num = intENDPOS - intSTARTPOS;
          }
          else
          {
            num = intSTARTPOS - intENDPOS;
          }
          for (int x = 0; x <= num; x++)
          {
            // Find Delay between leds
            if (intSTARTPOS <= intENDPOS)
            {
              tmeStart = tmeSTARTTIME + (x * intSPEED);
              pos = x + intSTARTPOS;
            }
            else // (intSTARTPOS > intENDPOS)
            {
              tmeStart = tmeSTARTTIME + (x * intSPEED);
              pos = intSTARTPOS - x;
            }

            lpLedArray[pos].set(tmeStart, 0, intDURATION, bytLEDANIMATION, crgbCOLORDEST);
          }
        }
    }

    //  Check to see if this event repeats.
    if (booREPEAT == false)
    {
      //  It doesn' to set it as expired.
      booExpired = true;
    }
    else
    {
      //  It does repeat so reschedule it to when we know all the light Paths have been
      //  complete, which is what I would love to do, but for now we will just go by the
      //  we took to start the event:  Its duration.  Never mind. Got it.
      //  (num of led * speed + duration)
      tmeSTARTTIME = tmeCurrentTime + (abs(intENDPOS - intSTARTPOS) * intSPEED) +
                     intDURATION;
      //num of led * speed + duration
    }
  }
};

// ---------------------------------------------------------------------------------------

struct hardware_monitor
//  Create a hardware :
{
  unsigned long tmeCHANGEDETECTEDTIME;
  boolean booPREVCHANGEDETECTED;
  unsigned long tmeLEEWAY;
  boolean booVALUE;
  //boolean booACTIVE;

  void set(boolean booOpen, unsigned long tmeLeeWay)
  {
    tmeCHANGEDETECTEDTIME = millis();
    tmeLEEWAY = tmeLeeWay;
    booVALUE = booOpen;
  }

  boolean changed(boolean booOpen)
  {
    if (booVALUE == booOpen)
    {
      booPREVCHANGEDETECTED = false;
      return false;
    }
    else if (booPREVCHANGEDETECTED == false)
    {
      tmeCHANGEDETECTEDTIME = millis;
      booPREVCHANGEDETECTED = true;
      return  false;
    }
    else if (millis() < tmeCHANGEDETECTEDTIME + tmeLEEWAY)
    {
      return false;
    }
    else
    {
      booVALUE = booOpen;
      booPREVCHANGEDETECTED = false;
      return true;
    }
  }
};

struct hardware_door
{
  hardware_monitor hwmSTATUS;
};

// ***************************************************************************************
// FUNCTION AND PROCEDURES
// ***************************************************************************************

void vdClearAllTimedEvent(timed_event teEventList[])
//  Sort through the timed event list a create an event at the first slot available.
{
  boolean booCreated = false;
  for (int x = 0; x < NUM_TIMED_EVENTS; x++)
  {
    teEventList[x].clear();
  }
}

void vdCreateTimedEvent(timed_event teEventList[], light_path lpLPs[],
                        unsigned long tmeCurrentTime, unsigned long tmeStartInTime,
                        unsigned long intDuration, unsigned int intSpeed,
                        byte bytAnimation, byte bytLEDAnimation, CRGB crgbColor,
                        int intStartPos, int intEndPos, boolean booRepeat)
//  Sort through the timed event list a create an event at the first slot available.
{
  boolean booCreated = false;
  for (int x = 0; ((x < NUM_TIMED_EVENTS) && (booCreated == false)) ; x++)
  {
    if (teEventList[x].is_expired())
    {
      teEventList[x].set(lpLPs, tmeCurrentTime, tmeStartInTime, intDuration, intSpeed,
                         bytAnimation, bytLEDAnimation, crgbColor, intStartPos, intEndPos, booRepeat);
      booCreated = true;
    }
  }
}

// --- ANIMATIONS ------------------------------------------------------------------------

// PARAMETER REFFERENCE

//  Reference:
//TIMED EVENT  PARAMETERS
//light_path lpLightPaths[],
//unsigned long tmeCurrentTime,
//unsigned long tmeStartInTime,
//unsigned long intDuration,
//unsigned int intSpeed,
//byte bytAnimation,
//byte bytLEDAnimation,
//CRGB crgbColor,
//int intStartPos,
//int intEndPos

void vdPowerOnAnimation(timed_event teEventList[], light_path lpLPs[], unsigned long tmeCurrentTime)
{
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 0000, 50, 2, 1, 2, CRGB(0, 0, 30), 0, 59, false);
}

void vdAlertAnimation(timed_event teEventList[], light_path lpLPs[], unsigned long tmeCurrentTime)
{

}

void vdDoorOpenAnimation(timed_event teEventList[], light_path lpLPs[], unsigned long tmeCurrentTime)
{
  // Door Open Animation
  //  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 1000, 500, 10, 1, 1, CRGB(25, 0, 0), 0, 59, false); // 1100
  //
  //  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 2300, 300, 1, 1, 2, CRGB(100, 100, 0), 0, 59, false); // 900
  //  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 3300, 400, 4, 1, 2, CRGB(80, 80, 0), 0, 59, false); // 900
  //  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 4300, 500, 6, 1, 2, CRGB(50, 50, 0), 0, 59, false); // 900
  //  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 5300, 600, 10, 1, 2, CRGB(40, 30, 0), 0, 59, false); // 1200
  //  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 6600, 1000, 30, 1, 2, CRGB(40, 20, 0), 0, 59, false); // 2800
  //  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 9500, 1500, 30, 1, 2, CRGB(50, 50, 0), 0, 59, true); //

  // or
  //vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 0000, 500, 0, 1, 1, CRGB(0, 0, 0), 0, 59, false); // 1100
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 1000, 500, 10, 1, 1, CRGB(25, 0, 0), 0, 59, false); // 1100
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 3300, 400, 4, 1, 2, CRGB(80, 80, 0), 0, 59, false); // 900
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 4300, 500, 6, 1, 2, CRGB(50, 50, 0), 0, 59, false); // 900
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 5300, 600, 10, 1, 2, CRGB(40, 30, 0), 0, 59, false); // 1200
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 6600, 1000, 30, 1, 2, CRGB(128, 128, 0), 0, 59, false); // 2800
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 9500, 2000, 60, 1, 2, CRGB(255, 255, 0), 29, 0, true); //
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 9500, 2000, 60, 1, 2, CRGB(255, 255, 0), 30, 59, true); //
}

void vdDoorCloseAnimation(timed_event teEventList[], light_path lpLPs[], unsigned long tmeCurrentTime)
{
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 0000, 500, 0, 1, 1, CRGB(0, 0, 50), 0, 59, false); // 500
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 0600, 500, 10, 1, 2, CRGB(100, 100, 50), 0, 59, false); // 1100
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 1700, 0, 0, 1, 1, CRGB(50, 25, 0), 0, 59, false);  // 0
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 1800, 3000, 0, 1, 1, CRGB(0, 0, 0), 0, 59, false);
}

void vdPacificaishAnimation(timed_event teEventList[], light_path lpLPs[], unsigned long tmeCurrentTime)
{
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 1000, 500, 10, 1, 1, CRGB(0, 15, 25), 0, 59, false); // 1100
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 2000, 3500, 250, 1, 2, CRGB(40, 200, 160), 0, 15, true); // 900
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 2000, 3800, 220, 1, 2, CRGB(40, 200, 160), 16, 30, true); // 900
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 2000, 3600, 270, 1, 2, CRGB(40, 200, 160), 31, 45, true); // 900
  vdCreateTimedEvent (teEventList, lpLPs, tmeCurrentTime, 2000, 3200, 200, 1, 2, CRGB(40, 200, 160), 46, 59, true); // 900
}

// ***************************************************************************************
// MAIN
// ***************************************************************************************
// GLOBAL VARIABLES

//  Light Strip Hardware
CRGB hwLED[NUM_LEDS];

// Light Strip Event System
light_path lpLED[NUM_LEDS];
timed_event tmeEvent[NUM_TIMED_EVENTS];

// Door Sensor
boolean booFakeDoorSensor = false;

hardware_monitor hwDoor;

// Onboard LED to signify data being sent to LED strip.
const int ledPin =  LED_BUILTIN;

// Delay less Loop Variables
//    intRestTime defines the amount of time, in milliseconds, passed before another data
//    read pass is performed and transmitted from the controller to the main system.
unsigned long tmeCurrentMillis = millis();
unsigned long tmePrevMillis = 0;
int intRestTime = 0;             // Do not check for update until rest time is passed.

// Enable Serial Monitor for testing
//    Enabling booTest will slow the board time and enable the serial monitor to be read.
const boolean booTest = false;

// ---------------------------------------------------------------------------------------
// MAIN SETUP

void setup()
{
  // Prep and define Hardware.

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(hwLED, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //hwLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  LEDS.setBrightness(BRIGHTNESS);

  //if ((booTest == true)
  //{
  //  Serial.begin(9600);
  //}

  // Set Door
  hwDoor.set(false, 500);


  // Boot Animation
  // Make sure we have the current time before we try any test animations.
  tmeCurrentMillis = millis();
  vdPowerOnAnimation(tmeEvent, lpLED, tmeCurrentMillis);
}

// ---------------------------------------------------------------------------------------
// MAIN LOOP

void loop()
//  Main Loop:
//    Events and Light Path animations should only be called when their time is up.
{

  //  Only update the hardware when changes have been detected.
  boolean booUpdate = false;

  //  Get current time.  This will be our timeframe to work in.
  tmeCurrentMillis = millis();

  // --- Begin of Delayless Loop ---
  if (tmeCurrentMillis - tmePrevMillis >= intRestTime)
  {
    tmePrevMillis = tmeCurrentMillis;

    // --- TESTING AREA ---
    // Create fake changes.
    if ((tmeCurrentMillis > 5000) && (tmeCurrentMillis < 6000))
    {
      booFakeDoorSensor = true;
    }
    if ((tmeCurrentMillis > 20000) && (tmeCurrentMillis < 21000))
    {
      booFakeDoorSensor = false;
    }
    if ((tmeCurrentMillis > 25000) && (tmeCurrentMillis < 26000))
    {
        vdClearAllTimedEvent(tmeEvent);
        vdPacificaishAnimation(tmeEvent, lpLED, tmeCurrentMillis);
    }


    // --- Grabbing Data From Hardware inputs ---
    // Check door for changes.
    if (hwDoor.changed(booFakeDoorSensor == true))
    {
      if (booFakeDoorSensor == true)
      {
        vdClearAllTimedEvent(tmeEvent);
        vdDoorOpenAnimation(tmeEvent, lpLED, tmeCurrentMillis);
      }
      else
      {
        vdClearAllTimedEvent(tmeEvent);
        vdDoorCloseAnimation(tmeEvent, lpLED, tmeCurrentMillis);
      }
    }

    // --- Process Data From Hardware Inputs ---

    // --- Check and Execute Timed Events That Are Ready ---
    for (int x = 0; x < NUM_TIMED_EVENTS; x++)
    {
      if (tmeEvent[x].is_ready(tmeCurrentMillis) == true)
      {
        tmeEvent[x].execute(lpLED, tmeCurrentMillis);
      }
    }

    // --- Check and Execute Light Path Events That Are Ready ---
    for (int x = 0; x < NUM_LEDS; x++)
    {
      if (lpLED[x].is_ready(tmeCurrentMillis, hwLED[x]) == true)
      {
        hwLED[x] = (CRGB)lpLED[x].execute(tmeCurrentMillis);

        //  If we made it to this part of the code then we need to
        //    tell the LED hardware that it has a change to commit.
        booUpdate = true;
      }
    }

    // --- Execute LED Hardware Changes If Anything Was Updated ---
    if (booUpdate == true)
    {
      //  Turn on onboad LED when communicating with LED Hardware.
      digitalWrite(ledPin, HIGH);

      //  Update LED Hardware with changes.
      FastLED.show();

      //  Turn off onboad LED when communication complete.
      digitalWrite(ledPin, LOW);
    }

    // Debug Routines ---
    // Slow delay if in testing mode.
    if (booTest == true)
    {
      intRestTime = 250;
    }
  }   // End Delayless Loop
}
