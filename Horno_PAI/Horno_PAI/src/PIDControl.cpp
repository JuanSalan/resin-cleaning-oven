#include "PIDControl.h"
#include "Hardware.h"
#include <Arduino.h>
#include <PID_v1.h>

double Input;
double Output;
double Setpoint;

double Input_b;
double Output_b;
double Setpoint_b;

double Kp = 6;
double Ki = 0.05;
double Kd = 30;

double Kp_b = 8;
double Ki_b = 0.05;
double Kd_b = 30;

PID hornoPID(
    &Input,
    &Output,
    &Setpoint,
    Kp,
    Ki,
    Kd,
    DIRECT
);

PID BeakerPID(
    &Input_b,
    &Output_b,
    &Setpoint_b,
    Kp_b,
    Ki_b,
    Kd_b,
    DIRECT
);

const unsigned long WINDOW_SIZE = 5000;
const unsigned long TIEMPO_MEDICION = 1000;

unsigned long windowStart;

unsigned long ultimoCambioSSR = 0;   // NUEVO: timestamp del ultimo flanco del SSR
bool estadoSSRActual = false;        // NUEVO: para detectar el cambio (flanco), no solo el nivel

void iniciarPID()
{
    hornoPID.SetSampleTime(1000);
    windowStart = millis();

    hornoPID.SetOutputLimits(0, WINDOW_SIZE);

    hornoPID.SetMode(AUTOMATIC);

    BeakerPID.SetSampleTime(1000);

    BeakerPID.SetOutputLimits(0, WINDOW_SIZE);

    BeakerPID.SetMode(AUTOMATIC);
}

void setSetPoint(float sp, bool Modo)
{
    if (Modo == true)
    {
        Setpoint = sp;
    }
    else
    {
        Setpoint_b = sp;
    }
    
}

void actualizarPID(float temperatura, bool Modo)
{
    if (Modo == true)
    {
        Input = temperatura;

        double error = Setpoint - Input;

        // ANTI-WINDUP DINÁMICO:
        
        if (abs(error) > 15) 
        {
            hornoPID.SetTunings(Kp, 0, Kd); 
        } 
        else 
        {
            hornoPID.SetTunings(Kp, Ki, Kd); // Tu Ki original es 0.5
        }

        hornoPID.Compute();
    }
    else
    {
        Input = temperatura;

        double error = Setpoint - Input;

        // ANTI-WINDUP DINÁMICO:
        
        if (abs(error) > 15) 
        {
            BeakerPID.SetTunings(Kp, 0, Kd); 
        } 
        else 
        {
            BeakerPID.SetTunings(Kp, Ki, Kd); // Tu Ki original es 0.5
        }

        BeakerPID.Compute();
    }
    
}

double getSalidaPID()
{
    return Output;
}

void controlarSSR(bool Modo)
{
    if (millis() - windowStart > WINDOW_SIZE)
        windowStart += WINDOW_SIZE;

    unsigned long tiempoVentana = millis() - windowStart;

    bool nuevoEstado;

    // Últimos 300 ms reservados para lectura de termocuplas
    if (tiempoVentana >= (WINDOW_SIZE - TIEMPO_MEDICION))
    {
        nuevoEstado = false;
    }
    else
    {
        if (Modo == true)
        {
            nuevoEstado = (Output > tiempoVentana);
        }
        else
        {
            nuevoEstado = (Output_b > tiempoVentana);
        }
        
    }

    if (nuevoEstado != estadoSSRActual)
    {
        ultimoCambioSSR = millis();
        estadoSSRActual = nuevoEstado;
    }

    digitalWrite(SSR_PIN, nuevoEstado ? HIGH : LOW);
}

bool ventanaMedicionActiva()
{
    unsigned long tiempoVentana = millis() - windowStart;

    return (tiempoVentana >= (WINDOW_SIZE - TIEMPO_MEDICION));
}

bool getEstadoSSR()
{
    return estadoSSRActual;
}