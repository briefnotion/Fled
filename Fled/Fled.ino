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
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.55a
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
// *  V 0.55 _200717
// *      - Still no randomness.  Trying another thing.  Random numbers are incredibly 
// *          difficult to generate.  Perhaps the guys at Arduino should create a 
// *          random seed function.
// *      - Changed a few more animations.  An artist's work is never done. 
// *
// *  V 0.54 _200711
// *      - Random waves from previous update didn't work.  Added randomSeed command to 
// *          get things started correctly.
// *      - 500ms bug is still persistant.  Actually, not a bug at all.  The program is 
// *          generating new animations before the preveous ones has started.  After the 
// *          animation queue is full, the new animations are discarded.  Needs thought. 
// *      - Modified a few of the animations. 
// *
// *  V 0.53 _200630-0708
// *      - Increased rear door amber animation.
// *      - Decrease debounce on switches by 10x.
// *      - Start clear animation after start off previous animations (500ms error.) 
// *          I'd have to think to do that.  Rather than that, adding an extra clear 
// *          animation 14 seconds after the first as backup.
// *      - Added random numers to the Paificaish animations to make the waves looks 
// *          less organized. 
// *        
// *  V 0.52 _200620
// *      - Corrected several problems, mostly, focusing on the having the animations 
// *          behave the way I envisioned them. As well as removing inconsistencies.
// *      - Testing is getting more and more difficult because so many of the animations 
// *          now work with each other on multiple levels.  I have only one short test 
// *          strip, with only one switch.  I do my best to prepare, but as it is now, 
// *          and as it will be from now on, I can't forsee all of the problems before 
// *          without seeing the lights in action and live. And, even then, changes will 
// *          be restricted and slow. in all practicallity, this is the end of Step 1. 
// *          It is what it is. Only bug fixes to this step will be made. Unless, of 
// *          course, I think of something truely amazing. 
// *
// *  V 0.51 _200618
// *      - I didn't like the ways some animations behaved. So changes where made.
// *      - Coded but did not implement the Global Event "AnEvClear"
// *
// *  V 0.50 _200613-17
// *      - "break;" statatements added to all applicable "switch" statements. The filthy 
// *          little mongrels. 
// *      - Developed a Global event system  "AnEvSchedule", that runs with the existing 
// *          event system, to allow current animations to run in stages, with less 
// *          memory and for future Global Event Calls.
// *      - Created a new global pixel animation named "AnEvSetToEnd".  This event will 
// *          set all repeating animations to end and fade to destination any pulse 
// *          types.
// *      - Pacificaish animation lights will now turn on all strips through the 
// *          Automatic Auxililary Lighting Control Subsystem Module if any of the doors 
// *          are open.  And when all the doors are closed, the lights will fade off. 
// *      - Created new animation for door close and changed some of the animation 
// *          scripts to manage memory in a better way.
// *      - I had resorvations about changing the Pacificaish close door animation color 
// *          from blue to orange because, it looks like the ocean changes to a burning
// *          lake of lava.  However, the dark military orange, or old school amber, 
// *          color has very distinctive practical applications. Any new passenger will 
// *          have some ambiant light to fiddle with their phones, get settled and 
// *          comfortable, with a light color that should not intefere with me seeing 
// *          out the windows. These are "safety lights," of course, and not show lights.
// *          Never forget that.
// *      - I will need to make changes to the way the two Arduinos are wired, but that 
// *          is a hardware change. I'll try to post a schematic. 
// *      - Ok, I really lost track of all the changes and additions. All in all,
// *          any open door should light up the car like a blistering pulsar. i cant't
// *          definitivly say if I will like it or not.  We will see.
// *      - Nearly all of the new code is uncommented and I left many of the debug 
// *          routines in.  I need to get things working before I clean things up.
// *      - This compile, "Arduino Nano clone" runs at:
// *          Sketch uses 20456 bytes (66%) of program storage space. Maximum is 30720 
// *             bytes.
// *          Low memory available, stability problems may occur.
// *          Global variables use 1722 bytes (84%) of dynamic memory, leaving 326 bytes 
// *            for local variables. Maximum is 2048 bytes.
// *
// *  V 0.41 _200531
// *      - Changed some of the Animations. 
// *      - Added 2 more LED strips that go inside the car above the doors. One for 
// *         the back and one for the front.
// *      - Put in Pacificaish Animations for the inside animations when the door is 
// *          open.  My animation is based on Mark Kriegsman and Mary Corey March's Pacifica
// *          (https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino)
// *          I didn't copy their code.  Honestly, I can't understand it.  They are using 
// *          sin and math functions that are way beyond me.  I'm just not that smart.
// *          If its beyond a linear curve, then its beyond me.  I do thing it looks good 
// *          though so I put a few pulses on a blue background and played with the numbers
// *          until I found something I liked.  I will be expanding on this later, but 
// *          for now, the next step is installing the hardware, which is, at this time,
// *          sitting on my floor.
// *      - Made a few more changes to the animations and I am still working on ways to 
// *          improve memory use. 
// *      - Lower Frame Rate are noticable now that I am aproachig 250 LEDs on a single 
// *          Nano Arduino 16mhz clone.  I think I am reaching its LED amount limits while 
// *          using my style of animation types.
// *      - I layed down steps when creating this project.  I am nearing the completion
// *          of the first step.  In about 3 or more new animation types and a few more 
// *          functions and I will be there.
// *  
// *
// *  V 0.40 _200528
// *      - Rewrote code to save memory.  Animations will now need to be carefully 
// *          controlled when being designed.
// *      - Removed Start Array.
// *      - Removed Collision Tracker.
// *      - Removed Lock Array.
// *      - Rewrote Animation Setup Events.
// *      - Added a vabible (ClearOnEnd) that will clear an event after complete.
// *      - Fade starting color and ending color has been removed and replaced with 
// *          start color and end color.
// *      - Didn't like that. So I rewote some code to bring it back.
// *          A new Animatition type called AnPiFadeDith has been added.  It is the 
// *          same as AnPiFade except that it will consider the Start 2 and Dest 2
// *          as its end point colors.  
// *      - Also, added a booClearOnEnd parameter to the event set routine.  If true 
// *          the event will clear and the pixel color will not be not be maintained.
// *          If false, then the pixel color will always return to its destination 
// *          color.  This is important when setting a background color.
// *      - Also, the dither routine consistantly uses more cycles to run, so use 
// *          AnPiFade when just setting a static color across pixels.
// *      - Artifacts have been observed when multiple things are running at once.
// *          They are controlable by manipulating the event timings.  Problem is 
// *          still unknown.  That's the half of it.
// *      - Removed large chunks of code. 
// *      - Because of the removals, some of the code could stand some optimization
// *          rewriting.
// *      - This version is an "as was" version.  More changes will be made after 
// *          the soon installation and testing of a 3rd and 4th stip.
// *        
// *
// *  V 0.31 _200517
// *      - Preping for 2 LED strip operation and testing.
// *      - A few layout changes.
// *      - a few variable name changes.
// *      - May have fixed the Fade bug.
// *      - Removed the Expired varible.  I didnt like it because it was confusing.
// *      - Fighting for memory.  System seems stable at 91% ram. Unstable at 94%.
// *          and will not run at 97%.
// *          Clean house some but there remains much more to do.  Current memory
// *          improvements were a result of changing the timed event array count to
// *          13 from 15.  The largest Animation is 9 events so testing on lowering
// *          this value may help.   Alternativly, I could get rid of the Start Array. 
// *      - Copied and customized front and back door animations.
// *      - Modifie Power On animation.
// *      - Debugged a large amount of code so that 2 animatiion strips will run at
// *          the same time.
// *      - Only thing I can thing of doing at this time is install it and see if 
// *          it works.
// *
// *  V 0.30 _200513
// *      - Adding communication code to recieve commands from a control module.
// *          It doesn't work fully, but it is part of the next phase of the process so
// *          I did start testing it.  Leaving it in for now.
// *      - Spent what felt like the better part of a year trying to rewrite the
// *          fade animations to not use the StartArray.  First by care and finess,
// *          second by maticulus calculations, and third by hammer, duck tape, and
// *          zip ties.  Nothing worked.  Threw the whole thing in the trash can
// *          and picked out the things that actully worked.  Which are:
// *      - New compile time definitions for BOOTEST, BOORESTIME, and BOOTESTRESTTIME.
// *      - Fast, quickly expiring animations should now finish before purging
// *          themselves.  Although I picked it out of the trash, and it worked before.
// *          I haven't tested it on this build because it isn't critical with the current
// *          animations.  It's a part of the flash animation, which is useless anyway
// *          untill I get a control moudule coded, tested, and built.
// *      - The one major bug that still exist is within the fade animation.
// *          If a fade animation is called ontop of another active one, weird things
// *          happen, glitchy type stuff.
// *          This error also becomes appearant when calling to animations back to back.
// *          Even though one should be expired, the problem apears.  I found out I
// *          can get around this by padding them, but I shouldn't have to.
// *          Also, a strange backwash is apearant if the LEDs were dithered to another
// *          color.
// *      - The program takes up way too much ram.  I need to clean house.  Waiting on
// *          it for now untill I install my second set of light strips tomorrow or the
// *          day after.  I would really like to get 3 LED light strips, or about 200 LEDs
// *          on a single Arduino NANO.  If I could do away with the Start LED array...
// *          if only.
// *      - Brought the frame rate back to 1000 - 1 FPS.  I don't know what the frame
// *          rate is, really.  I think I had to tone it down in the prev build because
// *          I didn't know about the fade animation bug.
// *
// *  V 0.23 _200505
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
#define NUM_LEDSs0    123 //71
#define s0As          0       // s0 A Start
#define s0Ae          69      // s0 A End
#define s0Bs          70      // s0 B Start
#define s0Be          121     // s0 B End

