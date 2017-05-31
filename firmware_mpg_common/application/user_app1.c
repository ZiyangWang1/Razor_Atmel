/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */
extern LedDisplayListHeadType UserApp2_sUserLedCommandList;   /* From user_app2.c */
extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */

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
  u8 au8UserApp1Start1[] = "LED program task started\n\r";
  
  /* Turn off the Debug task command processor and announce the task is ready */
  DebugSetPassthrough();
  DebugPrintf(au8UserApp1Start1);
  
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
Function: OutPutCmdLineList

Description:
Output the user command.

Requires:
  - A pointer point to a bool variable show the output status.
  - A pointer point to another pointer point to the currunt node need to be output.

Promises:
  - Output the UserLedCommandList according to the request.
*/
void OutPutCmdLineList(bool* pbOutputFlag,LedDisplayListNodeType** ppsPresent)
{
  if(*pbOutputFlag)
  {
    DebugPrintf("LED   ON TIME    OFF TIME\n\r-----------------------\n\r");
    *pbOutputFlag=FALSE;
    *ppsPresent=UserApp2_sUserLedCommandList.psFirstCommand;
  }
  DebugPrintf(" ");
  /* Output the LED type */
  switch((*ppsPresent)->eCommand.eLED)
  {
  case RED: DebugPrintf("R\t");break;
  case ORANGE: DebugPrintf("O\t");break;
  case YELLOW: DebugPrintf("Y\t");break;
  case GREEN: DebugPrintf("G\t");break;
  case CYAN: DebugPrintf("C\t");break;
  case BLUE: DebugPrintf("B\t");break;
  case PURPLE: DebugPrintf("P\t");break;
  case WHITE: DebugPrintf("W\t");break;
  default: DebugPrintf("ERROR");
  }
  /* Output the on time */
  DebugPrintNumber((*ppsPresent)->eCommand.u32Time);
  DebugPrintf("\t\t");
  *ppsPresent=(*ppsPresent)->psNextNode;
  /* Output the off time */
  DebugPrintNumber((*ppsPresent)->eCommand.u32Time);
  DebugPrintf("\n\r");
  *ppsPresent=(*ppsPresent)->psNextNode;
  if(*ppsPresent==NULL)
  {
    DebugPrintf("-----------------------\n\r");
    *pbOutputFlag=TRUE;
  }

}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for input */
static void UserApp1SM_Idle(void)
{
  static bool bOutputFlag=FALSE;
  static LedDisplayListNodeType* psPresent=NULL;
  static LedDisplayListNodeType** ppsPresent=&psPresent;
  static bool* pbOutputFlag=&bOutputFlag;
  
  static u32 u32Counter=0;
  
  if(u32Counter==5)
  {
    OutPutCmdLineList(pbOutputFlag,ppsPresent);
    u32Counter=4;
  }
  u32Counter++;
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
