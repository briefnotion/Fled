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
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.56a
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
// *  V 0.57 _201007b
// *      - 0.57 _201007b - Small Correction with the channel 2 overhead not turning off.
// *      - After the privous update failed to work, I gave up for a while.
// *      - Classified and Typed the Strips for managment reasons.
// *      - Started Consolidating the Animations to run independantly, disregarding their 
// *          size.
// *      - Trying to fix problems that I've been putting off for a long time, while 
// *          working on the on the 500ms bug.  Hopefully the code will be cleaner 
// *          so that when I create the "watcher" subroutines, I will be able to clear 
// *          the bug and also correct the incomplete problem I had before.  
// *          With any luck, the code will be able to be ready before Halloween.  At least 
// *          I will have animations that will not need to be repeated for strips of 
// *          different sizes. 
// *      - Yes, the control panel is delayed again, doesn't matter much when I can't get 
// *          a proper soldering iron.  I cant't stand soldering, anyway, so I have no 
// *          desire to get money for something I dont want, don't want to spend, and 
// *          can't even afford, even if I did have the money for it. 
// *      - Done away with the AuxLightControlModule and a large chunk of code in the 
// *          main program.  The removed code was replaced by a new routine called 
// *          "DoorMonitorAndAnimationControlModule".  Maybe now I am finally rid of the 
// *          500ms bug.
// *      - Rewrote and cleaned up most, if not all of the animations.  It still looks 
// *          like a mess to me.  
// *      - Added an Individual Strip system to manage the varibles in a better way, also 
// *          added a trailing manual varible, just to contain the main theme of the 
// *          strips animation, to be referenced for status. 
// *      - I really wish the animations were more channel independant.
// *      - Sketch uses 18944 bytes (61%) of program storage space. Maximum is 30720 
// *          bytes.
// *          Low memory available, stability problems may occur.
// *          Global variables use 1777 bytes (86%) of dynamic memory, leaving 271 bytes 
// *          for local variables. Maximum is 2048 bytes.
// *
// ***************************************************************************************

#include <FastLED.h>
//#include <Wire.h>

// -------------------------------------------------------------------------------------
// HARDWARE SETUP

// Testing and Debugging
#define BOOTEST       false
#define RESTTIME      8      // Ok, 125 FPS, max and estimated.      
#define TESTRESTTIME  16
// LED Strip

#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
//#define CLK_PIN     5       // If your LED_TYPE requires one.

// LED data pins to transmit on off and colors for strip s0 and s1
#define DATA_PINs0      3       // 3 // A3 - Data Pin for Strip 1
#define DATA_PINs1      4       // 4 // A4 - Data Pin for Strip 2


// Amount of LEDs per strip s0 and s1.


#define NUM_LEDSs0    123     
#define s0As          0       // s0 A Start
#define s0Ae          69      // s0 A End
#define s0Bs          70      // s0 B Start
#define s0Be          121     // s0 B End


#define NUM_LEDSs1    118 
#define s1As          0       // s1 A Start
#define s1Ae          65      // s1 A End
#define s1Bs          66      // s1 B Start
#define s1Be          117     // s1 B End

/*
// *** TEST BLOCK FOR DOOR AND OVERHEAD LIGHTS ***
#define NUM_LEDSs0    123     
#define s0As          0       // s0 A Start
#define s0Ae          29      // s0 A End
#define s0Bs          30      // s0 B Start
#define s0Be          60     // s0 B End


#define NUM_LEDSs1    118 
#define s1As          0       // s1 A Start
#define s1Ae          29      // s1 A End
#define s1Bs          30      // s1 B Start
#define s1Be          60     // s1 B End
*/


// On off buttons, door sensors, switches.
#define SWITCH_PINs0    8       // 8 // A8 - Hardware Open Close Door Sensor 1
#define SWITCH_PINs1    9       // 9 // A9 - Hardware Open Close Door Sensor 2
#define SWITCH_PINs2    10      // 9 // A9 - Hardware Open Close Door Sensor 3
#define SWITCH_PINs3    11      // 9 // A9 - Hardware Open Close Door Sensor 3
#define AUXDRLINGERFRT  15000    // How long the Front Door lights stay on after close
#define AUXDRLINGERBCK  25000    // How long the Back Door lights stay on after close

// -------------------------------------------------------------------------------------
// SOFTWARE MANAGEMENT
#define NUM_CHANNELS        2   // Amount of LED strips we will be controlling.
#define NUM_TIMED_EVENTS  15  // Untill I can remember how LL, this is being
//  Also, total number of this will be multiplied by the
//  amount of LED strips you have setup.  Watch your memory.
#define NUM_SWITCHES      4   // 4

#define BRIGHTNESS        96  // Using Example Code.  Max unknown
#define FRAMES_PER_SECOND 120 // Will not be necessary, but keeping, for now, just in 
//  case.

// -------------------------------------------------------------------------------------
// COMMUNICATIONS
/*
  // Handled by Wire library I2C/TWI.  Pins are hardwired and only defined here for
  //  reference.
  #define COMMDATA      D4      // D4 - Data line.
  #define COMMCLK       D5      // D5 - Clock line.

  #define SLAVEID       5       // ID of this device on the bus.  Needs to change for
  //  every device.  A percistand unique ID would make
  //  this much easier during deployment.
  #define DATASIZE      1       // The size of the data packet.
*/

// -------------------------------------------------------------------------------------
// BASIC DEFINITIONS FOR EASY REFERENCE

// Door Switch Reference
#define Door_Back     0 // Back Door Switch
#define Door_Front    1 // Front Door Switch
#define Door_Aux      2 // Aux Door Switch 

// Strip Reference
#define Back_Door     0 // Back Door 
#define Back_Over     1 // Back Overhead 
#define Front_Door    2 // Front Door 
#define Front_Over    3 // Front Overhead 

// Event Animations
#define AnEvClear         0
#define AnEvClearRunning  8
#define AnEvSweep         1
#define AnEvSchedule      2
#define AnEvSetToEnd      3
// Pixel Animations
#define AnPiFade          4
#define AnPiFadeDith      5
#define AnPiPulse         6
#define AnPiPulseTo       7

// Animations (Global)
// Hardcoding this for now.
#define AnTaOff              0
#define AnTaPowerOn             1

#define AnTaDoorOpen            2

#define AnTaDoorOpen00          20
#define AnTaDoorOpen01          21
#define AnTaDoorOpen02          22

/*
#define AnTaDoorOpenFront       3
*/
#define AnTaStripOverOff        31
/*
#define AnTaDoorOpenFront02     32
*/

#define AnTaDoorClose           4
#define AnTaDoorClose00         40
#define AnTaDoorClose01         41

#define AnTaChannelPulseGreen   5

/*
#define AnTaDoorCloseFront01    51
#define AnTaDoorCloseFront02    52
*/

#define AnTavdPacificaish       6
//#define AnTavdPacificaishFront  7

#define AnTavdPaAnimClose        8
#define AnTavdPaAnimClose00      80
//#define AnTavdPaAnimFrontClose  9

// ***************************************************************************************
// STRUCTURES
// ***************************************************************************************

// LedStrip Structures 
// -------------------------------------------------------------------------------------
// Defines a basic LED Strip
struct led_strip
{
  int Cl = 0; // Channel
  int St = 0; // Start Led Pos
  int Ed = 0; // End Led Pos
  int AnimationStatus = 0;

  void set(int intChannel, int intStart, int intEnd)
  {
    Cl = intChannel;
    St = intStart;
    Ed = intEnd;
  }

  int Ct()
  {
    int count;
    return Ed - St + 1;
  }
};

//  TIMED EVENT
// -------------------------------------------------------------------------------------

struct bigCRGB
// Simple RGB varible for computation.
{
  int r = 0;
  int g = 0;
  int b = 0;
  boolean complete = true;
};