#define NUM_LEDSs1    118 //66
#define s1As          0       // s1 A Start
#define s1Ae          65      // s1 A End
#define s1Bs          66      // s1 B Start
#define s1Be          117     // s1 B End

/*
// *** TEST ***
#define NUM_LEDSs0    60 //71
#define s0As          0       // s0 A Start
#define s0Ae          29      // s0 A End
#define s0Bs          30      // s0 B Start
#define s0Be          59      // s0 B End

#define NUM_LEDSs1    60 //66
#define s1As          0       // s1 A Start
#define s1Ae          29      // s1 A End
#define s1Bs          30      // s1 B Start
#define s1Be          59      // s1 B End
// *** TEST *** 
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
#define NUM_STRIPS        2   // Amount of LED strips we will be controlling.
#define NUM_TIMED_EVENTS  15  // Untill I can remember how LL, this is being
//  Also, total number of this will be multiplied by the
//  amount of LED strips you have setup.  Watch your memory.
#define NUM_SWITCHES      4

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

// Event Animations
#define AnEvClear     0
#define AnEvSweep     1
#define AnEvSchedule  2
#define AnEvSetToEnd  3
// Pixel Animations
#define AnPiFade      4
#define AnPiFadeDith  5
#define AnPiPulse     6
#define AnPiPulseTo   7

// Animations (Global)
// Hardcoding this for now.
#define AnTaAllOff              0
#define AnTaPowerOn             1

#define AnTaDoorOpenBack        2
#define AnTaDoorOpenBack01      21
#define AnTaDoorOpenBack02      22

#define AnTaDoorOpenFront       3
#define AnTaDoorOpenFront01     31
#define AnTaDoorOpenFront02     32

#define AnTaDoorCloseBack       4
#define AnTaDoorCloseBack01     41
#define AnTaDoorCloseBack02     42

#define AnTaDoorCloseFront      5
#define AnTaDoorCloseFront01    51
#define AnTaDoorCloseFront02    52

#define AnTavdPacificaishBack   6
#define AnTavdPacificaishFront  7

#define AnTavdPaAnimBackClose   8
#define AnTavdPaAnimFrontClose  9

// ***************************************************************************************
// STRUCTURES
// ***************************************************************************************
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
void vdTESTFLASHAnimation(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;
}
*/

