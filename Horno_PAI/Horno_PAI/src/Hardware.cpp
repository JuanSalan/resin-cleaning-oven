#include "Hardware.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>




I2CKeyPad keypad(0x20);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void inicializarHardware()
{
    Wire.begin();
    keypad.begin();
    lcd.begin(16,2);
    lcd.backlight();
}