// -------------------------------------------------------------------------------------
struct timed_event_data
// Varibles to control the timed_event.
{
  unsigned long tmeSTARTTIME;
  unsigned int intDURATION;
  unsigned int intSPEED;
  byte bytANIMATION;
  byte bytLEDANIMATION;
  CRGB crgbCOLORSTART1;
  CRGB crgbCOLORSTART2;
  CRGB crgbCOLORDEST1;
  CRGB crgbCOLORDEST2;
  byte intSTARTPOS;
  byte intENDPOS;
  boolean booREPEAT;
  boolean booCLEARONEND = true;

  boolean booCOMPLETE = true;


  void PostCheck(unsigned long tmeCurrent)
  // Check to see if the event is over, unlock and release Start Colors.
  {
    //  (num of led * speed + duration)
    // Grabbing actual current time to process decayed events, because alot could've happened
    // since then.

    if (booCOMPLETE == true)
      //if (millis() >= tmeSTARTTIME + intDURATION +
      //    ((abs(intENDPOS - intSTARTPOS) + 1) * intSPEED))
    {
      // The event repeats, collision is irrelevant.
      if (booREPEAT == true)
      {
        // Keep the event active by resetting its start time and setting it
        //  to be incomplete.
        booCOMPLETE = false;
        tmeSTARTTIME = tmeCurrent;  // Using current time here because we want
        //  to keep the frames the same.
      }
    }
  }
};

