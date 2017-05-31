/**********************************************************************************************************************
File: user_app2.h                                                                

Description:
Header file for user_app2.c

**********************************************************************************************************************/

#ifndef __USER_APP2_H
#define __USER_APP2_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/
typedef enum {DEMO_LIST, USER_LIST} LedDisplayListNameType;



/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
//#define USER_LIST_EXAMPLE         /* Define to load a default User LED command list */
#define DEMO_KNIGHT_RIDER         /* Define to load the Knight Rider DEMO LED command list */


#define MAX_LIST_SIZE             (u8)200               /* Maximum number of list entries (100 commands, each with START and END */
#define LED_FADE_TIME             (u32)20               /* On time in ms for each level of PWM during fade */
#define LED_FADE_STEP             (u32)2                /* PWM steps for each iteration of LED fade */
#define LED_TOTAL_FADE_TIME       (u32)200              /* Total fading time in ms for an LED turning on or off */

#define LCD_DMO_ON_CHAR_ADDRESS   (u8)0x43              /* Address to show '<' when DMO code is running */
#define LCD_USR_ON_CHAR_ADDRESS   (u8)0x49              /* Address to show '<' when USR code is running */
#define LCD_PAUSE_CHAR_ADDRESS    (u8)0x4D              /* Starting address for the '||' symbol on the LCD */
#define LCD_DARK_CHAR_ADDRESS     (u8)0x53              /* LCD address for the 'X' (dark) symbol */


/**********************************************************************************************************************
Function Declarations
***********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
void LedDisplayStartList(void);
bool LedDisplayAddCommand(LedDisplayListNameType eListName_, LedCommandType* pCommandInfo_);
bool LedDisplayPrintListLine(u8 u8ListItem_);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp2Initialize(void);
void UserApp2RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
void AllLedsOff(void);
void LoadLcdScreen(void);
void ResetListFades(LedDisplayListNodeType* psTargetList_);


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp2SM_Idle(void);    
static void UserApp2SM_RunCommandList(void);

static void UserApp2SM_Delay(void); 
static void UserApp2SM_Error(void);         
static void UserApp2SM_FailedInit(void);        


#endif /* __USER_APP2_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
