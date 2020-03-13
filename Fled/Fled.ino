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
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.02B
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
// *  V 0.02b _200313
// *    - Dirty Birdy update:
// *        The code is a big hot mess.  Nearly completely undocumented.  Crap is 
// *        everywhere.  Stuff is scattered and unorganized.  Varibles are huge
// *        and nothing is unorganized.  The thing is ...
// *    - It works.
// *    - Next step: clean this bird up to and get it ready for a stew.
// *
// *  V 0.02  _200311
// *    - Creating main working system.
// *
// *  V 0.01  _200309
// *    - Project Start.
// *
// ***************************************************************************************

#include <FastLED.h>

// Structures

// ---------------------------------------------------------------------------------------

struct light_path
{
  unsigned long tmeSTARTTIME;
  unsigned long intDURATION;
  byte bytANIMATION;
  CRGB crgbCOLORDEST;
  CRGB crgbCOLORSTART;
  boolean booExpired = true;
  boolean booFirstRun = false;

  void set(unsigned long tmeCurrentTime, unsigned long tmeStartInTime, unsigned long intDuration, byte bytAnimation, CRGB crgbDestinationColor)
  //  Create a light_path:
  //  Parameters:
  //
  //  CurrentTime   - For the cycle or actual real time of when the event is said to be created.
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
  {
    switch (bytANIMATION)
    {
      case 0:
        {
          booExpired = false;
          booFirstRun = true;
        }
      case 1:
        {
          //Fill Color
          if (tmeCurrentTime >= tmeSTARTTIME + intDURATION)
          {
            booExpired = true;
            return crgbCOLORDEST;
          }
          else
          {
            //int tmeElapse = tmeCurrentTime - tmeSTARTTIME;

            float fltPower = (float)(tmeCurrentTime - tmeSTARTTIME) / (float)intDURATION;

            byte R = (fltPower * crgbCOLORDEST.r) + ((1 - fltPower) * crgbCOLORSTART.r);
            byte G = (fltPower * crgbCOLORDEST.g) + ((1 - fltPower) * crgbCOLORSTART.g);
            byte B = (fltPower * crgbCOLORDEST.b) + ((1 - fltPower) * crgbCOLORSTART.b);

            return CRGB(R, G, B);
          } // END CASE 1
        case 2:
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
              
              if ((tmeElapsed *2) <= (intDURATION))
              {
                fltPower = (float)(tmeElapsed) *2 / ((float)intDURATION);
              }
              else
              {
                fltPower = 1 - (((float)((tmeElapsed *2) - intDURATION) / (float)intDURATION));
              }
              
              //            byte R = (fltPower * crgbCOLORDEST.r);
              //            byte G = (fltPower * crgbCOLORDEST.g);
              //            byte B = (fltPower * crgbCOLORDEST.b);

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
{
  unsigned long tmeSTARTTIME;
  unsigned long intDURATION;
  unsigned int intSPEED;
  byte bytANIMATION;
  byte bytLEDANIMATION;
  CRGB crgbCOLORDEST;;
  int intSTARTPOS;
  int intENDPOS;
  boolean booExpired = true;

  void set(light_path lpLightPaths[], unsigned long tmeCurrentTime, unsigned long tmeStartInTime, unsigned long intDuration, unsigned int intSpeed, byte bytAnimation, byte bytLEDAnimation, CRGB crgbColor, int intStartPos, int intEndPos)
  //  Create a light event:
  //  Parameters:

  //  LightPaths    - LightPath Array to be set.
  //  CurrentTime   - For the cycle or actual real time of when the event is said to be created.
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
    bytLEDANIMATION;
    crgbCOLORDEST = crgbColor;
    intSTARTPOS = intStartPos;
    intENDPOS = intEndPos;
    booExpired = false;
  }

  boolean is_ready(unsigned long tmeCurrentTime)
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

  void execute(light_path lpLedArray[], unsigned long tmeCurrentTime)
  {
    switch (bytANIMATION)
    {
      case 0:
        {
          booExpired = false;
        }
      case 1:
        {
          unsigned long tmeStart;
          for (int x = intSTARTPOS; x <= intENDPOS; x++)
          {
            // Find Delay between leds
            if (intSTARTPOS == intENDPOS)
            {
              tmeStart = tmeSTARTTIME;
            }
            else if (intSTARTPOS < intENDPOS)
            {
              tmeStart = tmeSTARTTIME + (x * intSPEED);
            }
            else if (intSTARTPOS > intENDPOS)
            {
              tmeStart = tmeSTARTTIME + intSPEED - (x * intSPEED);
            }


            //             tmeStart = tmeSTARTTIME + (x * 300);

            //should I pass current or start time?
            lpLedArray[x].set(tmeStart, 0, intDURATION, bytLEDANIMATION, crgbCOLORDEST);
          }
        }
    }
    booExpired = true;
  }
};


// ***************************************************************************************


// Hardware Setup
#define DATA_PIN    3
//#define CLK_PIN     4       // If your LED_TYPE requires one.
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    60


#define BRIGHTNESS        96  // Using Example Code.  Max unknown
#define FRAMES_PER_SECOND 120 // Will not be necessary, but keeping, for now, just in case.


#define NUM_TIMED_EVENTS  10  // Untill I can remember how pointers work, this is being
// hardcoded.




// ***************************************************************************************
// --- Global Variables ---


light_path lpLED[NUM_LEDS];
timed_event tmeEvent[NUM_TIMED_EVENTS];

CRGB hwLED[NUM_LEDS];
//CRGB hwLEDs[NUM_LEDS];

// Some simple throwaway global varibles.
int intX = 0;


// Delay less Loop Variables
//    intRestTime defines the amount of time, in milliseconds, passed before another data
//    read pass is performed and transmitted from the controller to the main system.

// unsigned long tmeCurrentMillis = millis();          // 1 Second = 1000 millis
unsigned long tmeCurrentMillis = millis();   // PROBLEMS WITH 32bit opperation
// on 16 bit boards. Grrr.

unsigned long tmePrevMillis = 0;
int intRestTime = 0;             // Do not check for update until rest time is passed.
boolean booLEDChanged = false;    // Indicating a led changed color value.

// Enable Serial Monitor for testing
//    Enabling booTest will slow the board time and enable the serial monitor to be read.
const boolean booTest = false;


// ***************************************************************************************
//  Notes:
//    EventTypes:
//      byte 0 :  Clear Event
//      byte 1 :  Set All to Color


// ***************************************************************************************
// EVENT TRIGGERS

// Power On

// Alert

// Door Open

// Door Close

// ***************************************************************************************


void setup()
{

  //tempvars
  //CRGB tmpColor;
  // Prep and define Hardware.



  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(hwLED, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //hwLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  LEDS.setBrightness(BRIGHTNESS);

  //if ((booTest == true)
  //{
  //  Serial.begin(9600);
  //}

  // Startup Routines


  // Clear all lights and wait 2 seconds
  fill_solid( &(hwLED[0]), NUM_LEDS, CRGB( 0, 0, 128) );
  FastLED.show();
  delay(100);
  fill_solid( &(hwLED[0]), NUM_LEDS, CRGB( 0, 0, 0) );
  FastLED.show();
  delay(2000);



  tmeCurrentMillis = millis();
  // Clear the field in 1 second immediatly.
  tmeEvent[0].set(lpLED, tmeCurrentMillis, 1000, 50, 10, 1, 1, CRGB(50, 0, 0), 1, 59);
  tmeEvent[1].set(lpLED, tmeCurrentMillis, 4000, 250, 20, 1, 2, CRGB(50, 50, 0), 1, 59);
  tmeEvent[2].set(lpLED, tmeCurrentMillis, 8000, 500, 30, 1, 2, CRGB(50, 50, 0), 1, 59);
  tmeEvent[3].set(lpLED, tmeCurrentMillis, 12000, 1500, 40, 1, 2, CRGB(50, 50, 0), 1, 59);
  tmeEvent[4].set(lpLED, tmeCurrentMillis, 16000, 2000, 50, 1, 2, CRGB(50, 50, 0), 1, 59);
  //tmeEvent[2].set(lpLED, tmeCurrentMillis, 1200, 50, 10, 1, 1, CRGB(0, 0, 50), 1, 30);
  //tmeEvent[3].set(lpLED, tmeCurrentMillis, 1300, 50, 10, 1, CRGB(50, 50, 50), 1, 30);
  //tmeEvent[2].set(lpLED, tmeCurrentMillis, 2100, 50, 10, 1, CRGB(0, 0, 50), 9, 11);
  //tmeEvent[3].set(lpLED, tmeCurrentMillis, 4000, 2000, 300, 1, CRGB(50, 0, 50), 12, 14);
  //tmeEvent[4].set(lpLED, tmeCurrentMillis, 20000, 10000, 300, 1, CRGB(0, 0, 0), 7, 20);
  //tmeEvent[5].set(lpLED, tmeCurrentMillis, 6000, 2000, 300, 1, CRGB(50, 0, 00), 18, 20);
  //tmeEvent[6].set(lpLED, tmeCurrentMillis, 7000, 2000, 300, 1, 1, CRGB(50, 50, 50), 21, 23);
  //tmeEvent[7].set(lpLED, tmeCurrentMillis, 8000, 2000, 300, 1, 1, CRGB(0, 0, 00), 24, 26);
  //tmeEvent[8].set(lpLED, tmeCurrentMillis, 10000, 5000, 300, 1, 1, CRGB(0, 50, 00), 0, 14);
  //tmeEvent[9].set(lpLED, tmeCurrentMillis, 12000, 300, 300, 1, 1, CRGB(0, 0, 0), 1, 30);
  //tmeEvent[1].set(lpLED, tmeCurrentMillis, 11000, 500, 1, CRGB(50,50,00), 33, 35);
  tmeEvent[9].set(lpLED, tmeCurrentMillis, 22000, 250, 1, 1, 1, CRGB(0, 0, 0), 1, 59);
  


}


// ***   Scratpad
// leds[i].setRGB( 255, 68, 221);
// FastLED.show(); //Write it out.

void loop()
{
  // put your main code here, to run repeatedly:
  boolean booUpdate = false;

  tmeCurrentMillis = millis();
  // Begin of Delay less Loop
  if (tmeCurrentMillis - tmePrevMillis >= intRestTime)
  {
    tmePrevMillis = tmeCurrentMillis;

    // Grabbing Data ---

    // Process Data ---

    // Check and Execute Timed Events ---
    for (int x = 0; x < NUM_TIMED_EVENTS; x++)
    {
      if (tmeEvent[x].is_ready(tmeCurrentMillis) == true)
      {
        tmeEvent[x].execute(lpLED, tmeCurrentMillis);
      }
    }

    // Check and Execute Display Events ---


    for (int x = 0; x < NUM_LEDS; x++)
    {
      if (lpLED[x].is_ready(tmeCurrentMillis, hwLED[x]) == true)
      {
        //CRGB rgbTemp = lpLED[x].getcolor();

        //hwLED[x].setRGB(10, 10, 10);

        //hwLED[x].setRGB(lpLED[x].execute().r,lpLED[x].execute().g,lpLED[x].execute().b);
        //hwLED[x].setRGB(x*3,x*5,x*10);
        //hwLED[x].setRGB(10, 10, 10);
        hwLED[x] = (CRGB)lpLED[x].execute(tmeCurrentMillis);


        booUpdate = true;
      }
    }

    // Execute Post Processes ---

    if (booUpdate == true)
    {
      hwLED[0] = CRGB(25, 0, 0);
      FastLED.show();
      //delay(2);
      hwLED[0] = CRGB(0, 0, 0);
      FastLED.show();
    }


    // Debug Routines ---
    // Slow delay if in testing mode.
    if (booTest == true)
    {
      intRestTime = 250;
    }
  }   // End Delay less Loop
}