// -------------------------------------------------------------------------------------
struct timed_event
//  Create a Timed LED Event:
//    Controls when a group of LEDs are to begin their individual light path animation.
//    If an event is over written the previous animation will be lost.
{
  timed_event_data teDATA[NUM_TIMED_EVENTS];
  unsigned int      intLEDCOUNT;

  void create(unsigned intLedCount)
  // On startup, define the amount of LEDs in this event list.
  {
    intLEDCOUNT = intLedCount;
  }

  void set(unsigned long tmeCurrentTime,
           unsigned long tmeStartInTime, unsigned long intDuration, 
           unsigned int intSpeed, byte bytAnimation, byte bytLEDAnimation, 
           CRGB crgbStart1, CRGB crgbDest1, CRGB crgbStart2, CRGB crgbDest2, 
           int intStartPos, int intEndPos, boolean booRepeat, boolean booClearOnEnd)
  // Prepare an animation to start at a specific time.
  {

    boolean booCreated = false;

    for (int x = 0; ((x < NUM_TIMED_EVENTS) && (booCreated == false)) ; x++)
    {
      if (teDATA[x].booCOMPLETE == true)
      {
        unsigned long tmeStartTime = tmeCurrentTime + tmeStartInTime;

        teDATA[x].tmeSTARTTIME = tmeStartTime;
        teDATA[x].intDURATION = intDuration;
        teDATA[x].intSPEED = intSpeed;
        teDATA[x].bytANIMATION = bytAnimation;
        teDATA[x].bytLEDANIMATION = bytLEDAnimation;

        teDATA[x].crgbCOLORSTART1.r = crgbStart1.r;
        teDATA[x].crgbCOLORSTART1.g = crgbStart1.g;
        teDATA[x].crgbCOLORSTART1.b = crgbStart1.b;

        teDATA[x].crgbCOLORSTART2.r = crgbStart2.r;
        teDATA[x].crgbCOLORSTART2.g = crgbStart2.g;
        teDATA[x].crgbCOLORSTART2.b = crgbStart2.b;

        teDATA[x].crgbCOLORDEST1.r = crgbDest1.r;
        teDATA[x].crgbCOLORDEST1.g = crgbDest1.g;
        teDATA[x].crgbCOLORDEST1.b = crgbDest1.b;

        teDATA[x].crgbCOLORDEST2.r = crgbDest2.r;
        teDATA[x].crgbCOLORDEST2.g = crgbDest2.g;
        teDATA[x].crgbCOLORDEST2.b = crgbDest2.b;

        teDATA[x].intSTARTPOS = intStartPos;
        teDATA[x].intENDPOS = intEndPos;
        teDATA[x].booREPEAT = booRepeat;
        teDATA[x].booCLEARONEND = booClearOnEnd;

        teDATA[x].booCOMPLETE = false;

        booCreated = true;
      }
    }
  }

  void ClearAll(int intStartPos, int intEndPos)
  //Remove all timed event animations on a specific animation strip.
  {
    for (int x = 0; x < NUM_TIMED_EVENTS; x++)

    if (  ((teDATA[x].intSTARTPOS >= intStartPos)  
          && (teDATA[x].intSTARTPOS <= intEndPos))
                ||
          ((teDATA[x].intENDPOS >= intStartPos)  
          && (teDATA[x].intENDPOS <= intEndPos))  )
    {

      {
        teDATA[x].booCOMPLETE = true;
        //teDATA[x].booSETCOMPLETE = false;
      }
    }
  }


  // -------------------------------------------------------------------------------------
  // Timed Event LED Animations

  bigCRGB crgb_anim_color(int intDur, byte bytLedAnimation,
                          CRGB crgbCDest1, CRGB crgbCDest2,
                          unsigned long tmeCurrentTime, unsigned long tmeAnimTime, 
                          boolean booClearOnEnd)
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
      case AnPiFade:
        // Fade Animation Path
        {
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            tmpColor.r = crgbCDest2.r;
            tmpColor.g = crgbCDest2.g;
            tmpColor.b = crgbCDest2.b;
            
            tmpColor.complete = booClearOnEnd;

            return tmpColor;
            break;
          }
          else
          {
            float fltPower = (float)tmeElapsed / (float)intDur;

            tmpColor.r = (fltPower * crgbCDest2.r) + ((1 - fltPower) * crgbCDest1.r);
            tmpColor.g = (fltPower * crgbCDest2.g) + ((1 - fltPower) * crgbCDest1.g);
            tmpColor.b = (fltPower * crgbCDest2.b) + ((1 - fltPower) * crgbCDest1.b);
            
            tmpColor.complete = false;

            return tmpColor;
            break;
          }
        } // END CASE AnPiFade
              
        case AnPiFadeDith:
        // Fade Animation Path (Dither, this is duplicate code!)
        {
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            tmpColor.r = crgbCDest2.r;
            tmpColor.g = crgbCDest2.g;
            tmpColor.b = crgbCDest2.b;
            
            tmpColor.complete = booClearOnEnd;

            return tmpColor;
            break;
          }
          else
          {
            float fltPower = (float)tmeElapsed / (float)intDur;

            tmpColor.r = (fltPower * crgbCDest2.r) + ((1 - fltPower) * crgbCDest1.r);
            tmpColor.g = (fltPower * crgbCDest2.g) + ((1 - fltPower) * crgbCDest1.g);
            tmpColor.b = (fltPower * crgbCDest2.b) + ((1 - fltPower) * crgbCDest1.b);
            
            tmpColor.complete = false;

            return tmpColor;
            break;
          }
        } // END CASE AnPiFadeDith

      case AnPiPulse:
        // Pulse Animation Path
        // This can be done beter with a simple ABS function.
        {
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            tmpColor.complete = true;
            
            return tmpColor;
            break;
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
            tmpColor.r = (fltPower * crgbCDest1.r) + ((1 - fltPower) * tmpColor.r);
            tmpColor.g = (fltPower * crgbCDest1.g) + ((1 - fltPower) * tmpColor.g);
            tmpColor.b = (fltPower * crgbCDest1.b) + ((1 - fltPower) * tmpColor.b);
            
            tmpColor.complete = false;

            return tmpColor;
            break;
          }
        } // END CASE AnPiPulse

      case AnPiPulseTo:
        // PulseTo Animation Path
        // This can be done beter with a simple ABS function.
        {
          if (tmeCurrentTime >= tmeAnimTime + intDur)
          {
            tmpColor.r = crgbCDest2.r;
            tmpColor.g = crgbCDest2.g;
            tmpColor.b = crgbCDest2.b;
            
            tmpColor.complete = booClearOnEnd;

            return tmpColor;
            break;
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

              bigcrgbB.r = crgbCDest1.r;
              bigcrgbB.g = crgbCDest1.g;
              bigcrgbB.b = crgbCDest1.b;
            }
            else
            {
              fltPower = 1 - (((float)((tmeElapsed * 2) - intDur) / (float)intDur));
              bigcrgbA.r = crgbCDest2.r;
              bigcrgbA.g = crgbCDest2.g;
              bigcrgbA.b = crgbCDest2.b;

              bigcrgbB.r = crgbCDest1.r;
              bigcrgbB.g = crgbCDest1.g;
              bigcrgbB.b = crgbCDest1.b;
            }

            tmpColor.r = (fltPower * bigcrgbB.r) + ((1 - fltPower) * bigcrgbA.r);
            tmpColor.g = (fltPower * bigcrgbB.g) + ((1 - fltPower) * bigcrgbA.g);
            tmpColor.b = (fltPower * bigcrgbB.b) + ((1 - fltPower) * bigcrgbA.b);
            
            tmpColor.complete = false;

            return tmpColor;
            break;
          }
        } // END CASE AnPiPulseTo
    } // END SWITCH STATEMENT
  }

  // -------------------------------------------------------------------------------------
  boolean execute(CRGB hwLEDArray[], unsigned long tmeCurrentTime)
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
    boolean booPixelColorChanged;
    CRGB crgbLED;
    int activeeventcount;
    //boolean booEventColision[NUM_TIMED_EVENTS];
    boolean booEventActive[NUM_TIMED_EVENTS];
    boolean booEventComplete[NUM_TIMED_EVENTS];
    bigCRGB bigcrgbNewColor;
    bigCRGB tempColor;
    CRGB crgbTempStart;
    CRGB crgbTempDest;

    boolean booChanged = false;
    unsigned long tmeStartAnim;

    // Process Event Animations  
    // Prepare Arrays
    for (int e = 0; e < NUM_TIMED_EVENTS; e++) //Duplicate
    {
      // Prepare Temporary Complete Array With Events That May Run
      if (tmeCurrentTime >= teDATA[e].tmeSTARTTIME  && teDATA[e].booCOMPLETE == false)
      {
        booEventComplete[e] = true;
      }

      // Clear Active Event Array
      booEventActive[e] = false;
    }

    // Process LEDs, one by one.
    for (int led = 0; led < intLEDCOUNT; led++)
    {
      //  Attempt to restrict the amount of times the activity tracker routine is ran.
      activeeventcount = 0;

      // Clear the value of the new LED color.  Should only update when it has changed.
      bigcrgbNewColor.r = 0;
      bigcrgbNewColor.g = 0;
      bigcrgbNewColor.b = 0;

      booPixelColorChanged = false;

      // Process each event, one by one.
      for (int event = 0; event < NUM_TIMED_EVENTS; event++)
      {
        // Clear the tmp colors, in case they have data in them at the start of each 
        //  event process.
        tempColor.r = 0;
        tempColor.g = 0;
        tempColor.b = 0;

        // Only continue processing the event if the event hasnt been completed.
        if (teDATA[event].booCOMPLETE == false)
        {
          // Is the LED we are processing within the event?
          if (((led >= teDATA[event].intSTARTPOS) && (led <= teDATA[event].intENDPOS))
              || ((led <= teDATA[event].intSTARTPOS) && (led >= teDATA[event].intENDPOS)))
          {
            // OK, so an event is schedule, but is it ready to start?

            if (tmeCurrentTime >= teDATA[event].tmeSTARTTIME)
            {
              // Collision Tracker
              activeeventcount++;   //  Attempt to restrict the amount of times this 
                                    //    routine is ran.
              booEventActive[event] = true;

              // Grab Event Data that may change or be processed within this switch 
              //  statement. We will be calculating the change of the pixel color, not 
              //  the actual color.
              crgbTempStart.r = teDATA[event].crgbCOLORSTART1.r;
              crgbTempStart.g = teDATA[event].crgbCOLORSTART1.g;
              crgbTempStart.b = teDATA[event].crgbCOLORSTART1.b;

              crgbTempDest.r = teDATA[event].crgbCOLORDEST1.r;
              crgbTempDest.g = teDATA[event].crgbCOLORDEST1.g;
              crgbTempDest.b = teDATA[event].crgbCOLORDEST1.b;


              // Figure out when the LED is suposed to start doing something.
              tmeStartAnim = teDATA[event].tmeSTARTTIME
                             + (abs((led - teDATA[event].intSTARTPOS))
                                * teDATA[event].intSPEED);

              // The Pixel on this Event is ready to change.
              if ((tmeCurrentTime >= tmeStartAnim))
              {
                // Preprocess Dest 2 for dynamic DEST2 on Pixel Fade.
                if (teDATA[event].bytLEDANIMATION == AnPiFadeDith)
                {

                  // Get value of light based on animation at current time.
                  float fltPower;
                  if ((float)(abs(teDATA[event].intENDPOS - teDATA[event].intSTARTPOS)) 
                        == 0)
                  {
                    fltPower = 0;
                  }
                  else
                  {
                    fltPower = (float)abs(led - teDATA[event].intSTARTPOS)
                               / (float)(abs(teDATA[event].intENDPOS 
                               - teDATA[event].intSTARTPOS));
                  }

                  crgbTempStart.r = (fltPower * teDATA[event].crgbCOLORSTART2.r)
                                       + ((1 - fltPower) * teDATA[event].crgbCOLORSTART1.r);

                  crgbTempStart.g = (fltPower * teDATA[event].crgbCOLORSTART2.g)
                                       + ((1 - fltPower) * teDATA[event].crgbCOLORSTART1.g);

                  crgbTempStart.b = (fltPower * teDATA[event].crgbCOLORSTART2.b)
                                       + ((1 - fltPower) * teDATA[event].crgbCOLORSTART1.b);

                  // ---
                  crgbTempDest.r = (fltPower * teDATA[event].crgbCOLORDEST2.r)
                                       + ((1 - fltPower) * teDATA[event].crgbCOLORDEST1.r);

                  crgbTempDest.g = (fltPower * teDATA[event].crgbCOLORDEST2.g)
                                       + ((1 - fltPower) * teDATA[event].crgbCOLORDEST1.g);

                  crgbTempDest.b = (fltPower * teDATA[event].crgbCOLORDEST2.b)
                                       + ((1 - fltPower) * teDATA[event].crgbCOLORDEST1.b);
                }
                
                switch (teDATA[event].bytANIMATION)
                {
                  case AnEvSweep:
                    {
                      // Calculate how much this Event will chaange the pixel.
                      tempColor = crgb_anim_color(teDATA[event].intDURATION,
                                                  teDATA[event].bytLEDANIMATION,
                                                  crgbTempStart, crgbTempDest, 
                                                  tmeCurrentTime, tmeStartAnim, 
                                                  teDATA[event].booCLEARONEND);

                      //  Update the events completeness if its still active.
                      if (tempColor.complete == false)
                      {
                        booEventComplete[event] = false;
                      }

                      bigcrgbNewColor.r = bigcrgbNewColor.r + tempColor.r;
                      bigcrgbNewColor.g = bigcrgbNewColor.g + tempColor.g;
                      bigcrgbNewColor.b = bigcrgbNewColor.b + tempColor.b;

                      booPixelColorChanged = true;
                      break;
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
    for (int e = 0; e < NUM_TIMED_EVENTS; e++)
    {
      if (booEventActive[e] == true && booEventComplete[e] == true)
      {
        teDATA[e].booCOMPLETE = true;
        //teDATA[e].PostCheck(tmeCurrentTime, booEventColision[e]);
        teDATA[e].PostCheck(tmeCurrentTime);
      }
    }

  return booChanged;
  
  }
};


// ---------------------------------------------------------------------------------------
// HARDWARE MONITOR

struct hardware_monitor
// Create a hardware switch or button varable type.
{
  // External
  boolean booVALUE;
  unsigned long tmeTOGGLEDTIME;
  boolean ISHARDWARE = false;

  // Internal
  unsigned long tmeCHANGEDETECTEDTIME;
  boolean booPREVCHANGEDETECTED;
  unsigned int tmeLEEWAY;
  boolean booFIRSTRUN = true;

  void set(boolean booValue, int tmeLeeWay, boolean isHardware)
  // Prepare the switch.
  {
    tmeCHANGEDETECTEDTIME = millis();
    tmeLEEWAY = tmeLeeWay;
    booVALUE = booValue;
    booPREVCHANGEDETECTED = false;
    ISHARDWARE = isHardware;
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

      if (BOOTEST == false)
      {
        return booValue;  // Comment this line out when testing
        //return false;   // Comment this line out when not testing
      }
      else
      {
        //return booValue;// Comment this line out when testing
        return false;     // Comment this line out when not testing
      }
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
      tmeTOGGLEDTIME = tmeTme;
      return true;
    }
  }
};

int intRandomHD(int intBase)
// Generate a random number between half and double of the base
{
  //if (booRandom == false) {booRandom = booRandomSet(millis());}
  return (random((intBase / 2),(intBase * 2)));
}


// ***************************************************************************************
// FUNCTION AND PROCEDURES
// ***************************************************************************************
//
void vdClearAllTimedEvent(timed_event teEvent[], int intPos, int intStartPos, int intEndPos)
//  Sort through the timed event list a create an event at the first slot available.
{
  teEvent[intPos].ClearAll(intStartPos, intEndPos);
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
//int intEndPos,
//boolean booRepeat,
//boolean booClearOnEnd,

// -------------------------------------------------------------------------------------
int intAnTmDly(int intTm, int intDur, int intCt, int intSp)
// Calculate and return the next delay time based on duration of event, number of pixels,
//  and animation speed of each pixel.
//  Value in return statement is buffer time.
{
  return (1 + (RESTTIME) + intTm + intDur + (intSp * intCt));
}

/*
// -------------------------------------------------------------------------------------
void vdTESTFLASHAnimation(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;

  intDur = 500; intSp = 50; intCt = lsSt.Ct();
  teEvent[lsSt.Cl].set(tmeCurrentTime, 50, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(20, 20, 20), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, true);
}
*/

// -------------------------------------------------------------------------------------
void vdPowerOnAnimation(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Just to show the lights work when the program starts.
{
  // Update Animation Status 
  lsStrips[Back_Door].AnimationStatus = AnTaPowerOn;
  lsStrips[Back_Over].AnimationStatus = AnTaPowerOn;
  lsStrips[Front_Door].AnimationStatus = AnTaPowerOn;
  lsStrips[Front_Over].AnimationStatus = AnTaPowerOn;

  int intTm;
  int intDur;
  int intCt;
  int intSp;

  intDur = 250; intSp = 5; intCt = 118;
  // Pulse Front Door
  teEvent[1].set(tmeCurrentTime, 50, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(125, 125, 125), CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[Front_Over].Ed, lsStrips[Front_Door].St, false, true);
  // Pulse Back Door
  teEvent[0].set(tmeCurrentTime, 640, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(125, 125, 125), CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[Back_Over].Ed, lsStrips[Back_Door].St, false, true);


  intDur = 2000; intSp = 15; intCt = 71;
  // Clear Front Door
  teEvent[1].set(tmeCurrentTime, 890, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[Front_Over].Ed, lsStrips[Front_Door].St, false, true);
  // Clear Back Door
  teEvent[0].set(tmeCurrentTime, 1480, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[Back_Over].Ed, lsStrips[Back_Door].St, false, true);
}


// -------------------------------------------------------------------------------------
void vdSimplePulse(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Just to show the lights work when the program starts.        
// vdSimplePulse(teEvent, 1, tmeCurrentTime);
{
  int intTm;
  int intDur;
  int intSp;

  intDur = 250; intSp = 5;
  // Pulse Front Door
  teEvent[1].set(tmeCurrentTime, 50, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(125, 125, 125), CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[Front_Over].Ed, lsStrips[Front_Door].St, false, true);
  // Pulse Back Door
  teEvent[0].set(tmeCurrentTime, 50, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(125, 125, 125), CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[Back_Over].Ed, lsStrips[Back_Door].St, false, true);
}


/*
// -------------------------------------------------------------------------------------
void vdAlertAnimation(timed_event teEvent, unsigned long tmeCurrentTime)
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;
}
*/

void vdStripOverOff(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Turn (force) Lights Off ona Strip
{
  int intTm;
  int intDurW;
  int intDurG;
  int intSp;

  int intDelay;

    // Swap sweep start and end, depending on front or back.
  int start;
  int end;
  if (lsSt.Cl == 0)
  {
    start = lsSt.St;
    end = lsStrips[lsSt.Cl +1 ].Ed;
  }
  else
  {
    start = lsSt.Ed;
    end = lsStrips[lsSt.Cl +1 ].St;
  }

  // Clear and Pulse colors background to green then ending in blueish, starting with the center.
  intTm = 50; intDurW = 200; intDurG = 1500; intSp = 5;
  intDelay = intAnTmDly(intTm, 0, lsSt.Ct(), intSp);

  // Door White
  //teEvent[lsSt.Cl].set(tmeCurrentTime, intTm + 50, intDurW, intSp, AnEvSweep, AnPiPulse, CRGB(64, 128, 64), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl].St, lsStrips[lsSt.Cl].Ed, false, true);
  // Overhead White
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm + intDelay + 50, intDurW, intSp, AnEvSweep, AnPiPulse, CRGB(64, 128, 64), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0),  start, end, false, true);
 
  // Door Green Fadeout
  //teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDurG, intSp, AnEvSweep, AnPiPulse, CRGB(0, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl].St, lsStrips[lsSt.Cl].Ed, false, true);
  // Overhead Green Fadeout
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm + intDelay, intDurG, intSp, AnEvSweep, AnPiPulse, CRGB(0, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0),  start, end, false, true);
}


void vdChannelLightPulseGreen(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Turn (force) Green Pulse on Full Channel. Strip Length Aware. 
{
  int intTm;
  int intDurW;
  int intDurG;
  int intCt;
  int intSp;

  int intDelay;

    // Swap sweep start and end, depending on front or back.
  int start;
  int end;
  if (lsSt.Cl == 0)
  {
    start = lsSt.St;
    end = lsStrips[lsSt.Cl +1 ].Ed;
  }
  else
  {
    start = lsSt.Ed;
    end = lsStrips[lsSt.Cl +1 ].St;
  }

  // Clear and Pulse colors background to green then ending in blueish, starting with the center.
  intTm = 50; intDurW = 200; intDurG = 1500; intSp = 5; intCt = 36;
  intDelay = intAnTmDly(intTm, 0, lsSt.Ct(), intSp);

  // Door White
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm + 50, intDurW, intSp, AnEvSweep, AnPiPulse, CRGB(64, 128, 64), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl].St, lsStrips[lsSt.Cl].Ed, false, true);
  // Overhead White
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm + intDelay + 50, intDurW, intSp, AnEvSweep, AnPiPulse, CRGB(64, 128, 64), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), start, end, false, true);
 
  // Door Green Fadeout
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDurG, intSp, AnEvSweep, AnPiPulse, CRGB(0, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl].St, lsStrips[lsSt.Cl].Ed, false, true);
  // Overhead Green Fadeout
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm + intDelay, intDurG, intSp, AnEvSweep, AnPiPulse, CRGB(0, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), start, end, false, true);
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

void vdDoorOpenAnimation(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Door Open 
// Door is open, engage safety lights and start door overhead lights.
{
  // Update Animation Status 
  lsStrips[lsSt.Cl].AnimationStatus = AnTaDoorOpen;
  lsStrips[lsSt.Cl +1 ].AnimationStatus = AnTaDoorOpen;

  //Door Animation
  // Clear Events On Door
  vdClearAllTimedEvent(teEvent, lsSt.Cl, lsStrips[lsSt.Cl].St, lsStrips[lsSt.Cl].Ed);
  teEvent[lsSt.Cl].set(tmeCurrentTime, 10, 0, 0, AnEvSchedule, AnTaDoorOpen00, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl].St, lsStrips[lsSt.Cl].Ed, false, true);

  // Clear Events for the Overhead Animation
  vdClearAllTimedEvent(teEvent, lsSt.Cl, lsStrips[lsSt.Cl + 1 ].St, lsStrips[lsSt.Cl +1 ].Ed);
  teEvent[lsSt.Cl].set(tmeCurrentTime, 10, 0, 0, AnEvSchedule, AnTavdPacificaish, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl +1 ].St, lsStrips[lsSt.Cl +1 ].Ed, false, true);
}

void vdDoorOpenAnimation00(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Door Open Stage 0
// Prepare red backgrounds and puddle lights for the caution lights, and start shimmer effect.
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;
  // Door Open Animation
  intTm = 100; intDur = 500; intSp = 10; intCt = lsSt.Ct(); // was 60

  // Clear set background to door open colors.
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFadeDith, CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(255, 64, 64), lsSt.St, lsSt.St + 4, false, false);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFadeDith, CRGB(0, 0, 0), CRGB(255, 64, 64), CRGB(0, 0, 0), CRGB(25, 0, 0), lsSt.St + 5, lsSt.St + 10, false, false);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(25, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St + 11, lsSt.Ed - 6, false, false);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm + 500, intDur, intSp, AnEvSweep, AnPiFadeDith, CRGB(0, 0, 0), CRGB(25, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.Ed - 5, lsSt.Ed, false, false);

  // Shimmer
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, 10000, 500, AnEvSweep, AnPiPulse, CRGB(15, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, false);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, 6000, 280, AnEvSweep, AnPiPulse, CRGB(15, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, false);

  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, 0, 0, AnEvSchedule, AnTaDoorOpen01, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, false, true);
}

