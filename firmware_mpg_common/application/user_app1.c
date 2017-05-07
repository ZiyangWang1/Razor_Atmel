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

/*--------------------------------------------------------------------------------------------------------------------
Function UserApp1_CutoutModule()

Description:
Cutout the latest 4 letters in the G_au8DebugScanfBuffer array.

Requires:
  - Return a pointer pointed to an empty array .

Promises:
  - Call this function to fill an array with the 4 letters.
*/
void UserApp1_CutoutModule(u8* au8temp)
{
  u32 i;
  /* Cutout the latest 4 letters in the G_au8DebugScanfBuffer array. */
  for(i=0;i<4;i++)
  {
    au8temp[i] = G_au8DebugScanfBuffer[G_u8DebugScanfCharCount-1-i];
  }
  return ;
}

/*--------------------------------------------------------------------------------------------------------------------
Function UserApp1_CheckModule()

Description:
Check the latest 4 letters in the G_au8DebugScanfBuffer array if match
my name.

Requires:
  - Require a pointer pointed to an array made up with the 4 letters,
    and another pointer pointed to a counter counts the number of your name has been called. 

Promises:
  - Call this function to operate checking procedure.
*/
void UserApp1_CheckModule(u8* au8temp,u32* pu32Counter,u8* pu8CharCount)
{
  u32 i;
  static u8 au8Name[]="gnaw";
  for(i=0;i<4;i++)
  {
    if(au8temp[i]!=au8Name[i])
    {
      /* Do noting if not match */
      return;
    }
  }
  /* Output and clear the buffer if match */
  UserApp1_OutputModule(pu32Counter);
  for(i = 0; i < G_u8DebugScanfCharCount; i++)
  {
    G_au8DebugScanfBuffer[i] = '\0';
  }
  G_u8DebugScanfCharCount = 0;
  *pu8CharCount = 2;
  return;
}

/*--------------------------------------------------------------------------------------------------------------------
Function UserApp1_OutputModule()

Description:
Output the number of your name has been called.

Requires:
  - Require a pointer pointed to a counter counts the number of your name has been called.

Promises:
  - Call this function to print the number of your name has been called surrounded with stars.
*/
void UserApp1_OutputModule(u32* pu32Counter)
{
  u32 i,StarNumber=0;
  (*pu32Counter)++;
  /* Figure out haw many figures within u32Counter */
  for(i=0;*pu32Counter/pow(10,i)!=0;i++)
  {
    StarNumber++;
  }
  DebugPrintf("\r\n");
  /* Print the above stars */
  for(i=0;i<StarNumber+2;i++)
  {
    DebugPrintf("*");
  }
  DebugPrintf("\r\n");
  /* Print the number and two stars */
  DebugPrintf("*");
  DebugPrintNumber(*pu32Counter);
  DebugPrintf("*");
  DebugPrintf("\r\n");
  /* Print the below stars */
  for(i=0;i<StarNumber+2;i++)
  {
    DebugPrintf("*");
  }
  DebugPrintf("\r\n");
}

/*--------------------------------------------------------------------------------------------------------------------
Function pow()

Description:
Calculate the power of a number

Requires:
  - Require 2 u32 number to be powered

Promises:
  - Return the Yth power of X
*/
u32 pow(u32 X,u32 Y)
{
  u32 i,u32result=1;
  for(i=0;i<Y;i++)
  {
    u32result*=X;
  }
  return u32result;
}
/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
#if 1 /* Set this to 1 to run the name check code */
  static u8 u8CharCount=2;
  static u8 *pu8CharCount = &u8CharCount;
  static u8 au8temp[5];
  static u32 u32Counter=0;
  static u32 *pu32Counter = &u32Counter;
  
  /* Check if there are more than 3 letters in the buffer */
  if(G_u8DebugScanfCharCount>2)
  {
    /* Check if new letter entered */
    if(G_u8DebugScanfCharCount>u8CharCount)
    {
      u8CharCount++;
      UserApp1_CutoutModule(au8temp);
      UserApp1_CheckModule(au8temp,pu32Counter,pu8CharCount);   
    }
  }
#endif
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
