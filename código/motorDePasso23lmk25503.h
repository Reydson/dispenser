#ifndef TMOTORDEPASSO23LMK25503
#define TMOTORDEPASSO23LMK25503
#include <Arduino.h>
#include <string.h>

#include "motor.h"

class TMotorDePasso23lmk25503 : public TMotor {

private:

  int passosADar;
  int voltasADar;
  int pinoEnable;
  bool nivelLogicoEnable;
  int pinoStep;
  bool nivelLogicoStep;
  unsigned long ultimoPasso;

public:

  TMotorDePasso23lmk25503(int pinoEnable, int pinoStep) {
    this->passosADar = 0;
    this->voltasADar = 0;
    this->pinoEnable = pinoEnable;
    this->pinoStep = pinoStep;
    pinMode(pinoEnable, OUTPUT);
    this->nivelLogicoEnable = true;
    pinMode(pinoStep, OUTPUT);
    this->nivelLogicoStep = false;
    this->ultimoPasso = millis();
  }

  void darVoltas(int numeroDeVoltas) {
    this->voltasADar += numeroDeVoltas;
  }

  void processar() {
    if(this->ultimoPasso > millis()) { //caso a função millis() tenha atingido o limite de tempo e tenha resetado
      this->ultimoPasso = millis();
    }
    
    if(this->passosADar == 0 && this->voltasADar > 0) {
      this->passosADar += PASSOSPORVOLTA;
      this->voltasADar--;
    }

    if(this->passosADar == 0 && this->voltasADar == 0 && ((millis() - this->ultimoPasso) >= TEMPOENTREPASSOS)) {
      this->nivelLogicoEnable = true;
    }

    if(this->passosADar > 0 && ((millis() - this->ultimoPasso) >= TEMPOENTREPASSOS)) {
      this->nivelLogicoEnable = false;
      this->nivelLogicoStep = true;
      this->ultimoPasso = millis();
      this->passosADar--;
    }

    if(this->nivelLogicoStep && millis() - this->ultimoPasso >= TEMPOENTREPASSOS/2) {
      this->nivelLogicoStep = false;
    }
    pinMode(this->pinoEnable, OUTPUT);
    pinMode(this->pinoStep, OUTPUT);
    digitalWrite(this->pinoEnable, this->nivelLogicoEnable);
    digitalWrite(this->pinoStep, this->nivelLogicoStep);
  }
  
};

#endif
