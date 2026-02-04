#ifndef __MYRTC_H
#define __MYRTC_H

// 允许外部调用时间数组
extern int MyRTC_Time[];

void MyRTC_Init		(void);
void MyRTC_SetTime	(void);
void MyRTC_ReadTime	(void);

#endif
