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
extern u32 G_u32AntApiCurrentMessageTimeStamp;                    /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;              /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                        /* Timeout counter used across states */

static AntAssignChannelInfoType UserApp1_CHANNEL1_sChannelInfo; /* ANT setup parameters */
static AntAssignChannelInfoType UserApp1_CHANNEL2_sChannelInfo; /* ANT setup parameters */

static u8 UserApp1_au8MessageFail1[] = "\n\r***ANT channel 1 setup failed***\n\n\r";
static u8 UserApp1_au8MessageFail2[] = "\n\r***ANT channel 2 setup failed***\n\n\r";



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
  u8 au8ANTPlusNetworkKey[] = {0xB9,0xA5,0x21,0xFB,0xBD,0x72,0xC3,0x45};
  
  LCDCommand(LCD_CLEAR_CMD);
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);


 /* Configure Channel 1 for this application */
  UserApp1_CHANNEL1_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP_CHANNEL1;
 
  UserApp1_CHANNEL1_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP_CHANNEL1;
  UserApp1_CHANNEL1_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP_CHANNEL1;

  UserApp1_CHANNEL1_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_CHANNEL1_sChannelInfo.AntNetworkKey[i] = au8ANTPlusNetworkKey[i];
  }
  
 /* Configure Channel 2 for this application */
  UserApp1_CHANNEL2_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP_CHANNEL2;
 
  UserApp1_CHANNEL2_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP_CHANNEL2;
  UserApp1_CHANNEL2_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP_CHANNEL2;

  UserApp1_CHANNEL2_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_CHANNEL2_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
  /* Attempt to queue the ANT channel 1 setup */
  if( AntAssignChannel(&UserApp1_CHANNEL1_sChannelInfo) )
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_AntChannelAssign;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    DebugPrintf(UserApp1_au8MessageFail1);
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
/* Wait for ANT channel assignment */
static void UserApp1SM_AntChannelAssign()
{
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP_CHANNEL1) == ANT_CONFIGURED )
  {
    /* Attempt to queue the ANT channel 2 setup */
    if( AntAssignChannel(&UserApp1_CHANNEL2_sChannelInfo) )
    {
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_AntMasterChannelAssign;
    }
    else
    {
      /* The task isn't properly initialized, so shut it down and don't run */
      DebugPrintf(UserApp1_au8MessageFail2);
      UserApp1_StateMachine = UserApp1SM_Error;
    }
  }
  else
  {
    /* Watch for time out */
    if(IsTimeUp(&UserApp1_u32Timeout, 3000))
    {
      DebugPrintf(UserApp1_au8MessageFail1);
      UserApp1_StateMachine = UserApp1SM_Error;    
    }
  }
     
} /* end UserApp1SM_AntChannelAssign */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT master channel assignment */
static void UserApp1SM_AntMasterChannelAssign(void)
{
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP_CHANNEL1) == ANT_CONFIGURED )
  {
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitForPairing;
  }
  else
  {
    /* Watch for time out */
    if(IsTimeUp(&UserApp1_u32Timeout, 3000))
    {
      DebugPrintf(UserApp1_au8MessageFail2);
      UserApp1_StateMachine = UserApp1SM_Error;    
    }
  }

} /* end UserApp1SM_AntMasterChannelAssign */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for pairing */
static void UserApp1SM_WaitForPairing(void)
{
  static bool bDisplayed = FALSE;
  static bool bHRMPaired = FALSE;
  static bool bControlPaired = FALSE;
  static u8 au8TestMessage[] = {0,0,0,0,0,0,0,0};
  static u16 u16Timer = 0;
  
  if(u16Timer == 1000)
  {
    if(!bDisplayed)
    {
      AntOpenChannelNumber(ANT_CHANNEL_USERAPP_CHANNEL1);
      AntOpenChannelNumber(ANT_CHANNEL_USERAPP_CHANNEL2);
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Waiting for pairing");
      LCDMessage(LINE2_START_ADDR,".  .  .  .  .  .");
      bDisplayed = TRUE;
    }
    
    if( AntReadAppMessageBuffer() )
    {
       /* New message from ANT task: check what it is */
      if(G_eAntApiCurrentMessageClass == ANT_DATA)
      {
        if(G_sAntApiCurrentMessageExtData.u8Channel == 1)
        {
          bHRMPaired = TRUE;
        }
        
        if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
        {
          if(G_au8AntApiCurrentMessageBytes[0] == 0)
          {
            bControlPaired = TRUE;
          }
        }
      }
      else if(G_eAntApiCurrentMessageClass == ANT_TICK)
      {
       /* Update and queue the new message data */
        au8TestMessage[7]++;
        if(au8TestMessage[7] == 0)
        {
          au8TestMessage[6]++;
          if(au8TestMessage[6] == 0)
          {
            au8TestMessage[5]++;
          }
        }
        AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
      }
    }
    
    if(/*bHRMPaired && */bControlPaired)
    {
      DebugPrintf("HRM and remote control paired\n\r");
      UserApp1_StateMachine = UserApp1SM_Idle;
    }
  }
  else
  {
    u16Timer++;
  }

  
} /* end UserApp1SM_WaitForParing */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for commands */
static void UserApp1SM_Idle(void)
{
  static bool bDisplayed = FALSE;
  static u8 au8TestMessage[] = {0xFF, 0, 0, 0, 0, 0, 0, 0};
  
  if(!bDisplayed)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Channel paired !");
    LCDMessage(LINE2_START_ADDR,"Waiting for commands");
    bDisplayed = TRUE;
  }
  
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        switch(G_au8AntApiCurrentMessageBytes[0])
        {
        case 1: UserApp1_StateMachine = UserApp1SM_Function1;
                bDisplayed = FALSE;
                break;
        case 2: UserApp1_StateMachine = UserApp1SM_Function2;
                bDisplayed = FALSE;
                break;
        case 3: UserApp1_StateMachine = UserApp1SM_Function3;
                bDisplayed = FALSE;
                break;
        case 4: UserApp1_StateMachine = UserApp1SM_Function4;
                bDisplayed = FALSE;
                break;
        default: ;
        }
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
    }
  } /* end AntReadData() */
  
} /* end UserApp1SM_Idle() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 1 */
static void UserApp1SM_Function1(void)
{
  static bool bDisplayed = FALSE;
  static u8 au8TestMessage[] = {1, 0, 0, 0, 0, 0, 0, 0};
  
  if(!bDisplayed)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"  Ave. rate:   bpm");
    LCDMessage(LINE2_START_ADDR," Time: 0h00min00sec");
    bDisplayed = TRUE;
  }
  
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        if(G_au8AntApiCurrentMessageBytes[0] == 0xFF)
        {
          bDisplayed = FALSE;
          UserApp1_StateMachine = UserApp1SM_Idle;
        }
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
    }
  } /* end AntReadData() */

} /* end UserApp1SM_Function1() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 2 */
static void UserApp1SM_Function2(void)
{
  static bool bDisplayed = FALSE;
  static u8 au8TestMessage[] = {2, 0, 0, 0, 0, 0, 0, 0};
  
  if(!bDisplayed)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Max:     Min:    bpm");
    LCDMessage(LINE2_START_ADDR," Time: 0h00min00sec");
    bDisplayed = TRUE;
  }
  
  if( AntReadAppMessageBuffer() )
  {
     /* New message from ANT task: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      if(G_sAntApiCurrentMessageExtData.u8Channel == 2)
      {
        if(G_au8AntApiCurrentMessageBytes[0] == 0xFF)
        {
          bDisplayed = FALSE;
          UserApp1_StateMachine = UserApp1SM_Idle;
        }
      }
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* Update and queue the new message data */
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP_CHANNEL2, au8TestMessage);
    }
  } /* end AntReadData() */
} /* end UserApp1SM_Function2() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 3 */
static void UserApp1SM_Function3(void)
{
  
} /* end UserApp1SM_Function3() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Realize function 4 */
static void UserApp1SM_Function4(void)
{
  
} /* end UserApp1SM_Function4() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error (for now, do nothing) */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
