
#ifndef _BT_THREAD_H_
#define _BT_THREAD_H_

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/devicetree.h>
#include <stddef.h>

void BTHomeThread(void *p1, void *p2, void *p3);

#endif