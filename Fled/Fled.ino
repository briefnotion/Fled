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
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.01B
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
// *    to create a control module multi colored light strips. The individuals module, 
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
// *  V 0.01  _200309
// *    - Project Start.
// *
// ***************************************************************************************

// #include

// --- Variables ---

// Delay less Loop Variables
//    intRestTime defines the amount of time, in milliseconds, passed before another data read pass is performed
//    and transmitted from the controller to the main system.
unsigned long tmeCurrentMillis = millis();  // 1 Second = 1000 millis
unsigned long tmePrevMillis = 0;
int intRestTime = 46;        // Do not check for update until rest time is passed

// Enable Serial Monitor for testing
//    Enabling booTest will slow the board time and enable the serial monitor to be read.
const boolean booTest = false;

void setup()
{
  // put your setup code here, to run once:

  //if ((booTest == true)
  //{
  //  Serial.begin(9600);
  //}

}

void loop()
{
  // put your main code here, to run repeatedly:

  tmeCurrentMillis = millis();
  // Begin of Delay less Loop
  if (tmeCurrentMillis - tmePrevMillis >= intRestTime)
  {
    tmePrevMillis = tmeCurrentMillis;

    // Grabbing Data ---

    // Process Data ---

    // Execute Processes ---

    // Debug Routines ---
    // Slow delay if in testing mode.
    if (booTest == true)
    {
      intRestTime = 250;
    }
  }   // End Delay less Loop
}
