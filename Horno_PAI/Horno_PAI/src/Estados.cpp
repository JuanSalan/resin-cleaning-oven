#include "Estados.h"
#include <Arduino.h>
#include <Wire.h>
#include <I2CKeyPad.h>
#include "Hardware.h"
#include "PIDControl.h"
#include "Sensores.h"
#include "Alarmas.h"
#include "main.h"

EstadoProceso estado = INGRESO_DATOS;

ParametrosProceso parametros;

PasoIngreso pasoIngreso = ING_TEMP_HORNO;

String bufferEntrada = "";

unsigned long tiempoInicio = 0;

unsigned long tiempo_acumulado = 0;
unsigned long tiempo_acumulado_beaker= 0;

unsigned long tiempoInicio_beaker = 0;

bool estadoAnterior_beaker = false;
bool estadoAnterior = false;

static unsigned long ultimoTecla = 0;


void estadoIngresoDatos()
{
    uint8_t tecla = keypad.getKey();

    char keys[] = "123A456B789C*0#DNF";

    if (keys[tecla] == 'N')
        return;
    if (millis() - ultimoTecla < 200)
        return;

    ultimoTecla = millis();

    if (keys[tecla] >= '0' && keys[tecla] <= '9')
    {
        bufferEntrada += keys[tecla];
    }

    if (keys[tecla] == 'B')
    {
        if (bufferEntrada.length() > 0)
        {
            bufferEntrada.remove(bufferEntrada.length() - 1);
            lcd.clear();
        }
            
    }

    if (keys[tecla] == 'A')
    {
        lcd.clear();
        switch (pasoIngreso)
        {
            case ING_TEMP_HORNO:
                parametros.tempHorno = bufferEntrada.toFloat();
                bufferEntrada = "";
                pasoIngreso = ING_TIEMPO_HORNO;
                break;

            case ING_TIEMPO_HORNO:
                parametros.tiempoHorno = bufferEntrada.toInt()*60*1000;
                bufferEntrada = "";
                pasoIngreso = ING_TEMP_BEAKER;
                break;

            case ING_TEMP_BEAKER:
                parametros.tempBeaker = bufferEntrada.toFloat();
                bufferEntrada = "";
                pasoIngreso = ING_TIEMPO_BEAKER;
                break;

            case ING_TIEMPO_BEAKER:
                parametros.tiempoBeaker = bufferEntrada.toInt()*60000;
                bufferEntrada = "";
                pasoIngreso = CONFIRMAR;
                break;
            
            
            case CONFIRMAR:
                BeepConfirmar();
                bool puerta =digitalRead(FC_PUERTA);
                Serial.println(puerta);
                if ( puerta == HIGH){
                    estado = PRECALENTAR_HORNO;
                }
                break;
            
        }
    }
    if (keys[tecla] == 'D')
    {
        pasoIngreso = ING_TEMP_HORNO;
        lcd.clear();
    }
}

void estadoPrecalentarHorno()
{
    bool puerta =digitalRead(FC_PUERTA);  
    if (puerta == LOW)
    {
        lcd.setCursor(0,1);
        lcd.print("CIERRE LA PUERTA");
        lcd.clear();
        
    }
    else
    {
        
        setSetPoint(parametros.tempHorno);

        actualizarPID(temp);

        controlarSSR();
        if (temp >= parametros.tempHorno )
        {
            beepListo();
            estado = ESPERAR_CARGA_HORNO;
        }
    }
    
    
}

void estadoEsperarCargaHorno()

{
    setSetPoint(25);
    actualizarPID(temp);
    controlarSSR();
    estadoAnterior = false;
    bool puerta =digitalRead(FC_PUERTA);
    if (digitalRead(FC_PUERTA)==LOW)
    {

        while (true)
        {
            delay(500);
            if (digitalRead(FC_PUERTA)==HIGH)
                {
                    beepListo();
                    tiempoInicio = millis();
                    estado = MANTENER_HORNO;
                    break;
                }
        }
    }
}

