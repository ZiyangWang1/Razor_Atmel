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
  /* Enable PIO and output of PA11 and PB03 */
  AT91C_BASE_PIOA->PIO_PER = PA_11_BLADE_UPIMO;
  AT91C_BASE_PIOB->PIO_PER = PB_03_BLADE_AN0;
  AT91C_BASE_PIOA->PIO_OER = PA_11_BLADE_UPIMO;
  AT91C_BASE_PIOB->PIO_OER = PB_03_BLADE_AN0;
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
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
                                        /* OLD MEMORY HIGH */                                                                                                                                                         /*GG*/                                                                                                                                                                                                                                                                          /* MLH*/
  static u16 au16NoteBuzzer1[] = /*{NO,C6,D6,E6,G6,D6,A5,B5,C6,D6,E6,F6,E6,NO,E6,D6,E6,G6,D6,A5,B5,C6,B5,A5,G5,E5,NO,C6,D6,E6,G6,D6,A5,B5,C6,D6,E6,F6,E6,NO,E6,C6,C6,E6,D6,B5,C6,B5,A5,NO}*/ /*{NO,D4,G4,G4,G4,G4,D4,E4,F4,G4,G4,NO,B4,G4,A4,B4,D5,D5,B4,B4,G4,B4,D5,B4,A4,A4,NO,E5,D5,A4,B4,D5,B4,NO,D5,B4,A4,B4,G4,B4,NO,D4,E4,G4,G4,B4,B4,D5,D5,A4,A4,A4,E4,A4,D4,G4,G4,B4,B4,D5,NO,G4,B4,D5,D5,E5,D5,B4,G4,D5,B4,NO,G4,NO,D4,G4,B4,G4,D5,B4,NO,G4,NO,D4,G4,D4,G4,D4,G4,G4,NO}*/{F4,NO,F4,A4,B4,D5,D5,B4,A4,NO,A4,B4,A4,NO,F4,NO,F4,A4,B4,D5,NO,D5,B4,A4,NO,A4,B4,A4,NO,A4,NO,A4,F4,A4,B4,NO,B4,A4,NO,F4,E4,F4,A4,F4,E4,D4,NO,D4,E4,D4,NO,F4,E4,D4,F4,E4,F4,A4,B4,D5,A4,NO,E4,F4,A4,E4,F4,D4,B3,A3,NO,B3,D4,E4,F4,D4,E4,D4,B3,A3,NO,NO,NO};
                                        /* OLD MEMORY LOW */
  //static u16 au16NoteBuzzer2[] = {NO,A4,NO,G4,NO,F4,NO,C4,NO,A4,NO,G4,NO,F4,G4,C5,NO,A4,NO,G4,NO,F4,NO,C4,NO,D4,NO,E4,NO,F4,NO};
  static u16 au16LengthBuzzer1[] = /*{FN,EN,EN,EN,EN,QN,EN,EN,EN,EN,EN,EN,QN,QN,EN,EN,EN,EN,QN,EN,EN,EN,EN,EN,EN,QN,QN,EN,EN,EN,EN,QN,EN,EN,EN,EN,EN,EN,QN,QN,EN,EN,EN,EN,QN,QN,EN,EN,FN,QN*3}*//*{EN,EN,QN+EN,EN,EN+SN,SN,EN,SN,SN,QN,QN,EN,EN,EN,SN,SN,QN,QN,EN+SN,SN,EN+SN,SN,EN+SN,SN,QN,QN,QN,QN,QN,QN,QN,EN,EN,EN,EN,EN,SN,SN,QN,QN,QN,EN+SN,SN,EN,EN,EN+SN,SN,EN,EN,EN,SN,SN,QN,QN+EN,EN,QN+EN,EN,QN+EN,EN,QN,QN,EN+SN,SN,EN,EN,QN,QN,EN+SN,SN,QN,EN,EN,EN,EN,QN,QN,EN+SN,SN,QN,EN,EN,EN,EN,QN,QN,QN,QN,QN,QN,QN,QN}*/{QN,QN,QN,EN,EN,EN,EN,EN,EN,EN,EN,EN,QN,QN,EN,EN,EN,EN,EN,SN,SN,EN,EN,EN,EN,EN,EN,QN,QN,QN,QN,QN,EN,EN,QN,EN,EN,QN,QN,QN,EN,EN,QN,EN,EN,EN,EN,EN,EN,QN,QN,EN,EN,EN,EN,QN+EN,EN,QN,EN,EN,QN,QN,QN,EN,EN,EN,EN,EN,EN,QN,QN,QN,QN,QN+EN,EN,EN,EN,EN,EN,QN,QN,QN,QN};
  //static u16 au16LengthBuzzer2[] = {FN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN,QN*5,QN*3};
  static u16 u16Counter1 = 0;
  //static u16 u16Counter2 = 0;
  static u16 u16NoteCount1 = 0;
  //static u16 u16NoteCount2 = 0;
  
  u16Counter1++;
  //u16Counter2++;
  
  /* Check if the button is pressed */
  if((AT91C_BASE_PIOA->PIO_PDSR & PA_15_BLADE_SCK)&&(AT91C_BASE_PIOA->PIO_PDSR & PA_17_BUTTON0))
  {
    /* Turn off buzzers and LEDs when not pressed */
    //AT91C_BASE_PIOB->PIO_CODR = PB_20_LED_RED;
    PWMAudioOff(BUZZER1);
    PWMAudioOff(BUZZER2);
    AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
    AT91C_BASE_PIOB->PIO_CODR = PB_03_BLADE_AN0;
  }
  
  else 
  {
    /* Turn on buzzers and blink LEDs when pressed */
    //AT91C_BASE_PIOB->PIO_SODR = PB_20_LED_RED;
    PWMAudioOn(BUZZER1);
    PWMAudioOn(BUZZER2);
    
    /* Change LEDs when change notes */
    if(u16Counter1 == au16LengthBuzzer1[u16NoteCount1] / 2)
    {
      AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
      AT91C_BASE_PIOB->PIO_SODR = PB_03_BLADE_AN0;
    }

    if(u16Counter1 == au16LengthBuzzer1[u16NoteCount1])
    {
      AT91C_BASE_PIOA->PIO_SODR = PA_11_BLADE_UPIMO;
      AT91C_BASE_PIOB->PIO_CODR = PB_03_BLADE_AN0;
    }
  }
  
  /* Change notes when reach the notes' length */
  if(u16Counter1 == au16LengthBuzzer1[u16NoteCount1])
  {
    u16Counter1 = 0;
    u16NoteCount1++;
    PWMAudioSetFrequency(BUZZER1,au16NoteBuzzer1[u16NoteCount1]);
  }
  
  /*if(u16Counter2 == au16LengthBuzzer2[u16NoteCount2])
  {
    u16Counter2 = 0;
    u16NoteCount2++;
    PWMAudioSetFrequency(BUZZER2,au16NoteBuzzer2[u16NoteCount2]);
  }*/
  
  /* Cycle the music */
  if(u16NoteCount1 == (sizeof(au16LengthBuzzer1)/2-1))
  {
    u16NoteCount1 = 0;
  }
  
  /*if(u16NoteCount2 == (sizeof(au16LengthBuzzer2)/2-1))
  {
    u16NoteCount2 = 0;
  }*/
  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
