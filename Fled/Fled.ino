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
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.11A
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
// *  V 0.11 _200319
// *      - Duplicated portions of the code to handle a second light strip on pin 4
// *          that will run concurrently with the first light strip.  Testing ended just
// *          short of running both light strips at the same time.  That was because,
// *          even though I had a second light strip available, I ran out of power
// *          feeds.
// *          
// *  V 0.10b _200318
// *      - Got some rest.  Fixed a bunch of stuff.
// *      - Seperated Dest Color to Dest 1 and Dest 2 to dither from start to end.
// *      - Created Pixil Animation 3.  Same as flash but ends on Color 2.
// *      - Seperated Event handling of to handle Pixel Animation 1 differently than
// *          Pixel Animation 2 and 3.  Considering merging Pixel Anination 2 and 3
// *          then getting around this seperation issue.
// *
// *  V 0.10 _200318
// *    - Rewrote the code when disposing of the LightPath methodology.  This freed up
// *        enough memory to allow 150 lights within 2k of memory.  I really dont like
// *        This way of doing things.  It takes more overhead in a starting heap process
// *        and its much more complex.  Maybe I'm just that I'm in a bad mood because it
// *        looks so much messier and it took so efing long to debug, only to end up with
// *        code that does the exact same thing, except worst.  Maybe its good.  I don't
// *        know.  Had no choice.  Moving on.
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

#define DATA_PINs1    3       // Data Pin for Strip 1
#define DATA_PINs2    4       // Data Pin for Strip 2
//#define CLK_PIN     5       // If your LED_TYPE requires one.
#define LED_TYPE    WS2812B   
#define COLOR_ORDER GRB
#define NUM_LEDSs1    60
#define NUM_LEDSs2    60

#define BRIGHTNESS        96  // Using Example Code.  Max unknown
#define FRAMES_PER_SECOND 120 // Will not be necessary, but keeping, for now, just in case.


#define NUM_TIMED_EVENTS  10  // Untill I can remember how LL, this is being
                              //  Also, total number of this will be multiplied by the
                              //  amount of LED strips you have setup.  Watch your memory.