void vdDoorOpenAnimation01(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Door Open Stage 1
{
  int intTm = 50;
  int intDur;
  int intCt;
  int intSp;

  // Caution Door Open Flash
  intDur = 100; intSp = 2; intCt = lsSt.Ct();
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, false, true);
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 600; intSp = 12; intCt = lsSt.Ct();
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, false, true);
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 1500; intSp = 30; intCt = lsSt.Ct();
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(128, 128, 0), CRGB(128, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, false, true);

  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, 0, 0, AnEvSchedule, AnTaDoorOpen02, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, false, true);
}

void vdDoorOpenAnimation02(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Door Open Stage 2
{
  int intTm = 0;
  int intDur;
  int intCt;
  int intSp;
  int intMid = lsSt.Ct() / 2;

  // Repeat Pulse
  intDur = 1500; intSp = 125; intCt = lsSt.Ct(); // was 36
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.St + intMid, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.Ed, lsSt.Ed - intMid, true, true);
}


// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

void vdDoorCloseAnimation(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Door Close Stage 0
// Door is closed, disengage safety lights and stop door overhead lights.
{
  // Update Animation Status 
  lsStrips[lsSt.Cl].AnimationStatus = AnTaDoorClose;
  lsStrips[lsSt.Cl +1 ].AnimationStatus = AnTaDoorClose;

  int intTm;
  int intDur;
  int intSp;

  //  Clear Currently Running Animations First
  //  Door
  vdClearAllTimedEvent(teEvent, lsSt.Cl, lsStrips[lsSt.Cl].St, lsStrips[lsSt.Cl].Ed);
  //  Overhead
  vdClearAllTimedEvent(teEvent, lsSt.Cl, lsStrips[lsSt.Cl +1 ].St, lsStrips[lsSt.Cl  +1 ].Ed);

  // Pulse Clear All Animations
  teEvent[lsSt.Cl].set(tmeCurrentTime, 100, 0, 0, AnEvSchedule, AnTaChannelPulseGreen, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl].St, lsStrips[lsSt.Cl].Ed, false, true);
  
  //Restore the privouse Blue to fade out
  intTm = 200; intDur = 6000; intSp = 5;
  teEvent[lsSt.Cl].set(tmeCurrentTime, intTm , intDur, intSp, AnEvSweep, AnPiPulse, CRGB(0, 0, 60), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl +1 ].Ed, lsStrips[lsSt.Cl +1 ].St, false, true);

  // Door Animation
  //  No Need While Green Whipe Is Clearing

  // Overhead Animation
  teEvent[lsSt.Cl].set(tmeCurrentTime, 1500, 0, 0, AnEvSchedule, AnTavdPaAnimClose, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl +1 ].St, lsStrips[lsSt.Cl +1 ].Ed, false, true);
}

