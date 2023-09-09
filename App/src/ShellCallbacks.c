/**
 * @file ShellCallbacks.c
 * @author Guilherme Freire (guilherme.freire@fieb.org.br)
 * @brief 
 * @version 0.1
 * @date 2023-07-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>


/* Functions */
static int cmd_get_current_thread(const struct shell *sh, size_t argc, char **argv)
{
	k_tid_t running;
	// char buff[10] = {0};
	running = k_current_get();
	shell_print(sh, "Current thread running: %d",running);
	return 0;
}


/* Shell Commands */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_Thread,
	SHELL_CMD(test, NULL, "Print", &cmd_get_current_thread)
);
SHELL_CMD_REGISTER(thread, &sub_Thread, "Thread commands", NULL);
