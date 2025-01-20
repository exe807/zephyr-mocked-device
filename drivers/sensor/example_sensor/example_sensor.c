/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT zephyr_example_sensor

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(example_sensor, CONFIG_SENSOR_LOG_LEVEL);

struct example_sensor_data {
    int state;
    FILE *csv_file;
};

struct example_sensor_config {

};

static int example_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    struct example_sensor_data *data = dev->data;

    if (fscanf(data->csv_file, "%d\n", &data->state) < 0)
    {
        LOG_ERR("Error reading file");
        return -EIO;
    }

    return 0;
}

static int example_sensor_channel_get(const struct device * dev,
                                      enum sensor_channel   chan,
                                      struct sensor_value * val)
{
    struct example_sensor_data *data = dev->data;

    if (chan !=  SENSOR_CHAN_AMBIENT_TEMP) {
        return -ENOTSUP;
    }

    val->val1 = data->state;

    return 0;
}

static DEVICE_API(sensor, example_sensor_api) = {
    .sample_fetch = &example_sensor_sample_fetch,
    .channel_get = &example_sensor_channel_get,
};

static int example_sensor_init(const struct device *dev)
{

    struct example_sensor_data *data = dev->data;
    if(!data->csv_file)
    {
        data->csv_file = fopen(CONFIG_PATH_TO_CSV_FILE, "r");
        if(!data->csv_file){
            LOG_ERR("Error opening file");
            return -ENOENT;
        }
    }

    return 0;
}

#define EXAMPLE_SENSOR_INIT(i)						       					\
    static struct example_sensor_data example_sensor_data_##i;	       		\
                                                                            \
    static const struct example_sensor_config example_sensor_config_##i = {	\
    };								       									\
                                                                            \
    DEVICE_DT_INST_DEFINE(i, example_sensor_init, NULL,		       			\
                  &example_sensor_data_##i,			       					\
                  &example_sensor_config_##i, POST_KERNEL,	                \
                  CONFIG_SENSOR_INIT_PRIORITY, &example_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(EXAMPLE_SENSOR_INIT)