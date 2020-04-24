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
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.20A
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
// *  V 0.20 _200424
// *      - Rewrote a large portion of the code to change the way the LED animations are 
// *          calculated with respects to time.  
// *          In short, previously the values of the LEDs were calculated in chunks, beginning
// *          to end, per event animation.  Now, I am calculating the value of each pixel 
// *          from the begging of the LED strip, down to the end of the strip.  Think of
// *          it as moving from an LCD Timex watch, to a progressive scan Sony 4k Gaming 
// *          Monitor.  Ok, so its not that extreme, but in the future, I will be able 
// *          to make more accurate calculations when multiple events overlap the same 
// *          pixel.  We aren't there yet, but we could be.
// *
// *  V 0.13 _200420
// *      - Been a month now because of the pandemic. Finally recieved a more spare parts.
// *      - Added some basic Animation Definitions to make things easyier to understand.
// *      - Added a simple routine to calculate time delays in between event animations.
// *      - Made some cosmetic changes to some of the animations.
// *
// *  V 0.12 _200326
// *      - Fine tuned the door animations till it was visually appealing to me.
// *      - The behavior of the first state animation changed.  I cant get it to
// *          work like it was and the way it was prefered.  Hacked together and
// *          ugly fix that I will just need to get rid of at a later time.
// *
// *  V 0.11b _200325
// *      - Small changes made.  Preparing and installing hardware in test ship.
// *      - Lost #2 of 3 Arduino boards because of a failure on my part to disconnect one
// *          of the power leads while updating.  Although it was not powered, a short
// *          occured and the board was burnt.  I loved you little Nano, but I can't
// *          fix you.
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
// *        The code is a  hot mess.  Nearly completely undocumented.  Crap is
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

// BASIC DEFINITIONS FOR EASY Reference

// Event Animations
#define AnEvClear     0
#define AnEvSweep     1
// Pixel Animations
#define AnPiFade      1
#define AnPiPulse     2
#define AnPiPulseTo   3

// HARDWARE SETUP

#define DATA_PINs1    3       // Data Pin for Strip 1
#define DATA_PINs2    4       // Data Pin for Strip 2
//#define CLK_PIN     5       // If your LED_TYPE requires one.
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDSs1    71
#define NUM_LEDSs2    60

#define SWITCH_PINs1  8       // Hardware Open Close Door Sensor 1
#define SWITCH_PINs2  9       // Hardware Open Close Door Sensor 1

#define BRIGHTNESS        96  // Using Example Code.  Max unknown
#define FRAMES_PER_SECOND 120 // Will not be necessary, but keeping, for now, just in case.

#define NUM_STRIPS  2
#define NUM_TIMED_EVENTS  15  // Untill I can remember how LL, this is being
//  Also, total number of this will be multiplied by the
//  amount of LED strips you have setup.  Watch your memory.

// ***************************************************************************************
// STRUCTURES
// ***************************************************************************************
//  TIMED EVENT

struct timed_event_data
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


  void PreCheck(CRGB hwLEDArray[], CRGB hwLEDStartArray[], unsigned long tmeCurrentTime)
  {
    if (booSETCOMPLETE == false)
    {
      if ((booEXPIRED == false) && (tmeCurrentTime >= tmeSTARTTIME))
      {
        int ps;
        int pe;

        if (intSTARTPOS <= intENDPOS)
        {
          ps = intSTARTPOS;
          pe = intENDPOS;
        }
        else
        {
          pe = intSTARTPOS;
          ps = intENDPOS;
        }
        
        for (int x= ps; x <= pe; x++)
        {
          hwLEDStartArray[x] = hwLEDArray[x];
        }
          booSETCOMPLETE = true;
      }
    }
  }

  void PostCheck(unsigned long tmeCurrent)
  {
    //  Check to see if this event repeats.

    //  It doesn' to set it as expired.
    if (booEXPIRED == false)
    {
      if (tmeCurrent >= tmeSTARTTIME + intDURATION + (abs(intENDPOS - intSTARTPOS) * intSPEED))
      {
        booEXPIRED = true;
        ////Serial.println("Event PostCheck  EVENT EXPIRED");
      }

      if (booEXPIRED == true && booREPEAT == true)
      {
        //  It does repeat so reschedule it to when we know all the light Paths have been
        //  complete, which is what I would love to do, but for now we will just go by the
        //  we took to start the event:  Its duration.  Never mind. Got it.
        //  (num of led * speed + duration)



        booSETCOMPLETE = false;
        booEXPIRED = false;
        tmeSTARTTIME = tmeCurrent;
      }
    }
  }
};




