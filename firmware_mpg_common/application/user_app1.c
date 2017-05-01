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
static bool bLocked = TRUE;
static bool bLockdown = FALSE;
static bool bSetting = FALSE;
static bool bInputting = FALSE;
static bool bScaning = FALSE;
static bool bSetComplete = FALSE;
static u8 au8key[] = {10,10,10,10,10,10,10,10,10,10};
static u8 au8password[] = {10,10,10,10,10,10,10,10,10,10};
  

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
  /* Turn off all LEDs */
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  LedOff(GREEN);

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
Function: ScanToArray

Description:
Scan if there are buttons pressed, save which buttons have been pressed 
to an array ( maximum 10 buttons )

Requires:
  - paTarget is a pointer of target array
 
Promises:
  - Save which buttons have been pressed to target array ( maximum 10 buttons )

*/
void ScanToArray(u8* paTarget)
{
  static u8 i = 0;
  /* Save each pressed button to an array unit */
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    paTarget[i] = 1;
    i++;
  }
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    paTarget[i] = 2;
    i++;
  }
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    paTarget[i] = 3;
    i++;
  }
  /* Finish scan when BUTTON3 has been pressed */
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    bScaning = FALSE;
    i = 0;
  }
  /* Finish scan when reach the limit (10) */
  if(i == 10)
  {
    bScaning = FALSE;
    i = 0;
  }
  return;
}

/*----------------------------------------------------------------------------------------------------------------------
Function: SetPassword

Description:
Blink red and green LED and set the password

Requires:
 
Promises:
  - Call this function to set the password

*/
void SetPassword(void)
{
  /* Blink red and green LED at 2Hz */ 
  static u32 u32Counter = 0;
  u32Counter++;
  if(u32Counter == 250)
  {
    LedToggle(RED);
    LedToggle(GREEN);
    u32Counter = 0;
  } 
  /* Scan pressed buttons to key array */ 
  bScaning = TRUE;
  ScanToArray(au8key);
  /* Set setting to complete when finish scaning */
  if(bScaning == FALSE)
  {
    bSetting = FALSE; 
    bSetComplete = TRUE;
  }
  return;
}

/*----------------------------------------------------------------------------------------------------------------------
Function: Input

Description:
Input the password whose attempt to unlock 

Requires:
 
Promises:
  - Call this function to input the password

*/
void Input(void)
{
  /* Turn off green LED in case it is on */
  LedOff(GREEN);
  /* Scan pressed buttons to password array */ 
  bScaning = TRUE;
  ScanToArray(au8password);
  /* Set inputting to complete when finish scaning */
  if(bScaning == FALSE)
  { 
  bInputting = FALSE;
  }
  return;
}

/*----------------------------------------------------------------------------------------------------------------------
Function: CheckPassword

Description:
Check the set key and the password input by who attempt to unlock
Requires:
 
Promises:
  - Call this function to check key array and password array, if 
they are the same, set to unlocked. Otherwise, set to lockdown

*/
void CheckPassword(void)
{
  /* Check each array unit */
  for(u8 j = 0;j != 10;j++)
  {
    if(au8key[j] != au8password[j])
    {
      /* Set to lockdown when they are different */
      bLockdown = TRUE;
      return;
    }
  }
  /* Set to unlocked when they are the same */
  bLocked = FALSE;
  return;
}
  

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
#if 1 /* Set this to 1 to run the ButtonLock code */

  if(bLocked == FALSE)
  {
    /* Turn off red LED and blink green LED when unlocked*/
    LedOff(RED);
    static u32 u32Counter = 0;
    u32Counter++;
    if(u32Counter == 250)
    {
      LedToggle(GREEN);
      u32Counter = 0;
    } 
    if(IsButtonHeld(BUTTON3,2000))
    {
      /* Hold BUTTON3 for 2s to go to password setting mode when unlocked */
      bSetting = TRUE;
      bLocked = TRUE;
      bSetComplete = FALSE;
    }
  }
  else/* If locked */
  {
    if(bLockdown == TRUE)
    {
      /* Blink red LED at 2Hz in lockdown mode */
      static u32 u32Counter = 0;
      u32Counter++;
      if(u32Counter == 250)
      {
        LedToggle(RED);
        u32Counter = 0;
      }   
      /* Press BUTTON3 to exit lockdown mode to try it again */
      if(WasButtonPressed(BUTTON3))
      {
        ButtonAcknowledge(BUTTON3);
        bLockdown = FALSE;
      }
    }
    else/* If not in lockdown mode */
    {
      if(bSetting == TRUE)
      {
        /* Call function SetPassword when key setting is still in progress */
        SetPassword();
      }
      else/* If setting finished */
      {
        if(IsButtonHeld(BUTTON3,2000))
        {
          /* Hold BUTTON3 for 2s to enter password setting mode */
          bSetting = TRUE;
          SetPassword();
        }
        else
        {
          if(bSetComplete == TRUE)
          {
            /* Turn red LED on to show locked and start inputting password whose attempt to unlock */
            LedOn(RED);
            bInputting = TRUE;
            Input();
            if(bInputting == FALSE)
            {
              /* Call function CheckPassword when finish inputting */
              CheckPassword();
            }
          }
        }
      }
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
