#ifndef __MENU_H
#define __MENU_H
#include "sys.h"
extern uint8_t mode_selection;
extern uint8_t displayFlag;
extern uint8_t thresholdFlag;
extern uint8_t manualFlag;

extern uint8_t TimeMode_Selection;
extern uint8_t FeedTimeFlag;
extern uint8_t OxTimeFlag;
extern uint8_t TimeFlag;
extern uint8_t FeedRun;
extern uint8_t OxRun;

void Display(void);
void Automatic(void);
void Threshold_Settings(void);
void Mode_selection(void);
void Manual(void);
void Action(void);
void Asterisk(uint8_t A);
void Timed_mode(void);
#endif

