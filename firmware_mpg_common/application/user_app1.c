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
#if 0 /* Set this to 1 to run the Task1 code */
   static u32 u32Counter1 = 0;
   static u32 u32Counter2 = 0;
   static bool bLightSt = FALSE;
   static u16 u16CounterLimitMs = 500;
   static bool bInOrDe = FALSE;
   
   /* Do blink according to the limit*/
   u32Counter1++;
   u32Counter2++;
   /* Switch light state when timer reaches the limit */
   if(u32Counter1 > u16CounterLimitMs)
   {
     if(bLightSt == TRUE)
     {
       HEARTBEAT_OFF();
       bLightSt = FALSE;
     }
     else
     {
       HEARTBEAT_ON();
       bLightSt = TRUE;
     }
     /* Reset the timer */
     u32Counter1 = 0;
   }
   
   /* Change the limit according to the requirement */
   /* Check if reach the swift-time */
   if(u32Counter2 == SWIFTTIME_MS)
   {
     /* Check the limit is increasing or decreasing */
     if(bInOrDe == TRUE)
     {
       /* Fix the data lose */
       if(u16CounterLimitMs == 480)
       {
         u16CounterLimitMs += 20;
       }/* end if */
       /* Check if the limit reach the maximum */
       if(u16CounterLimitMs == 500)
       {
         bInOrDe = FALSE;
         u16CounterLimitMs /= 2;
       }
       else 
       {
           u16CounterLimitMs *= 2;
       }
     }/* end if(bInOrDe == TRUE) */
     else/* If the limit is decreasing */
     {
       /* Check if the limit reach the minimum */
       if(u16CounterLimitMs == 15)
       {
         bInOrDe = TRUE;
         u16CounterLimitMs *= 2;
       }
       else
       {
         u16CounterLimitMs /= 2;
       }
     }/* end else */
     /* Reset the timer */
     u32Counter2 = 0;
   }
#endif
   
#if 0 /* Set this to 1 to run the Task2 code */
   static u32 u32Counter1 = 0;
   static u32 u32Counter2 = 0;
   static u32 u32DutyTime = 0;
   static bool bInOrDe = FALSE;
   
   /* Do blink at 100Hz */
   u32Counter2++;
   /* Keep light off when duty time is 0 */
   if(u32DutyTime == 0)
   {
     HEARTBEAT_OFF();
   }
   else
   {
     /* Turn light off when finish the duty time */
     if(u32Counter1 == u32DutyTime)
     {
       HEARTBEAT_OFF();
     }
     u32Counter1++;
     
     /* Turn light on when finish the cycle */
     if(u32Counter1 == 10)
     {
       HEARTBEAT_ON();
       /*Reset the timer */
       u32Counter1 = 0;
     }
   }

   /* Change the duty time according to the requirement */
   /* Check if reach the swift-time */
   if(u32Counter2 == 100)
   {
     /* Check the duty time is increasing or decreasing */
     if(bInOrDe == TRUE)
     {
       /* Check if the duty time reach the maximum */
       if(u32DutyTime == 10)
       {
         bInOrDe = FALSE;
         u32DutyTime--;
       }
       else 
       {
         u32DutyTime++;
       }
     }/* end if(bInOrDe == TRUE) */
     else/* If the duty time is decreasing */
     {
       /* Check if the duty time reach the minimum */
       if(u32DutyTime == 0)
       {
         bInOrDe = TRUE;
         u32DutyTime++;
       }
       else
       {
         u32DutyTime--;
       }
     }/* end else */
     /* Reset the timer */
     u32Counter2 = 0;
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
