/*
 * lcd.c
 *
 *  Created on: Nov 9, 2026
 *      Author: Hoang
 */

#include "lcd.h"

#define LCD_HAL_ADDR      (LCD_ADDR << 1)
#define LCD_I2C_TIMEOUT   10       /* ms, longest frame ~70 bytes = ~6.5 ms */
#define LCD_CLEAR_TIME    3        /* ms, Clear execution 1.52 ms */

/* PCF8574 bits */
#define LCD_RS            0x01
#define LCD_EN            0x04
#define LCD_BL            0x08

/* HD44780 commands (Arduino library) */
#define LCD_CLEARDISPLAY    0x01
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_FUNCTIONSET     0x20
#define LCD_SETDDRAMADDR    0x80

#define LCD_ENTRYLEFT       0x02
#define LCD_DISPLAYON       0x04
#define LCD_2LINE           0x08

int LCD_flag = 0;

static uint8_t  backlight  = LCD_BL;
static uint8_t  init_step  = 0;
static uint32_t clear_tick = 0;
static uint8_t  frame[1 + 4 * (LCD_COLS + 1)];

/* ------------------------------------------------------------------ */
/* Private helpers                                                    */
/* ------------------------------------------------------------------ */

static uint8_t LCD_Tx(uint8_t *p, uint16_t n)
{
    return HAL_I2C_Master_Transmit(&hi2c1, LCD_HAL_ADDR, p, n,
                                   LCD_I2C_TIMEOUT) == HAL_OK;
}

/* Arduino write4bits(): one nibble (upper 4 bits of n), RS = 0.
 * Bytes: data -> data|EN -> data (EN falls, LCD latches). */
static uint8_t LCD_Write4Bits(uint8_t n)
{
    uint8_t b = (n & 0xF0) | backlight;
    uint8_t seq[3] = { b, b | LCD_EN, b };
    return LCD_Tx(seq, 3);
}

/* Arduino send(): n bytes, each as high then low nibble, in ONE I2C frame.
 * First byte sets RS with EN low (t_AS); each nibble is EN high -> EN low.
 * Each I2C byte ~90 us >= PW_EH 450 ns and >= 37 us execution time. */
static uint8_t LCD_Send(const uint8_t *val, uint8_t n, uint8_t rs)
{
    uint16_t k = 0;
    uint8_t ctl = rs | backlight;

    frame[k++] = ctl;
    for (uint8_t i = 0; i < n; i++)
    {
        uint8_t hi = (val[i] & 0xF0) | ctl;
        uint8_t lo = (uint8_t)(val[i] << 4) | ctl;
        frame[k++] = hi | LCD_EN;  frame[k++] = hi;
        frame[k++] = lo | LCD_EN;  frame[k++] = lo;
    }
    return LCD_Tx(frame, k);
}

static uint8_t LCD_Command(uint8_t cmd)
{
    return LCD_Send(&cmd, 1, 0);
}

/* Clear/Home need 1.52 ms: block other writes until it has elapsed */
static uint8_t LCD_Ready(void)
{
    return (LCD_flag == 2) && (HAL_GetTick() - clear_tick >= LCD_CLEAR_TIME);
}

/* ------------------------------------------------------------------ */
/* Public API                                                         */
/* ------------------------------------------------------------------ */

/* Called while LCD_flag == 0, after LCD_INIT_TIME from power-on.
 * Checks the PCF8574 ACKs, then resets expander outputs (RS = RW = EN = 0). */
void LCD_Connect(void)
{
    uint8_t b = backlight;

    if (HAL_I2C_IsDeviceReady(&hi2c1, LCD_HAL_ADDR, 1, LCD_I2C_TIMEOUT) != HAL_OK)
        return;
    if (!LCD_Tx(&b, 1))
        return;

    init_step = 0;
    LCD_flag  = 1;
}

/* Called while LCD_flag == 1, every LCD_STEP_TIME. Figure 24 (4-bit):
 * step 0: 0011            wait > 4.1 ms
 * step 1: 0011            wait > 100 us
 * step 2: 0011
 * step 3: 0010            4-bit mode
 * step 4: Function set (2 lines, 5x8), Display off, Display clear
 *                         wait > 1.52 ms
 * step 5: Entry mode set, Display on -> ready
 * I2C error -> flag = 0 (reconnect, restart init). */
void LCD_Init(void)
{
    static const uint8_t cfg[3] = {
        LCD_FUNCTIONSET | LCD_2LINE,
        LCD_DISPLAYCONTROL,                 /* display off */
        LCD_CLEARDISPLAY
    };
    static const uint8_t fin[2] = {
        LCD_ENTRYMODESET | LCD_ENTRYLEFT,
        LCD_DISPLAYCONTROL | LCD_DISPLAYON  /* cursor off, blink off */
    };
    uint8_t ok = 0;

    switch (init_step)
    {
    case 0: ok = LCD_Write4Bits(0x30);    break;
    case 1: ok = LCD_Write4Bits(0x30);    break;
    case 2: ok = LCD_Write4Bits(0x30);    break;
    case 3: ok = LCD_Write4Bits(0x20);    break;
    case 4: ok = LCD_Send(cfg, 3, 0);     break;
    case 5: ok = LCD_Send(fin, 2, 0);     break;
    default:                              break;
    }

    if (!ok)
    {
        LCD_flag = 0;
        return;
    }

    if (++init_step > 5)
    {
        clear_tick = HAL_GetTick();
        LCD_flag   = 2;
    }
}

/* status: 0 = off, else on. Works in any state once connected. */
void LCD_SetBacklight(int status)
{
    backlight = status ? LCD_BL : 0;
    if (LCD_flag == 0) return;

    uint8_t b = backlight;
    LCD_Tx(&b, 1);
}

/* Takes 1.52 ms in the LCD; LCD_Print is ignored for LCD_CLEAR_TIME after.
 * Usually not needed: LCD_Print pads the row with spaces. */
void LCD_ClearDisplay(void)
{
    if (!LCD_Ready()) return;

    if (LCD_Command(LCD_CLEARDISPLAY))
        clear_tick = HAL_GetTick();
    else
        LCD_flag = 0;
}

/* setCursor(0, row) + print(str), padded with spaces to 16 columns.
 * row: 0 or 1. Text longer than 16 chars is cut. */
void LCD_Print(char* str, int row)
{
    static const uint8_t row_offsets[2] = { 0x00, 0x40 };
    uint8_t line[LCD_COLS];
    uint8_t i = 0;

    if (!LCD_Ready()) return;
    if (row < 0) row = 0;
    if (row > 1) row = 1;

    while (i < LCD_COLS && str[i] != '\0') { line[i] = (uint8_t)str[i]; i++; }
    while (i < LCD_COLS)                   { line[i++] = ' '; }

    if (!LCD_Command(LCD_SETDDRAMADDR | row_offsets[row]) ||
        !LCD_Send(line, LCD_COLS, LCD_RS))
        LCD_flag = 0;
}