void vdDoorCloseAnimation00(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Door closed.  Quick animation to show and turn off lights.
{
  // Fade remaining colors out.

  /*  // Not Necesary because clearing door with Green Whipe
  intTm = intAnTmDly(intTm, intDur, intCt, intSp) + 300;
  intDur = 5000; intSp = 100; intCt = 36;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, false, true);
  */
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

void vdPacificaishAnimation(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Blue Waves
{
  // Swap sweep start and end, depending on front or back.
  int start;
  int end;
  if (lsSt.Cl == 0)
  {
    start = lsSt.St;
    end = lsSt.Ed;
  }
  else
  {
    start = lsSt.Ed;
    end = lsSt.St;
  }
  
  // Set the background color.
  teEvent[lsSt.Cl].set(tmeCurrentTime, 1000, 500, 30, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 60), CRGB(0, 0, 0), CRGB(0, 0, 0), start, end, false, false);

  // The waves.
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3500), intRandomHD(250), AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(1500), intRandomHD(150), AnEvSweep, AnPiPulse, CRGB(160, 200, 40), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3600), intRandomHD(270), AnEvSweep, AnPiPulse, CRGB(20, 200, 180), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3200), intRandomHD(200), AnEvSweep, AnPiPulse, CRGB(60, 200, 140), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, true);
}

// -------------------------------------------------------------------------------------

void vdPacificaishAnimationClose(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
{
  // Stop the currently running Pacificaish animation.
  // Schedule clear animation events ahead of time in case animations don't get completed.
  teEvent[lsSt.Cl].set(tmeCurrentTime, AUXDRLINGERBCK + 2000, 0, 0, AnEvClearRunning, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, false, false);
  teEvent[lsSt.Cl].set(tmeCurrentTime, AUXDRLINGERBCK + 2100, 3000, 30, AnEvSweep, AnPiFade, CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, false, false);

  teEvent[lsSt.Cl].set(tmeCurrentTime, 50, 0, 0, AnEvSchedule, AnTavdPaAnimClose00, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[lsSt.Cl].St, lsStrips[lsSt.Cl].Ed, false, true);
}

void vdPacificaishAnimationClose00(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// Amber Waves
{
  // Swap sweep start and end, depending on front or back.
  int start;
  int end;
  if (lsSt.Cl == 0)
  {
    start = lsSt.St;
    end = lsSt.Ed;
  }
  else
  {
    start = lsSt.Ed;
    end = lsSt.St;
  }
  // Set the background color to Amber.
  teEvent[lsSt.Cl].set(tmeCurrentTime, 50 + 2000, 3000, 30, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), start, end, false, false);

  // Amber waves.
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000) + 2000, intRandomHD(3500), intRandomHD(250), AnEvSweep, AnPiPulse, CRGB(20, 4, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000) + 2000, intRandomHD(1500), intRandomHD(150), AnEvSweep, AnPiPulse, CRGB(16, 20, 0), CRGB(16, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000) + 2000, intRandomHD(3600), intRandomHD(270), AnEvSweep, AnPiPulse, CRGB(15, 6, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000) + 2000, intRandomHD(3200), intRandomHD(200), AnEvSweep, AnPiPulse, CRGB(25, 2, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsSt.St, lsSt.Ed, true, true);
}


// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