// -------------------------------------------------------------------------------------
void vdPowerOnAnimation(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Just to show the lights work when the program starts.
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;

  intDur = 250; intSp = 5; intCt = 118;
  // Pulse Front Door
  teEvent[1].set(tmeCurrentTime, 50, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(125, 125, 125), CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Be, 0, false, true);
  // Pulse Back Door
  teEvent[0].set(tmeCurrentTime, 640, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(125, 125, 125), CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Be, 0, false, true);


  intDur = 2000; intSp = 15; intCt = 71;
  // Clear Front Door
  teEvent[1].set(tmeCurrentTime, 890, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Be, 0, false, true);
  // Clear Back Door
  teEvent[0].set(tmeCurrentTime, 1480, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Be, 0, false, true);
}

// -------------------------------------------------------------------------------------
void vdSimplePulse(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Just to show the lights work when the program starts.        
// vdSimplePulse(teEvent, 1, tmeCurrentTime);
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;

  intDur = 250; intSp = 5; intCt = 118;
  // Pulse Front Door
  teEvent[1].set(tmeCurrentTime, 50, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(125, 125, 125), CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Be, 0, false, true);
  // Pulse Back Door
  teEvent[0].set(tmeCurrentTime, 50, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(125, 125, 125), CRGB(0, 0, 25), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Be, 0, false, true);
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

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

void vdDoorOpenAnimationBack(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Door Open Back Stage 0

// Door is open, engage safety lights.
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;
  // Door Open Animation
  intTm = 100; intDur = 500; intSp = 10; intCt = 60;

  teEvent[intPos].set(tmeCurrentTime, intTm , 0, 0, AnEvSchedule, AnTavdPacificaishBack, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);

  // Clear set background to door open colors.
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFadeDith, CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(255, 64, 64), s0As, s0As + 4, false, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFadeDith, CRGB(0, 0, 0), CRGB(255, 64, 64), CRGB(0, 0, 0), CRGB(25, 0, 0), 5, 10, false, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(25, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 11, s0Ae - 6, false, false);
  teEvent[intPos].set(tmeCurrentTime, intTm + 500, intDur, intSp, AnEvSweep, AnPiFadeDith, CRGB(0, 0, 0), CRGB(25, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Ae - 5, s0Ae, false, false);

  // Shimmer
  teEvent[intPos].set(tmeCurrentTime, intTm, 10000, 500, AnEvSweep, AnPiPulse, CRGB(15, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, 6000, 280, AnEvSweep, AnPiPulse, CRGB(15, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, false);

  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  teEvent[intPos].set(tmeCurrentTime, intTm, 0, 0, AnEvSchedule, AnTaDoorOpenBack01, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);
}

void vdDoorOpenAnimationBack01(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Door Open Back Stage 1
{
  int intTm = 50;
  int intDur;
  int intCt;
  int intSp;

  //teEvent[intPos].set(tmeCurrentTime, intTm , 0, 0, AnEvSchedule, AnTavdPacificaishBack, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);
  //  BUG:  Previous line not running if intTm is set to 0 or less.  Seems related to the previous calling AnEvSchedule event and this event running at the 
  //          same exact time.  if intTm is set to 0, all events in vdDoorOpenAnimationFront01 do not run. Not, just the AnTavdPacificaishFront.

  // Flash
  intDur = 100; intSp = 2; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, false, true);
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 600; intSp = 12; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, false, true);
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 1500; intSp = 30; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(128, 128, 0), CRGB(128, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, false, true);

  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  teEvent[intPos].set(tmeCurrentTime, intTm, 0, 0, AnEvSchedule, AnTaDoorOpenBack02, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);
}

void vdDoorOpenAnimationBack02(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Door Open Back Stage 2
{
  int intTm = 0;
  int intDur;
  int intCt;
  int intSp;

  // Repeat Pulse
  intDur = 1500; intSp = 125; intCt = 36;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, 34, true, true);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Ae, 35, true, true);
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

void vdDoorOpenAnimationFront(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Door Open Front Stage 0

// Door is open, engage safety lights.
{
int intTm;
  int intDur;
  int intCt;
  int intSp;
  // Door Open Animation
  intTm = 100; intDur = 500; intSp = 10; intCt = 60;

  teEvent[intPos].set(tmeCurrentTime, intTm , 0, 0, AnEvSchedule, AnTavdPacificaishFront, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);

  // Clear set background to door open colors.
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFadeDith, CRGB(0, 0, 0), CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(255, 64, 64), s1As, s1As + 4, false, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFadeDith, CRGB(0, 0, 0), CRGB(255, 64, 64), CRGB(0, 0, 0), CRGB(25, 0, 0), 5, 10, false, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(25, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 11, s1Ae - 6, false, false);
  teEvent[intPos].set(tmeCurrentTime, intTm + 500, intDur, intSp, AnEvSweep, AnPiFadeDith, CRGB(0, 0, 0), CRGB(25, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Ae - 5, s1Ae, false, false);

  // Shimmer
  teEvent[intPos].set(tmeCurrentTime, intTm, 10000, 500, AnEvSweep, AnPiPulse, CRGB(15, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1As, s1Ae, true, false);
  teEvent[intPos].set(tmeCurrentTime, intTm, 6000, 280, AnEvSweep, AnPiPulse, CRGB(15, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1As, s1Ae, true, false);

  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  teEvent[intPos].set(tmeCurrentTime, intTm, 0, 0, AnEvSchedule, AnTaDoorOpenFront01, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);
}

void vdDoorOpenAnimationFront01(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Door Open Front Stage 1
{
  int intTm = 50;
  int intDur;
  int intCt;
  int intSp;

  //teEvent[intPos].set(tmeCurrentTime, intTm , 0, 0, AnEvSchedule, AnTavdPacificaishBack, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);
  //  BUG:  Previous line not running if intTm is set to 0 or less.  Seems related to the previous calling AnEvSchedule event and this event running at the 
  //          same exact time.  if intTm is set to 0, all events in vdDoorOpenAnimationFront01 do not run. Not, just the AnTavdPacificaishFront.

  // Flash
  intDur = 100; intSp = 2; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1As, s1Ae, false, true);
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 600; intSp = 12; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(80, 80, 0), CRGB(80, 80, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1As, s1Ae, false, true);
  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  intDur = 1500; intSp = 30; intCt = 71;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(128, 128, 0), CRGB(128, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1As, s1Ae, false, true);

  intTm = intAnTmDly(intTm, intDur, intCt, intSp);
  teEvent[intPos].set(tmeCurrentTime, intTm, 0, 0, AnEvSchedule, AnTaDoorOpenFront02, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);
}

void vdDoorOpenAnimationFront02(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Door Open Front Stage 2
{
  int intTm = 0;
  int intDur;
  int intCt;
  int intSp;

  // Repeat Pulse
  intDur = 1500; intSp = 125; intCt = 36;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1As, 32, true, true);
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(255, 255, 0), CRGB(255, 255, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Ae, 33, true, true);
 }

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

void vdDoorCloseAnimationBack(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Door closed.  Quick animation to show and turn off lights.
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;

  // Stop the currently running Pacificaish animation.
  teEvent[intPos].set(tmeCurrentTime, 10, 6000, 0, AnEvSetToEnd, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, false, true);

  // Clear and Pulse colors background to green then ending in blueish, starting with the center.
  intTm = 50; intDur = 750; intSp = 30; intCt = 36;

  teEvent[intPos].set(tmeCurrentTime, intTm, 100, intSp, AnEvSweep, AnPiPulse, CRGB(128, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, false, true);
  teEvent[intPos].set(tmeCurrentTime, intTm + 1500, 100, intSp, AnEvSweep, AnPiPulse, CRGB(128, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Be, s0Bs, false, true);
 
  teEvent[intPos].set(tmeCurrentTime, intTm + 50, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(0, 255, 0), CRGB(0, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, false, true);
  teEvent[intPos].set(tmeCurrentTime, intTm + 50 + 1500, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(0, 255, 0), CRGB(0, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Be, s0Bs, false, true);
 
  // Fade remaining colors out.
  intTm = intAnTmDly(intTm, intDur, intCt, intSp) + 300;
  intDur = 5000; intSp = 100; intCt = 36;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, false, true);
  //teEvent[intPos].set(tmeCurrentTime, intTm + 1500, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Be, s0Bs, false, true);

  teEvent[intPos].set(tmeCurrentTime, intTm + 2000, 0, 0, AnEvSchedule, AnTavdPaAnimBackClose, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);
}

// -------------------------------------------------------------------------------------
void vdDoorCloseAnimationFront(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// Door closed.  Quick animation to show and turn off lights.
{
  int intTm;
  int intDur;
  int intCt;
  int intSp;

  // Stop the currently running Pacificaish animation.
  teEvent[intPos].set(tmeCurrentTime, 10, 6000, 0, AnEvSetToEnd, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, false, true);

  // Clear and Pulse colors background to green then ending in blueish, starting with the center.
  intTm = 50; intDur = 750; intSp = 30; intCt = 36;

  teEvent[intPos].set(tmeCurrentTime, intTm, 100, intSp, AnEvSweep, AnPiPulse, CRGB(128, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1As, s1Ae, false, true);
  teEvent[intPos].set(tmeCurrentTime, intTm + 1500, 100, intSp, AnEvSweep, AnPiPulse, CRGB(128, 128, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Be, s1Bs, false, true);
 
  teEvent[intPos].set(tmeCurrentTime, intTm + 50, intDur, intSp, AnEvSweep, AnPiPulseTo, CRGB(0, 255, 0), CRGB(0, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1As, s1Ae, false, true);
  teEvent[intPos].set(tmeCurrentTime, intTm + 50 + 1500, intDur, intSp, AnEvSweep, AnPiPulse, CRGB(0, 255, 0), CRGB(0, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Be, s1Bs, false, true);
 
  // Fade remaining colors out.
  intTm = intAnTmDly(intTm, intDur, intCt, intSp) + 300;
  intDur = 5000; intSp = 100; intCt = 36;
  teEvent[intPos].set(tmeCurrentTime, intTm, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1As, s1Ae, false, true);
  //teEvent[intPos].set(tmeCurrentTime, intTm + 1500, intDur, intSp, AnEvSweep, AnPiFade, CRGB(0, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Be, s1Bs, false, true);

  teEvent[intPos].set(tmeCurrentTime, intTm + 2000, 0, 0, AnEvSchedule, AnTavdPaAnimFrontClose, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, true);
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

void vdPacificaishAnimationBack(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// This animation was my personal chalenge to see if I could make a similar animation that was
// provided in the demos and examples, as PacificaAnimation, of the FastLed library.
{
  // Set the background color.
  teEvent[intPos].set(tmeCurrentTime, 1000, 500, 30, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 60), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, false, false);

  // The waves.
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3500), intRandomHD(250), AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(1500), intRandomHD(150), AnEvSweep, AnPiPulse, CRGB(160, 200, 40), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3600), intRandomHD(270), AnEvSweep, AnPiPulse, CRGB(20, 200, 180), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3200), intRandomHD(200), AnEvSweep, AnPiPulse, CRGB(60, 200, 140), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, true, true);
}
// -------------------------------------------------------------------------------------
void vdPacificaishAnimationFront(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// This animation was my personal chalenge to see if I could make a similar animation that was
// provided in the demos and examples, as PacificaAnimation, of the FastLed library.
{
  // Set the background color.
  teEvent[intPos].set(tmeCurrentTime, 1000, 500, 30, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 60), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Be, s1Bs, false, false);

  // The waves.
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3500), intRandomHD(250), AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(1500), intRandomHD(150), AnEvSweep, AnPiPulse, CRGB(160, 200, 40), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3600), intRandomHD(270), AnEvSweep, AnPiPulse, CRGB(20, 200, 180), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3200), intRandomHD(200), AnEvSweep, AnPiPulse, CRGB(60, 200, 140), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, true, true);
}

// -------------------------------------------------------------------------------------

void vdPacificaishAnimationBackClose(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// This animation was my personal chalenge to see if I could make a similar animation that was
// provided in the demos and examples, as PacificaAnimation, of the FastLed library.
{
  teEvent[intPos].set(tmeCurrentTime, AUXDRLINGERBCK, 1000, 0, AnEvSetToEnd, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, false, true);

  // Set the background color.
  teEvent[intPos].set(tmeCurrentTime, 50, 3000, 30, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, false, false);

  // The waves.
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3500), intRandomHD(250), AnEvSweep, AnPiPulse, CRGB(20, 4, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(1500), intRandomHD(150), AnEvSweep, AnPiPulse, CRGB(16, 20, 0), CRGB(16, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3600), intRandomHD(270), AnEvSweep, AnPiPulse, CRGB(15, 6, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3200), intRandomHD(200), AnEvSweep, AnPiPulse, CRGB(25, 2, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, true, true);
}

// -------------------------------------------------------------------------------------
void vdPacificaishAnimationFrontClose(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// This animation was my personal chalenge to see if I could make a similar animation that was
// provided in the demos and examples, as PacificaAnimation, of the FastLed library.
{
  teEvent[intPos].set(tmeCurrentTime, AUXDRLINGERFRT, 1000, 0, AnEvSetToEnd, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, false, true);

  // Set the background color.
  teEvent[intPos].set(tmeCurrentTime, 50, 3000, 30, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, false, false);

  // The waves.
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3500), intRandomHD(250), AnEvSweep, AnPiPulse, CRGB(20, 4, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(1500), intRandomHD(150), AnEvSweep, AnPiPulse, CRGB(16, 20, 0), CRGB(16, 20, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3600), intRandomHD(270), AnEvSweep, AnPiPulse, CRGB(15, 6, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3200), intRandomHD(200), AnEvSweep, AnPiPulse, CRGB(25, 2, 0), CRGB(20, 4, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, true, true);
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

void vdPacificaishAnimationOPENTest(timed_event teEvent[], int intPos, unsigned long tmeCurrentTime)
// This animation was my personal chalenge to see if I could make a similar animation that was
// provided in the demos and examples, as PacificaAnimation, of the FastLed library.
{
  // Set the background color.
  teEvent[intPos].set(tmeCurrentTime, 1000, 500, 30, AnEvSweep, AnPiFade, CRGB(0, 0, 0), CRGB(0, 0, 60), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Ae, s0As, false, false);

  // The waves.

  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3500), intRandomHD(250), AnEvSweep, AnPiPulse, CRGB(40, 200, 160), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(1500), intRandomHD(150), AnEvSweep, AnPiPulse, CRGB(160, 200, 40), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3600), intRandomHD(270), AnEvSweep, AnPiPulse, CRGB(20, 200, 180), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
  teEvent[intPos].set(tmeCurrentTime, intRandomHD(2000), intRandomHD(3200), intRandomHD(200), AnEvSweep, AnPiPulse, CRGB(60, 200, 140), CRGB(40, 200, 160), CRGB(0, 0, 0), CRGB(0, 0, 0), s0As, s0Ae, true, true);
}

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

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
void teGLOBAL(timed_event teEvent[], unsigned long tmeCurrentTime)
// Example:
//  teEvent[0].set(tmeCurrentMillis, X, 0, 0, AnEvSchedule, AnTaDoorCloseBack, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), 0, 0, false, false);
//    or      ( X is the values that can be set )
//  teEvent[0].set(tmeCurrentMillis, X, X, 0, AnEvSchedule, AnEvSetToEnd, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(X, X, X), CRGB(X, X, X), 0, 0, false, false);
//    or 
//  teEvent[0].set(tmeCurrentMillis, X, 0, 0, AnEvClear, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), X, X, false, false);
{
  for (int strip = 0; strip < NUM_STRIPS; strip++)
  {
    for (int event = 0; event < NUM_TIMED_EVENTS; event++)
    {
      if ((teEvent[strip].teDATA[event].booCOMPLETE == false) && (tmeCurrentTime >= teEvent[strip].teDATA[event].tmeSTARTTIME))
      {
        // Process Timed Global Events (keeping the LED type event scheduler format for 
        //  now)
        switch (teEvent[strip].teDATA[event].bytANIMATION)
        {
          case AnEvClear:
          {
            teEvent[strip].teDATA[event].booCOMPLETE = true;
            teEvent[strip].ClearAll(teEvent[strip].teDATA[event].intSTARTPOS,teEvent[strip].teDATA[event].intENDPOS);
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
                vdPowerOnAnimation(teEvent, 9, tmeCurrentTime);
                break;
              }

              // Open Back Door
              case AnTaDoorOpenBack:
              {
                vdDoorOpenAnimationBack(teEvent, strip, tmeCurrentTime);
                break;
              }
              case AnTaDoorOpenBack01:
              {
                vdDoorOpenAnimationBack01(teEvent, strip, tmeCurrentTime);
                break;
              }
              case AnTaDoorOpenBack02:
              {
                vdDoorOpenAnimationBack02(teEvent, strip, tmeCurrentTime);
                break;
              }

              // Open Front Door
              case AnTaDoorOpenFront:
              {
                vdDoorOpenAnimationFront(teEvent, strip, tmeCurrentTime);
                break;
              }
              case AnTaDoorOpenFront01:
              {
                vdDoorOpenAnimationFront01(teEvent, strip, tmeCurrentTime);
                break;
              }
              case AnTaDoorOpenFront02:
              {
                vdDoorOpenAnimationFront02(teEvent, strip, tmeCurrentTime);
                break;
              }

              // Close Back Door
              case AnTaDoorCloseBack:
              {
                vdDoorCloseAnimationBack(teEvent, strip, tmeCurrentTime);
                break;
              }
              
              // Close Front Door
              case AnTaDoorCloseFront:
              {
                vdDoorCloseAnimationFront(teEvent, strip, tmeCurrentTime);
                break;
              }

              case AnTavdPacificaishBack:
              {
                vdPacificaishAnimationBack(teEvent, strip, tmeCurrentTime);
                break;
              }
              case AnTavdPacificaishFront:
              {
                vdPacificaishAnimationFront(teEvent, strip, tmeCurrentTime);
                break;
              }

              case AnTavdPaAnimBackClose:
              {
                vdPacificaishAnimationBackClose(teEvent, strip, tmeCurrentTime);
                break;
              }
              case AnTavdPaAnimFrontClose:
              {
                vdPacificaishAnimationFrontClose(teEvent, strip, tmeCurrentTime);
                break;
              }
            } // End Case Animation Set
            break;
          } // End Case AnEvSchedule

          case AnEvSetToEnd:
          {
            // Clear the Event whether the event ran or not.
            teEvent[strip].teDATA[event].booCOMPLETE = true;
     
            // Is the Event we are processing within the event?

            for (int eventscan = 0; eventscan < NUM_TIMED_EVENTS; eventscan++)
            {

              if(teEvent[strip].teDATA[eventscan].tmeSTARTTIME < tmeCurrentTime)
              {

                if (  ((teEvent[strip].teDATA[eventscan].intSTARTPOS >= teEvent[strip].teDATA[event].intSTARTPOS)  
                      && (teEvent[strip].teDATA[eventscan].intSTARTPOS <= teEvent[strip].teDATA[event].intENDPOS))
                            ||
                      ((teEvent[strip].teDATA[eventscan].intENDPOS >= teEvent[strip].teDATA[event].intSTARTPOS)  
                      && (teEvent[strip].teDATA[eventscan].intENDPOS <= teEvent[strip].teDATA[event].intENDPOS))  )
                {
                  teEvent[strip].teDATA[eventscan].booREPEAT = false;

                  if (event != eventscan)
                  {
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

boolean AuxLightControlModule(timed_event teEvent[], boolean booSensors[], boolean booIsOn, hardware_monitor hwmDoor[], unsigned long tmeCurrentTime)
{
  int oncount = 0;
  for (int door=0; door < NUM_SWITCHES; door++)
  {
    if (booSensors[door] == true)
    {
      oncount++;
    }
  }

  // If off and needs to be on
  if ((oncount > 0) && booIsOn == false)
  {
    for (int door=0; door < NUM_SWITCHES; door++)
    {
      if (hwmDoor[door].booVALUE == false)
      {
        if (door == 0)
        {
          vdClearAllTimedEvent(teEvent, 0, s0Bs, s0Be);
          vdPacificaishAnimationBack(teEvent, 0, tmeCurrentTime);
        }
        if (door == 1)
        {
          vdClearAllTimedEvent(teEvent, 1, s1Bs, s1Be);
          vdPacificaishAnimationFront(teEvent, 1, tmeCurrentTime);
        }
      }
    }
    return true;
  }

  // If on and needs to be off
  if ((oncount == 0) && (booIsOn == true))
  {
    for (int door=0; door < NUM_SWITCHES; door++)
    {
      if (hwmDoor[door].booVALUE == false)
      {
        // Check recent changes
        if (hwmDoor[door].tmeCHANGEDETECTEDTIME < (tmeCurrentTime - AUXDRLINGERBCK))
        {
          if (door == 0)
          {
            teEvent[0].set(tmeCurrentTime, 0, 4000, 0, AnEvSetToEnd, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, false, true);
            teEvent[0].set(tmeCurrentTime, 0, 15000, 0, AnEvSetToEnd, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s0Bs, s0Be, false, true);
          }
          if (door == 1)
          {
            teEvent[1].set(tmeCurrentTime, 1, 4000, 0, AnEvSetToEnd, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, false, true);
            teEvent[1].set(tmeCurrentTime, 1, 15000, 0, AnEvSetToEnd, 0, CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0), s1Bs, s1Be, false, true);
          }
        }
      }
    }
    return false;
  }

}




// ***************************************************************************************
// MAIN
// ***************************************************************************************
// GLOBAL VARIABLES

//  Light Strip Hardware
CRGB hwLEDs0[NUM_LEDSs0];       // LED Strip 1 values.
CRGB hwLEDs1[NUM_LEDSs1];       // LED Strip 2 values.

// Light Strip Event System
timed_event teEvent[NUM_STRIPS];

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
  hwDoors[0].set(SWITCH_PINs0, 50);
  hwDoors[1].set(SWITCH_PINs1, 50);
  hwDoors[2].set(SWITCH_PINs2, 50);
  hwDoors[3].set(SWITCH_PINs3, 50);

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
    vdPowerOnAnimation(teEvent, 9, tmeCurrentMillis);
  }
  else
  {
    // Boot Animation
    // Make sure we have the current time before we try any test animations.
    vdPowerOnAnimation(teEvent, 9, tmeCurrentMillis);
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

    // Check door 1 for changes.
    // Back Door
    if (hwDoors[0].changed(booSensors[0]))
    {
      if (booSensors[0] == HIGH)
      {
        vdClearAllTimedEvent(teEvent, 0, s0As, s0Be);
        vdDoorOpenAnimationBack(teEvent, 0, tmeCurrentMillis); 
        boAuxLightsIsOn = AuxLightControlModule(teEvent, booSensors, boAuxLightsIsOn, hwDoors, tmeCurrentMillis);
      }
      else
      {
        vdClearAllTimedEvent(teEvent, 0, s0As, s0Ae);
        vdDoorCloseAnimationBack(teEvent, 0, tmeCurrentMillis);
        //vdPacificaishAnimationCLOSETest(teEvent, 0, tmeCurrentMillis); 
        boAuxLightsIsOn = AuxLightControlModule(teEvent, booSensors, boAuxLightsIsOn, hwDoors, tmeCurrentMillis);
      }
    }

    // Check door 2 for changes.
    // Front Door
    if (hwDoors[1].changed(booSensors[1]))
    {
      if (booSensors[1] == HIGH)
      {
        vdClearAllTimedEvent(teEvent, 1, s1As, s1Be);
        vdDoorOpenAnimationFront(teEvent, 1, tmeCurrentMillis);
        boAuxLightsIsOn = AuxLightControlModule(teEvent, booSensors, boAuxLightsIsOn, hwDoors, tmeCurrentMillis);
      }
      else
      {
        vdClearAllTimedEvent(teEvent, 1, s1As, s1Ae);
        vdDoorCloseAnimationFront(teEvent, 1, tmeCurrentMillis);
        boAuxLightsIsOn = AuxLightControlModule(teEvent, booSensors, boAuxLightsIsOn, hwDoors, tmeCurrentMillis);
      }
    }

    if (hwDoors[2].changed(booSensors[2]))
    {
      boAuxLightsIsOn = AuxLightControlModule(teEvent, booSensors, boAuxLightsIsOn, hwDoors, tmeCurrentMillis);
    }

    if (hwDoors[3].changed(booSensors[3]))
    {
      boAuxLightsIsOn = AuxLightControlModule(teEvent, booSensors, boAuxLightsIsOn, hwDoors, tmeCurrentMillis);
    }

    // --- Check and Execute Timed Events That Are Ready ---

    //  Timed Event ALL GLOBAL
    teGLOBAL(teEvent, tmeCurrentMillis);

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
    case AnTaAllOff:
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