#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>



/* Specs and Peripherals */
const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));




/* Functions */
static int cmd_test_display(const struct shell *sh, size_t argc, char **argv)
{

	shell_print(sh, "Device ready!");
	return 0;
}


/* Shell Commands */
/** Peripherals **/
SHELL_STATIC_SUBCMD_SET_CREATE(sub_display,
	SHELL_CMD(test, NULL, "Print", &cmd_test_display)
);
SHELL_CMD_REGISTER(display, &sub_display, "display commands", NULL);


/*  */



void main(void)
{
	return 0;
}