struct timed_event
//  Create a Timed LED Event:
//    Controls when a group of LEDs are to begin their individual light path animation.
//    If an event is over written the previous animation will be lost.
{
  timed_event_data teDATA[NUM_TIMED_EVENTS];
  unsigned int      intLEDCOUNT;
  boolean booEXPIRED = true;

  void create(unsigned intLedCount)
  {
    intLEDCOUNT = intLedCount;
  }

  void set(unsigned long tmeCurrentTime,
           unsigned long tmeStartInTime, unsigned long intDuration, unsigned int intSpeed,
           byte bytAnimation, byte bytLEDAnimation, CRGB crgbColor1, CRGB crgbColor2,
           int intStartPos, int intEndPos, boolean booRepeat)
  {

    boolean booCreated = false;

    for (int x = 0; ((x < NUM_TIMED_EVENTS) && (booCreated == false)) ; x++)

    {
      if (teDATA[x].booEXPIRED == true)
      {
        unsigned long tmeStartTime = tmeCurrentTime + tmeStartInTime;

        teDATA[x].tmeSTARTTIME = tmeStartTime;
        teDATA[x].intDURATION = intDuration;
        teDATA[x].intSPEED = intSpeed;
        teDATA[x].bytANIMATION = bytAnimation;
        teDATA[x].bytLEDANIMATION = bytLEDAnimation;
        teDATA[x].crgbCOLORDEST1 = crgbColor1;
        teDATA[x].crgbCOLORDEST2 = crgbColor2;
        teDATA[x].intSTARTPOS = intStartPos;
        teDATA[x].intENDPOS = intEndPos;
        teDATA[x].booREPEAT = booRepeat;

        teDATA[x].booEXPIRED = false;
        booCreated = true;


      }
    }
  }

  void ClearAll()
  {
    for (int x = 0; x < NUM_TIMED_EVENTS; x++)
    {
      teDATA[x].booEXPIRED = true;
      teDATA[x].booSETCOMPLETE = false;
    }
  }


  CRGB crgb_anim_color(int intDur, byte bytLedAnimation, CRGB crgbCSTart, CRGB crgbCDest1, CRGB crgbCDest2,
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

    switch (bytLedAnimation)
    {
      case 1:
        // Fade Animation Path
        {
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            return crgbCDest1;
          }
          else
          {
            float fltPower = (float)tmeElapsed / (float)intDur;

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
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            return crgbCSTart;
          }
          else
          {
            if ((tmeElapsed * 2) <= (intDur))
            {
              fltPower = (float)(tmeElapsed) * 2 / ((float)intDur);
            }
            else
            {
              fltPower = 1 - (((float)((tmeElapsed * 2) - intDur) / (float)intDur));
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
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            return crgbCDest2;
          }
          else
          {
            CRGB crgbA;
            CRGB crgbB;
            //  Swap to second color and power down at mid point.
            if ((tmeElapsed * 2) <= (intDur))
            {
              fltPower = (float)(tmeElapsed) * 2 / ((float)intDur);
              crgbA = crgbCSTart;
              crgbB = crgbCDest1;
            }
            else
            {
              fltPower = 1 - (((float)((tmeElapsed * 2) - intDur) / (float)intDur));
              crgbA = crgbCDest2;
              crgbB = crgbCDest1;
            }

            byte R = (fltPower * crgbB.r) + ((1 - fltPower) * crgbA.r);
            byte G = (fltPower * crgbB.g) + ((1 - fltPower) * crgbA.g);
            byte B = (fltPower * crgbB.b) + ((1 - fltPower) * crgbA.b);

            return CRGB(R, G, B);
          }

        } // END CASE 3

    } // END SWITCH STATEMENT

  }

  boolean execute(CRGB hwLEDArray[], CRGB hwLEDStartArray[], unsigned long tmeCurrentTime)
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

    boolean booChanged = false;

    for (int event = 0; event < NUM_TIMED_EVENTS; event ++)
    {
      teDATA[event].PreCheck(hwLEDArray, hwLEDStartArray, tmeCurrentTime);
    }

    CRGB crgbLED;

    for (int led = 0; led < intLEDCOUNT; led ++)
    {
      for (int event = 0; event < NUM_TIMED_EVENTS; event ++)
      {
        if (teDATA[event].booEXPIRED == false)
        {
          if (tmeCurrentTime >= teDATA[event].tmeSTARTTIME)
          {
            if (((led >= teDATA[event].intSTARTPOS) && (led <= teDATA[event].intENDPOS)) || ((led <= teDATA[event].intSTARTPOS) && (led >= teDATA[event].intENDPOS)))
            {

              // Grab Data that may change or be processed within this switch statement.
              CRGB crgbTempDest1 = teDATA[event].crgbCOLORDEST1;
              CRGB crgbTempDest2 = teDATA[event].crgbCOLORDEST2;

              // Preprocess Dest 2 for dynamic DEST2 on Pixel Fade.
              if (teDATA[event].bytLEDANIMATION == AnPiFade)
              {
                //int count = 
                // Get value of light based one animation at current time.
                float fltPower = (float)abs(led - teDATA[event].intSTARTPOS) / (float)(abs(teDATA[event].intENDPOS - teDATA[event].intSTARTPOS));

                crgbTempDest1.r = (fltPower * crgbTempDest2.r) + ((1 - fltPower) * crgbTempDest1.r);
                crgbTempDest1.g = (fltPower * crgbTempDest2.g) + ((1 - fltPower) * crgbTempDest1.g);
                crgbTempDest1.b = (fltPower * crgbTempDest2.b) + ((1 - fltPower) * crgbTempDest1.b);
              }

              switch (teDATA[event].bytANIMATION)
              {
                case AnEvSweep:
                  {
                    unsigned long tmeStartAnim;
                  
                    tmeStartAnim = teDATA[event].tmeSTARTTIME + (abs((led - teDATA[event].intSTARTPOS)) * teDATA[event].intSPEED);
  
                    if (tmeCurrentTime >= tmeStartAnim)
                    {
                      crgbLED = crgb_anim_color(teDATA[event].intDURATION, teDATA[event].bytLEDANIMATION, hwLEDStartArray[led], crgbTempDest1, crgbTempDest2, tmeCurrentTime, tmeStartAnim);

                      hwLEDArray[led] = crgbLED;
                      booChanged = true;
                    }
                  } // End Case AnEvSweep
                } // End Switch Statement
            } // End LED Postion Check
          } // End Time Check
        } // End Expiration Check
      } // End For Event Loop
    } // End For LED Loop

    // All Leds Processed

    //  Check to see if any events expire or repeats.

    for (int event = 0; event < NUM_TIMED_EVENTS; event ++)
    {
      teDATA[event].PostCheck(tmeCurrentTime);
    }

    return booChanged;
  }
};

