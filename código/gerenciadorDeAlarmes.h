#ifndef TGERENCIADORDEALARMES
#define TGERENCIADORDEALARMES
#include <Arduino.h>
#include <string.h>

#include "alarme.h"
#include "motor.h"

class TGerenciadorDeAlarmes {

private:

  TArmazenamentoPermanente * armazenamentoPermanente;
  TMotor * motor;
  TAlarme alarmes [QUANTIDADEDEALARMES];
  int executado [QUANTIDADEDEALARMES];
  
public:

  TGerenciadorDeAlarmes(TArmazenamentoPermanente * armazenamentoPermanente, TMotor * motor) {
    this->armazenamentoPermanente = armazenamentoPermanente;
    this->motor = motor;
    for(int i = 0; i < QUANTIDADEDEALARMES; i++) {
      this->alarmes[i] = this->armazenamentoPermanente->lerAlarme(i);
      this->executado[i] = false;
    }
  }

  void processar(int diaDaSemana, int hora, int minuto) {
    for(int i = 0; i < QUANTIDADEDEALARMES; i++) {
      if(!this->executado[i] && this->alarmes[i].getAtivo() && this->alarmes[i].getDiaDaSemana(diaDaSemana) && this->alarmes[i].getHora() == hora && this->alarmes[i].getMinuto() == minuto) {
        this->motor->darVoltas(this->alarmes[i].getValor());
        this->executado[i] = true;
      } else {
        if(this->executado[i] && this->alarmes[i].getMinuto() != minuto) {
          this->executado[i] = false;
        }
      }
    }
  }

  int quantidadeDeAlarmesAtivos() {
    int temp = 0;
    for(int i = 0; i < QUANTIDADEDEALARMES; i++) {
      if(this->alarmes[i].getAtivo()) {
        temp++;
      }
    }
    return temp;
  }

  void resetaTodosOsAlarmes() {
    for(int i = 0; i < QUANTIDADEDEALARMES; i++) {
      TAlarme alarme = TAlarme(i);
      this->armazenamentoPermanente->gravarAlarme(alarme);
      this->alarmes[i] = alarme;
    }
  }

  String jsonEncode() {
    String temp = "{\"quantidadeDeAlarmes\": ";
    temp.concat(QUANTIDADEDEALARMES);
    temp.concat(", \"alarmes\": [ ");
    for(int i = 0; i < QUANTIDADEDEALARMES; i++) {
      temp.concat(this->alarmes[i].jsonEncode());
      if (i != QUANTIDADEDEALARMES - 1) {
        temp.concat(", ");
      }
    }
    temp.concat(" ]}");
    return temp;
  }

  void atualizaAlarme(TAlarme alarme) {
    this->alarmes[alarme.getNumero()] = alarme;
    this->armazenamentoPermanente->gravarAlarme(alarme);
  }
 
};

#endif
