/* MIT License
*
* Copyright (c) 2022 ma-lwa-re
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "sht3x.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define UART_STACK_SIZE             (4096)

static const char *MAIN_TAG = "main";
static const char *SENSORS_TAG = "sensors";

char scale = SCALE_CELCIUS;
float temperature = 0.0;
float humidity = 0.0;

void sensors_task(void *arg) {
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, i2c_config.mode,
                    I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));

    esp_log_level_set(SENSORS_TAG, ESP_LOG_INFO);

    #if defined(SENSORS_SCALE_F)
    scale = SCALE_FAHRENHEIT;
    #elif defined(SENSORS_SCALE_K)
    scale = SCALE_KELVIN;
    #endif

    vTaskDelay(INIT_DELAY / portTICK_PERIOD_MS);

    for(;;) {
        sht3x_start_periodic_measurement();

        sht3x_sensors_values_t sensors_values = {
            .temperature = 0x00,
            .humidity = 0x00
        };
        vTaskDelay(INIT_DELAY / portTICK_PERIOD_MS);

        if(sht3x_read_measurement(&sensors_values) != ESP_OK) {
            ESP_LOGE(SENSORS_TAG, "Sensors read measurement error!");
        }
        vTaskDelay(INIT_DELAY / portTICK_PERIOD_MS);

        float temperature = sensors_values.temperature;
        float humidity = sensors_values.humidity;

        #if defined(SENSORS_SCALE_F)
        temperature = FAHRENHEIT(temperature);
        #elif defined(SENSORS_SCALE_K)
        temperature = KELVIN(temperature);
        #endif

        ESP_LOG_BUFFER_HEX_LEVEL(SENSORS_TAG, &sensors_values, sizeof(sensors_values), ESP_LOG_DEBUG);

        sht3x_stop_periodic_measurement();

        ESP_LOGI(SENSORS_TAG, "Temperature %2.1f °%c - Humidity %2.1f%%", temperature, scale, humidity);
        vTaskDelay(SLEEP_DELAY / portTICK_PERIOD_MS);
    }
}

void app_main() {
    esp_log_level_set(MAIN_TAG, ESP_LOG_INFO);
    ESP_LOGI(MAIN_TAG, "Hello there!");

    xTaskCreate(sensors_task, "sensors_task", UART_STACK_SIZE, NULL, configMAX_PRIORITIES-9, NULL);
}
