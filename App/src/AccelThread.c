/**
 * @file AccelThread.c
 * @author Guilherme Freire (guilherme.freire@fieb.org.br)
 * @brief 
 * @version 0.1
 * @date 2023-06-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "AccelThread.h"

K_MSGQ_DEFINE(accel_queue, 7*sizeof(double), 5, 2);

const struct device *const mpu6050 = DEVICE_DT_GET_ONE(invensense_mpu6050);
static int process_mpu6050(const struct device *dev);

void AccelThread(void *p1, void *p2, void *p3)
{
	uint8_t qTest = "Testing queue\n";
	//k_queue_init(&accel_queue);
  if (!device_is_ready(mpu6050)) {
    printk("Device %s is not ready\n", mpu6050->name);
    return 0;
	}
	while(1)
	{	
		process_mpu6050(mpu6050);
    k_msleep(10);
	}
}

static int process_mpu6050(const struct device *dev)
{
	struct sensor_value temperature;
	struct sensor_value accel[3];
	struct sensor_value gyro[3];
	double buffer[7];
	int rc = sensor_sample_fetch(dev);

	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ,
					accel);
	}
	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ,
					gyro);
	}
	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP,
					&temperature);
	}
	if (rc == 0) {
		printf("  temp %f C\n"
					 "  accel %f %f %f m/s/s\n"
		       "  gyro  %f %f %f rad/s\n",
		       sensor_value_to_double(&temperature),
		       sensor_value_to_double(&accel[0]),
		       sensor_value_to_double(&accel[1]),
		       sensor_value_to_double(&accel[2]),
		       sensor_value_to_double(&gyro[0]),
		       sensor_value_to_double(&gyro[1]),
		       sensor_value_to_double(&gyro[2]));
	} else {
		printf("sample fetch/get failed: %d\n", rc);
	}
	buffer[0] = sensor_value_to_double(&temperature);
	buffer[1] = sensor_value_to_double(&accel[0]);
	buffer[2] = sensor_value_to_double(&accel[1]);
	buffer[3] = sensor_value_to_double(&accel[2]);
	buffer[4] = sensor_value_to_double(&gyro[0]);
	buffer[5] = sensor_value_to_double(&gyro[1]);
	buffer[6] = sensor_value_to_double(&gyro[2]);
	k_msgq_put(&accel_queue, buffer, K_NO_WAIT);

	return 0;
}

/* Functions */
static int cmd_get_accel_read(const struct shell *sh, size_t argc, char **argv)
{
  // process_mpu6050(mpu6050);
	return 0;
}


/* Shell Commands */
SHELL_CMD_REGISTER(stdal, NULL, "Stand Alone Command", &cmd_get_accel_read);


/*  */