#ifndef HARDWARE_H
#define HARDWARE_H

#define SSR_PIN        25

#define BUZZER_PIN     26

#define LED_STANDBY    13
#define LED_CALENTANDO 0
#define LED_LISTO      23

#define FC_PUERTA      32
#define BTN_STOP       33

#define TC1_CS         5
#define TC2_CS         16
#define TC3_CS         17
#define TC4_CS         4

#define TC_SCK         18
#define TC_SO          19

#define I2C_SDA        21
#define I2C_SCL        22



#include <I2CKeyPad.h>
#include <LiquidCrystal_I2C.h>

extern I2CKeyPad keypad;
extern LiquidCrystal_I2C lcd;

void inicializarHardware();

#endif