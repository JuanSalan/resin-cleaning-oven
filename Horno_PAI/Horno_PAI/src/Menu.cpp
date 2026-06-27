#include "Estados.h"
#include "Hardware.h"
#include "Sensores.h"
#include "main.h"
static EstadoProceso estadoAnterior = ERROR_SISTEMA;
void mostrarIngresoDatos()
{
    
    
    switch (pasoIngreso)
    {
        case ING_TEMP_HORNO:
            lcd.setCursor(0,0);
            lcd.print("Temp Horno:");
            lcd.setCursor(0,1);
            lcd.print("");
            lcd.print(bufferEntrada);
            break;

        case ING_TIEMPO_HORNO:
            lcd.setCursor(0,0);
            lcd.print("Tiempo Horno:");
            lcd.print("");
            lcd.setCursor(0,1);
            lcd.print("");
            lcd.print(bufferEntrada);
            break;

        case ING_TEMP_BEAKER:
            lcd.setCursor(0,0);
            lcd.print("Temp Beaker:");
            lcd.setCursor(0,1);
            lcd.print(bufferEntrada);
            break;

        case ING_TIEMPO_BEAKER:
            lcd.setCursor(0,0);
            lcd.print("Tiempo Beaker:");
            lcd.setCursor(0,1);
            lcd.print(bufferEntrada);
            break;

        case CONFIRMAR:
            lcd.setCursor(0,0);
            lcd.print("Cierre la Puerta");
            lcd.setCursor(0,1);
            lcd.print("y oprima A");
            break;
    }
}

void mostrarPrecalentamiento()
{
    lcd.setCursor(0,0);
    lcd.print("PRECALENTANDO ");

    lcd.setCursor(0,1);

    lcd.print("T:");
    lcd.print(temp,1);
    lcd.print(" C    ");
}

void mostrarCargaHorno()
{
    
    lcd.setCursor(0,0);
    lcd.print("POR FAVOR CARGUE");

    lcd.setCursor(0,1);
    lcd.print("EL MATERIAL");
}

void mostrarHorneado()
{
    lcd.setCursor(0,0);
    lcd.print("SP:");
    lcd.print(parametros.tempHorno,0);

    lcd.print(" PV:");
    lcd.print(temp,1);

    lcd.setCursor(0,1);

    unsigned long tiempo = (millis() - tiempoInicio) / 1000;

    lcd.print("t:");
    lcd.print(tiempo);
    lcd.print("s");
}

void mostrarRetiro()
{
    
    lcd.setCursor(0,0);
    lcd.print("RETIRE LA");

    lcd.setCursor(0,1);
    lcd.print("PROBETA");
}

void mostrarProcesoBeaker()
{
    lcd.setCursor(0,0);
    lcd.print("SP:");
    lcd.print(parametros.tempBeaker,0);

    lcd.print(" PV:");
    lcd.print(temp_b,1);

    lcd.setCursor(0,1);

    unsigned long tiempo = (millis() - tiempoInicio) / 1000;

    lcd.print("t:");
    lcd.print(tiempo);
    lcd.print("s");
}

void mostrarFin()
{
    
    lcd.setCursor(0,0);
    lcd.print("PROCESO");

    lcd.setCursor(0,1);
    lcd.print("TERMINADO");
}

void mostrarError()
{
    
    lcd.setCursor(0,0);
    lcd.print("ERROR");

    lcd.setCursor(0,1);
    lcd.print("ERROR");
}
void iniciarMenu()
{
    estadoAnterior = ERROR_SISTEMA;
}
void actualizarMenu()
{
    if (estado != estadoAnterior)
    {
        lcd.clear();
        estadoAnterior = estado;
    }
    switch(estado)
    {
        case INGRESO_DATOS:
            mostrarIngresoDatos();
            break;

        case PRECALENTAR_HORNO:
            mostrarPrecalentamiento();
            break;

        case ESPERAR_CARGA_HORNO:
            mostrarCargaHorno();
            break;

        case MANTENER_HORNO:
            mostrarHorneado();
            break;

        case ESPERAR_RETIRO:
            mostrarRetiro();
            break;

        case CALENTAR_BEAKER:
            mostrarPrecalentamiento();
            break;

        case ESPERAR_CARGA_BEAKER:
            mostrarCargaHorno();
            break;

        case MANTENER_BEAKER:
            mostrarProcesoBeaker();
            break;

        case PROCESO_TERMINADO:
            mostrarFin();
            break;

        case ERROR_SISTEMA:
            mostrarError();
            break;
    }
}

