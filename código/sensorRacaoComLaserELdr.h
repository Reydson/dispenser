#ifndef TSENSORRACAOCOMLASERELDR
#define TSENSORRACAOCOMLASERELDR
#include <Arduino.h>
#include <string.h>
#include "sensorRacao.h";

class TSensorRacaoComLaserELdr : public TSensorRacao {

private:

int pinoLaser;
int pinoLdr;
unsigned long ultimaLeitura;
unsigned long momentoSensorLigado;
bool possuiRacao;
bool laserLigado;

public:

TSensorRacaoComLaserELdr(int pinoLaser, int pinoLdr){
  pinMode(pinoLaser, OUTPUT);
  digitalWrite(pinoLaser, LOW);
  this->pinoLaser = pinoLaser;
  this->pinoLdr = pinoLdr;
  this->ultimaLeitura = millis();
  this->processar(true);
}

void processar(bool forcarLeitura = false) {
  if(this->ultimaLeitura > millis()) { //caso a função millis() tenha atingido o limite de tempo e tenha resetado
    this->ultimaLeitura = millis();
  }
  if(this->momentoSensorLigado > millis()) { //caso a função millis() tenha atingido o limite de tempo e tenha resetado
    this->momentoSensorLigado = millis();
  }
  if(forcarLeitura || ((millis() - this->ultimaLeitura) >= (INTERVALOLEITURAS * 1000))) {
    this->laserLigado = true;
    this->momentoSensorLigado = millis();
    this->ultimaLeitura = millis();
  }
  if(this->laserLigado && (millis() - this->momentoSensorLigado) >= 20) {
    int valorLido = analogRead(this->pinoLdr);
    this->possuiRacao = (valorLido < VALORACIONAMENTO);
    this->laserLigado = false;
  }
  digitalWrite(this->pinoLaser, this->laserLigado);
}

bool temRacao() {
  return this->possuiRacao;
}
 
};

#endif
