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
extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 au8LastAntData[ANT_APPLICATION_MESSAGE_BYTES] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static u8 au8Buffer[16][2];

TWIPeripheralType FM24CL16B;


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
  AntAssignChannelInfoType sAntSetupData;
  
  /* Configure ANT as a slave */
  sAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  sAntSetupData.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  sAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  
  sAntSetupData.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sAntSetupData.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;

  sAntSetupData.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sAntSetupData.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
  LedOff(BLUE);
  
  /* Enable PIO and output */
  AT91C_BASE_PIOA->PIO_PER = PA_00_GND;
  AT91C_BASE_PIOA->PIO_PER = PA_05_CD_STB;
  AT91C_BASE_PIOA->PIO_PER = PA_08_INH;
  AT91C_BASE_PIOA->PIO_PER = PA_03_A;
  AT91C_BASE_PIOA->PIO_PER = PA_07_B;
  AT91C_BASE_PIOA->PIO_PER = PA_06_C;
  AT91C_BASE_PIOA->PIO_PER = PA_04_D;
  AT91C_BASE_PIOA->PIO_PER = PA_09_I2C_M_SCL;
  AT91C_BASE_PIOA->PIO_PER = PA_10_I2C_M_SDA;
  AT91C_BASE_PIOA->PIO_PER = PA_12_M_OE;
  AT91C_BASE_PIOA->PIO_PER = PA_11_M_LE;
  AT91C_BASE_PIOA->PIO_PER = PA_15_M_SDI;
  AT91C_BASE_PIOA->PIO_PER = PA_14_M_CLK;
  AT91C_BASE_PIOB->PIO_PER = PB_06_F_CS;
  AT91C_BASE_PIOB->PIO_PER = PB_05_F_SDO;
  AT91C_BASE_PIOB->PIO_PER = PB_08_SCLK;
  AT91C_BASE_PIOB->PIO_PER = PB_07_SI;
  
  AT91C_BASE_PIOA->PIO_OER = PA_00_GND;
  AT91C_BASE_PIOA->PIO_OER = PA_05_CD_STB;
  AT91C_BASE_PIOA->PIO_OER = PA_08_INH;
  AT91C_BASE_PIOA->PIO_OER = PA_03_A;
  AT91C_BASE_PIOA->PIO_OER = PA_07_B;
  AT91C_BASE_PIOA->PIO_OER = PA_06_C;
  AT91C_BASE_PIOA->PIO_OER = PA_04_D;
  AT91C_BASE_PIOA->PIO_OER = PA_09_I2C_M_SCL;
  AT91C_BASE_PIOA->PIO_OER = PA_10_I2C_M_SDA;
  AT91C_BASE_PIOA->PIO_OER = PA_12_M_OE;
  AT91C_BASE_PIOA->PIO_OER = PA_11_M_LE;
  AT91C_BASE_PIOA->PIO_OER = PA_15_M_SDI;
  AT91C_BASE_PIOA->PIO_OER = PA_14_M_CLK;
  AT91C_BASE_PIOB->PIO_OER = PB_06_F_CS;
  AT91C_BASE_PIOB->PIO_OER = PB_05_F_SDO;
  AT91C_BASE_PIOB->PIO_OER = PB_08_SCLK;
  AT91C_BASE_PIOB->PIO_OER = PB_07_SI;
  
  /* Initialize the GPIO ports */
  
  AT91C_BASE_PIOA->PIO_CODR = PA_00_GND;
  AT91C_BASE_PIOA->PIO_SODR = PA_05_CD_STB;
  AT91C_BASE_PIOA->PIO_CODR = PA_08_INH;
  AT91C_BASE_PIOA->PIO_CODR = PA_03_A;
  AT91C_BASE_PIOA->PIO_CODR = PA_07_B;
  AT91C_BASE_PIOA->PIO_CODR = PA_06_C;
  AT91C_BASE_PIOA->PIO_CODR = PA_04_D;
  AT91C_BASE_PIOA->PIO_CODR = PA_12_M_OE;
  AT91C_BASE_PIOA->PIO_SODR = PA_11_M_LE;
  AT91C_BASE_PIOA->PIO_CODR = PA_15_M_SDI;
  AT91C_BASE_PIOA->PIO_CODR = PA_14_M_CLK;
  AT91C_BASE_PIOB->PIO_SODR = PB_06_F_CS;
  AT91C_BASE_PIOB->PIO_CODR = PB_05_F_SDO;
  AT91C_BASE_PIOB->PIO_CODR = PB_08_SCLK;
  AT91C_BASE_PIOB->PIO_CODR = PB_07_SI;
  
  
  /* If good initialization, set state to Idle */
  if(AntAssignChannel(&sAntSetupData))
  {
    /* Channel assignment is queued so start timer */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
   
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
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
    AT91C_BASE_PIOA->PIO_SODR = PA_04_D;
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_04_D;
  }
  
  if((u8Data % 8) / 4)
  {
    AT91C_BASE_PIOA->PIO_SODR = PA_06_C;
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_06_C;
  }
  
  if((u8Data % 4) / 2)
  {
    AT91C_BASE_PIOA->PIO_SODR = PA_07_B;
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_07_B;
  }
  
  if(u8Data % 2)
  {
    AT91C_BASE_PIOA->PIO_SODR = PA_03_A;
  }
  else
  {
    AT91C_BASE_PIOA->PIO_CODR = PA_03_A;
  }
  
  AT91C_BASE_PIOA->PIO_CODR = PA_05_CD_STB;
  
  //for(int i=0;i<5;i++);
  
  AT91C_BASE_PIOA->PIO_SODR = PA_05_CD_STB;
  

  
} /* end UserApp1_CD4515BM_ChangeData */

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1_MBI5026GF_SendData

