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
  
  /* Enable PIO and output */
  AT91C_BASE_PIOA->PIO_PER = PA_00_GND;
  AT91C_BASE_PIOA->PIO_PER = PA_03_CD_STB;
  AT91C_BASE_PIOA->PIO_PER = PA_04_INH;
  AT91C_BASE_PIOA->PIO_PER = PA_05_A;
  AT91C_BASE_PIOA->PIO_PER = PA_06_B;
  AT91C_BASE_PIOA->PIO_PER = PA_07_C;
  AT91C_BASE_PIOA->PIO_PER = PA_08_D;
  AT91C_BASE_PIOA->PIO_PER = PA_11_M_OE;
  AT91C_BASE_PIOA->PIO_PER = PA_12_M_LE;
  AT91C_BASE_PIOA->PIO_PER = PA_14_M_SDI;
  AT91C_BASE_PIOA->PIO_PER = PA_15_M_CLK;
  AT91C_BASE_PIOB->PIO_PER = PB_05_F_CS;
  AT91C_BASE_PIOB->PIO_PER = PB_06_F_SDO;
  AT91C_BASE_PIOB->PIO_PER = PB_07_SCLK;
  AT91C_BASE_PIOB->PIO_PER = PB_08_SI;
  
  AT91C_BASE_PIOA->PIO_OER = PA_00_GND;
  AT91C_BASE_PIOA->PIO_OER = PA_03_CD_STB;
  AT91C_BASE_PIOA->PIO_OER = PA_04_INH;
  AT91C_BASE_PIOA->PIO_OER = PA_05_A;
  AT91C_BASE_PIOA->PIO_OER = PA_06_B;
  AT91C_BASE_PIOA->PIO_OER = PA_07_C;
  AT91C_BASE_PIOA->PIO_OER = PA_08_D;
  AT91C_BASE_PIOA->PIO_OER = PA_11_M_OE;
  AT91C_BASE_PIOA->PIO_OER = PA_12_M_LE;
  AT91C_BASE_PIOA->PIO_OER = PA_14_M_SDI;
  AT91C_BASE_PIOA->PIO_OER = PA_15_M_CLK;
  AT91C_BASE_PIOB->PIO_OER = PB_05_F_CS;
  AT91C_BASE_PIOB->PIO_OER = PB_06_F_SDO;
  AT91C_BASE_PIOB->PIO_OER = PB_07_SCLK;
  AT91C_BASE_PIOB->PIO_OER = PB_08_SI;
  
  /* Initialize the GPIO ports */
  
  AT91C_BASE_PIOA->PIO_CODR = PA_00_GND;
  AT91C_BASE_PIOA->PIO_SODR = PA_03_CD_STB;
  AT91C_BASE_PIOA->PIO_CODR = PA_04_INH;
  AT91C_BASE_PIOA->PIO_CODR = PA_05_A;
  AT91C_BASE_PIOA->PIO_CODR = PA_06_B;
  AT91C_BASE_PIOA->PIO_CODR = PA_07_C;
  AT91C_BASE_PIOA->PIO_CODR = PA_08_D;
  AT91C_BASE_PIOA->PIO_CODR = PA_11_M_OE;
  AT91C_BASE_PIOA->PIO_SODR = PA_12_M_LE;
  AT91C_BASE_PIOA->PIO_CODR = PA_14_M_SDI;
  AT91C_BASE_PIOA->PIO_CODR = PA_15_M_CLK;
  AT91C_BASE_PIOB->PIO_SODR = PB_05_F_CS;
  AT91C_BASE_PIOB->PIO_CODR = PB_06_F_SDO;
  AT91C_BASE_PIOB->PIO_CODR = PB_07_SCLK;
  AT91C_BASE_PIOB->PIO_CODR = PB_08_SI;
  
  
  /* If good initialization, set state to Idle */
  if(1)
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

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1_CD4515BM_ChangeData

Description:
Change the data in the CD4515BM latch and send a falling edge

Requires:
  - The waitting data
  - The STB must be high

Promises:
  - Change the data in the CD4515BM latch
*/
static void UserApp1_CD4515BM_ChangeData(u8 u8Data)
{
  if(u8Data / 8)
  {
    AT91C_BASE_PIOA->PIO_SODR = PA_08_D;
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_08_D;
  }
  
  if((u8Data % 8) / 4)
  {
    AT91C_BASE_PIOA->PIO_SODR = PA_07_C;
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_07_C;
  }
  
  if((u8Data % 4) / 2)
  {
    AT91C_BASE_PIOA->PIO_SODR = PA_06_B;
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_06_B;
  }
  
  if(u8Data % 2)
  {
    AT91C_BASE_PIOA->PIO_SODR = PA_05_A;
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_05_A;
  }
  
  AT91C_BASE_PIOA->PIO_CODR = PA_03_CD_STB;
} /* end UserApp1_CD4515BM_ChangeData */

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u8 u8CD4515BMTimeCounter = 0;
  static u8 u8CD4515Data = 0;
  static bool bMBI5026GFDataSend = FALSE;
  static u8 u8MBI5026GFCounter = 0;
  
  if(!(AT91C_BASE_PIOA->PIO_ODSR & PA_15_BLADE_SCK))
  {
    AT91C_BASE_PIOA->PIO_SODR = PA_03_CD_STB;
  }
  
  u8CD4515BMTimeCounter++;
  UserApp1_CD4515BM_ChangeData(u8CD4515Data);
  u8CD4515Data++;
  if(u8CD4515Data == 16)
  {
    u8CD4515Data = 0;
  }
  
  if(!bMBI5026GFDataSend)
  {
      u8MBI5026GFCounter++;
      switch(u8MBI5026GFCounter)
      {
      case 1:;
      case 3:;
      case 5:;
      case 7:;
      case 9:AT91C_BASE_PIOA->PIO_SODR = PA_15_M_CLK;break;
      
      case 2:;
      case 4:;
      case 6:;
      case 8:;
      case 10: AT91C_BASE_PIOA->PIO_SODR = PA_14_M_SDI;
               AT91C_BASE_PIOA->PIO_CODR = PA_15_M_CLK;
               break;
      default: u8MBI5026GFCounter = 0;bMBI5026GFDataSend = TRUE;AT91C_BASE_PIOA->PIO_CODR = PA_12_M_LE;
      }
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
