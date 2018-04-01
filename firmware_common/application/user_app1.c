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
  /* Enable PIO of PA16£¬15£¬14£¬13£¬12£¬11 and PB04 */
  AT91C_BASE_PIOA->PIO_PER = PA_16_BLADE_CS;
  AT91C_BASE_PIOA->PIO_PER = PA_15_BLADE_SCK;
  AT91C_BASE_PIOA->PIO_PER = PA_14_BLADE_MOSI;
  AT91C_BASE_PIOA->PIO_PER = PA_13_BLADE_MISO;
  AT91C_BASE_PIOA->PIO_PER = PA_12_BLADE_UPOMI;
  AT91C_BASE_PIOA->PIO_PER = PA_11_BLADE_UPIMO;
  AT91C_BASE_PIOB->PIO_PER = PB_04_BLADE_AN1;
  
  /* Enable output of PA16, 15£¬14£¬13£¬12£¬11 and PB04 */
  AT91C_BASE_PIOA->PIO_OER = PA_16_BLADE_CS;
  AT91C_BASE_PIOA->PIO_OER = PA_15_BLADE_SCK;
  AT91C_BASE_PIOA->PIO_OER = PA_14_BLADE_MOSI;
  AT91C_BASE_PIOA->PIO_OER = PA_13_BLADE_MISO;
  AT91C_BASE_PIOA->PIO_OER = PA_12_BLADE_UPOMI;
  AT91C_BASE_PIOA->PIO_OER = PA_11_BLADE_UPIMO;
  AT91C_BASE_PIOB->PIO_OER = PB_04_BLADE_AN1;
  
  /* Enable Pull-up of PA16, 15£¬14£¬13£¬12£¬11 and PB04 */
  AT91C_BASE_PIOA->PIO_PPUER = PA_16_BLADE_CS;
  AT91C_BASE_PIOA->PIO_PPUER = PA_15_BLADE_SCK;
  AT91C_BASE_PIOA->PIO_PPUER = PA_14_BLADE_MOSI;
  AT91C_BASE_PIOA->PIO_PPUER = PA_13_BLADE_MISO;
  AT91C_BASE_PIOA->PIO_PPUER = PA_12_BLADE_UPOMI;
  AT91C_BASE_PIOA->PIO_PPUER = PA_11_BLADE_UPIMO;
  AT91C_BASE_PIOB->PIO_PPUER = PB_04_BLADE_AN1;
  
  /* Pull up the CS and INC pin */
  AT91C_BASE_PIOA->PIO_SODR = PA_13_BLADE_MISO;
  AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI;
  AT91C_BASE_PIOB->PIO_SODR = PB_04_BLADE_AN1;
  AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
  AT91C_BASE_PIOA->PIO_CODR = PA_15_BLADE_SCK;
  
  LedOn(PURPLE);
 
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