// ---------------------------------------------------------------------------------------
// HARDWARE MONITOR

struct hardware_monitor
//  Create a hardware switch or button varable type.
{
  unsigned long tmeCHANGEDETECTEDTIME;
  boolean booPREVCHANGEDETECTED;
  unsigned int tmeLEEWAY;
  boolean booVALUE;
  boolean booFIRSTRUN = true;

  void set(boolean booValue, int tmeLeeWay)
  {
    tmeCHANGEDETECTEDTIME = millis();
    tmeLEEWAY = tmeLeeWay;
    booVALUE = booValue;
    booPREVCHANGEDETECTED = false;
  }

  boolean changed(boolean booValue)
  {
    unsigned long tmeTme = millis();

    if (booFIRSTRUN == true)
    {
      booVALUE = booValue;
      booPREVCHANGEDETECTED = false;
      tmeCHANGEDETECTEDTIME = tmeTme;
      booFIRSTRUN = false;
      return false;
    }
    else if (booVALUE == booValue)
    {
      booPREVCHANGEDETECTED = false;
      return false;
    }
    else if (booPREVCHANGEDETECTED == false)
    {
      tmeCHANGEDETECTEDTIME = tmeTme;
      booPREVCHANGEDETECTED = true;
      return  false;
    }
    else if (tmeTme < (tmeCHANGEDETECTEDTIME + tmeLEEWAY))
    {
      return false;
    }
    else
    {
      booVALUE = booValue;
      booPREVCHANGEDETECTED = false;
      tmeCHANGEDETECTEDTIME = tmeTme;
      return true;
    }
  }
};


// ***************************************************************************************
// FUNCTION AND PROCEDURES
// ***************************************************************************************

