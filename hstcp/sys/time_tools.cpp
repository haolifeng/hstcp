/*
 * time_tools.cpp
 *
 *  Created on: 2017年4月13日
 *      Author: hlf
 */
#include "time_tools.h"
int getHHMMSSCCCC()
{
	struct timeval now;
	gettimeofday(&now, NULL);

	struct tm t;
	localtime_r(&now.tv_sec, &t);

	return t.tm_hour*100000000 + t.tm_min*1000000 + t.tm_sec*10000 + now.tv_usec/100;
}
int getTimespec(int iTimeout, timespec& ts)
{
	struct timeval now;
	gettimeofday(&now, NULL);

	ts.tv_sec = now.tv_sec + iTimeout / 1000; /* 秒 */
	ts.tv_nsec = now.tv_usec + (iTimeout % 1000) * 1000; /* 微秒 */
	if(ts.tv_nsec >= (1000*1000)) /* 秒数已经进位 */
	{
	    ts.tv_sec++; /* 秒+1 */
	    ts.tv_nsec = ts.tv_nsec % (1000 * 1000); /* 剩余微秒 */
	}
	ts.tv_nsec = ts.tv_nsec * 1000; /* 转换成纳秒 */

	return 0;
}
int timeElapse(uint32_t uiHHMMSSCCCC)
{
	 int iHH1 = uiHHMMSSCCCC / 100000000;
	 int iMM1 = uiHHMMSSCCCC % 100000000 / 1000000;
	 int iSS1 = uiHHMMSSCCCC % 1000000 / 10000;
	 int iCC1 = uiHHMMSSCCCC % 10000;

	 int iCurrTime = getHHMMSSCCCC();
	 int iHH2 = iCurrTime / 100000000;
	 int iMM2 = iCurrTime % 100000000 / 1000000;
	 int iSS2 = iCurrTime % 1000000 / 10000;
	 int iCC2 = iCurrTime % 10000;

	 return ((iHH1-iHH2)*360000 + (iMM1-iMM2)*6000 + (iSS1-iSS2))*100000 + (iCC1-iCC2)*1000;
}


