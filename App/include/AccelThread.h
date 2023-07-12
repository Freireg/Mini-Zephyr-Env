/**
 * @file AccelThread.h
 * @author Guilherme Freire (guilherme.freire@fieb.org.br)
 * @brief 
 * @version 0.1
 * @date 2023-06-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _ACCEL_THREAD_H_
#define _ACCEL_THREAD_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <stdio.h>
#include <string.h>

void AccelThread(void *p1, void *p2, void *p3);

#endif