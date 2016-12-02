
/***NOTE:
    This is an interrupt service routine.  You can NOT compile this
    program with Test Stack Overflow turned on and get an executable
    file which will operate correctly. */

/* 这个程序每隔1秒钟输出一个整数，10秒钟后结束程序。
   按escape键提前退出程序 。*/

#include <stdio.h>
#include <dos.h>
#include <conio.h>

/* Escape key */
#define VK_ESC 0x11b 

#define TIMER 0x1c    /* 时钟中断的中断号 */

/* 中断处理函数在C和C++中的表示略有不同。
   如果定义了_cplusplus则表示在C++环境下，否则是在C环境下。 */
#ifdef __cplusplus
    #define __CPPARGS ...
#else
	#define __CPPARGS
#endif

int TimerCounter=0; /* 计时变量，每秒钟增加18。 */

/* 指向原来时钟中断处理过程入口的中断处理函数指针 */
void interrupt ( *oldhandler)(__CPPARGS);

/* 新的时钟中断处理函数 */
void interrupt newhandler(__CPPARGS)
{
/* increase the global counter */
   TimerCounter++;

/* call the old routine */
   oldhandler();
}

/* 设置新的时钟中断处理过程 */
void SetTimer(void interrupt (*IntProc)(__CPPARGS))
{
	oldhandler=getvect(TIMER);
	disable(); /* 设置新的时钟中断处理过程时，禁止所有中断 */
	setvect(TIMER,IntProc);
	enable();  /* 开启中断 */
}

/* 恢复原有的时钟中断处理过程 */
void KillTimer()
{
	disable();
	setvect(TIMER,oldhandler);
	enable();
}


void main(void)
{
	int key,time=0;

	SetTimer(newhandler); /* 修改时钟中断 */

	for (;;)
	{
		if (bioskey(1))
		{
			key=bioskey(0);
			if (key==VK_ESC) /* 按escape键提前退出程序 */
			{
				printf("User cancel!\n");
				break;
			}
		}
		if (TimerCounter>18) /* 1秒钟处理一次 */
		{
			/* 恢复计时变量 */
			TimerCounter=0;
			time++;
			printf("%d\n",time);
			if (time==10) /* 10秒钟后结束程序 */
			{
				printf("Program terminated normally!\n");
				break;
			}
		}
	}
	KillTimer(); /* 恢复时钟中断 */

}
