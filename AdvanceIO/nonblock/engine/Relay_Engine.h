#ifndef RELAY_ENGINE_H__
#define RELAY_ENGINE_H__

#include "../../../apue.h"

#define	REL_JOBMAX	10000

enum{
	STATE_RUNNING = 1,
	STATE_CANCELED,
	STATE_OVER
};

struct rel_stat_st{
	int state;
	int fd1;
	int fd2;
	int64_t count12, count21;
	//struct timerval start, end;	//时间信息
};

int rel_addjob(int fd1, int fd2);
/*
 * return >= 0			成功返回当前任务ID
 * return == -EINVAL	失败，参数非法
 * return == -ENOSPC	失败，任务数组满
 * return == -ENOMEM	失败，内存分配有误
 */

int rel_canceljob(int id);
/*
 * return == 0			成功，指定任务已经成功取消
 * return == -EINVAL	失败，参数非法
 * return == -EBUSY		失败，任务早已被取消
 */

int rel_waitjob(int id, struct rel_stat_st *);
/*
 * return == 0			成功，指定任务已终止并返回状态
 * return == -EINVAL	失败，参数非法
 */

int rel_statjob(int id, struct rel_stat_st *);
/*
 * return == 0			成功，指定任务状态已经返回
 * return == -EINVAL	失败，参数非法
 */

#endif
