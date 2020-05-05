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
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.23A
// *  TEST CODE:                 QACODE: A565              CENSORCODE: EQK6}Lc`:Eg>
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
// *  V 0.23 _200502
// *      - Added a Lock LED Start parameter to the events to prevent multiple events on
// *          the same pixel set, starting at different times, to not step on each other.
// *      - Changed some varible sizes to save space.  LED counts are now one byte and
// *          duration and speed is now maxing at 65535ms (2 bytes).
// *      - For some odd reason, memory was not being saved when using a single LED
// *          strip.
// *      - Oh, boy.  Getting complex now.  Got it working and wishing I had some
// *          Lavian Brandy (https://elite-dangerous.fandom.com/wiki/Lavian_Brandy)
// *          to celebrate. A Start Color lock array was added and I dumped the old
// *          count multiple events on same pixel routine, that would drop the first
// *          event colision.
// *      - Changed a few animations to reflect the upgrade.
// *      - Still have no idea how to make Christmas lights.
// *
// *  V 0.22 _200427
// *      - Events crossing over the same pixels now work.  Not perfectly but does work
// *          when the Events start at the same time.  Will work on this more when I
// *          feel like making more complex animations.
// *      - Changed the intRestTime varible to 15.  Turns out, this arduino isn't as much
// *          of a turtle as I thought it was.  Actually, not a turtle at all.  Anyway,
// *          updating the refresh rate at 1000 times a second causes glitches as it writes
// *          over itself in the same frame.  I changed the varible to = 16 to represent
// *          something like 60FPS.
// *      - Changed a bunch of other things while trying to debug a/the problem, that was
// *          described and solved with the change above.
// *
// *  V 0.21 _200425
// *      - Small change, if the door is open when the system is powered on then the door
// *          open animation will be active.
// *      - Removed blank lines and added blank lines.
// *      - Manually word_wrapped some stuff.
// *
// *  V 0.20 _200424
// *      - Rewrote a large portion of the code to change the way the LED animations are
// *          calculated with respects to time.
// *          In short, previously the values of the LEDs were calculated in chunks,
// *          beginning to end, per event animation.  Now, I am calculating the value of
// *          each pixel from the begging of the LED strip, down to the end of the strip.
// *          Think of it as moving from an LCD Timex watch, to a progressive scan Sony 4k
// *          Gaming Monitor.  Ok, so its not that extreme, but in the future, I will be
// *          able to make more accurate calculations when multiple events overlap the
// *          same pixel.  We aren't there yet, but we could be.
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
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
#define NUM_LEDSs1    71
#define NUM_LEDSs2    60

#define SWITCH_PINs1  8       // Hardware Open Close Door Sensor 1
#define SWITCH_PINs2  9       // Hardware Open Close Door Sensor 1

#define BRIGHTNESS        96  // Using Example Code.  Max unknown
#define FRAMES_PER_SECOND 120 // Will not be necessary, but keeping, for now, just in 
//  case.

#define RESTTIME          16

#define NUM_STRIPS        2
#define NUM_TIMED_EVENTS  15  // Untill I can remember how LL, this is being
//  Also, total number of this will be multiplied by the
//  amount of LED strips you have setup.  Watch your memory.


// ***************************************************************************************
// STRUCTURES
// ***************************************************************************************
//  TIMED EVENT

struct bigCRGB
// Simple RGB varible for computation.
{
  int r = 0;
  int g = 0;
  int b = 0;
};

