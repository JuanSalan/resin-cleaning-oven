#ifndef PIDCONTROL_H
#define PIDCONTROL_H
extern unsigned long ultimoCambioSSR;
extern bool estadoSSRActual;

bool getEstadoSSR();

void iniciarPID();

void actualizarPID(float temperatura, bool Modo = true);

void controlarSSR( bool Modo = true);

void setSetPoint(float sp , bool Modo = true);

bool ventanaMedicionActiva();

#endif