Description:
Send a byte to the MBI5026GF

Requires:
  - The waitting data
  - The M_LE must be high
  - The M_OE must be low

Promises:
  - Send a byte to the MBI5026GF
*/
static void UserApp1_MBI5026GF_SendData(u8 u8Data)
{
  u8 u8Mask = 0x01;
  
  for(int i=0;i<8;i++)
  {
    if(u8Data & (u8Mask << i))
    {
      AT91C_BASE_PIOA->PIO_SODR = PA_15_M_SDI;
    }
    else
    {
      AT91C_BASE_PIOA->PIO_CODR = PA_15_M_SDI;
    }
    
    AT91C_BASE_PIOA->PIO_CODR = PA_14_M_CLK;
    
    for(int i=0;i<5;i++);
    
    AT91C_BASE_PIOA->PIO_SODR = PA_14_M_CLK;
    
    for(int i=0;i<5;i++);
    
  }
  
} /* end UserApp1_MBI5026GF_SendData */

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1_FM24CL16B_SendData

Description:
Send data to the required location of FM24CL16B

Requires:
  - The waitting data
  - The length of the data
  - The required location of FM24CL16B

Promises:
  - Send data to the FM24CL16B
*/
static void UserApp1_FM24CL16B_SendData(u8* pu8Data_,u8 u8DataLength,u8 u8WordLocation,u8 u8PageLocation)
{
  
  
} /* end UserApp1_FM24CL16B_SendData */


/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1_GT21L16S2W_SendData

Description:
Send a byte to the GT21L16S2W

Requires:
  - The waitting data

Promises:
  - Send data to the GT21L16S2W
*/

static void UserApp1_GT21L16S2W_SendData(u8 u8Data)
{
  u8 u8Mask = 0x01;
  
  for(int i=0;i<8;i++)
  {
    if(u8Data & (u8Mask << i))
    {
      AT91C_BASE_PIOA->PIO_SODR = PB_07_SI;
    }
    else
    {
      AT91C_BASE_PIOA->PIO_CODR = PB_07_SI;
    }
    
    AT91C_BASE_PIOA->PIO_CODR = PB_08_SCLK;
    
    for(int i=0;i<5;i++);
    
    AT91C_BASE_PIOA->PIO_SODR = PB_08_SCLK;
    
    for(int i=0;i<5;i++);
    
  }
  
} /* end UserApp1_GT21L16S2W_SendData */


/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1_GT21L16S2W_ReadData

Description:
Read 32 bytes from the GT21L16S2W

Requires:
  - /F_CS must be high after the operation