/*
void vdPacificaishAnimationOPENTest(led_strip lsStrips[], led_strip lsSt, timed_event teEvent[], unsigned long tmeCurrentTime)
// This animation was my personal chalenge to see if I could make a similar animation that was
// provided in the demos and examples, as PacificaAnimation, of the FastLed library.
{
  // Set the background color.
  teEvent[lsSt.Cl].set(tmeCurrentTime, 1000, 500, 30, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 60), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Ae, s0As, false, false);

  // The waves.

  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3500), intRandomHD(250), AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(1500), intRandomHD(150), AnEvSweep, AnPiPulse, CRGB(160, 200, 40), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3600), intRandomHD(270), AnEvSweep, AnPiPulse, CRGB(20, 200, 180), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
  teEvent[lsSt.Cl].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3200), intRandomHD(200), AnEvSweep, AnPiPulse, CRGB(60, 200, 140), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
}
*/

/*
void vdPacificaishAnimationCLOSETest(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// This animation was my personal chalenge to see if I could make a similar animation that was
// provided in the demos and examples, as PacificaAnimation, of the FastLed library.
{
  // Set the background color.
  teEvent[intPos].set(tmeCurrentTime, 50, 3000, 30, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, false, false);

  // The waves.

  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3500), intRandomHD(250), AnEvSweep, AnPiPulse, CRGB(20, 4, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(1500), intRandomHD(150), AnEvSweep, AnPiPulse, CRGB(16, 20, 0), CRGB(16, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3600), intRandomHD(270), AnEvSweep, AnPiPulse, CRGB(15, 6, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3200), intRandomHD(200), AnEvSweep, AnPiPulse, CRGB(25, 2, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Be, true, true);
}
*/

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
void teSystem(led_strip lsStripList[], timed_event teEvent[], unsigned long tmeCurrentTime)
// Example:
//  teEvent[0].set(tmeCurrentMillis, X, 0, 0, AnEvSchedule, AnTaDoorCloseBack, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), X, X, false, false);
//    or      ( X is the values that can be set )
//  teEvent[0].set(tmeCurrentMillis, X, X, 0, AnEvSchedule, AnEvSetToEnd, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(X, X, X), CRGB(X, X, X), 0, 0, false, false);
//                  if booRepeat set to true then pulse animations will continue till end of cycle.
//    or 
//  teEvent[0].set(tmeCurrentMillis, X, 0, 0, AnEvClear, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), X, X, false, false);

// Guide: (tmeCurrentTime, tmeStartInTime, intDuration, intSpeed, bytAnimation, bytLEDAnimation, crgbStart1, crgbDest1, crgbStart2, crgbDest2, intStartPos, intEndPos, booRepeat, booClearOnEnd)
{
  for (int strip = 0; strip < NUM_CHANNELS; strip++)
  {
    for (int event = 0; event < NUM_TIMED_EVENTS; event++)
    {
      if (tmeCurrentTime >= teEvent[strip].teDATA[event].tmeSTARTTIME && teEvent[strip].teDATA[event].booCOMPLETE == false)
      {
        // Process Timed System Events (keeping the LED type event scheduler format for 
        //  now)
        switch (teEvent[strip].teDATA[event].bytANIMATION)
        {
          case AnEvClear:   // Clear all events, whether running or not, if event is within Start and End Position.
          {
            teEvent[strip].teDATA[event].booCOMPLETE = true;
            teEvent[strip].ClearAll(teEvent[strip].teDATA[event].intSTARTPOS,teEvent[strip].teDATA[event].intENDPOS);
            break;
          }
          
          case AnEvClearRunning:  // Clear all active events if event is within Start and End Position.
          {                       // Possible problem if InTime is set to 0.
            teEvent[strip].teDATA[event].booCOMPLETE = true;

            for (int eventscan = 0; eventscan < NUM_TIMED_EVENTS; eventscan++)
            {
              if(tmeCurrentTime >= teEvent[strip].teDATA[eventscan].tmeSTARTTIME)
              {
                if (  ((teEvent[strip].teDATA[eventscan].intSTARTPOS >= teEvent[strip].teDATA[event].intSTARTPOS)  
                      && (teEvent[strip].teDATA[eventscan].intSTARTPOS <= teEvent[strip].teDATA[event].intENDPOS))
                            ||
                      ((teEvent[strip].teDATA[eventscan].intENDPOS >= teEvent[strip].teDATA[event].intSTARTPOS)  
                      && (teEvent[strip].teDATA[eventscan].intENDPOS <= teEvent[strip].teDATA[event].intENDPOS))  )
                {
                  if (event != eventscan)
                  {
                    teEvent[strip].teDATA[eventscan].booCOMPLETE = true;
                  }
                }
              }
            } 
            break;
          }

          case AnEvSchedule:
          //  Schedule an animation
          {  
            // Clear the Event whether the event ran or not.
            teEvent[strip].teDATA[event].booCOMPLETE = true;
            
            switch (teEvent[strip].teDATA[event].bytLEDANIMATION)
            // Activate an Animation Set
            {
              case AnTaPowerOn:
              {
                vdPowerOnAnimation(lsStripList, lsStripList[0], teEvent, tmeCurrentTime);
                break;
              }

              // Open Door
              case AnTaDoorOpen:
              {
                // Temp Hard Code Until Consolidated Animations. Delayed for now. More work than its worth.
                //led_strip Dr;
                //Dr.set(strip, teEvent[strip].teDATA[event].intSTARTPOS, teEvent[strip].teDATA[event].intENDPOS);

                vdDoorOpenAnimation(lsStripList, lsStripList[strip *2], teEvent, tmeCurrentTime);
                break;
              }
              case AnTaDoorOpen00:
              {
                vdDoorOpenAnimation00(lsStripList, lsStripList[strip *2], teEvent, tmeCurrentTime);
                break;
              }
              case AnTaDoorOpen01:
              {
                vdDoorOpenAnimation01(lsStripList, lsStripList[strip *2], teEvent, tmeCurrentTime);
                break;
              }
              case AnTaDoorOpen02:
              {
                vdDoorOpenAnimation02(lsStripList, lsStripList[strip *2], teEvent, tmeCurrentTime);
                break;
              }

              // Close Back Door
              case AnTaDoorClose:
              {
                vdDoorCloseAnimation(lsStripList, lsStripList[strip *2], teEvent, tmeCurrentTime);
                break;
              }
              case AnTaDoorClose00:
              {
                vdDoorCloseAnimation00(lsStripList, lsStripList[strip *2], teEvent, tmeCurrentTime);
                break;
              }

              case AnTaChannelPulseGreen:
              {
                vdChannelLightPulseGreen(lsStripList, lsStripList[strip *2], teEvent, tmeCurrentTime);
                break;
              }

              case AnTaStripOverOff:
              {
                vdStripOverOff(lsStripList, lsStripList[(strip *2) +1 ], teEvent, tmeCurrentTime);
                break;
              }

              case AnTavdPacificaish:
              {
                vdPacificaishAnimation(lsStripList, lsStripList[(strip *2) +1 ], teEvent, tmeCurrentTime);
                break;
              }

              case AnTavdPaAnimClose:
              {
                vdPacificaishAnimationClose(lsStripList, lsStripList[(strip *2) +1 ], teEvent, tmeCurrentTime);
                break;
              }

              case AnTavdPaAnimClose00:
              {
                vdPacificaishAnimationClose00(lsStripList, lsStripList[(strip *2) +1 ], teEvent, tmeCurrentTime);
                break;
              }

            } // End Case Animation Set
            break;
          } // End Case AnEvSchedule

          case AnEvSetToEnd:  // Schedules an animation to end. Fades out Fades and stops repeat on Pulses.
          {                   // Possible problem if InTime is set to 0.  
            // Clear the Event whether the event ran or not.
            teEvent[strip].teDATA[event].booCOMPLETE = true;   

            // Is the Event we are processing within the event?
            for (int eventscan = 0; eventscan < NUM_TIMED_EVENTS; eventscan++)
            {

              if(teEvent[strip].teDATA[eventscan].tmeSTARTTIME <= tmeCurrentTime)   // May Have Fixed It.  IDK
              {

                if (  ((teEvent[strip].teDATA[eventscan].intSTARTPOS >= teEvent[strip].teDATA[event].intSTARTPOS)  
                      && (teEvent[strip].teDATA[eventscan].intSTARTPOS <= teEvent[strip].teDATA[event].intENDPOS))
                            ||
                      ((teEvent[strip].teDATA[eventscan].intENDPOS >= teEvent[strip].teDATA[event].intSTARTPOS)  
                      && (teEvent[strip].teDATA[eventscan].intENDPOS <= teEvent[strip].teDATA[event].intENDPOS))  )
                {
                  // Stop the event.
                  teEvent[strip].teDATA[eventscan].booREPEAT = false;

                  // Check the event we are stopping to make sure its not the event calling the SetToEnd.
                  if (event != eventscan)
                  {
                    // Manage the Fade Animations to End.
                    if ((teEvent[strip].teDATA[eventscan].bytLEDANIMATION == AnPiFade) ||
                          (teEvent[strip].teDATA[eventscan].bytLEDANIMATION == AnPiFadeDith))
                    {
                      teEvent[strip].teDATA[eventscan].intDURATION = teEvent[strip].teDATA[event].intDURATION;
                      teEvent[strip].teDATA[eventscan].tmeSTARTTIME = tmeCurrentTime;

                      //teEvent[strip].teDATA[eventscan].intSPEED =  teEvent[strip].teDATA[event].intSPEED;
                      teEvent[strip].teDATA[eventscan].intSPEED =  0;
                    // The above two lines will need to be addressed at a future date. 
                    //  Problem occurs when an event current time is before the start 
                    //  pixel update begins. This mean that the start color will not be set until current 
                    //  time is past start time.  Need to choose: apples or oranges during 
                    //  the crgb_anim_color and event routine.

                      teEvent[strip].teDATA[eventscan].crgbCOLORSTART1 = teEvent[strip].teDATA[eventscan].crgbCOLORDEST1;
                      teEvent[strip].teDATA[eventscan].crgbCOLORSTART2 = teEvent[strip].teDATA[eventscan].crgbCOLORDEST2;
                      teEvent[strip].teDATA[eventscan].crgbCOLORDEST1 = teEvent[strip].teDATA[event].crgbCOLORDEST1;
                      teEvent[strip].teDATA[eventscan].crgbCOLORDEST2 = teEvent[strip].teDATA[event].crgbCOLORDEST2;
                    }

                    // Manage the Pulse Animations to End
                    if ((teEvent[strip].teDATA[eventscan].bytLEDANIMATION == AnPiPulse) ||
                          (teEvent[strip].teDATA[eventscan].bytLEDANIMATION == AnPiPulseTo))
                    {
                      // Tell the pulse to stop.
                      if (teEvent[strip].teDATA[event].booREPEAT == false)
                      {
                        // For now, we will just kill it and hope another animation clears the artifacts.
                        // It would be nice to have a gracreful end and let the pixel end its animation,
                        // but, thats for another time.
                        teEvent[strip].teDATA[eventscan].booCOMPLETE = true;
                      }

                      // Pulse will end on its on at end of animation.
                      /* if (teEvent[strip].teDATA[event].booREPEAT == true)
                      {
                      } */
                    }
                  }
                }
              }
            }        
            break;
          } // End Case AnEvSetToEnd
        } // End Switch 
      } // End If time check
    } // End FOR event
  } // End FOR strip
}