// ***************************************************************************************
// STRUCTURES
// ***************************************************************************************
//  TIMED EVENT

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
  CRGB crgbCOLORDEST1;
  CRGB crgbCOLORDEST2;
  int intSTARTPOS;
  int intENDPOS;
  boolean booREPEAT;

  boolean booSETCOMPLETE = false;
  boolean booEXPIRED = true;

  void set(unsigned long tmeCurrentTime,
           unsigned long tmeStartInTime, unsigned long intDuration, unsigned int intSpeed,
           byte bytAnimation, byte bytLEDAnimation, CRGB crgbColor1, CRGB crgbColor2,
           int intStartPos, int intEndPos, boolean booRepeat)
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
    crgbCOLORDEST1 = crgbColor1;
    crgbCOLORDEST2 = crgbColor2;
    intSTARTPOS = intStartPos;
    intENDPOS = intEndPos;
    booREPEAT = booRepeat;

    booEXPIRED = false;
    booSETCOMPLETE = false;
  }

  boolean is_ready(unsigned long tmeCurrentTime)
  //  Checks to see if the event animation is ready to be executed.
  //  Returns true if ready.
  {
    if ((booEXPIRED == false) && (tmeCurrentTime >= tmeSTARTTIME))
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
    booEXPIRED = true;
  }

  boolean is_expired()
  //  Checks to see if the event has ran and can be over written.
  //  Returns true if it can be recycled.
  {
    return booEXPIRED;
  }

  CRGB crgb_anim_color(CRGB crgbCSTart, CRGB crgbCDest1, CRGB crgbCDest2,
                       unsigned long tmeCurrentTime, unsigned long tmeAnimTime)
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

    // Only run pixel animation if it is ready.

    unsigned long tmeElapsed = tmeCurrentTime - tmeAnimTime;
    float fltPower;

    switch (bytLEDANIMATION)
    {
      case 1:
        // Fade Animation Path
        {
          if (tmeCurrentTime >= tmeAnimTime + intDURATION)
          {
            return crgbCDest1;
          }
          else
          {
            float fltPower = (float)tmeElapsed / (float)intDURATION;

            byte R = (fltPower * crgbCDest1.r) + ((1 - fltPower) * crgbCSTart.r);
            byte G = (fltPower * crgbCDest1.g) + ((1 - fltPower) * crgbCSTart.g);
            byte B = (fltPower * crgbCDest1.b) + ((1 - fltPower) * crgbCSTart.b);

            return CRGB(R, G, B);

          }
        } // END CASE 1
      case 2:
        // Pulse Animation Path
        // This can be done beter with a simple ABS function.
        {
          if (tmeCurrentTime >= tmeAnimTime + intDURATION)
          {
            return crgbCSTart;
          }
          else
          {
            if ((tmeElapsed * 2) <= (intDURATION))
            {
              fltPower = (float)(tmeElapsed) * 2 / ((float)intDURATION);
            }
            else
            {
              fltPower = 1 - (((float)((tmeElapsed * 2) - intDURATION) / (float)intDURATION));
            }

            //  Swap to second color at mid point
            byte R = (fltPower * crgbCDest1.r) + ((1 - fltPower) * crgbCSTart.r);
            byte G = (fltPower * crgbCDest1.g) + ((1 - fltPower) * crgbCSTart.g);
            byte B = (fltPower * crgbCDest1.b) + ((1 - fltPower) * crgbCSTart.b);

            return CRGB(R, G, B);
          }
        } // END CASE 2
      case 3:
        // Pulse Animation Path
        // This can be done beter with a simple ABS function.
        {
          if (tmeCurrentTime >= tmeAnimTime + intDURATION)
          {
            return crgbCDest2;
          }
          else
          {
            CRGB crgbA;
            CRGB crgbB;
            //  Swap to second color and power down at mid point.
            if ((tmeElapsed * 2) <= (intDURATION))
            {
              fltPower = (float)(tmeElapsed) * 2 / ((float)intDURATION);
              crgbA = crgbCSTart;
              crgbB = crgbCDest1;
            }
            else
            {
              fltPower = 1 - (((float)((tmeElapsed * 2) - intDURATION) / (float)intDURATION));
              crgbA = crgbCDest2;
              crgbB = crgbCDest1;
            }

            byte R = (fltPower * crgbB.r) + ((1 - fltPower) * crgbA.r);
            byte G = (fltPower * crgbB.g) + ((1 - fltPower) * crgbA.g);
            byte B = (fltPower * crgbB.b) + ((1 - fltPower) * crgbA.b);

            return CRGB(R, G, B);
          }
        } // END CASE 3
    } // END CASE STATEMENT

  }

  //void execute(CRGB hwLEDArray[], CRGB hwLEDStartArray[], CRGB hwLEDDestArray[], unsigned long tmeCurrentTime)
  void execute(CRGB hwLEDArray[], CRGB hwLEDStartArray[], unsigned long tmeCurrentTime)
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
    int count = abs(intENDPOS - intSTARTPOS);
    int pos;

    if (booSETCOMPLETE == false)
    {
      for (int x = 0; x <= count; x++)
        if (intSTARTPOS <= intENDPOS)
        {
          //hwLEDDestArray[x + intSTARTPOS] = crgbCOLORDEST;
          hwLEDStartArray[x + intSTARTPOS] = hwLEDArray[x + intSTARTPOS];
        }
        else
        {
          //hwLEDDestArray[x + intENDPOS] = crgbCOLORDEST;
          hwLEDStartArray[x + intENDPOS] = hwLEDArray[x + intENDPOS];
        }

      booSETCOMPLETE = true;
    }

    switch (bytANIMATION)
    {
      case 0:
        // Clear Event
        {
          booEXPIRED = false;
        }
      case 1:
        // Sweep Event - Can handle a reverse sweep.  But, is there a simpiler way?
        {
          unsigned long tmeStartAnim;

          for (int x = 0; x <= count; x++)
          {
            // Find Delay between leds
            if (intSTARTPOS <= intENDPOS)
            {
              tmeStartAnim = tmeSTARTTIME + (x * intSPEED);
              pos = x + intSTARTPOS;
            }
            else // (intSTARTPOS > intENDPOS)
            {
              tmeStartAnim = tmeSTARTTIME + (x * intSPEED);
              pos = intSTARTPOS - x;
            }

            if (tmeCurrentTime >= tmeStartAnim)
            {
              if (bytLEDANIMATION == 1 )
              {
                // Get value of light based one animation at current time.
                float fltPower = (float)x / (float)count;

                byte R = (fltPower * crgbCOLORDEST2.r) + ((1 - fltPower) * crgbCOLORDEST1.r);
                byte G = (fltPower * crgbCOLORDEST2.g) + ((1 - fltPower) * crgbCOLORDEST1.g);
                byte B = (fltPower * crgbCOLORDEST2.b) + ((1 - fltPower) * crgbCOLORDEST1.b);

                hwLEDArray[pos] = crgb_anim_color(hwLEDStartArray[pos], CRGB(R, G, B), crgbCOLORDEST2, tmeCurrentTime, tmeStartAnim);
              }
              else
              {
                hwLEDArray[pos] = crgb_anim_color(hwLEDStartArray[pos], crgbCOLORDEST1, crgbCOLORDEST2, tmeCurrentTime, tmeStartAnim);
              }
            }
          }
        }
    }

    //  Check to see if this event repeats.

    //  It doesn' to set it as expired.
    if (tmeCurrentTime >= tmeSTARTTIME + intDURATION + (count * intSPEED))
    {
      booEXPIRED = true;
    }

    if (booEXPIRED == true && booREPEAT == true)
    {
      //  It does repeat so reschedule it to when we know all the light Paths have been
      //  complete, which is what I would love to do, but for now we will just go by the
      //  we took to start the event:  Its duration.  Never mind. Got it.
      //  (num of led * speed + duration)

      booSETCOMPLETE = false;
      booEXPIRED = false;
      tmeSTARTTIME = tmeCurrentTime;
    }
  }
};

