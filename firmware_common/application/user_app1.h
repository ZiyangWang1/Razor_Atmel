/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/* Pin definition */
#define PA_00_GND        PA_00_TP54
#define PA_03_A          PA_03_HSMCI_MCCK 
#define PA_04_D          PA_04_HSMCI_MCCDA 
#define PA_05_CD_STB     PA_05_HSMCI_MCDA0 
#define PA_06_C          PA_06_HSMCI_MCDA1
#define PA_07_B          PA_07_HSMCI_MCDA2 
#define PA_08_INH        PA_08_SD_CS_MCDA3 
#define PA_09_I2C_M_SCL    PA_09_I2C_SDA 
#define PA_10_I2C_M_SDA    PA_10_I2C_SCL
#define PA_11_M_LE       PA_11_BLADE_UPIMO
#define PA_12_M_OE       PA_12_BLADE_UPOMI 
#define PA_14_M_CLK      PA_14_BLADE_MOSI
#define PA_15_M_SDI      PA_15_BLADE_SCK 
#define PA_18_U0_PIMO    PA_18_DEBUG_U0_POMI 
#define PA_19_U0_POMI    PA_19_DEBUG_U0_PIMO
#define PB_05_F_SDO      PB_05_TP56 
#define PB_06_F_CS       PB_06_TP58
#define PB_07_SI         PB_07_TP60 
#define PB_08_SCLK       PB_08_TP62

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void UserApp1_CD4515BM_ChangeData(u8 u8Data);
static void UserApp1_MBI5026GF_SendData(u8 u8Data);
static void UserApp1_FM24CL16B_SendData(u8* pu8Data_,u8 u8DataLength,u8 u8WordLocation,u8 u8PageLocation);


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         


#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