// -------------------------------------------------------------------------------------
//  AuxLightControlModule

void DoorMonitorAndAnimationControlModule(led_strip lsStrips[], timed_event teEvent[], hardware_monitor hwmDoor[], boolean booSensors[], unsigned long tmeCurrentTime)
// This routine is designed to scan all the doors or switches.  If anthing is open, opened 
//  closed or closed (odd twist of english there) then set the appropriate or maintain
//  animations.  
// Works like this.  If any door is open then make sure the overhead lights are on.  
//  If all the doors are closed, turn all the lights off.  Run door animations like 
//  normal.
{
  int opencount = 0;
  int strip;
  boolean changedetectted = false;

  // Scan Through Doors and Check for Changes

  // Check for newly opened and run animation on them.
  for (int door=0; door < NUM_SWITCHES; door++)
  {
    if (hwmDoor[door].changed(booSensors[door]) == true)
    {
      changedetectted = true; 
      if (hwmDoor[door].booVALUE)  // Door Just Opened
      {
        // Start Open Door Animation on This Door
        if (hwmDoor[door].ISHARDWARE == true)  // Only if its real switch (with lights attached to it)
        {
          strip = door *2;

          //Door Animation
          // Clear Events On Door
          vdClearAllTimedEvent(teEvent, door, lsStrips[strip].St, lsStrips[strip].Ed);
          teEvent[door].set(tmeCurrentTime, 10, 0, 0, AnEvSchedule, AnTaDoorOpen00, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[strip].St, lsStrips[strip].Ed, false, true);
        }
      }
      else                        // Door Just Closed
      {
        if (hwmDoor[door].ISHARDWARE == true)  // Only if its real switch (with lights attached to it)
        {
          strip = door *2;

          // Start Close Door Animation on This Door
          vdClearAllTimedEvent(teEvent, door, lsStrips[strip].St, lsStrips[strip].Ed);
          // Pulse Clear All Animations
          teEvent[door].set(tmeCurrentTime, 100, 0, 0, AnEvSchedule, AnTaChannelPulseGreen, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[strip].St, lsStrips[strip].Ed, false, true);
        }
      }
    }
  }

  // If a door was opened or close we need set the Overhead Lights.
  if (changedetectted == true)
  {

    // Count the amount of open doors
    for (int door=0; door < NUM_SWITCHES; door++)
    {
      if (hwmDoor[door].booVALUE  == true)
      {
        opencount = opencount  + 1;
      }
    }

    if (opencount > 0)
    {
      // There are open Doors.  Make sure all Overheads are running 
      for (int door=0; door < NUM_SWITCHES; door++)
      {
        strip = (door *2) +1; // Determine Strip from Door aka Channel
        
        if (hwmDoor[door].ISHARDWARE == true)  // Only if its real switch (with lights attached to it)
        {
          if (lsStrips[strip].AnimationStatus != AnTavdPacificaish)
          {
            // Clear Events for the Overhead Animation
            vdClearAllTimedEvent(teEvent, door, lsStrips[strip].St, lsStrips[strip].Ed);

            // Start Overhead Animation.
            lsStrips[strip].AnimationStatus = AnTavdPacificaish;
            teEvent[door].set(tmeCurrentTime, 10, 0, 0, AnEvSchedule, AnTavdPacificaish, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[strip].St, lsStrips[strip].Ed, false, true);
          }
        }
      }      

    }
    else  // All doors are closed.  Make sure lights are off or turning off and Amber Up the newly closded doors.
      {
      for (int door=0; door < NUM_SWITCHES; door++)
      {
        strip = (door *2) +1; // Determine Strip from Door aka Channel

        if (hwmDoor[door].ISHARDWARE == true)  // Only if its real switch (with lights attached to it)
        {
          if (lsStrips[strip].AnimationStatus == AnTavdPacificaish)
          {
            
            // Start Overhead Turn Off Animation.
            // See when the door was closed
            if ((tmeCurrentTime - hwmDoor[door].tmeTOGGLEDTIME) < 15000)
            {
              //  The door was recently closed. Run the Amber Pacifica on it.

              vdClearAllTimedEvent(teEvent, door, lsStrips[strip].St, lsStrips[strip].Ed);

              // This needs to be moved outside of this routine, int an animation event. 
              lsStrips[strip].AnimationStatus = AnTavdPaAnimClose;  

              int intTm;
              int intDur;
              int intSp;

              //Restore the privouse Blue to fade out
              intTm = 200; intDur = 6000; intSp = 5;
              teEvent[door].set(tmeCurrentTime, intTm , intDur, intSp, AnEvSweep, AnPiPulse, CRGB(0, 0, 60), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[strip].Ed, lsStrips[strip].St, false, true);
              teEvent[door].set(tmeCurrentTime, 10, 0, 0, AnEvSchedule, AnTavdPaAnimClose, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[strip].St, lsStrips[strip].Ed, false, true);
            }
            else
            {
              // The door was closed for a while, just turn off the lights.
              // Start the Close Door Overhead Animation

              // Clear Events for the Overhead Animation
              vdClearAllTimedEvent(teEvent, door, lsStrips[strip].St, lsStrips[strip].Ed);

              // Just turn off the lights.
              lsStrips[strip].AnimationStatus = AnTaStripOverOff;
              teEvent[door].set(tmeCurrentTime, 10, 0, 0, AnEvSchedule, AnTaStripOverOff, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), lsStrips[strip].St, lsStrips[strip].Ed, false, true);
            }
          }
        }
      }
    }
  }

  // *************************************************
  //
  //  This Section of code reserved for a routine 
  //  to check for lights should be off.  After
  //  ten minutes, manualy turn them off. 
  //
  // *************************************************

}


