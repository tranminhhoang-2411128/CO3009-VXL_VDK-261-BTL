/*
 * dht20.c
 *
 *  Created on: Nov 9, 2026
 *      Author: Hoang
 */

#include "dht20.h"

#define DHT20_HAL_ADDR     (DHT20_ADDR << 1)   /* HAL expects the 8-bit address */
#define DHT20_I2C_TIMEOUT  10                  /* ms */
#define DHT20_MAX_POLL	   100				   /* times */

I2C_HandleTypeDef hi2c1;
int DHT20_flag = 0;
float temp = 0.0f;
float humi = 0.0f;

static uint8_t cmd[3] = { 0xAC, 0x33, 0x00 };
static uint8_t data[7];
static uint8_t init_step = 0;   /* sub-state of DHT20_Connect()  */
static uint8_t poll_count  = 0;   /* busy polls in DHT20_ReadData() */

/* ------------------------------------------------------------------ */
/* Private helpers                                                    */
/* ------------------------------------------------------------------ */

/* Read 1 status byte (datasheet 7.3, Table 9). Returns 0 on I2C error. */
static uint8_t DHT20_ReadStatus(void)
{
    uint8_t status;
    if (HAL_I2C_Master_Receive(&hi2c1, DHT20_HAL_ADDR, &status, 1,
                               DHT20_I2C_TIMEOUT) != HAL_OK)
        return 0;
    return status;
}

/* CRC8: init 0xFF, poly x^8 + x^5 + x^4 + 1 (0x31) - datasheet 7.4 step 4 */
static uint8_t DHT20_CRC8(const uint8_t *ptr, uint8_t len)
{
    uint8_t crc = 0xFF;
    while (len--)
    {
        crc ^= *ptr++;
        for (uint8_t i = 0; i < 8; i++)
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
    }
    return crc;
}

/* One step of the Aosong register-init routine (Tillaart _resetRegister).
 * phase 0: write {reg, 0, 0}
 * phase 1: read 3 bytes
 * phase 2: write {0xB0 | reg, b1, b2}
 * Returns 1 on success. */
static uint8_t DHT20_ResetStep(uint8_t reg, uint8_t phase)
{
    static uint8_t buf[3];

    switch (phase)
    {
    case 0:
        buf[0] = reg; buf[1] = 0x00; buf[2] = 0x00;
        return HAL_I2C_Master_Transmit(&hi2c1, DHT20_HAL_ADDR, buf, 3,
                                       DHT20_I2C_TIMEOUT) == HAL_OK;
    case 1:
        return HAL_I2C_Master_Receive(&hi2c1, DHT20_HAL_ADDR, buf, 3,
                                      DHT20_I2C_TIMEOUT) == HAL_OK;
    case 2:
        buf[0] = 0xB0 | reg;
        return HAL_I2C_Master_Transmit(&hi2c1, DHT20_HAL_ADDR, buf, 3,
                                       DHT20_I2C_TIMEOUT) == HAL_OK;
    default:
        return 0;
    }
}

/* ------------------------------------------------------------------ */
/* Public API                                                         */
/* ------------------------------------------------------------------ */

/* Datasheet 7.4 step 1. Called repeatedly while DHT20_flag == 0.
 * step 0     : read status; calibrated -> flag = 1
 * step 1..9  : init 0x1B, 0x1C, 0x1E (3 phases each), one phase per call
 * Any I2C error restarts from step 0 (sensor absent / bus error). */
void DHT20_Connect(void)
{
    static const uint8_t regs[3] = { 0x1B, 0x1C, 0x1E };
    uint8_t status;

    if (init_step == 0)
    {
        status = DHT20_ReadStatus();
    	if (!status)
            return;                         /* not connected, retry later */

        if ((status & 0x18) == 0x18)
        {
            DHT20_flag = 1;                 /* calibrated, ready */
            return;
        }
        init_step = 1;                      /* needs register init */
        return;
    }

    uint8_t idx   = (init_step - 1) / 3;
    uint8_t phase = (init_step - 1) % 3;

    if (!DHT20_ResetStep(regs[idx], phase))
    {
        init_step = 0;
        return;
    }

    if (++init_step > 9)
        init_step = 0;                      /* done -> re-check status */
}

/* Datasheet 7.4 step 2. flag 1 -> 2 on success, stays 1 on error. */
void DHT20_TriggerMeasurement(void){

	/* Step 2: trigger measurement */
    if (HAL_I2C_Master_Transmit(&hi2c1, DHT20_HAL_ADDR, cmd, 3,
                                DHT20_I2C_TIMEOUT) != HAL_OK)
    	return;
    DHT20_flag = 2;
    poll_count = 0;
}

/* Datasheet 7.4 steps 3-5. Called while DHT20_flag == 2.
 * busy          -> stay 2 (re-polled after DHT20_CALIB_TIME)
 * error/timeout -> back to 1 (re-trigger)
 * success       -> 3, temp/humi updated */
void DHT20_ReadData(void)
{
    uint32_t raw;

    if (!DHT20_ReadStatus())
    {
        DHT20_flag = 1;
        return;
    }

    if (data[0] & 0x80)                      /* Bit[7] = busy */
    {
        if (++poll_count >= DHT20_MAX_POLL)
            DHT20_flag = 1;
        return;
    }

    /* status + 5 data bytes + CRC */
    if (HAL_I2C_Master_Receive(&hi2c1, DHT20_HAL_ADDR, data, 7,
                               DHT20_I2C_TIMEOUT) != HAL_OK)
    {
        DHT20_flag = 1;
        return;
    }

    if (DHT20_CRC8(data, 6) != data[6])
    {
        DHT20_flag = 1;
        return;
    }

    /* Section 8: 20-bit humidity, 20-bit temperature */
    raw  = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
    humi = (float)raw * 100.0f / 1048576.0f;

    raw  = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
    temp = (float)raw * 200.0f / 1048576.0f - 50.0f;

    DHT20_flag = 3;
}