struct timed_event_data
// Varibles to control the timed_event.
{
  unsigned long tmeSTARTTIME;
  unsigned int intDURATION;
  unsigned int intSPEED;
  byte bytANIMATION;
  byte bytLEDANIMATION;
  bigCRGB bigcrgbCOLORDEST1;
  bigCRGB bigcrgbCOLORDEST2;
  byte intSTARTPOS;
  byte intENDPOS;
  boolean booREPEAT;
  //boolean booLOCK;

  boolean booSETCOMPLETE = false;
  boolean booEXPIRED = true;

  void PreCheck(CRGB hwLEDArray[], CRGB hwLEDStartArray[], boolean hwLEDStartLockArray[], unsigned long tmeCurrentTime)
  // If an event is starting, we need to store and lock the Start Colors.
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

        for (int x = ps; x <= pe; x++)
        {
          if (hwLEDStartLockArray[x] == false)
          {
            hwLEDStartArray[x] = hwLEDArray[x];
          }
          hwLEDStartLockArray[x] = true;
        }
        booSETCOMPLETE = true;
      }
    }
  }

  void PostCheck(unsigned long tmeCurrent, boolean booColision, boolean hwLEDStartLockArray[])
  // Check to see if the event is over, unlock and release Start Colors.
  {
    //  Check to see if this event repeats and outside its life time.
    if (booEXPIRED == false)
    {
      //  (num of led * speed + duration)
      // Grabbing actual current time to process decayed events, because alot could've happened
      // since then.
      if (millis() >= tmeSTARTTIME + intDURATION +
          ((abs(intENDPOS - intSTARTPOS) + 1) * intSPEED))
      {
        // The event is old and nothing else is using its start colors.
        if (booColision == false && booREPEAT == false)
        {
          // Clear the event.
          booSETCOMPLETE = false;
          booEXPIRED = true;

          // Unlock the Start Pixel Colors.
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
          for (int x = ps; x <= pe; x++)
          {
            hwLEDStartLockArray[x] = false;
          }
        }

        // The event is old but other events are using these start colors.
        if (booColision == true && booREPEAT == false)
        {
          // Clear the event but keep it active.
          booEXPIRED = true;
        }

        // The event repeats, collision is irrelevant.
        if (booREPEAT = true)
        {
          // Keep the event active by resetting its start time.
          tmeSTARTTIME = tmeCurrent;  // Using current time here because we want
          //  to keep the frames the same.
        }
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
  // On startup, define the amount of LEDs in this event list.
  {
    intLEDCOUNT = intLedCount;
  }

  void set(unsigned long tmeCurrentTime,
           unsigned long tmeStartInTime, unsigned long intDuration, unsigned int intSpeed,
           byte bytAnimation, byte bytLEDAnimation, CRGB crgbColor1, CRGB crgbColor2,
           int intStartPos, int intEndPos, boolean booRepeat)
  // Prepare an animation to start at a specific time.
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

        teDATA[x].bigcrgbCOLORDEST1.r = crgbColor1.r;
        teDATA[x].bigcrgbCOLORDEST1.g = crgbColor1.g;
        teDATA[x].bigcrgbCOLORDEST1.b = crgbColor1.b;

        teDATA[x].bigcrgbCOLORDEST2.r = crgbColor2.r;
        teDATA[x].bigcrgbCOLORDEST2.g = crgbColor2.g;
        teDATA[x].bigcrgbCOLORDEST2.b = crgbColor2.b;

        teDATA[x].intSTARTPOS = intStartPos;
        teDATA[x].intENDPOS = intEndPos;
        teDATA[x].booREPEAT = booRepeat;

        teDATA[x].booEXPIRED = false;
        booCreated = true;
      }
    }
  }

  void ClearAll()
  //Remove all timed event animations on a specific animation strip.
  {
    for (int x = 0; x < NUM_TIMED_EVENTS; x++)
    {
      teDATA[x].booEXPIRED = true;
      teDATA[x].booSETCOMPLETE = false;
    }
  }

  bigCRGB crgb_anim_color(int intDur, byte bytLedAnimation,
                          bigCRGB bigcrgbCDest1, bigCRGB bigcrgbCDest2, unsigned long tmeCurrentTime,
                          unsigned long tmeAnimTime)
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
    bigCRGB tmpColor;

    switch (bytLedAnimation)
    {
      case 1:
        // Fade Animation Path
        {
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            return bigcrgbCDest1;
          }
          else
          {
            float fltPower = (float)tmeElapsed / (float)intDur;

            tmpColor.r = (fltPower * bigcrgbCDest1.r) + ((1 - fltPower) * tmpColor.r);
            tmpColor.g = (fltPower * bigcrgbCDest1.g) + ((1 - fltPower) * tmpColor.g);
            tmpColor.b = (fltPower * bigcrgbCDest1.b) + ((1 - fltPower) * tmpColor.b);

            return tmpColor;

          }
        } // END CASE 1

      case 2:
        // Pulse Animation Path
        // This can be done beter with a simple ABS function.
        {
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            return tmpColor;
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
            tmpColor.r = (fltPower * bigcrgbCDest1.r) + ((1 - fltPower) * tmpColor.r);
            tmpColor.g = (fltPower * bigcrgbCDest1.g) + ((1 - fltPower) * tmpColor.g);
            tmpColor.b = (fltPower * bigcrgbCDest1.b) + ((1 - fltPower) * tmpColor.b);

            return tmpColor;
          }
        } // END CASE 2

      case 3:
        // PulseTo Animation Path
        // This can be done beter with a simple ABS function.
        {
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            return bigcrgbCDest2;
          }
          else
          {
            bigCRGB bigcrgbA;
            bigCRGB bigcrgbB;
            //  Swap to second color and power down at mid point.
            if ((tmeElapsed * 2) <= (intDur))
            {
              fltPower = (float)(tmeElapsed) * 2 / ((float)intDur);
              bigcrgbA = tmpColor;

              bigcrgbB.r = bigcrgbCDest1.r;
              bigcrgbB.g = bigcrgbCDest1.g;
              bigcrgbB.b = bigcrgbCDest1.b;
            }
            else
            {
              fltPower = 1 - (((float)((tmeElapsed * 2) - intDur) / (float)intDur));
              bigcrgbA.r = bigcrgbCDest2.r;
              bigcrgbA.g = bigcrgbCDest2.g;
              bigcrgbA.b = bigcrgbCDest2.b;

              bigcrgbB.r = bigcrgbCDest1.r;
              bigcrgbB.g = bigcrgbCDest1.g;
              bigcrgbB.b = bigcrgbCDest1.b;
            }

            tmpColor.r = (fltPower * bigcrgbB.r) + ((1 - fltPower) * bigcrgbA.r);
            tmpColor.g = (fltPower * bigcrgbB.g) + ((1 - fltPower) * bigcrgbA.g);
            tmpColor.b = (fltPower * bigcrgbB.b) + ((1 - fltPower) * bigcrgbA.b);

            return tmpColor;
          }

        } // END CASE 3

    } // END SWITCH STATEMENT

  }

  boolean execute(CRGB hwLEDArray[], CRGB hwLEDStartArray[], boolean hwLEDStartArrayL[], unsigned long tmeCurrentTime)
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
      teDATA[event].PreCheck(hwLEDArray, hwLEDStartArray, hwLEDStartArrayL, tmeCurrentTime);
    }
    boolean booPixelColorChanged;
    CRGB crgbLED;
    int activeeventcount;
    boolean booEventColision[NUM_TIMED_EVENTS];
    boolean booEventActive[NUM_TIMED_EVENTS];
    bigCRGB bigcrgbNewColor;
    bigCRGB bigcrgbTempStart;
    bigCRGB tempColor;
    bigCRGB bigcrgbTempDest1;
    bigCRGB bigcrgbTempDest2;

    unsigned long tmeStartAnim;

    // Clear Collision Tracker
    for (int x = 0; x < NUM_TIMED_EVENTS; x++)
    {
      booEventColision[x] = false;
    }

    // Process LEDs, one by one.
    for (int led = 0; led < intLEDCOUNT; led ++)
    {
      activeeventcount = 0;
      for (int x = 0; x < NUM_TIMED_EVENTS; x++)
      {
        booEventActive[x] = false;
      }

      // Clear the value of the new LED color.  Should only update when it has changed.
      bigcrgbNewColor.r = 0;
      bigcrgbNewColor.g = 0;
      bigcrgbNewColor.b = 0;

      booPixelColorChanged = false;

      // Store the value of the Start color, once per event.
      //  No matter what event is called, all changes are based on it.
      bigcrgbTempStart.r = hwLEDStartArray[led].r;
      bigcrgbTempStart.g = hwLEDStartArray[led].g;
      bigcrgbTempStart.b = hwLEDStartArray[led].b;

      // Process each event, one by one.
      for (int event = 0; event < NUM_TIMED_EVENTS; event ++)
      {
        // Clear the tmp colors, in case they have data in them at the start of each event process.
        tempColor.r = 0;
        tempColor.g = 0;
        tempColor.b = 0;

        // Only continue processing the event if it is scheduled.
        if (teDATA[event].booEXPIRED == false)
        {

          // Is the LED we are processing within the event?
          if (((led >= teDATA[event].intSTARTPOS) && (led <= teDATA[event].intENDPOS))
              || ((led <= teDATA[event].intSTARTPOS) && (led >= teDATA[event].intENDPOS)))

          {
            // OK, so an event is schedule, but is it ready to start?
            if (tmeCurrentTime >= teDATA[event].tmeSTARTTIME)
            {

              // Collision Tracker
              activeeventcount++;   //  Attempt to restrict the amount of times this routine is ran.
              booEventActive[event] = true;
              if (booEventColision[event] == false && activeeventcount > 1)
              {
                for (int e = 0; e < NUM_TIMED_EVENTS; e ++)
                {
                  if (booEventActive[e] == true && e != event)
                  {
                    booEventColision[e] = true;
                    booEventColision[event] = true;
                  }
                }
              }

              // Grab Event Data that may change or be processed within this switch statement.
              //  We will be calculating the change of the pixel color, not the actual color.
              bigcrgbTempDest1.r = teDATA[event].bigcrgbCOLORDEST1.r - bigcrgbTempStart.r;
              bigcrgbTempDest1.g = teDATA[event].bigcrgbCOLORDEST1.g - bigcrgbTempStart.g;
              bigcrgbTempDest1.b = teDATA[event].bigcrgbCOLORDEST1.b - bigcrgbTempStart.b;

              bigcrgbTempDest2.r = teDATA[event].bigcrgbCOLORDEST2.r - bigcrgbTempStart.r;
              bigcrgbTempDest2.g = teDATA[event].bigcrgbCOLORDEST2.g - bigcrgbTempStart.g;
              bigcrgbTempDest2.b = teDATA[event].bigcrgbCOLORDEST2.b - bigcrgbTempStart.b;

              // Figure out when the LED is suposed to start doing something.
              tmeStartAnim = teDATA[event].tmeSTARTTIME
                             + (abs((led - teDATA[event].intSTARTPOS))
                                * teDATA[event].intSPEED);

              // The Pixel on this Event is ready to change.
              if ((tmeCurrentTime >= tmeStartAnim))
              {
                // Keep track of how many events changed the same pixel.
                booEventActive[event] = true;

                // Preprocess Dest 2 for dynamic DEST2 on Pixel Fade.
                if (teDATA[event].bytLEDANIMATION == AnPiFade)
                {

                  // Get value of light based on animation at current time.
                  float fltPower;
                  if ((float)(abs(teDATA[event].intENDPOS - teDATA[event].intSTARTPOS))  == 0)
                  {
                    fltPower = 0;
                  }
                  else
                  {
                    fltPower = (float)abs(led - teDATA[event].intSTARTPOS)
                               / (float)(abs(teDATA[event].intENDPOS - teDATA[event].intSTARTPOS));
                  }

                  bigcrgbTempDest1.r = (fltPower * bigcrgbTempDest2.r)
                                       + ((1 - fltPower) * bigcrgbTempDest1.r);

                  bigcrgbTempDest1.g = (fltPower * bigcrgbTempDest2.g)
                                       + ((1 - fltPower) * bigcrgbTempDest1.g);

                  bigcrgbTempDest1.b = (fltPower * bigcrgbTempDest2.b)
                                       + ((1 - fltPower) * bigcrgbTempDest1.b);
                }

                switch (teDATA[event].bytANIMATION)
                {
                  case AnEvSweep:
                    {
                      // Calculate how much this Event will chaange the pixel.
                      tempColor = crgb_anim_color(teDATA[event].intDURATION,
                                                  teDATA[event].bytLEDANIMATION,
                                                  bigcrgbTempDest1,
                                                  bigcrgbTempDest2, tmeCurrentTime,
                                                  tmeStartAnim);

                      bigcrgbNewColor.r = bigcrgbNewColor.r + tempColor.r;
                      bigcrgbNewColor.g = bigcrgbNewColor.g + tempColor.g;
                      bigcrgbNewColor.b = bigcrgbNewColor.b + tempColor.b;

                      booPixelColorChanged = true;

                    } // End Case AnEvSweep
                } // End Switch Statement
              }
            } // End Time Check

          } // End LED Postion Check
        } // End Expiration Check
      } // End For Event Loop

      // Now that all event are processeds, update the Main LED to the new value.
      // But only if it changed.
      if (booPixelColorChanged == true)
      {
        // Calclulate the color of LED by adding the colors togegher.
        bigcrgbNewColor.r = bigcrgbNewColor.r + bigcrgbTempStart.r;
        bigcrgbNewColor.g = bigcrgbNewColor.g + bigcrgbTempStart.g;
        bigcrgbNewColor.b = bigcrgbNewColor.b + bigcrgbTempStart.b;

        // If the lights are out range, either put them at full on or full off.
        if (bigcrgbNewColor.r > 255)
        {
          bigcrgbNewColor.r = 255;
        }
        if (bigcrgbNewColor.g > 255)
        {
          bigcrgbNewColor.g = 255;
        }
        if (bigcrgbNewColor.b > 255)
        {
          bigcrgbNewColor.b = 255;
        }
        if (bigcrgbNewColor.r  < 0)
        {
          bigcrgbNewColor.r = 0;
        }
        if (bigcrgbNewColor.g  < 0)
        {
          bigcrgbNewColor.g = 0;
        }
        if (bigcrgbNewColor.b  < 0)
        {
          bigcrgbNewColor.b = 0;
        }

        // Set the color of the LED on the strip.
        hwLEDArray[led].r = bigcrgbNewColor.r;
        hwLEDArray[led].g = bigcrgbNewColor.g;
        hwLEDArray[led].b = bigcrgbNewColor.b;

        booChanged = true;
      }
    } // End For LED Loop

    // All Leds Processed

    //  Check to see if any events expire or repeats.
    for (int event = 0; event < NUM_TIMED_EVENTS; event ++)
    {
      teDATA[event].PostCheck(tmeCurrentTime, booEventColision[event], hwLEDStartArrayL);
    }

    return booChanged;
  }
};


