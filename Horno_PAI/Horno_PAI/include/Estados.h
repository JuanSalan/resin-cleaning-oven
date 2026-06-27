#include <cstdint>
#include <Arduino.h>
#ifndef ESTADOS_H
#define ESTADOS_H

enum EstadoProceso
{
    INGRESO_DATOS = 1,

    PRECALENTAR_HORNO,

    ESPERAR_CARGA_HORNO,

    MANTENER_HORNO,

    ESPERAR_RETIRO,

    CALENTAR_BEAKER,

    ESPERAR_CARGA_BEAKER,

    MANTENER_BEAKER,

    PROCESO_TERMINADO,

    ERROR_SISTEMA
};

enum PasoIngreso
{
    ING_TEMP_HORNO,
    ING_TIEMPO_HORNO,
    ING_TEMP_BEAKER,
    ING_TIEMPO_BEAKER,
    CONFIRMAR
};



struct ParametrosProceso
{
    float tempHorno;
    uint32_t tiempoHorno;
    float tempBeaker;
    uint32_t tiempoBeaker;

};


extern PasoIngreso pasoIngreso;
extern EstadoProceso estado;
extern String bufferEntrada;
extern ParametrosProceso parametros;
extern EstadoProceso estado;
extern unsigned long tiempoInicio;
extern unsigned long tiempoInicio_beaker;

void actualizarEstado();

#endif