//
void vdClearAllTimedEvent(timed_event teEvent[], int intPos)
//  Sort through the timed event list a create an event at the first slot available.
{
  teEvent[intPos].ClearAll();
}

// --- ANIMATIONS ------------------------------------------------------------------------

// PARAMETER REFFERENCE

//  Reference:
//TIMED EVENT  PARAMETERS
//light_path lpLightPaths[],
//unsigned long tmeCurrentTime,
//unsigned long tmeStartInTime,
//unsigned int intDuration,
//unsigned int intSpeed,
//byte bytAnimation,
//byte bytLEDAnimation,
//CRGB crgbColor,
//int intStartPos,
//int intEndPos

int intAnTmDly(int intTm, int intDur, int intCt, int intSp)
// Calculate and return the next delay time based on duration of event, number of pixels, and animation
//  speed of each pixel.
//  Value in return statement is buffer time.
{
  return (5 + intTm + intDur + (intSp * intCt));
}

//void vdTESTFLASHAnimation(timed_event teEvent, unsigned long tmeCurrentTime)
//{
//  vdCreateTimedEvent (teEvent, tmeCurrentTime, 2000, 3000, 200, AnEvSweep, AnPiPulseTo, CRGB(100, 100, 100), CRGB(50, 0, 0), 10, 49, true);
//}

void vdPowerOnAnimation(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;
    
  // Pulse
  intTm = 500; intDur = 250; intSp = 5; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(125, 125, 125), CRGB(0, 0, 25), 0, NUM_LEDSs1 - 1, false);
  // Clear

  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 2000; intSp = 50; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 0), NUM_LEDSs1 - 1, 0, false);

}

//void vdAlertAnimation(timed_event teEvent, unsigned long tmeCurrentTime)
//{
//
//}

void vdDoorOpenAnimation(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;
  // Door Open Animation

  // Clear
  intTm = 100; intDur = 500; intSp = 10; intCt = 60;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(255, 64, 64), CRGB(255, 255, 255), 7, 0, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(255, 64, 64), 15, 8, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(25, 0, 0), 55, 16, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(0, 0, 0), 56, NUM_LEDSs1 - 1, false);
  // Flash
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 400; intSp = 1; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), 0, NUM_LEDSs1 - 1, false); // 900
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 400; intSp = 2; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), 0, NUM_LEDSs1 - 1, false); // 900
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 500; intSp = 6; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(50, 50, 0), CRGB(50, 50, 0), 0, NUM_LEDSs1 - 1, false); // 900
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 600; intSp = 10; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(40, 30, 0), CRGB(40, 30, 0), 0, NUM_LEDSs1 - 1, false); // 1200
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 1000; intSp = 30; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(128, 128, 0), CRGB(128, 128, 0), 0, NUM_LEDSs1 - 1, false); // 2800
  // Repeat Pulse
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  teEvent[intPos].set(tmeCurrentTime, intTm, 2000, 60, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), 0, 35, true); //
  teEvent[intPos].set(tmeCurrentTime, intTm, 2000, 60, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), NUM_LEDSs1, 36, true); //
}

void vdDoorCloseAnimation(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;

  // Clear
  intTm = 100; intDur = 100; intSp = 0; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(255, 64, 64), CRGB(255, 255, 255), 7, 0, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(255, 64, 64), 15, 8, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(25, 0, 0), 55, 16, false); // 1100
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(0, 0, 0), 56, NUM_LEDSs1 - 1, false); // 1100
  // Set
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 750; intSp = 30; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(0, 255, 0), CRGB(0, 20, 25), 36, NUM_LEDSs1 - 1, false); // 900
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(0, 255, 0), CRGB(0, 20, 25), 35, 0, false); // 900
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 1000; intSp = 0; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 20, 25), 0, 20, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 20, 25), CRGB(0, 20, 25), 21, 50, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 20, 25), CRGB(0, 0, 0), 51, NUM_LEDSs1 - 1, false);
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 5000; intSp = 100; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 35, false); // 1200
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 0), NUM_LEDSs1 - 1, 36, false); // 1200

}