// ---------------------------------------------------------------------------------------
// HARDWARE MONITOR

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
      tmeCHANGEDETECTEDTIME = millis();
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

void vdCreateTimedEvent(timed_event teEventList[],
                        unsigned long tmeCurrentTime, unsigned long tmeStartInTime,
                        unsigned long intDuration, unsigned int intSpeed,
                        byte bytAnimation, byte bytLEDAnimation, CRGB crgbColor1,
                        CRGB crgbColor2, int intStartPos, int intEndPos, boolean booRepeat)
//  Sort through the timed event list a create an event at the first slot available.
{
  boolean booCreated = false;
  for (int x = 0; ((x < NUM_TIMED_EVENTS) && (booCreated == false)) ; x++)
  {
    if (teEventList[x].is_expired())
    {
      teEventList[x].set(tmeCurrentTime, tmeStartInTime, intDuration, intSpeed,
                         bytAnimation, bytLEDAnimation, crgbColor1, crgbColor2, intStartPos,
                         intEndPos, booRepeat);
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

void vdTESTFLASHAnimation(timed_event teEventList[], unsigned long tmeCurrentTime)
{
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 2000, 3000, 200, 1, 3, CRGB(100, 100, 100), CRGB(50, 0, 0), 10, 49, true);
}
void vdPowerOnAnimation(timed_event teEventList[], unsigned long tmeCurrentTime)
{
 //vdCreateTimedEvent (teEventList, tmeCurrentTime, 0100, 750, 25, 1, 1, CRGB(0, 0, 50), 0, 59, false);
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 100, 250, 5, 1, 3, CRGB(125, 125, 125), CRGB(0, 0, 25), 0, 59, false);
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 600, 0, 0, 1, 1, CRGB(0, 0, 25), CRGB(0, 0, 25), 0, 59, false);
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 700, 2000, 0, 1, 1, CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 59, false);

  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 100, 250, 5, 1, 3, CRGB(125, 125, 125), CRGB(0, 0, 25), 0, 119, false);
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 600, 0, 0, 1, 1, CRGB(0, 0, 25), CRGB(0, 0, 25), 0, 59, false);
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 700, 2000, 0, 1, 1, CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 59, false);
}

void vdAlertAnimation(timed_event teEventList[], unsigned long tmeCurrentTime)
{

}

