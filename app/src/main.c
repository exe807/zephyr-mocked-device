/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <app_version.h>
const struct device *sensor;
void my_timer_handler(struct k_timer *dummy);

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

void my_timer_handler(struct k_timer *dummy)
{
    /* do the processing that needs to be done periodically */
	LOG_INF("work handler...\n");
	int ret;
	struct sensor_value value_x;
	ret = sensor_sample_fetch(sensor);
	if (ret < 0)
	{
		LOG_ERR("End of file, ret= %d\n", ret);
		k_timer_stop(&my_timer);
	}
	else
	{
		ret = sensor_channel_get(sensor, SENSOR_CHAN_AMBIENT_TEMP, &value_x);
		LOG_INF("sensor_sample_get ret: %d  valor del sensor: %d\n", ret, value_x.val1);
	}
}



int main(void)
{
	sensor = DEVICE_DT_GET(DT_NODELABEL(example_sensor));

	if (!device_is_ready(sensor))
	{
		LOG_ERR("Sensor not ready");
		return 0;
	}

	/* start a periodic timer that expires once every second */
	k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));
	LOG_INF("main function, enabling timmer...\n");
	// while (1)
	// {
	// 	/* empty, waiting for the timer to expire and call the handler */
	// 	k_sleep(K_FOREVER); // Put the main thread to sleep
	// }

	return 0;
}