void vdPacificaishAnimation(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
{
  teEvent[intPos].set(tmeCurrentTime, 1000, 500, 10, AnEvSweep, AnPiFade, CRGB(0, 15, 25), CRGB(0, 15, 25), 0, 59, false); // 1100
  teEvent[intPos].set(tmeCurrentTime, 2000, 3500, 250, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 0, 15, true); // 900
  teEvent[intPos].set(tmeCurrentTime, 2000, 3800, 220, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 16, 30, true); // 900
  teEvent[intPos].set(tmeCurrentTime, 2000, 3600, 270, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 31, 45, true); // 900
  teEvent[intPos].set(tmeCurrentTime, 2000, 3200, 200, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 46, 59, true); // 900

}


// ***************************************************************************************
// MAIN
// ***************************************************************************************
// GLOBAL VARIABLES

//  Light Strip Hardware
CRGB hwLEDs1[NUM_LEDSs1];       // LED Strip 1 values.
//CRGB hwLEDs2[NUM_LEDSs2];       // LED Strip 2 values.

CRGB hwLEDSTARTs1[NUM_LEDSs1];  // I cant stand this but its necessary at the time.

// Light Strip Event System
timed_event teEvent[NUM_STRIPS];


// Door Sensor
hardware_monitor hwDoors1;
//hardware_monitor hwDoors2;

// Onboard LED to signify data being sent to LED strip.
const int ledPin =  LED_BUILTIN;

// Delay less Loop Variables
//    intRestTime defines the amount of time, in milliseconds, passed before another data
//    read pass is performed and transmitted from the controller to the main system.
unsigned long tmeCurrentMillis = millis();
unsigned long tmePrevMillis = 0;
int intRestTime = 0;             // Do not check for update until rest time is passed.

// Enable //Serial Monitor for testing
//    Enabling booTest will slow the board time and enable the //Serial monitor to be read.
const boolean booTest = false;

// ---------------------------------------------------------------------------------------
// MAIN SETUP

void setup()
{
  // Define LED Strip.
  FastLED.addLeds<LED_TYPE, DATA_PINs1, COLOR_ORDER>(hwLEDs1, NUM_LEDSs1).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE, DATA_PINs2, COLOR_ORDER>(hwLEDs2, NUM_LEDSs2).setCorrection(TypicalLEDStrip);
  LEDS.setBrightness(BRIGHTNESS);

  // Define Door Sensors.
  pinMode(SWITCH_PINs1, INPUT);
  pinMode(SWITCH_PINs2, INPUT);

  // Arduino Onboard LED
  pinMode(ledPin, OUTPUT);

  teEvent[0].create(NUM_LEDSs1);

  if (booTest == true)
  {
    Serial.begin(9600);
  }

  // Set Door
  hwDoors1.set(SWITCH_PINs1, 500);
  //hwDoors2.set(false, 100);

  // Boot Animation
  // Make sure we have the current time before we try any test animations.
  tmeCurrentMillis = millis();
  vdPowerOnAnimation(teEvent, 0, tmeCurrentMillis);
  //vdPowerOnAnimation(teEvents2, tmeCurrentMillis);

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

    // --- Grabbing Data From Hardware inputs ---

    // Give sensors time to adjust.
    // if (tmeCurrentMillis > 5000)
    // {
    // Check door for changes.

    // Check door 1 for changes.
    boolean booSensorReads1 = digitalRead(SWITCH_PINs1);
    if (hwDoors1.changed(booSensorReads1))
    {
      if (booSensorReads1 == HIGH)
      {
        vdClearAllTimedEvent(teEvent, 0);
        vdDoorOpenAnimation(teEvent, 0, tmeCurrentMillis);
      }
      else
      {
        vdClearAllTimedEvent(teEvent, 0);
        vdDoorCloseAnimation(teEvent, 0, tmeCurrentMillis);
      }
    }

    // Check door 2 for changes.
    // boolean booSensorReads2 = !digitalRead(SWITCH_PINs2);
    // if (hwDoors2.changed(booSensorReads2))
    // {
    // if (booSensorReads2 == HIGH)
    // {
    // vdClearAllTimedEvent(teEvents2);
    // vdDoorOpenAnimation(teEvents2, tmeCurrentMillis);
    // }
    // else
    // {
    // vdClearAllTimedEvent(teEvents2);
    // vdDoorCloseAnimation(teEvents2, tmeCurrentMillis);
    // }
    // }
    //}


    // --- Check and Execute Timed Events That Are Ready ---

    booUpdate = teEvent[0].execute(hwLEDs1, hwLEDSTARTs1, tmeCurrentMillis);


    //  If we made it to this part of the code then we need to
    //    tell the LED hardware that it has a change to commit.

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