// ---------------------------------------------------------------------------------------
// HARDWARE MONITOR

struct hardware_monitor
// Create a hardware switch or button varable type.
{
  unsigned long tmeCHANGEDETECTEDTIME;
  boolean booPREVCHANGEDETECTED;
  unsigned int tmeLEEWAY;
  boolean booVALUE;
  boolean booFIRSTRUN = true;

  void set(boolean booValue, int tmeLeeWay)
  // Prepare the switch.
  {
    tmeCHANGEDETECTEDTIME = millis();
    tmeLEEWAY = tmeLeeWay;
    booVALUE = booValue;
    booPREVCHANGEDETECTED = false;
  }

  boolean changed(boolean booValue)
  // Return true if the switch state change from on to off or off to on.
  {
    unsigned long tmeTme = millis();

    // If the switch was just activated then run any set up its initial state and run
    //  any special routines.
    if (booFIRSTRUN == true)
    {
      booVALUE = booValue;
      booPREVCHANGEDETECTED = false;
      tmeCHANGEDETECTEDTIME = tmeTme;
      booFIRSTRUN = false;
      return booValue;  // Change this if you feel like testing
      //return false;   // you feel like testing the door open on start behaviour
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
    // Only report change of status when Leeway time is passed.  This is a essentially and
    //  debouncer.
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
void vdClearAllTimedEvent(timed_event teEvent[], int intPos, boolean LockArray[])
//  Sort through the timed event list a create an event at the first slot available.
{
  teEvent[intPos].ClearAll();
  for (int x = 0; x < teEvent[intPos].intLEDCOUNT ; x++)
  {
    LockArray[x] = false;
  }
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
// Calculate and return the next delay time based on duration of event, number of pixels,
//  and animation speed of each pixel.
//  Value in return statement is buffer time.
{
  return (1 + RESTTIME + intTm + intDur + (intSp * intCt));
}

//void vdTESTFLASHAnimation(timed_event teEvent, unsigned long tmeCurrentTime)
//{
//  vdCreateTimedEvent (teEvent, tmeCurrentTime, 2000, 3000, 200, AnEvSweep, AnPiPulseTo, CRGB(100, 100, 100), CRGB(50, 0, 0), 10, 49, true);
//}

void vdPowerOnAnimation(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Just to show the lights work when the program starts.
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
// Door is open, engage safety lights.
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;
  // Door Open Animation

  // Clear set background to door open colors.
  intTm = 100; intDur = 500; intSp = 10; intCt = 60;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(255, 255, 255), CRGB(255, 64, 64), 0, 7, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(255, 64, 64), CRGB(25, 0, 0), 8, 15, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(25, 0, 0), 16, 55, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(0, 0, 0), 56, NUM_LEDSs1 - 1, false);

  // Flash
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 100; intSp = 2; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), 0, NUM_LEDSs1 - 1, false); // 900
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 200; intSp = 4; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), 0, NUM_LEDSs1 - 1, false); // 900
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 600; intSp = 12; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), 0, NUM_LEDSs1 - 1, false); // 900
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 1000; intSp = 20; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), 0, NUM_LEDSs1 - 1, false); // 900
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 1500; intSp = 30; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(128, 128, 0), CRGB(128, 128, 0), 0, NUM_LEDSs1 - 1, false); // 2800

  // Repeat Pulse
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 1500; intSp = 125; intCt = 36;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), 0, 35, true);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), NUM_LEDSs1 - 1, 35, true);
  // and
  teEvent[intPos].set(tmeCurrentTime, intTm + 3000, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), 0, 35, true);
  teEvent[intPos].set(tmeCurrentTime, intTm + 3000, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), NUM_LEDSs1 - 1, 35, true);
}

