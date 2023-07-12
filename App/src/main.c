/**
 * @file main.c
 * @author Guilherme Freire (freireg1503@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "DisplayThread.h"
#include "AccelThread.h"

#define STACK_SIZE 	2048 
#define PRIORITY		0

K_THREAD_DEFINE(
	AccelHandler, STACK_SIZE,
	AccelThread, NULL, NULL, NULL,
	PRIORITY, 0, 0);

K_THREAD_DEFINE(
	DisplayHandler, STACK_SIZE,
	DisplayThread, NULL, NULL, NULL,
	PRIORITY, 0, 0);
