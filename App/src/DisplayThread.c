/**
 * @file DisplayThread.c
 * @author Guilherme Freire (guilherme.freire@fieb.org.br)
 * @brief 
 * @version 0.1
 * @date 2023-06-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "DisplayThread.h"

#define LED0_NODE DT_ALIAS(led0)

extern struct k_msgq accel_queue;
extern struct k_msgq temp_queue;
extern struct k_event kEvent;

const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
uint8_t displayState = 0;

void DisplayThread(void *p1, void *p2, void *p3)
{
  /* Initialization of the Thread */
  static lv_style_t style;
  lv_obj_t *readings_label;
	uint32_t events = 0;
	double accelBuffer[7];
	double tempBuffer[3];
	char buff[100];

	if (!device_is_ready(display_dev)) {
		printf("Device %s is not ready\n", display_dev->name);
		return;
	}

	if (!device_is_ready(led.port)) {
		printf("Device %s is not ready\n", display_dev->name);
		return;
	}

	if (!gpio_pin_configure_dt(&led, GPIO_OUTPUT)) {
			printk("Error: failed to configure LED device %s pin %d\n",
			     	led.port->name, led.pin);
		} else{
			gpio_pin_set_dt(&led, 0);
		}
	

	lv_style_init(&style);
	lv_style_set_bg_color(&style, lv_color_black());
	lv_style_set_text_color(&style, lv_color_white());
	lv_style_set_text_decor(&style, LV_TEXT_DECOR_NONE);
	
	lv_obj_add_style(lv_scr_act(), &style, 0);
	
	readings_label = lv_label_create(lv_scr_act());
	lv_obj_add_style(readings_label, &style, 0);
	lv_label_set_text(readings_label, "Running Zephyr!");
	lv_obj_align(readings_label, LV_ALIGN_TOP_MID, 0, 0);

	lv_task_handler();
  
  while(1)
  {	
		k_msgq_get(&accel_queue, &accelBuffer, K_MSEC(5));
		k_msgq_get(&temp_queue, &tempBuffer, K_MSEC(5));
		memset(buff, 0, sizeof(buff));
		
		switch (displayState)
		{
		case 0:
			sprintf(buff, "Temp: %.1f C\n\n"
										"Pres: %.1fPa\nHumi: %.1f%\n\nTemperature",
										tempBuffer[0], tempBuffer[1], tempBuffer[2]);

			break;
		case 1:
			sprintf(buff, "Temp: %.1f C\n\n"
										"AX: %.1f m/s/s\nAY: %.1f m/s/s\nAZ: %.1f m/s/s\n\nAccelerometer",
										accelBuffer[0], accelBuffer[1], accelBuffer[2], accelBuffer[3]);
			break;
		case 2:
			sprintf(buff, "Temp: %.1f C\n\n"
										"GX: %.1f m/s/s\nGY: %.1f m/s/s\nGZ: %.1f m/s/s\n\nGyroscope",
										accelBuffer[0], accelBuffer[4], accelBuffer[5], accelBuffer[6]);
			break;
		case 3:
			/* code */
			break;
			
		default:
			break;
		}
		lv_label_set_text(readings_label, buff);

		lv_task_handler();
		k_msleep(50);
  }
}

static int changeView(const struct shell *sh, size_t argc, char **argv, void *data)
{
	int val = (intptr_t)data;

	switch (val)
	{
	case 0:
		displayState = 0;
		shell_print(sh, "Changed to temperature view!");
		break;
	case 1:
		displayState = 1;
		shell_print(sh, "Changed to acceleration view!");
		break;
	case 2:
		displayState = 2;	
		shell_print(sh, "Changed to rotation view!");
		break;
	default:
			shell_print(sh, "Wrong arg");
		break;
	}

	return 0;
}

static int toggleLed(const struct shell *sh, size_t argc, char **argv)
{
	
	gpio_pin_toggle_dt(&led);
	return 0;
}

//Creating a subcommand dictionary
SHELL_SUBCMD_DICT_SET_CREATE(sub_change_view, changeView,
	(temp, 0, "Temperature readings"), (accel, 1, "Acceleration readings"),
	(gyro, 2, "Rotation readings"), (value_3, 3, "value 3")
);

SHELL_CMD_REGISTER(display, &sub_change_view, "Change display view", NULL);
SHELL_CMD_REGISTER(toggle, NULL, "Toggle the board LED", &toggleLed);
