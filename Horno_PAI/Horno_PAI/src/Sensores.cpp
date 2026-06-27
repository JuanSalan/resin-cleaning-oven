#include "Sensores.h"
#include "Hardware.h"

#include <max6675.h>

MAX6675 tc1(TC_SCK, TC1_CS, TC_SO);
MAX6675 tc2(TC_SCK, TC2_CS, TC_SO);
MAX6675 tc3(TC_SCK, TC3_CS, TC_SO);
MAX6675 tc4(TC_SCK, TC4_CS, TC_SO);

void iniciarSensores()
{
    delay(500);
}

float temperaturaHorno()
{
    /*
    Serial.print("TC1: ");
    Serial.println(tc1.readCelsius());
    Serial.print("TC2: ");
    Serial.println(tc2.readCelsius()-1.5);
    Serial.print("TC3: ");
    Serial.println(tc3.readCelsius());
    Serial.print("TC4: ");
    Serial.println(tc4.readCelsius()+1.5);
    */
    return (
        (tc1.readCelsius()+0.5) +
        (tc2.readCelsius()-1.5 )+
        tc3.readCelsius()
    ) / 3.0;
}

float temperaturaBeaker()
{
    return (tc4.readCelsius()+1.5);
}

bool sensoresOK()
{
    return !(
        isnan(tc1.readCelsius()) ||
        isnan(tc2.readCelsius()) ||
        isnan(tc3.readCelsius()) ||
        isnan(tc4.readCelsius())
    );
}