void vdDoorOpenAnimation(timed_event teEventList[], unsigned long tmeCurrentTime)
{
  // Door Open Animation

 vdCreateTimedEvent (teEventList, tmeCurrentTime, 100, 500, 10, 1, 1, CRGB(25, 0, 0), CRGB(0, 0, 0), 29, 0, false); // 1100
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 100, 500, 10, 1, 1, CRGB(25, 0, 0), CRGB(0, 0, 0), 30, 59, false); // 1100
 //vdCreateTimedEvent (teEventList, tmeCurrentTime, 100, 500, 10, 1, 1, CRGB(0, 0, 0), CRGB(25, 0, 0), 0, 59, false); // 1100
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 3300, 400, 4, 1, 2, CRGB(80, 80, 0), CRGB(80, 80, 0), 0, 59, false); // 900
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 4300, 500, 6, 1, 2, CRGB(50, 50, 0), CRGB(50, 50, 0), 0, 59, false); // 900
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 5300, 600, 10, 1, 2, CRGB(40, 30, 0), CRGB(40, 30, 0), 0, 59, false); // 1200
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 6600, 1000, 30, 1, 2, CRGB(128, 128, 0), CRGB(128, 128, 0), 0, 59, false); // 2800
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 9500, 2000, 60, 1, 2, CRGB(255, 255, 0), CRGB(255, 255, 0), 0, 29, true); //
 vdCreateTimedEvent (teEventList, tmeCurrentTime, 9500, 2000, 60, 1, 2, CRGB(255, 255, 0), CRGB(255, 255, 0), 59, 30, true); //

  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 100, 500, 10, 1, 1, CRGB(25, 0, 0), CRGB(0, 0, 0), 29, 0, false); // 1100
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 100, 500, 10, 1, 1, CRGB(25, 0, 0), CRGB(0, 0, 0), 30, 119, false); // 1100
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 3300, 400, 4, 1, 2, CRGB(80, 80, 0), CRGB(80, 80, 0), 0, 119, false); // 900
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 4300, 500, 6, 1, 2, CRGB(50, 50, 0), CRGB(50, 50, 0), 0, 119, false); // 900
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 5300, 600, 10, 1, 2, CRGB(40, 30, 0), CRGB(40, 30, 0), 0, 119, false); // 1200
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 6600, 1000, 30, 1, 2, CRGB(128, 128, 0), CRGB(128, 128, 0), 0, 119, false); // 2800
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 9500, 2000, 60, 1, 2, CRGB(255, 255, 0), CRGB(255, 255, 0), 0, 59, true); //
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 9500, 2000, 60, 1, 2, CRGB(255, 255, 0), CRGB(255, 255, 0), 119, 60, true); //
}

void vdDoorCloseAnimation(timed_event teEventList[], unsigned long tmeCurrentTime)
{
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 000, 100, 0, 1, 1, CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 59, false); // 500
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 150, 500, 5, 1, 3, CRGB(255, 255, 0), CRGB(50, 25, 0), 0, 59, false);
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 1200, 0, 0, 1, 1, CRGB(50, 25, 0), CRGB(50, 25, 0), 0, 59, false);  // 0
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 1300, 3000, 0, 1, 1, CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 59, false);
}

void vdPacificaishAnimation(timed_event teEventList[], unsigned long tmeCurrentTime)
{
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 1000, 500, 10, 1, 1, CRGB(0, 15, 25), CRGB(0, 15, 25), 0, 59, false); // 1100
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 2000, 3500, 250, 1, 2, CRGB(40, 200, 160), CRGB(40, 200, 160), 0, 15, true); // 900
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 2000, 3800, 220, 1, 2, CRGB(40, 200, 160), CRGB(40, 200, 160), 16, 30, true); // 900
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 2000, 3600, 270, 1, 2, CRGB(40, 200, 160), CRGB(40, 200, 160), 31, 45, true); // 900
  vdCreateTimedEvent (teEventList, tmeCurrentTime, 2000, 3200, 200, 1, 2, CRGB(40, 200, 160), CRGB(40, 200, 160), 46, 59, true); // 900

  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 1000, 500, 10, 1, 1, CRGB(0, 15, 25), CRGB(0, 15, 25), 60, 119, false); // 1100
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 2000, 3500, 250, 1, 2, CRGB(40, 200, 160), CRGB(40, 200, 160), 59 + 0, 59 + 15, true); // 900
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 2000, 3800, 220, 1, 2, CRGB(40, 200, 160), CRGB(40, 200, 160), 59 + 16, 59 + 30, true); // 900
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 2000, 3600, 270, 1, 2, CRGB(40, 200, 160), CRGB(40, 200, 160), 59 + 31, 59 + 45, true); // 900
  // vdCreateTimedEvent (teEventList, tmeCurrentTime, 2000, 3200, 200, 1, 2, CRGB(40, 200, 160), CRGB(40, 200, 160), 59 + 46, 59 + 59, true); // 900
}