void vdDoorCloseAnimation(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Door closed.  Quick animation to show and turn off lights.
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;

  // Clear by setting colors to default door open colors.
  intTm = 100; intDur = 500; intSp = 10; intCt = 40;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(255, 255, 255), CRGB(255, 64, 64), 0, 7, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(255, 64, 64), CRGB(25, 0, 0), 8, 15, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(25, 0, 0), 16, 55, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(25, 0, 0), CRGB(0, 0, 0), 56, NUM_LEDSs1 - 1, false);

  // Clear and Pulse colors background to green then ending in blueish, starting with the center.
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 750; intSp = 30; intCt = 36;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(0, 255, 0), CRGB(0, 20, 25), 36, NUM_LEDSs1 - 1, false); // 900
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(0, 255, 0), CRGB(0, 20, 25), 35, 0, false); // 900

  // Apply color gradient to 0 on ends of the strip.
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 1000; intSp = 0; intCt = 30;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 20, 25), 0, 20, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 20, 25), CRGB(0, 20, 25), 21, 50, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 20, 25), CRGB(0, 0, 0), 51, NUM_LEDSs1 - 1, false);

  // Fade remaining colors out.
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 5000; intSp = 100; intCt = 36;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 35, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 0), NUM_LEDSs1 - 1, 36, false);
}

