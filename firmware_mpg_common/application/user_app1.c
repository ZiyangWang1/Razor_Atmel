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
extern volatile u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* Space to latch characters for DebugScanf() */
extern volatile u8 G_u8DebugScanfCharCount;                    /* Counter for # of characters in Debug_au8ScanfBuffer */


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

/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RawStringCreationModule()

Description:
Add 40 blanks to target string, and save as raw string.

Requires:
  - Two pointers pointed to target string and raw string.
  - The length of target string.

Promises:
  - Calls the function to create raw string.
*/
void UserApp1RawStringCreationModule(u8* au8TargetString,u8* au8RawString,u8 u8StringLength)
{
  u8 i=0;
  
  /* Add 40 blanks at the begining of target string */
  for(i=0;i<40;i++)
  {
    au8RawString[i]=' ';
  }
  /* Put target string after the blanks */
  for(;i<(u8StringLength+40);i++)
  {
    au8RawString[i]=au8TargetString[i-40];
  }
  return;
}

/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1CutoutModule()

Description:
Cut out a string with 20 letters from the raw string according to the roll counter.

Requires:
  - Three pointers pointed to raw string, roll counter, and output string.
  - The length of target string.

Promises:
  - Calls the function to create a 20-letter-string for output.
*/
void UserApp1CutoutModule(u8* au8RawString,u8* pu8RollCount,u8* au8OutputString,u8 u8StringLength)
{
  u8 i=0;
  
  for(;i<20;i++)
  {
    /* Cut from the begining when reach the last one */
    if((i+(*pu8RollCount))>(u8StringLength+40-1))
    {
      au8OutputString[i]=au8RawString[i+(*pu8RollCount)-u8StringLength];
    }
    /* Cut one by one when have not reach the last one */
    else
    {
      au8OutputString[i]=au8RawString[i+(*pu8RollCount)];
    }
  }
  return;
}

/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1OutputString1CreationModule()

Description:
Increase the roll counter and create output string 1 .

Requires:
  - Three pointers pointed to raw string, roll counter, and output string 1. 
  - The length of target string.

Promises:
  - Calls the function to create output string 1, and prepare for string 2 creation.
*/
void UserApp1OutputString1CreationModule(u8* au8RawString,u8* pu8RollCount,u8* au8OutputString1,u8 u8StringLength)
{
  /* Move forward 1 step per second */
  (*pu8RollCount)++;
  UserApp1CutoutModule(au8RawString,pu8RollCount,au8OutputString1,u8StringLength);
  return;
}

/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1OutputString2CreationModule()

Description:
Create output string 2 following string 1.

Requires:
  - Three pointers pointed to raw string, roll counter, and output string 2.
  - The length of target string.

Promises:
  - Calls the function to Create output string 2.
*/
void UserApp1OutputString2CreationModule(u8* au8RawString,u8* pu8RollCount,u8* au8OutputString2,u8 u8StringLength)
{
  /* Cut another 20 letters after string 1 */
  (*pu8RollCount)+=20;
  UserApp1CutoutModule(au8RawString,pu8RollCount,au8OutputString2,u8StringLength);
  (*pu8RollCount)-=20;
  /* Go to the first letter when roll to the last one */
  if((*pu8RollCount)==(u8StringLength+40-1))
  {
    (*pu8RollCount)=39;
  }
  return ;
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u32 u32Count=0;
  static u8 au8OutputString1[20]="\0";
  static u8 au8OutputString2[20]="\0";
  static u8 u8RollCount=0;
  static u8* pu8RollCount=&u8RollCount;
  static u8 au8TargetString[128]="\0";
  static u8 u8StringLength=0;
  static u8 au8RawString[168]="\0";
  static bool bInputComplete=FALSE;
  static bool bScreenDisplay=FALSE;
  u8 i=0;
  
  /* Get the string when press Enter */
  if(G_au8DebugScanfBuffer[G_u8DebugScanfCharCount-1]=='\r')
  {
    u8StringLength=DebugScanf(au8TargetString)-1;
    bInputComplete=TRUE;
    bScreenDisplay=FALSE;
    /* Clean string 1&2 */
    for(i=0;i<20;i++)
    {
      au8OutputString1[i]='\0';
    }
    for(i=0;i<20;i++)
    {
      au8OutputString2[i]='\0';
    }
  }
  if(bInputComplete)
  {
    /* Display on the first line when letter number is less than 21 */
    if(u8StringLength<21)
    {
      for(i=0;i<u8StringLength;i++)
      {
        au8OutputString1[i]=au8TargetString[i];
      }
      if(bScreenDisplay==FALSE)
      {
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR,au8OutputString1);
        bScreenDisplay=TRUE;
      }
    }
    else
    {
      /* Display on the second line when letter number is less than 41 */
      if(u8StringLength<41)
      {
        for(i=0;i<20;i++)
        {
          au8OutputString1[i]=au8TargetString[i];
        }
        for(;i<u8StringLength;i++)
        {
          au8OutputString2[i-20]=au8TargetString[i];
        }
        if(bScreenDisplay==FALSE)
        {
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE1_START_ADDR,au8OutputString1);
          LCDMessage(LINE2_START_ADDR,au8OutputString2);
          bScreenDisplay=TRUE;
        }
      }
      else
      {
        /* Roll to display when letter number is more than 40 */
        UserApp1RawStringCreationModule(au8TargetString,au8RawString,u8StringLength);
        u32Count++;
        if(u32Count==500)
        {
          u32Count=0;
          UserApp1OutputString1CreationModule(au8RawString,pu8RollCount,au8OutputString1,u8StringLength);
          UserApp1OutputString2CreationModule(au8RawString,pu8RollCount,au8OutputString2,u8StringLength);
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE1_START_ADDR,au8OutputString1);
          LCDMessage(LINE2_START_ADDR,au8OutputString2);
        }
      }
    }
  }
} /* end UserApp1SM_Idle() */
    
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