// ***************************************************************************************
// MAIN
// ***************************************************************************************
// GLOBAL VARIABLES

//  Light Strip Hardware
CRGB hwLEDs1[NUM_LEDSs1];       // LED Strip 1 values.
CRGB hwLEDs2[NUM_LEDSs2];       // LED Strip 2 values.

CRGB hwLEDSTARTs1[NUM_LEDSs1];  // I cant stand this but its necessary at the time.
CRGB hwLEDSTARTs2[NUM_LEDSs2];

// Light Strip Event System
timed_event tmeEvents1[NUM_TIMED_EVENTS];
timed_event tmeEvents2[NUM_TIMED_EVENTS];

// Door Sensor
hardware_monitor hwDoor;
boolean booFakeDoorSensor = false;

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

  FastLED.addLeds<LED_TYPE, DATA_PINs1, COLOR_ORDER>(hwLEDs1, NUM_LEDSs1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PINs2, COLOR_ORDER>(hwLEDs2, NUM_LEDSs2).setCorrection(TypicalLEDStrip);
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
  vdPowerOnAnimation(tmeEvents1, tmeCurrentMillis);
  vdPowerOnAnimation(tmeEvents2, tmeCurrentMillis);
  
  //test
  //vdTESTFLASHAnimation(tmeEvent, tmeCurrentMillis);
  //vdPacificaishAnimation(tmeEvent, tmeCurrentMillis);
  //vdDoorCloseAnimation(tmeEvent, tmeCurrentMillis);

}

// ---------------------------------------------------------------------------------------
// MAIN LOOP

void loop()
//  Main Loop:
//    Events and Light Path animations should only be called when their time is up.
{

  //  Only update the hardware when changes have been detected.
  //    This vabiable will be checked at the end of the loop.  If nothing was updated,
  //    the loop will just walk on past any hardware updates that would otherwise be
  //    sent.
  boolean booUpdate = false;

  //  Get current time.  This will be our timeframe to work in.
  tmeCurrentMillis = millis();

  // --- Begin of Delayless Loop ---
  if (tmeCurrentMillis - tmePrevMillis >= intRestTime)
  {
    tmePrevMillis = tmeCurrentMillis;

    // --- TESTING AREA ---
    // Create fake changes for like open and closing doors and things.
    if (true)
    {
      if ((tmeCurrentMillis > 5000) && (tmeCurrentMillis < 6000))
      {
        booFakeDoorSensor = true;
      }
      if ((tmeCurrentMillis > 20000) && (tmeCurrentMillis < 21000))
      {
        booFakeDoorSensor = false;
      }
      if ((tmeCurrentMillis > 26000) && (tmeCurrentMillis < 27000))
      {
        vdClearAllTimedEvent(tmeEvents1);
        vdPacificaishAnimation(tmeEvents1, tmeCurrentMillis);
      }
    }

    // --- Grabbing Data From Hardware inputs ---
    // Check door for changes.
    if (hwDoor.changed(booFakeDoorSensor == true))
    {
      if (booFakeDoorSensor == true)
      {
        vdClearAllTimedEvent(tmeEvents1);
        vdDoorOpenAnimation(tmeEvents1, tmeCurrentMillis);
      }
      else
      {
        vdClearAllTimedEvent(tmeEvents1);
        vdDoorCloseAnimation(tmeEvents1, tmeCurrentMillis);
      }
    }

    // --- Check and Execute Timed Events That Are Ready ---

    for (int x = 0; x < NUM_TIMED_EVENTS; x++)
    {
      // Execute check for timed events for Strip 1
      if (tmeEvents1[x].is_ready(tmeCurrentMillis) == true)
      {
        tmeEvents1[x].execute(hwLEDs1, hwLEDSTARTs1, tmeCurrentMillis);
 
        //  If we made it to this part of the code then we need to
        //    tell the LED hardware that it has a change to commit.
        booUpdate = true;
      }
      // Execute check for timed events for Strip 2
      if (tmeEvents2[x].is_ready(tmeCurrentMillis) == true)
      {
        tmeEvents2[x].execute(hwLEDs2, hwLEDSTARTs2, tmeCurrentMillis);

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