void estadoMantenerHorno()
{
    bool puerta =digitalRead(FC_PUERTA);
    if (puerta ==  LOW)
    {
        tiempo_acumulado=millis() - tiempoInicio;
        while (true)
        {
            if (digitalRead(FC_PUERTA) ==  HIGH)
            {
                parametros.tiempoHorno -= tiempo_acumulado;
                break;
            }
            else
            {
                lcd.setCursor(0,1);
                lcd.print("CIERRE LA PUERTA");
                setSetPoint(18);
                actualizarPID(temp);
                controlarSSR();
                lcd.clear();
            }
        }     
    }
    else
    {
        if (millis() - tiempoInicio >= parametros.tiempoHorno)
        {
            digitalWrite(SSR_PIN, LOW);
            estado = ESPERAR_RETIRO;
        }
        else
        {
            setSetPoint(parametros.tempHorno);
            actualizarPID(temp);
            controlarSSR();
        }
    }
}

void estadoEsperarRetiro()

{
    setSetPoint(18);
    actualizarPID(temp);
    controlarSSR();
    estadoAnterior_beaker = false;
    bool puerta =digitalRead(FC_PUERTA);
    if (digitalRead(FC_PUERTA)==HIGH)
    {
        Serial.println("paso 1");
        while (true)
        {
            delay(500);
            Serial.println("paso 2");
            if (digitalRead(FC_PUERTA)==LOW)
                {
                    Serial.println("paso 3");
                    beepListo();
                    estado = CALENTAR_BEAKER;
                    break;
                }
        }
    }
}

void estadoPrecalentarBeaker()
{
    bool puerta =digitalRead(FC_PUERTA);  
    if (puerta == LOW)
    {
        
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("CIERRE LA PUERTA");
        
    }
    else
    {
        
        setSetPoint(parametros.tempBeaker, false);

        actualizarPID(temp_b, false);

        controlarSSR(false);
        if (temp_b >= parametros.tempBeaker )
        {
            beepListo();
            estado = ESPERAR_CARGA_HORNO;
        }
    }
    
}

void estadoEsperarCargaBeaker()

{
    setSetPoint(50);
    actualizarPID(temp_b);
    controlarSSR();
    estadoAnterior = false;
    bool puerta =digitalRead(FC_PUERTA);
    if (digitalRead(FC_PUERTA)==LOW)
    {
        while (true)
        {
            delay(500);
            if (digitalRead(FC_PUERTA)==HIGH)
                {
                    beepListo();
                    tiempoInicio_beaker = millis();
                    estado = MANTENER_BEAKER;
                    break;
                }
        }
    }
}

void estadoMantenerBeaker()
{
    bool puerta =digitalRead(FC_PUERTA);
    if (puerta ==  LOW)
    {
        tiempo_acumulado_beaker=millis() - tiempoInicio_beaker;
        while (true)
        {
            if (digitalRead(FC_PUERTA) ==  HIGH)
            {
                parametros.tiempoBeaker -= tiempo_acumulado_beaker;
                break;
            }
            else
            {
                lcd.setCursor(0,1);
                lcd.print("CIERRE LA PUERTA");
                setSetPoint(50,false);
                actualizarPID(temp_b,false);
                controlarSSR(false);
                lcd.clear();
            }
        }     
    }
    else
    {
        if (millis() - tiempoInicio_beaker >= parametros.tiempoBeaker)
        {
            digitalWrite(SSR_PIN, LOW);
            estado = ESPERAR_RETIRO;
        }
        else
        {
            setSetPoint(parametros.tempBeaker , false);
            actualizarPID(temp_b , false);
            controlarSSR(false);
        }
    }
}

void estadoProcesoTerminado()
{
    digitalWrite(SSR_PIN, LOW);
    beepFin();
}

void actualizarEstado()
{
    switch (estado)
    {
        case INGRESO_DATOS:
            estadoIngresoDatos();
            break;

        case PRECALENTAR_HORNO:
            estadoPrecalentarHorno();
            break;

        case ESPERAR_CARGA_HORNO:
            estadoEsperarCargaHorno();
            break;
        case MANTENER_HORNO:
            estadoMantenerHorno();
            break;
        case ESPERAR_RETIRO:
            estadoEsperarRetiro();
            break;
        case CALENTAR_BEAKER:
            estadoPrecalentarBeaker();
            break;
        case ESPERAR_CARGA_BEAKER:
            estadoEsperarCargaBeaker();
            break;
        case MANTENER_BEAKER:
            estadoMantenerBeaker();
            break;
        case PROCESO_TERMINADO:
            estadoProcesoTerminado();
            break;
        case ERROR_SISTEMA:
            estadoPrecalentarHorno();
            break;
        default:
            break;
    }
}