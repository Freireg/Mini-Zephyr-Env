/**
 * @file DisplayThread.h
 * @author Guilherme Freire (guilherme.freire@fieb.org.br)
 * @brief 
 * @version 0.1
 * @date 2023-06-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _DISPLAY_THREAD_H_
#define _DISPLAY_THREAD_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/shell/shell.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>

void DisplayThread(void *p1, void *p2, void *p3);

#endif