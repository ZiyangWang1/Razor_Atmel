/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern AntSetupDataType G_stAntSetupData;             /* From ant.c */

extern AntApplicationMessageType G_eAntApiCurrentMessageClass;   /* From ant_api.c */
extern u8 G_au8AntApiCurrentData[ANT_APPLICATION_MESSAGE_BYTES]; /* From ant_api.c */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */


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
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{
  /* Start with all LEDs off */
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  LCDCommand(LCD_CLEAR_CMD);
  
  /* Configure the ANT radio */
  G_stAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  G_stAntSetupData.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  G_stAntSetupData.AntNetwork          = ANT_NETWORK_USERAPP;
  G_stAntSetupData.AntSerialLo         = ANT_SERIAL_LO_USERAPP;
  G_stAntSetupData.AntSerialHi         = ANT_SERIAL_HI_USERAPP;
  G_stAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  G_stAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  G_stAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  G_stAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  G_stAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  G_stAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;
  
  AntChannelConfig(ANT_MASTER);
  AntOpenChannel();
  /* If good initialization, set state to Idle */
  if( 1 /* Add condition for good init */)
  {
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp_StateMachine = UserAppSM_FailedInit;
  }

} /* end UserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserAppRunActiveState(void)
{
  UserApp_StateMachine();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
  static u8 au8CheckAck[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
  static u8 au8TestMessage[] = {0x5b,0,0,0,0xff,0,0,0};
  static u8 au8Display[] = "0000  0000     %in5s";
  static u16 u16CountSent = 0;
  static u16 u16CountMissed = 0;
  static u8 u8CountSentIn5s = 0;
  static u8 u8CountMissedIn5s = 0;
  bool bAckCfm = TRUE;
  
  /* Process ANT message */
  
  if(AntReadData())
  {
    if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      /* Count sent messages and queue the new message data */
      u16CountSent++;
      u16CountMissed++;
      u8CountSentIn5s++;
      u8CountMissedIn5s++;
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      
      au8TestMessage[3]++;
      if(au8TestMessage[3] == 0)
      {
        au8TestMessage[2]++;
        if(au8TestMessage[2] == 0)
        {
          au8TestMessage[1]++;
        }
      }
      
      AntQueueBroadcastMessage(au8TestMessage);
      
      /* Calculate the missing rate in 5 sec */
      if(u8CountSentIn5s == 5 * (32768 / ANT_CHANNEL_PERIOD_DEC_USERAPP))
      {
        au8Display[14] = (((u8CountMissedIn5s *100) / u8CountSentIn5s) % 10) + '0';
        au8Display[13] = (((u8CountMissedIn5s *100) / u8CountSentIn5s) % 100) / 10 + '0';
        au8Display[12] = (((u8CountMissedIn5s *100) / u8CountSentIn5s) % 1000) / 100 + '0';
        u8CountSentIn5s = 0;
        u8CountMissedIn5s = 0;
      }
      
      /* Display sent messages, missed messages and the missing rate */
      au8Display[0] = (u16CountSent % 10000) / 1000 + '0';
      au8Display[1] = (u16CountSent % 1000) / 100 + '0';
      au8Display[2] = (u16CountSent % 100) / 10 + '0';
      au8Display[3] = (u16CountSent % 10) + '0';

      au8Display[6] = (u16CountMissed % 10000) / 1000 + '0';
      au8Display[7] = (u16CountMissed % 1000) / 100 + '0';
      au8Display[8] = (u16CountMissed % 100) / 10 + '0';
      au8Display[9] = (u16CountMissed % 10) + '0';

      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Sent Missed  Rate  ");
      LCDMessage(LINE2_START_ADDR,au8Display);
    }
    
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      /* Process received message */
      for(u8 i = 0; i < 8;i++)
      {
        /* Check whether it is a acknowledged message */
        if(G_au8AntApiCurrentData[i] != au8CheckAck[i])
        {
          bAckCfm = FALSE;
          break;
        }
      }
      
      /* Count down the missed counter if received a acknowledged message */
      if(bAckCfm)
      {
        u16CountMissed--;
        u8CountMissedIn5s--;
        if(au8TestMessage[3] == 0)
        {
          au8TestMessage[2]--;
          if(au8TestMessage[2] == 0)
          {
            au8TestMessage[1]--;
          }
        }
        au8TestMessage[3]--;
      }
    }
  }

} /* end UserAppSM_Idle() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{
  UserApp_StateMachine = UserAppSM_Idle;
  
} /* end UserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
