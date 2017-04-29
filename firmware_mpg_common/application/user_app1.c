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
    /* Turn off all LED */
    LedOff(RED);
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
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
u8 ButtonScan(void)
{
   u8 u8ButtonValue = 0;
   if(WasButtonPressed(BUTTON0))
   {
     ButtonAcknowledge(BUTTON0);
     u8ButtonValue = 1;
   }
   if(WasButtonPressed(BUTTON1))
   {
     ButtonAcknowledge(BUTTON1);
     u8ButtonValue = 2;
   }
   if(WasButtonPressed(BUTTON2))
   {
     ButtonAcknowledge(BUTTON2);
     u8ButtonValue = 3;
   }
   if(WasButtonPressed(BUTTON3))
   {
     ButtonAcknowledge(BUTTON3);
     u8ButtonValue = 4;
   }
   return u8ButtonValue;
}
  
static void UserApp1SM_Idle(void)
{
#if 0
   /* Set an LED to blink at 2Hz */
   static u32 u32Counter = 0;
   u32Counter++;
   if(u32Counter == 250)
   {
     LedToggle(RED);
     u32Counter = 0;
   }
#endif
   
#if 1
   static u8 u8ButtonValue = 9;/* No Button has been pressed */
   static u32 u32Counter1 = 0;
   static u32 u32Counter2 = 0;
   static u8 au8PassWords[6] = {10,10,10,10,10,10};
   
   /* Turn off BLUE and RED LED per second */
   u32Counter2++;
   if(u32Counter2 == 1000)
   {
     LedOff(BLUE);
     LedOff(RED);
     u32Counter2 = 0;
   }
   /* Get which button has been pressed */
   u8ButtonValue = ButtonScan();
   /* Save the number and turn on the red LED when there is a button pressed */
   switch(u8ButtonValue)
   {
   case 1:
     au8PassWords[u32Counter1] = 1;
     u32Counter1++;
     LedOn(RED);
     break;
   case 2:
     au8PassWords[u32Counter1] = 2;
     u32Counter1++;
     LedOn(RED);
     break;
   case 3:
     au8PassWords[u32Counter1] = 3;
     u32Counter1++;
     LedOn(RED);
     break;
   case 4:
     au8PassWords[u32Counter1] = 4;
     u32Counter1++;
     LedOn(RED);
     break;
   }
   /* Check the password when reach six numbers */
   if(u32Counter1 == 6)
   {
     /* Turn on the green LED when password is correct */
     if(au8PassWords[0] == 2&&au8PassWords[1] == 2&&au8PassWords[2] == 3&&au8PassWords[3] == 3&&au8PassWords[4] == 4&&au8PassWords[5] == 4)
     {
       LedOn(GREEN);
     }
     else/* Turn on the green LED when password is incorrect and reset the password */
     {
       LedOn(BLUE);
       u32Counter1 = 0;
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
