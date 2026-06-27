#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

// Variables globales de temperatura
// Definidas en main.cpp, accesibles desde cualquier .cpp que incluya este header
extern float temp;     // Temperatura del horno
extern float temp_b;   // Temperatura del beaker

#endif // MAIN_H