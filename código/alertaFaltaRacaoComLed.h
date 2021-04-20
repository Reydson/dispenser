#ifndef TALERTAFALTARACAOCOMLED
#define TALERTAFALTARACAOCOMLED
#include <Arduino.h>
#include <string.h>
#include "alertaFaltaRacao.h";

class TAlertaFaltaRacaoComLed : public TAlertaFaltaRacao {

private:

int pinoLed;
unsigned long ligadoEm;

public:

TAlertaFaltaRacaoComLed(int pinoLed){
  pinMode(pinoLed, OUTPUT);
  digitalWrite(pinoLed, LOW);
  this->pinoLed = pinoLed;
  this->ligadoEm = millis();
}

void processar(bool possuiRacao) {
  if(this->ligadoEm > millis()) { //caso a função millis() tenha atingido o limite de tempo e tenha resetado
    this->ligadoEm = millis();
  }
  if(!possuiRacao) {
    if((millis() - this->ligadoEm) >= 2000) {
      digitalWrite(pinoLed, HIGH);
      this->ligadoEm = millis();
    }
    if((millis() - this->ligadoEm) >= 100) {
      digitalWrite(pinoLed, LOW);
    }
  } else {
    digitalWrite(pinoLed, LOW);
  }
}

};

#endif
