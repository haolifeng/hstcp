/*
 * time_tools.h
 *
 *  Created on:
 *      Author: hlf
 */

#ifndef TIME_TOOLS_H_
#define TIME_TOOLS_H_
#include "common.h"
//时分秒毫秒：HHMMSSCCCC
int getHHMMSSCCCC();

//iTimeout 毫秒为单位
int getTimespec(int iTimeout, timespec& ts);

int timeElapse(uint32_t uiHHMMSSCCCC);



#endif /* TIME_TOOLS_H_ */
