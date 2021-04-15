#ifndef TARMAZENAMENTOPERMANENTEEMEEPROM
#define TARMAZENAMENTOPERMANENTEEMEEPROM
#include <Arduino.h>
#include <string.h>
#include <EEPROM.h>

#include "armazenamentoPermanente.h";

class TArmazenamentoPermanenteEmEeprom : public TArmazenamentoPermanente{

private:

  void gravaString(String texto, int posicaoInicial, int tamanhoMaximo) {
    int i;
    for(i = 0; i < texto.length() && i < tamanhoMaximo; i++) {
      EEPROM.write(i + posicaoInicial,(int) texto[i]);
    }
    if (i != tamanhoMaximo) {
      EEPROM.write(i + posicaoInicial, '\n');
    }
    this->salvar();
  }

  String leString(int posicaoInicial, int tamanhoMaximo) {
    String texto = "";
    for(int i = posicaoInicial; EEPROM.read(i) != '\n' && i < tamanhoMaximo + posicaoInicial; i++) {
      texto.concat((char)EEPROM.read(i));
    }
    return texto;
  }

  bool validaAlarme(String alarmeStr) {
    for(int i = 0; i < 8; i++) {
      if(alarmeStr[i] != '0' && alarmeStr[i] != '1') {
        return false;
      }
    }
    String horario = alarmeStr.substring(8, 12);
    int hora = (horario.substring(0, horario.indexOf(':'))).toInt();
    int minuto = (horario.substring(horario.indexOf(':')+1)).toInt();
    if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59) {
      return false;
    }
    return true;
  }

public:

  TArmazenamentoPermanenteEmEeprom() {
    EEPROM.begin(2048);
  }

  void gravarSsid(String ssid) { //máximo de 1 + 20 caracteres (0-20)
    if(ssid == "") {
      String temp = "0";
      this->gravaString(temp, 0, 1);
      return;
    }
    String temp = "1";
    temp.concat(ssid);
    this->gravaString(temp, 0, 21);
  }
  
  String lerSsid() {
    if(this->leString(0, 1) != "1") {
      return SSIDPADRAO;
    }
    return this->leString(1, 20);
  }

  void gravarSenhaWifi(String senha) { //maximo de 1 + 15 caracteres (21-36)
    if(senha == "") {
      String temp = "0";
      this->gravaString(temp, 21, 1);
      return;
    }
    String temp = "1";
    temp.concat(senha);
    this->gravaString(temp, 21, 16);
  }
  
  String lerSenhaWifi() {
    if(this->leString(21, 1) != "1") {
      return "";
    }
    return this->leString(22, 15);
  }

  void gravarAlarme(TAlarme alarme) {
    int posicaoInicial = 37 + (alarme.getNumero() * 15);
    String temp = "";
    temp.concat(alarme.getAtivo() ? '1' : '0');
    temp.concat(alarme.getDiasDaSemana());
    temp.concat(alarme.getHorario());
    if(alarme.getValor() < 10) {
      temp.concat('0');
    }
    temp.concat(alarme.getValor());
    this->gravaString(temp, posicaoInicial, 15);
  }
  
  TAlarme lerAlarme(int numero) {
    int posicaoInicial = 37 + (numero * 15);
    String temp = this->leString(posicaoInicial, 15);
    if(!this->validaAlarme(temp)) {
      return TAlarme(numero);
    }
    bool ativo = temp[0] == '1';
    String diasDaSemana = temp.substring(1, 8);
    String horario = temp.substring(8, 13);
    int valor = temp.substring(13, 15).toInt();
    return TAlarme(numero, ativo, diasDaSemana, horario, valor);
  }

  void salvar() {
    EEPROM.commit();
  }
  
};

#endif