// ***************************************************************************************
// MAIN
// ***************************************************************************************
// GLOBAL VARIABLES

//  Light Strip Hardware
CRGB hwLEDs0[NUM_LEDSs0];       // LED Strip 1 values.
CRGB hwLEDs1[NUM_LEDSs1];       // LED Strip 2 values.

// Define Led Strips
led_strip lsStrips[4];

// Light Strip Event System
timed_event teEvent[NUM_CHANNELS];

// Door Sensor
hardware_monitor hwDoors[NUM_SWITCHES];
boolean boAuxLightsIsOn = false;

// Onboard LED to signify data being sent to LED strip.
const int ledPin =  LED_BUILTIN;

// Delay less Loop Variables
//    intRestTime defines the amount of time, in milliseconds, passed before another data
//    read pass is performed and transmitted from the controller to the main system.
unsigned long tmeCurrentMillis = millis();
unsigned long tmePrevMillis = 0;
int intRestTime = RESTTIME;  // 16 = 60 fps     // Do not check for update until rest 
                                                //  time is passed.

//boolean booRandom = false;


// ***************************************************************************************
// MAIN PROGRAM
// ***************************************************************************************
void setup()
{
  // Generate RandomSeed based on Temperture, Humidity, Air Quality, Gravametric Indifferences, 
  //  Planatery Alignment, and Current Horoscope Predictions.
  randomSeed(analogRead(0));

  // Define LED Strip.
  FastLED.addLeds<LED_TYPE, DATA_PINs0, COLOR_ORDER>(hwLEDs0, NUM_LEDSs0).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PINs1, COLOR_ORDER>(hwLEDs1, NUM_LEDSs1).setCorrection(TypicalLEDStrip);
  LEDS.setBrightness(BRIGHTNESS);

  // Define Light Strips
  lsStrips[Back_Door].set(0, s0As, s0Ae); // Back Door
  lsStrips[Back_Over].set(0, s0Bs, s0Be); // Back Overhead
  lsStrips[Front_Door].set(1, s1As, s1Ae); // Front Door
  lsStrips[Front_Over].set(1, s1Bs, s1Be); // Front Overhed

  // Define Door Sensors.
  pinMode(SWITCH_PINs0, INPUT_PULLUP);
  pinMode(SWITCH_PINs1, INPUT_PULLUP);
  pinMode(SWITCH_PINs2, INPUT_PULLUP);
  pinMode(SWITCH_PINs3, INPUT_PULLUP);

  // Arduino Onboard LED
  pinMode(ledPin, OUTPUT);

  teEvent[0].create(NUM_LEDSs0);
  teEvent[1].create(NUM_LEDSs1);

  // Define Door Sensors.
  hwDoors[0].set(SWITCH_PINs0, 50, true);
  hwDoors[1].set(SWITCH_PINs1, 50, true);
  hwDoors[2].set(SWITCH_PINs2, 50, false);
  hwDoors[3].set(SWITCH_PINs3, 50, false);

  /*
    // Define Communications.
    Wire.begin(SLAVEID);              // Connect to I2C bus with defined slave ID as 
                                      //  address
    Wire.onReceive(onDataAvailable);  // Register data avaiable event.
  */

  tmeCurrentMillis = millis();

  // Open serial communications if in debug mode.
  if (BOOTEST == true)
  {
    //Serial.begin(115200);
    //Serial.println("Program Start");
    //vdTESTFLASHAnimation(teEvent, 0, tmeCurrentMillis);
    vdPowerOnAnimation(lsStrips, lsStrips[0], teEvent, tmeCurrentMillis);
  }
  else
  {
    // Boot Animation
    // Make sure we have the current time before we try any test animations.
    vdPowerOnAnimation(lsStrips, lsStrips[0], teEvent, tmeCurrentMillis);
  }
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
  boolean booUpdates0 = false;
  boolean booUpdates1 = false;

  //  Get current time.  This will be our timeframe to work in.
  tmeCurrentMillis = millis();

  // --- Begin of Delayless Loop ---
  if (tmeCurrentMillis - tmePrevMillis >= intRestTime)
  {
    tmePrevMillis = tmeCurrentMillis;

    // --- Grabbing Data From Hardware inputs ---

    // Sensor Array (QUICK FIX)
    boolean booSensors[NUM_SWITCHES];

    booSensors[0] = digitalRead(SWITCH_PINs0);
    booSensors[1] = digitalRead(SWITCH_PINs1);
    booSensors[2] = digitalRead(SWITCH_PINs2);
    booSensors[3] = digitalRead(SWITCH_PINs3);


    // Check the doors and start or end all animations
    DoorMonitorAndAnimationControlModule(lsStrips, teEvent, hwDoors, booSensors, tmeCurrentMillis);
    

    // --- Check and Execute Timed Events That Are Ready ---

    //  Timed Event ALL GLOBAL
    teSystem(lsStrips, teEvent, tmeCurrentMillis);

    //  Timed Event 0
    booUpdates0 = teEvent[0].execute(hwLEDs0, tmeCurrentMillis);

    //  Timed Event 1
    booUpdates1 = teEvent[1].execute(hwLEDs1, tmeCurrentMillis);

    //  If we made it to this part of the code then we need to
    //    tell the LED hardware that it has a change to commit.

    // --- Execute LED Hardware Changes If Anything Was Updated ---
    if ((booUpdates0 == true) || (booUpdates1 == true))
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
    //if (BOOTEST == true)
    //{
    //  intRestTime = TESTRESTTIME;
    //}
  }   // End Delayless Loop
}

// ***************************************************************************************
// EVENTS
// ***************************************************************************************
/*
  void onDataAvailable(int intCount)
  {
  byte bytData[DATASIZE];
  int x = 0;


  Serial.print("Receiving Data: ");

  // Read comm data into array;
  while (Wire.available() > 0)
  {
    bytData[x] = Wire.read();
    x++;
  }

  // Debug
  for (int cnt = 0; cnt < x; cnt++)
  {
    Serial.print(bytData[cnt]);
  }
  Serial.println();

  // Figure out what task needs to be done, and do it.
  switch (bytData[x])
  {
    case AnTaOff:
      {
        vdClearAllTimedEvent(teEvent, 0, hwLEDSTARTLs0);
      }
    case AnTaPowerOn:
      {
        vdPowerOnAnimation(teEvent, 0, tmeCurrentMillis);
      }

    case AnTaDoorOpen:
      {
        vdDoorOpenAnimation(teEvent, 0, tmeCurrentMillis);
      }
    case AnTaDoorClose:
      {
        vdDoorCloseAnimation(teEvent, 0, tmeCurrentMillis);
      }
    case AnTaFlash:
      {
        vdFlashAnimation(teEvent, 0, tmeCurrentMillis);
      }
  }
  }
*/