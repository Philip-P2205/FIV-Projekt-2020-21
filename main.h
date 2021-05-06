/*
 * main.h
 *
 *  Created on: 01.04.2021
 *      Author: Philip Prohaska
 */
#include <msp430.h>
#include <stdint.h>
#include "Library/debounce.h"
#include "Library/timer.h"
#include "Library/util.h"

#ifndef MAIN_H_
#define MAIN_H_

#define TASK_DEBOUNCE  BIT0
#define TASK_1  BIT1
#define TASK_2  BIT2
#define TASK_3  BIT3
#define TASK_4  BIT4
#define TASK_5  BIT5
#define TASK_6  BIT6
#define TASK_7  BIT7
#define TASK_8  BIT8
#define TASK_9  BIT9
#define TASK_A  BITA
#define TASK_B  BITB
#define TASK_C  BITC
#define TASK_E  BITE
#define TASK_F  BITF

/* Macros */

#define TASK_ENABLE(task)   todo_tasks  |=  (task)
#define TASK_DISABLE(task)  todo_tasks  &= ~(task)
#define TASK_ENABLED(task)  (todo_tasks &   (task))

extern uint16_t todo_tasks;

#endif /* MAIN_H_ */