void UserApp_AdcCallback(u16 u16Result_)
{
  Adc12AssignCallback(ADC12_CH2, UserApp_AdcCallback);
}


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
  static u8 u8Timer = 0;
  static u8 u8States = 0;
  static bool bVUp = FALSE;
  static bool bVDown = FALSE;
  static bool bTimerStart = FALSE;
  static bool bAdcReady = FALSE;
  static u16 u16AdcResult = 0;
  static u8 u8VolLevl = 0;
  static float fVolta = 0;
  static u8 au8Display[] = "    .  V";
  
  if(bTimerStart)       // A ms timer
  {
    u8Timer++;
  }
  
  if(u8Timer == 100)    // Reset the timer when reach 100ms
  {
    LedOff(RED);
    bTimerStart = FALSE;
    u8Timer = 0;
  }
  
  if(WasButtonPressed(BUTTON0))         // Set a up-flag when press B0
  {
    ButtonAcknowledge(BUTTON0);
    bVUp = TRUE;
    bTimerStart = TRUE;
    LedOn(RED);
  }
  
  if(WasButtonPressed(BUTTON1))         // Set a down-flag when press B1
  {
    ButtonAcknowledge(BUTTON1);
    bVDown = TRUE;
    bTimerStart = TRUE;
    LedOn(RED);
  }
  
  if(bVUp)      // Handle up-flag
  {
    switch(u8Timer)
    {
    case 0: AT91C_BASE_PIOA->PIO_CODR = PA_13_BLADE_MISO;break;
    case 1: AT91C_BASE_PIOA->PIO_SODR = PA_14_BLADE_MOSI;break;
    case 2: AT91C_BASE_PIOA->PIO_CODR = PA_12_BLADE_UPOMI;break;
    case 3: AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI;break;
    case 4: AT91C_BASE_PIOA->PIO_SODR = PA_13_BLADE_MISO;break;
    case 5: bVUp = FALSE;break;
    default:;
    }
  }
  
  if(bVDown)    // Handle down-flag
  {
    switch(u8Timer)
    {
    case 0: AT91C_BASE_PIOA->PIO_CODR = PA_13_BLADE_MISO;break;
    case 1: AT91C_BASE_PIOA->PIO_CODR = PA_14_BLADE_MOSI;break;
    case 2: AT91C_BASE_PIOA->PIO_CODR = PA_12_BLADE_UPOMI;break;
    case 3: AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI;break;
    case 4: AT91C_BASE_PIOA->PIO_SODR = PA_13_BLADE_MISO;break;
    case 5: bVDown = FALSE;break;
    default:;
    }
  }

  if(WasButtonPressed(BUTTON3)) // Switch status when press B3
  {
    ButtonAcknowledge(BUTTON3);
    bTimerStart = TRUE;
    LedOn(RED);
    switch(u8States)
    {
    case 0: AT91C_BASE_PIOA->PIO_SODR = PA_16_BLADE_CS;
            AT91C_BASE_PIOB->PIO_CODR = PB_04_BLADE_AN1;
            AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
            LedOff(PURPLE);LedOn(BLUE);u8States = 1;break;
    case 1: AT91C_BASE_PIOA->PIO_CODR = PA_16_BLADE_CS;
            AT91C_BASE_PIOB->PIO_CODR = PB_04_BLADE_AN1;
            AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
            LedOff(BLUE);LedOn(GREEN);u8States = 2;break;
    case 2: AT91C_BASE_PIOB->PIO_SODR = PB_04_BLADE_AN1;
            AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
            LedOff(GREEN);LedOn(PURPLE);u8States = 0;break;
    default:;
    }
  }
  
  if(WasButtonPressed(BUTTON2)) // Get ready for ADC when press B2
  {
    ButtonAcknowledge(BUTTON2);
    bTimerStart = TRUE;
    LedOn(RED);
    if(AT91C_BASE_PIOA->PIO_ODSR & PA_15_BLADE_SCK)
    {
     AT91C_BASE_PIOA->PIO_CODR = PA_15_BLADE_SCK;
     LedOff(WHITE);
     AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
     switch(u8States)
     {
     case 0:LedOn(PURPLE);break;
     case 1:LedOn(BLUE);break;
     case 2:LedOn(GREEN);break;
     default:;
     }
    }
    else
    {
      AT91C_BASE_PIOA->PIO_SODR = PA_15_BLADE_SCK;
      AT91C_BASE_PIOA->PIO_SODR = PA_11_BLADE_UPIMO;
      LedOff(GREEN);
      LedOff(BLUE);
      LedOff(PURPLE);
      LedOn(WHITE);
      bTimerStart = TRUE; 
      bAdcReady = TRUE;
    }
  }
  
  if(bAdcReady && u8Timer == 50)        // Convert after 50ms
  {
    Adc12StartConversion(ADC12_CH2);
  }
  
  if(bAdcReady && u8Timer == 99)        // Display after 99ms
  {
    bAdcReady = FALSE;
    u16AdcResult = AT91C_BASE_ADC12B->ADC12B_CDR[2];
    u8VolLevl = u16AdcResult / 41;
    fVolta = u16AdcResult;
    fVolta *= 3.3;
    fVolta /= 4095;
    au8Display[0] = u8VolLevl / 10 + '0';
    au8Display[1] = u8VolLevl % 10 + '0';
    au8Display[3] = (u8)fVolta + '0';
    au8Display[5] = (u8)(fVolta * 10) % 10 + '0';
    au8Display[6] = (u16)(fVolta * 100) % 10 + '0';
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,au8Display);
  }
  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
