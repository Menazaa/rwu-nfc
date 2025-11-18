#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ========== NFC CONFIGURATION ==========
// Uncomment ONE of the following:
// #define USE_I2C
#define USE_SPI

// ========== PIN DEFINITIONS ==========

// PN532 NFC Module Pins
#define PN532_SCK       13
#define PN532_MISO      12
#define PN532_MOSI      11
#define PN532_SS        10
#define PN532_IRQ       2
#define PN532_RESET     3

// ST7735 TFT Display Pins (1.8" 128x160)
#define TFT_CS          9
#define TFT_RST         8
#define TFT_DC          7
// TFT shares hardware SPI: MOSI=11, SCLK=13

// Push Button Pins
#define BTN_UP          4
#define BTN_DOWN        5
#define BTN_SELECT      6
#define BTN_BACK        A0

// ========== EEPROM CONFIGURATION ==========
#define EEPROM_START_ADDR   0
#define MAX_STORED_CARDS    20
#define CARD_DATA_SIZE      64

// ========== FREERTOS CONFIGURATION ==========
#define TASK_STACK_DISPLAY  256
#define TASK_STACK_NFC      256
#define TASK_STACK_BUTTON   128
#define TASK_PRIORITY_HIGH  2
#define TASK_PRIORITY_LOW   1

// ========== DISPLAY CONFIGURATION ==========
#define SCREEN_WIDTH    160
#define SCREEN_HEIGHT   128
#define TEXT_SIZE       1
#define MENU_ITEMS      5

// ========== DEBOUNCE SETTINGS ==========
#define BUTTON_DEBOUNCE_MS  200
#define CARD_SCAN_DELAY_MS  2000

#endif // CONFIG_H
