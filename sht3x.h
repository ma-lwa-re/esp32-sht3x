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
#include <stdio.h>
#include <stdbool.h>
#include "esp_err.h"
#include "driver/i2c.h"

#define FAHRENHEIT(celcius)         (((celcius * 9.0) / 5.0) + 32.0)
#define KELVIN(celcius)             (celcius + 273.15)
#define SCALE_CELCIUS               ('C')
#define SCALE_FAHRENHEIT            ('F')
#define SCALE_KELVIN                ('K')

#define I2C_MASTER_SDA              (GPIO_NUM_4)
#define I2C_MASTER_SCL              (GPIO_NUM_5)
#define I2C_MASTER_RX_BUF_DISABLE   (0)
#define I2C_MASTER_TX_BUF_DISABLE   (0)
#define I2C_MASTER_FREQ_HZ          (100000)
#define I2C_MASTER_TIMEOUT_MS       (1000)
#define I2C_MASTER_NUM              (0)
#define I2C_ACK_CHECK_DIS           (0x00)
#define I2C_ACK_CHECK_EN            (0x01)
#define I2C_ACK_VAL                 (0x00)
#define I2C_NACK_VAL                (0x01)

#define SHT3X_SENSOR_ADDR           (0x44)
#define SHT3X_READ_ERROR            (0xFFFF)
#define SHT3X_HEX_CODE_SIZE         (0x02)

#define CRC8_POLYNOMIAL             (0x31)
#define CRC8_INIT                   (0xFF)

#ifndef SCD4X_H
#define SCD4X_H
typedef struct sht3x_msb_lsb {
    uint8_t msb;
    uint8_t lsb;
} sht3x_msb_lsb_t;

typedef struct sht3x_sensor_value {
    sht3x_msb_lsb_t value;
    uint8_t crc;
} sht3x_sensor_value_t;

typedef struct sht3x_sensors_values {
    float temperature;
    float humidity;
} sht3x_sensors_values_t;
#endif

uint8_t sht3x_generate_crc(const uint8_t* data, uint16_t count);

esp_err_t sht3x_send_command(uint8_t *command);

esp_err_t sht3x_read(uint8_t *hex_code, uint8_t *measurements, uint8_t size);

esp_err_t sht3x_write(uint8_t *hex_code, uint8_t *measurements, uint8_t size);

esp_err_t sht3x_send_command_and_fetch_result(uint8_t *command, uint8_t *measurements, uint8_t size);

esp_err_t sht3x_start_periodic_measurement();

esp_err_t sht3x_start_periodic_measurement_with_art();

esp_err_t sht3x_read_measurement(sht3x_sensors_values_t *sensors_values);

esp_err_t sht3x_stop_periodic_measurement();

esp_err_t sht3x_soft_reset();

esp_err_t sht3x_general_call_reset();

esp_err_t sht3x_enable_heater();

esp_err_t sht3x_disable_heater();

esp_err_t sht3x_read_status_register(sht3x_sensor_value_t *sensors_value);

esp_err_t sht3x_clear_status_register();
