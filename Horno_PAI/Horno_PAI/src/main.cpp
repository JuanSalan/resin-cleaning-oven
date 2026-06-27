#include <Arduino.h>
#include "Estados.h"
#include "Hardware.h"
#include "Menu.h"
#include "PIDControl.h"
#include "BluetoothSerial.h"
#include "Sensores.h"
#include "main.h"

BluetoothSerial SerialBT;
float temp = 0;
float temp_b = 0;
String dato;

// --- Muestreo y promedio de temperatura del horno ---
const unsigned long INTERVALO_MUESTREO_HORNO = 300;   // lectura cruda cada 300ms
const unsigned long HORNO_INTERVALO = 1000;           // 6 muestras x 300ms = 1.8s
const int NUM_MUESTRAS_HORNO = HORNO_INTERVALO / INTERVALO_MUESTREO_HORNO; // 6 muestras

float muestrasHorno[NUM_MUESTRAS_HORNO];
int indiceMuestraHorno = 0;
unsigned long ultimoMuestreoHorno = 0;
unsigned long ultimoPromedioHorno = 0;

// Ventana de silencio: evita confiar en una lectura muy cerca de un cambio de estado
// del SSR, ya que la conmutacion induce ruido en la referencia del MAX6675.
//extern unsigned long ultimoCambioSSR; // definido en PIDControl.cpp
const unsigned long VENTANA_SILENCIO_SSR = 100; // ms de margen tras un flanco del SSR
float ultimaMuestraValidaHorno = 0;
bool primeraMuestraHorno = true;

// --- Beaker (sin cambios, igual que antes) ---
unsigned long ultimaLecturaBeaker = 0;
const unsigned long INTERVALO_BEAKER = 200;

void setup()
{
  inicializarHardware();
  iniciarMenu();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FC_PUERTA, INPUT);
  pinMode(SSR_PIN, OUTPUT);
  digitalWrite(SSR_PIN, LOW);
  iniciarPID();
  Serial.begin(115200);
  SerialBT.begin("Horno_PAI");
}

void loop()
{
  actualizarEstado();
  actualizarMenu();

  if (estado != CALENTAR_BEAKER && estado != MANTENER_BEAKER)
  {
    if (!getEstadoSSR())
    {
      if (millis() - ultimoMuestreoHorno >= INTERVALO_MUESTREO_HORNO)
        {
          ultimoMuestreoHorno = millis();
          float lectura = temperaturaHorno();
          ultimaMuestraValidaHorno = lectura;
          muestrasHorno[indiceMuestraHorno] = lectura;
          indiceMuestraHorno++;
          if (indiceMuestraHorno >= NUM_MUESTRAS_HORNO)
            {
                indiceMuestraHorno = 0;
            }
        }
    }

    if (millis() - ultimoPromedioHorno >= HORNO_INTERVALO)
    {
      ultimoPromedioHorno = millis();

      float suma = 0;
      for (int i = 0; i < NUM_MUESTRAS_HORNO; i++)
      {
        suma += muestrasHorno[i];
      }
      temp = suma / NUM_MUESTRAS_HORNO;
      
    Serial.print(millis()); Serial.print(",");
    Serial.print(temp); Serial.print(",");
    Serial.print(parametros.tempHorno); Serial.print(",");
    Serial.print(muestrasHorno[0]); Serial.print(",");
    Serial.print(muestrasHorno[1]); Serial.print(",");
    Serial.print(muestrasHorno[2]); Serial.print(",");
    Serial.println(getEstadoSSR()); 

    }
  }


  else
  {
    if (!getEstadoSSR())
    {
      if (millis() - ultimoMuestreoHorno >= INTERVALO_MUESTREO_HORNO)
        {
          ultimoMuestreoHorno = millis();
          float lectura = temperaturaBeaker();
          ultimaMuestraValidaHorno = lectura;
          muestrasHorno[indiceMuestraHorno] = lectura;
          indiceMuestraHorno++;
          if (indiceMuestraHorno >= NUM_MUESTRAS_HORNO)
            {
                indiceMuestraHorno = 0;
            }
        }
    }

    if (millis() - ultimoPromedioHorno >= HORNO_INTERVALO)
    {
      ultimoPromedioHorno = millis();

      float suma = 0;
      for (int i = 0; i < NUM_MUESTRAS_HORNO; i++)
      {
        suma += muestrasHorno[i];
      }
      temp = suma / NUM_MUESTRAS_HORNO;
      
    Serial.print(millis()); Serial.print(",");
    Serial.print(temp); Serial.print(",");
    Serial.print(parametros.tempBeaker); Serial.print(",");
    Serial.print(muestrasHorno[0]); Serial.print(",");
    Serial.print(muestrasHorno[1]); Serial.print(",");
    Serial.print(muestrasHorno[2]); Serial.print(",");
    Serial.println(getEstadoSSR()); 

    }
  }
}