Promises:
  - Send data to the GT21L16S2W
*/

static void UserApp1_GT21L16S2W_ReadData(void)
{
  for(int j=0;j<32;j++)
  {
    u8 u8Data=0;
    u8 u8Mask=0x80;
    for(int i=0;i<8;i++)
    {
      AT91C_BASE_PIOB->PIO_CODR = PB_08_SCLK;
      
      if(AT91C_BASE_PIOB->PIO_PDSR & PB_05_F_SDO)
      {
        u8Data |=u8Mask;
      }
      
      u8Mask=u8Mask>>1;
      
      AT91C_BASE_PIOB->PIO_SODR = PB_08_SCLK;
      for(int i=0;i<5;i++);
    }
    au8Buffer[j/2][j%2]=u8Data;
  }

} /* end UserApp1_GT21L16S2W_SendData */

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  bool bRollTemp = FALSE;
  static u8 u8CD4515Data = 0;
  static u16 u16RollCounter = 0;
  static bool bGotNewData = FALSE;
  static u8 u8Command = 0;
  static u8 u8MBI5026GFData[11][16][2] = 
  {
    {{0x00,0x80},
{0x20,0x40},
{0x10,0x40},
{0x17,0xFC},
{0x00,0x10},
{0x02,0x10},
{0xF1,0x20},
{0x10,0xA0},
{0x10,0x40},
{0x10,0xA0},
{0x11,0x10},
{0x12,0x08},
{0x14,0x08},
{0x28,0x00},
{0x47,0xFE},
{0x00,0x00},
    //"这",0
  },
  
    {{0x1F,0xF0},
{0x10,0x10},
{0x10,0x10},
{0x1F,0xF0},
{0x10,0x10},
{0x10,0x10},
{0x1F,0xF0},
{0x00,0x00},
{0xFF,0xFE},
{0x01,0x00},
{0x11,0x00},
{0x11,0xF8},
{0x11,0x00},
{0x29,0x00},
{0x45,0x00},
{0x83,0xFE},
    //"是",1
  },
  
    {{0x01,0x00},
{0x01,0x00},
{0x02,0x80},
{0x04,0x40},
{0x08,0x20},
{0x10,0x10},
{0x21,0x08},
{0xC1,0x06},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
    //"个",2
  },
  
  {{0x00,0x00},
{0x3F,0xF0},
{0x02,0x00},
{0x02,0x00},
{0x02,0x00},
{0x02,0x00},
{0x7F,0xFC},
{0x04,0x80},
{0x04,0x80},
{0x04,0x80},
{0x08,0x80},
{0x08,0x80},
{0x10,0x84},
{0x20,0x84},
{0x40,0x7C},
{0x80,0x00},/*"无",3*/
  },
  
  {{0x10,0x50},
{0x10,0x48},
{0x20,0x40},
{0x24,0x5C},
{0x45,0xE0},
{0xF8,0x40},
{0x10,0x5E},
{0x23,0xE0},
{0x40,0x44},
{0xFC,0x48},
{0x40,0x30},
{0x00,0x22},
{0x1C,0x52},
{0xE0,0x8A},
{0x43,0x06},
{0x00,0x02},/*"线",4*/
  },
  
    {{0x00,0x80},
{0x20,0x40},
{0x17,0xFC},
{0x11,0x10},
{0x82,0x48},
{0x44,0x84},
{0x41,0x20},
{0x13,0xF0},
{0x10,0x10},
{0x20,0x88},
{0xE1,0x50},
{0x23,0x20},
{0x25,0x10},
{0x29,0x48},
{0x21,0x86},
{0x01,0x00},
    //"滚",5
  },

    {{0x00,0x40},
{0x00,0x40},
{0x7C,0x40},
{0x00,0x40},
{0x01,0xFC},
{0x00,0x44},
{0xFE,0x44},
{0x20,0x44},
{0x20,0x44},
{0x20,0x84},
{0x48,0x84},
{0x44,0x84},
{0xFD,0x04},
{0x45,0x04},
{0x02,0x28},
{0x04,0x10},
//"动",6
  },

    {{0x00,0x00},
{0x1F,0xF0},
{0x10,0x10},
{0x10,0x10},
{0x1F,0xF0},
{0x10,0x10},
{0x10,0x10},
{0x1F,0xF0},
{0x04,0x40},
{0x44,0x44},
{0x24,0x44},
{0x14,0x48},
{0x14,0x50},
{0x04,0x40},
{0xFF,0xFE},
{0x00,0x00},
//"显",7
  },

    {{0x00,0x00},
{0x3F,0xF8},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0xFF,0xFE},
{0x01,0x00},
{0x01,0x00},
{0x11,0x10},
{0x11,0x08},
{0x21,0x04},
{0x41,0x02},
{0x81,0x02},
{0x05,0x00},
{0x02,0x00},
//"示",8
  },

    {{0x00,0x00},
{0x3F,0xF8},
{0x20,0x08},
{0x20,0x08},
{0x3F,0xF8},
{0x24,0x10},
{0x22,0x20},
{0x2F,0xF8},
{0x22,0x20},
{0x22,0x20},
{0x3F,0xFC},
{0x22,0x20},
{0x42,0x20},
{0x44,0x20},
{0x84,0x20},
{0x08,0x20},
//"屏",9
  },

    {{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x18,0x00},
{0x24,0x00},
{0x24,0x00},
{0x18,0x00},
{0x00,0x00},
{0x00,0x00},
//"。",10
  },
  };
  
  /* Check if got new data */
  if( AntReadAppMessageBuffer() )
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      bGotNewData = FALSE;
      for(u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++)
      {
        if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i])
        {
          bGotNewData = TRUE;
          au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i];
        }
      }
    }
    /* When got new data, apply the command */
    if(bGotNewData)
    {
      u8Command=au8LastAntData[0];
    }
  }
    
  AT91C_BASE_PIOA->PIO_SODR = PA_12_M_OE;
  
  if(u8Command & 0x01)
  {
  u16RollCounter++;

  if(u16RollCounter == 256)
  {
    u16RollCounter = 0;
    for(int j=0;j<16;j++)
    {
      bRollTemp = (bool)(u8MBI5026GFData[0][j][0] & 0x80);
      for(int i=0;i<11;i++)
      {
        u8MBI5026GFData[i][j][0] = u8MBI5026GFData[i][j][0] << 1;
        
        if(u8MBI5026GFData[i][j][1] & 0x80)
        {
          u8MBI5026GFData[i][j][0] = u8MBI5026GFData[i][j][0] | 0x01;
        }
        else
        {
          u8MBI5026GFData[i][j][0] = u8MBI5026GFData[i][j][0] & 0xFE;
        }
        
        u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] << 1;
        
        if(i == 10)
        {
          if(bRollTemp)
          {
            u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] | 0x01;
          }
          else
          {
            u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] & 0xFE;
          }
        }
        else
        {
          if(u8MBI5026GFData[i+1][j][0] & 0x80)
          {
            u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] | 0x01;
          }
          else
          {
            u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] & 0xFE;
          }
        }
        
      }
    }
  }
  }
  
  
  UserApp1_CD4515BM_ChangeData(u8CD4515Data);
  
  for(int i=5;i>0;i--)
  {
    UserApp1_MBI5026GF_SendData(u8MBI5026GFData[i-1][u8CD4515Data][1]);
    UserApp1_MBI5026GF_SendData(u8MBI5026GFData[i-1][u8CD4515Data][0]);
  }

  AT91C_BASE_PIOA->PIO_SODR = PA_11_M_LE;
  
  for(int j=0;j<5;j++);
  
  AT91C_BASE_PIOA->PIO_CODR = PA_11_M_LE;
  u8CD4515Data++;
  
  if(u8CD4515Data == 16)
  {
    u8CD4515Data = 0;
  }
  
  AT91C_BASE_PIOA->PIO_CODR = PA_12_M_OE;
 
  if(WasButtonPressed(BUTTON0)||(u8Command &0x02))
  {
    ButtonAcknowledge(BUTTON0);
    UserApp1_StateMachine = UserApp1SM_Show;
  }
} /* end UserApp1SM_Idle() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Show(void)
{
  bool bRollTemp = FALSE;
  static u8 u8CD4515Data = 0;
  static u16 u16RollCounter = 0;
  static bool bGotNewData = FALSE;
  static u8 u8Command = 0;
  static u8 u8MBI5026GFData[11][16][2] = 
  {
    {{0x00,0x80},
{0x00,0x80},
{0xFC,0x80},
{0x04,0xFC},
{0x05,0x04},
{0x49,0x08},
{0x2A,0x40},
{0x14,0x40},
{0x10,0x40},
{0x28,0xA0},
{0x24,0xA0},
{0x45,0x10},
{0x81,0x10},
{0x02,0x08},
{0x04,0x04},
{0x08,0x02},/*"欢",0*/
  },
  
    {{0x00,0x00},
{0x20,0x80},
{0x13,0x3C},
{0x12,0x24},
{0x02,0x24},
{0x02,0x24},
{0xF2,0x24},
{0x12,0x24},
{0x12,0x24},
{0x12,0xB4},
{0x13,0x28},
{0x12,0x20},
{0x10,0x20},
{0x28,0x20},
{0x47,0xFE},
{0x00,0x00},/*"迎",1*/
  },
  
    {{0x04,0x00},
{0x04,0x00},
{0x0F,0xF0},
{0x18,0x20},
{0x24,0xC0},
{0x03,0x00},
{0x0C,0xC0},
{0x30,0x30},
{0xC0,0x0E},
{0x1F,0xF0},
{0x10,0x10},
{0x10,0x10},
{0x10,0x10},
{0x10,0x10},
{0x1F,0xF0},
{0x10,0x10},/*"各",2*/
  },
  
    {{0x08,0x80},
{0x08,0x40},
{0x08,0x40},
{0x10,0x00},
{0x17,0xFC},
{0x30,0x00},
{0x30,0x08},
{0x52,0x08},
{0x92,0x08},
{0x11,0x10},
{0x11,0x10},
{0x11,0x10},
{0x11,0x20},
{0x10,0x20},
{0x1F,0xFE},
{0x10,0x00},/*"位",3*/
  },
  
    {{0x02,0x00},
{0x02,0x08},
{0x3F,0xD0},
{0x02,0x20},
{0x02,0x40},
{0xFF,0xFE},
{0x01,0x00},
{0x02,0x00},
{0x0C,0x10},
{0x18,0xE0},
{0x2F,0x00},
{0x48,0x08},
{0x88,0x08},
{0x08,0x08},
{0x07,0xF8},
{0x00,0x00},/*"老",4*/
  },

    {{0x08,0x00},
{0x0B,0xFE},
{0x48,0x20},
{0x48,0x20},
{0x48,0x20},
{0x49,0xFC},
{0x49,0x24},
{0x49,0x24},
{0x49,0x24},
{0x49,0x24},
{0x49,0x24},
{0x09,0x34},
{0x11,0x28},
{0x10,0x20},
{0x20,0x20},
{0x40,0x20},/*"师",5*/
  },

    {{0x10,0x10},
{0x08,0x10},
{0x08,0x20},
{0xFF,0xFE},
{0x00,0x00},
{0x3E,0x08},
{0x22,0x48},
{0x22,0x48},
{0x3E,0x48},
{0x22,0x48},
{0x22,0x48},
{0x3E,0x48},
{0x22,0x08},
{0x22,0x08},
{0x2A,0x28},
{0x24,0x10},/*"前",6*/
  },

    {{0x01,0x00},
{0x01,0x00},
{0x01,0x00},
{0x7F,0xFC},
{0x01,0x00},
{0x11,0x10},
{0x09,0x10},
{0x09,0x20},
{0xFF,0xFE},
{0x03,0x80},
{0x05,0x40},
{0x09,0x20},
{0x31,0x18},
{0xC1,0x06},
{0x01,0x00},
{0x01,0x00},/*"来",7*/
  },

    {{0x02,0x00},
{0x04,0x40},
{0x08,0x20},
{0x1F,0xF0},
{0x02,0x00},
{0x7F,0xFC},
{0x08,0x20},
{0x10,0x90},
{0x23,0x08},
{0xCC,0x46},
{0x01,0x80},
{0x06,0x10},
{0x18,0x20},
{0x00,0xC0},
{0x07,0x00},
{0x78,0x00},/*"参",8*/
  },

    {{0x00,0x00},
{0x01,0xFC},
{0x01,0x04},
{0xFD,0x04},
{0x05,0x24},
{0x05,0x24},
{0x49,0x24},
{0x29,0x24},
{0x11,0x24},
{0x11,0x54},
{0x28,0x50},
{0x24,0x90},
{0x44,0x90},
{0x81,0x12},
{0x02,0x12},
{0x04,0x0E},/*"观",9*/
  },
  
  {{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x00,0x00},
{0x18,0x00},
{0x24,0x00},
{0x24,0x00},
{0x18,0x00},
{0x00,0x00},
{0x00,0x00},
//"。",9
  },
  
  };
  
  /* Check if got new data */
  if( AntReadAppMessageBuffer() )
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      bGotNewData = FALSE;
      for(u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++)
      {
        if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i])
        {
          bGotNewData = TRUE;
          au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i];
        }
      }
    }
    /* When got new data, apply the command */
    if(bGotNewData)
    {
      u8Command=au8LastAntData[0];
    }
  }
    
  AT91C_BASE_PIOA->PIO_SODR = PA_12_M_OE;
  
  if(u8Command & 0x01)
  {
  u16RollCounter++;

  if(u16RollCounter == 256)
  {
    u16RollCounter = 0;
    for(int j=0;j<16;j++)
    {
      bRollTemp = (bool)(u8MBI5026GFData[0][j][0] & 0x80);
      for(int i=0;i<11;i++)
      {
        u8MBI5026GFData[i][j][0] = u8MBI5026GFData[i][j][0] << 1;
        
        if(u8MBI5026GFData[i][j][1] & 0x80)
        {
          u8MBI5026GFData[i][j][0] = u8MBI5026GFData[i][j][0] | 0x01;
        }
        else
        {
          u8MBI5026GFData[i][j][0] = u8MBI5026GFData[i][j][0] & 0xFE;
        }
        
        u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] << 1;
        
        if(i == 10)
        {
          if(bRollTemp)
          {
            u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] | 0x01;
          }
          else
          {
            u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] & 0xFE;
          }
        }
        else
        {
          if(u8MBI5026GFData[i+1][j][0] & 0x80)
          {
            u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] | 0x01;
          }
          else
          {
            u8MBI5026GFData[i][j][1] = u8MBI5026GFData[i][j][1] & 0xFE;
          }
        }
        
      }
    }
  }
  }
  
  
  UserApp1_CD4515BM_ChangeData(u8CD4515Data);
  
  for(int i=5;i>0;i--)
  {
    UserApp1_MBI5026GF_SendData(u8MBI5026GFData[i-1][u8CD4515Data][1]);
    UserApp1_MBI5026GF_SendData(u8MBI5026GFData[i-1][u8CD4515Data][0]);
  }

  AT91C_BASE_PIOA->PIO_SODR = PA_11_M_LE;
  
  for(int j=0;j<5;j++);
  
  AT91C_BASE_PIOA->PIO_CODR = PA_11_M_LE;
  u8CD4515Data++;
  
  if(u8CD4515Data == 16)
  {
    u8CD4515Data = 0;
  }
  
  AT91C_BASE_PIOA->PIO_CODR = PA_12_M_OE;
 
  if(WasButtonPressed(BUTTON1)||(!(u8Command &0x02)))
  {
    ButtonAcknowledge(BUTTON1);
    UserApp1_StateMachine = UserApp1SM_Idle;
    
  }
} /* end UserApp1SM_Show() */

    
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for the ANT channel assignment to finish */
static void UserApp1SM_WaitChannelAssign(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    /* Set timer and advance states */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
  }
  
  /* Monitor for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    UserApp1_StateMachine = UserApp1SM_Error;
  }
      
} /* end UserApp1SM_WaitChannelAssign() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to open */
static void UserApp1SM_WaitChannelOpen(void)
{
  /* Monitor the channel status to check if channel is opened */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE) )
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_Error;
    
  }
    
} /* end UserApp1SM_WaitChannelOpen() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
