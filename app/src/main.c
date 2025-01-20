/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main(void)
{
	int ret;
	struct sensor_value value_x;
	const struct device *sensor;

	sensor = DEVICE_DT_GET(DT_NODELABEL(example_sensor));

	if (!device_is_ready(sensor))
	{
		LOG_ERR("Sensor not ready");
		return 0;
	}

	while (1)
	{
	    ret = sensor_sample_fetch(sensor);
		if (ret) {
			printk("sensor_sample_fetch failed ret %d\n", ret);
			return 0;
		}

		ret = sensor_channel_get(sensor, SENSOR_CHAN_AMBIENT_TEMP, &value_x);
		LOG_INF("sensor_sample_get ret: %d  valor del sensor: %d\n", ret, value_x.val1);
		k_sleep(K_MSEC(2000));
	}

	return 0;
}