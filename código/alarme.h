#ifndef TALARME
#define TALARME
#include <Arduino.h>
#include <string.h>

class TAlarme {

private:

  int numero;
  bool ativo;
  bool diasDaSemana [7];
  int hora;
  int minuto;
  int valor;

public:

  TAlarme() {}

  TAlarme(int numero, bool ativo = false, String diasDaSemana = "1111111", int hora = 12, int minuto = 0, int valor = 1) {
    this->numero = numero;
    this->ativo = ativo;
    this->setDiasDaSemana(diasDaSemana);
    this->hora = hora;
    this->minuto = minuto;
    this->valor = valor;
  }

  TAlarme(int numero, bool ativo, String diasDaSemana, String horario, int valor) {
    this->numero = numero;
    this->ativo = ativo;
    this->setDiasDaSemana(diasDaSemana);
    this->setHorario(horario);
    this->valor = valor;
  }

  void setNumero(int numero) {
    this->numero = numero;
  }
  int getNumero() {
    return this->numero;
  }

  void setAtivo(bool ativo) {
    this->ativo = ativo;
  }
  bool getAtivo() {
    return this->ativo;
  }

  void setDiaDaSemana(int pos, bool ativo) {
    this->diasDaSemana[pos - 1] = ativo;
  }
  bool getDiaDaSemana(int pos) {
    return this->diasDaSemana[pos - 1];
  }

  void setDiasDaSemana(String diasDaSemanaStr) {
    for(int i = 0; i < 7; i++) {
      this->diasDaSemana[i] = diasDaSemanaStr[i] == '1';
    }
  }
  String getDiasDaSemana() {
    String temp = "";
    for(int i = 0; i < 7; i++) {
      temp.concat(this->diasDaSemana[i] ? '1' : '0');
    }
    return temp;
  }

  void setHora(int hora) {
    this->hora = hora;
  }
  int getHora() {
    return this->hora;
  }

  void setMinuto(int minuto) {
    this->minuto = minuto;
  }
  int getMinuto() {
    return this->minuto;
  }

  void setHorario(String horario) {
    this->hora = (horario.substring(0, horario.indexOf(':'))).toInt();
    this->minuto = (horario.substring(horario.indexOf(':')+1)).toInt();
  }
  String getHorario() {
    String temp = "";
    if(this->hora < 10) {
      temp.concat('0');
    }
    temp.concat(this->hora);
    temp.concat(":");
    if(this->minuto < 10) {
      temp.concat('0');
    }
    temp.concat(this->minuto);
    return temp;
  }

  void setValor(int valor) {
    this->valor = valor;
  }
  int getValor() {
    return this->valor;
  }

  String jsonEncode() {
    String temp = "{ \"numero\": ";
    temp.concat(this->getNumero());
    temp.concat(", \"ativo\": ");
    temp.concat(this->getAtivo()? '1' : '0');
    temp.concat(", \"diasDaSemana\": \"");
    temp.concat(this->getDiasDaSemana());
    temp.concat("\", \"horario\": \"");
    temp.concat(this->getHorario());
    temp.concat("\", \"valor\": ");
    temp.concat(this->getValor());
    temp.concat(" }");
    return temp;
  }
  
};

#endif
