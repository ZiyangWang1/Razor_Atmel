/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u8 au8DisplayMessage1[] = "Please sellect mode!";
  static u8 au8DisplayMessage2[] = "B0: Piano B1: Edit";
  static bool bDisplayed = FALSE;
  
  if(!bDisplayed)
  {
    bDisplayed = TRUE;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8DisplayMessage1);
    LCDMessage(LINE2_START_ADDR,au8DisplayMessage2);
  }
  
  if(WasButtonPressed(BUTTON0))
  {
    UserApp1_StateMachine = UserApp1SM_PianoMode;
    ButtonAcknowledge(BUTTON0);
    bDisplayed = FALSE;
  }
  
  if(WasButtonPressed(BUTTON1))
  {
    UserApp1_StateMachine = UserApp1SM_EditMode;
    ButtonAcknowledge(BUTTON1);
    bDisplayed = FALSE;
  }
    
  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_PianoMode(void)
{
  static u8 au8DisplayMessage1[] = "Piano Mode...";
  static u8 au8DisplayMessage2[] = "Press B0 to exit!";
  static bool bDisplayed = FALSE;
  static u8 au8Buffer[2] = {0,0};
  static u16 u16Frequence = 0;
  static u16 u16Counter = 0;
  
  if(!bDisplayed)
  {
    bDisplayed = TRUE;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8DisplayMessage1);
    LCDMessage(LINE2_START_ADDR,au8DisplayMessage2);
    DebugPrintf("\r\nPiano mode selected!\r\n");
  }
  
  if((bool)DebugScanf(au8Buffer))
  {
    PWMAudioOff(BUZZER1);
    switch (au8Buffer[0])
    {
    case '0' : u16Frequence = G4;break;
    case '.' : u16Frequence = A4;break;
    case '\r' : u16Frequence = B4;break;
    case '1' : u16Frequence = C5;break;
    case '2' : u16Frequence = D5;break;
    case '3' : u16Frequence = E5;break;
    case '4' : u16Frequence = F5;break;
    case '5' : u16Frequence = G5;break;
    case '6' : u16Frequence = A5;break;
    case '+' : u16Frequence = B5;break;
    default : ;
    }
    PWMAudioSetFrequency(BUZZER1,u16Frequence);
    PWMAudioOn(BUZZER1);
    u16Counter = 0;
  }
  else
  {
    if(u16Counter == 500)
    {
      u16Counter = 0;
      PWMAudioOff(BUZZER1);
    }
    u16Counter++;
  }
  
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    bDisplayed = FALSE;
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
} /* end UserApp1SM_PianoMode() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_EditMode(void)
{
  static u8 au8DisplayMessage1[] = "Edit Mode B0:Clean";
  static u8 au8DisplayMessage2[] = "B1:Play B2:Stop B3:E";
  static u8 au8DebugMessage1[] = "\n\rPlease enter notes (end with enter):";
  static u8 au8DebugMessage2[] = "\n\rPlease enter length (end with enter; '9' is sixteenth note):";
  static u16 u16Frequence[50];
  static u16 au16length[50];
  static u8 u8NoteCounts = 0;
  static u8 u8LengthCounts = 0;
  u8 au8Buffer[2];
  
  static bool bDisplayed = FALSE;
  static bool bNoteEntered = FALSE;
  static bool bLengthEntered = FALSE;
  
  if(!bDisplayed)
  {
    bDisplayed = TRUE;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8DisplayMessage1);
    LCDMessage(LINE2_START_ADDR,au8DisplayMessage2);
    DebugPrintf("\r\nEdit mode selected!\r\n");
  }
  
  if(!bNoteEntered)
  {
    DebugPrintf(au8DebugMessage1);
    
    if((bool)DebugScanf(au8Buffer))
    {
      if(au8Buffer[0] == '\r')
      {
        bNoteEntered = TRUE;
        DebugPrintf("\r\nNotes entered!\n\r");
      }
      else
      {
        switch (au8Buffer[0])
        {
        case '1' : u16Frequence[u8NoteCounts] = C5;u8NoteCounts++;break;
        case '2' : u16Frequence[u8NoteCounts] = D5;u8NoteCounts++;break;
        case '3' : u16Frequence[u8NoteCounts] = E5;u8NoteCounts++;break;
        case '4' : u16Frequence[u8NoteCounts] = F5;u8NoteCounts++;break;
        case '5' : u16Frequence[u8NoteCounts] = G5;u8NoteCounts++;break;
        case '6' : u16Frequence[u8NoteCounts] = A5;u8NoteCounts++;break;
        case '7' : u16Frequence[u8NoteCounts] = B5;u8NoteCounts++;break;
        default : DebugPrintf("Invalid note!");
        }
      }
    }
  }
  else
  {
    if(!bLengthEntered)
    {
      DebugPrintf(au8DebugMessage2);
      
      if((bool)DebugScanf(au8Buffer))
      {
        if(au8Buffer[0] == '\r')
        {
          bLengthEntered = TRUE;
          DebugPrintf("\r\nLength entered!\n\r");
        }
        else
        {
          switch (au8Buffer[0])
          {
          case '1' : au16length[u8LengthCounts] = FULL_NOTE;u8LengthCounts++;break;
          case '2' : au16length[u8LengthCounts] = HALF_NOTE;u8LengthCounts++;break;
          case '4' : au16length[u8LengthCounts] = QUARTER_NOTE;u8LengthCounts++;break;
          case '8' : au16length[u8LengthCounts] = EIGHTH_NOTE;u8LengthCounts++;break;
          case '9' : au16length[u8LengthCounts] = SIXTEENTH_NOTE;u8LengthCounts++;break;
          default : DebugPrintf("Invalid length!");
          }
        }
      }
    }
  }
  
  
  
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    bDisplayed = FALSE;
    UserApp1_StateMachine = UserApp1SM_Idle;
  }  
  
} /* end UserApp1SM_EditMode() */


#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