void vdPacificaishAnimation(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// This animation was my personal chalenge to see if I could make a similar animation that was
// provided in the demos and examples, as PacificaAnimation, of the FastLed library.
{
  /*  Old animation in that I keep around for testing.
    teEvent[intPos].set(tmeCurrentTime, 1000, 500, 10, AnEvSweep, AnPiFade, CRGB(0, 15, 25), CRGB(0, 15, 25), 0, 59, false); // 1100
    teEvent[intPos].set(tmeCurrentTime, 2000, 3500, 250, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 0, 15, true); // 900
    teEvent[intPos].set(tmeCurrentTime, 2000, 3800, 220, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 16, 30, true); // 900
    teEvent[intPos].set(tmeCurrentTime, 2000, 3600, 270, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 31, 45, true); // 900
    teEvent[intPos].set(tmeCurrentTime, 2000, 3200, 200, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 46, 59, true); // 900
  */

  // Set the background color.
  teEvent[intPos].set(tmeCurrentTime, 1000, 500, 10, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 60), 0, 59, false); // 1100

  // The waves.
  teEvent[intPos].set(tmeCurrentTime, 2000, 3500, 250, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 0, 59, true); // 900
  teEvent[intPos].set(tmeCurrentTime, 2000, 1500, 150, AnEvSweep, AnPiPulse, CRGB(160, 200, 40), CRGB(40, 200, 160), 0, 59, true); // 900
  teEvent[intPos].set(tmeCurrentTime, 2000, 3600, 270, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 0, 59, true); // 900
  teEvent[intPos].set(tmeCurrentTime, 2000, 3200, 200, AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), 0, 59, true); // 900

  // Just more testing.
  teEvent[intPos].set(tmeCurrentTime, 2000, 250, 60, AnEvSweep, AnPiPulse, CRGB(255, 0, 0), CRGB(50, 0, 0), 59, 0, true); // 900
  teEvent[intPos].set(tmeCurrentTime, 2000, 1500, 150, AnEvSweep, AnPiPulse, CRGB(0, 255, 0), CRGB(40, 200, 160), 59, 0, true); // 900
  teEvent[intPos].set(tmeCurrentTime, 2000, 1500, 40, AnEvSweep, AnPiPulse, CRGB(0, 0, 255), CRGB(40, 200, 160), 0, 59, true); // 900

}


