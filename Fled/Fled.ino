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
// *  PROJECTID: gi6$b*E>*q%;    Revision: 00000000.01A
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
// *  Purpose:
// *    To Be Determined.
// *
// *
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
