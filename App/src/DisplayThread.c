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

extern struct k_queue accel_queue;

const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));


void DisplayThread(void *p1, void *p2, void *p3)
{
  /* Initialization of the Thread */
  static lv_style_t style;
  lv_obj_t *hello_world_label;
  lv_obj_t *readings_label;
  char count_str[50] = {0};
	int counter = 0;

	if (!device_is_ready(display_dev)) {
		printf("Device %s is not ready\n", display_dev->name);
		return;
	}

  if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN)) {
		lv_obj_t *hello_world_button;

		hello_world_button = lv_btn_create(lv_scr_act());
		lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
		hello_world_label = lv_label_create(hello_world_button);
	} 
	else {
		hello_world_label = lv_label_create(lv_scr_act());
	}

	lv_style_init(&style);
	lv_style_set_bg_color(&style, lv_color_black());
	// lv_style_set_text_color(&style, lv_color_white());
	lv_style_set_text_decor(&style, LV_TEXT_DECOR_NONE);

	lv_label_set_text(hello_world_label, "Accel Readings");
	lv_obj_align(hello_world_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	readings_label = lv_label_create(lv_scr_act());
	lv_obj_add_style(readings_label, &style, 0);
	lv_label_set_text(readings_label, "Running Zephyr!");
	lv_obj_align(readings_label, LV_ALIGN_CENTER, 0, 0);

	lv_task_handler();
  
  while(1)
  {	
		// sprintf(count_str, "%d", counter);
		if (!k_queue_is_empty(&accel_queue))
		{
			if (k_queue_remove(&accel_queue, &count_str))
			{
				lv_label_set_text(hello_world_label, count_str);
			}
			
		}
		lv_task_handler();
		counter++;
		k_msleep(1000);
  }
}