// ***************************************************************************************
// MAIN
// ***************************************************************************************
// GLOBAL VARIABLES

//  Light Strip Hardware
CRGB hwLEDs1[NUM_LEDSs1];       // LED Strip 1 values.
CRGB hwLEDs2[NUM_LEDSs2];       // LED Strip 2 values.

CRGB    hwLEDSTARTs1[NUM_LEDSs1];  // I cant stand this but its necessary at the time.
boolean hwLEDSTARTLs1[NUM_LEDSs1]; // Lock Start Color

CRGB    hwLEDSTARTs2[NUM_LEDSs2];  // I cant stand this but its necessary at the time.
boolean hwLEDSTARTLs2[NUM_LEDSs2]; // Lock Start Color

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
int intRestTime = RESTTIME;  // 16 = 60 fps     // Do not check for update until rest time is passed.

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
        vdClearAllTimedEvent(teEvent, 0, hwLEDSTARTLs1);
        vdDoorOpenAnimation(teEvent, 0, tmeCurrentMillis);
        //vdPacificaishAnimation(teEvent, 0, tmeCurrentMillis); // If testing.
      }
      else
      {
        vdClearAllTimedEvent(teEvent, 0, hwLEDSTARTLs1);
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

    booUpdate = teEvent[0].execute(hwLEDs1, hwLEDSTARTs1, hwLEDSTARTLs1, tmeCurrentMillis);

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
