#ifndef RELOGIOCOMDS3231
#define RELOGIOCOMDS3231
#include <Arduino.h>
#include <string.h>

#include "relogio.h"

#include <Wire.h>
#include <RtcDS3231.h>

class TRelogioComDs3231 : public TRelogio {

private:

  RtcDS3231<TwoWire> * rtc;

public:
  
  TRelogioComDs3231() {
    Wire.begin(D2, D1);
    delay(100);
    pinMode(D0, OUTPUT);
    digitalWrite(D0, HIGH);
    this->rtc = new RtcDS3231<TwoWire>(Wire);
    this->rtc->Begin();
    RtcDateTime momentoCompilacao = RtcDateTime(__DATE__, __TIME__); //pega o horário da compilação
    if (!this->rtc->IsDateTimeValid() || this->rtc->GetDateTime() < momentoCompilacao) { //se o RTC não estiver com a hora setada ou estiver com um tempo anterior ao em que o código foi compilado
      this->rtc->SetDateTime(momentoCompilacao); //define o horário do RTC como o da compilação
    }
    if (this->rtc->GetIsRunning())
    {
        this->rtc->SetIsRunning(true);
    }
    this->rtc->Enable32kHzPin(false);
    this->rtc->SetSquareWavePin(DS3231SquareWavePin_ModeNone);
    while(this->dia() == 165) {
      digitalWrite(D0, LOW);
      delay(100);
      digitalWrite(D0, HIGH);
      delay(100);
    }
    delay(100);
  }

  String pegaHoraAtual() {
    String temp = "";
    RtcDateTime momentoAtual = this->rtc->GetDateTime();
    temp.concat(momentoAtual.Hour());
    temp.concat(":");
    temp.concat(momentoAtual.Minute());
    temp.concat(":");
    temp.concat(momentoAtual.Second());
    return temp;
  }
  
  String pegaDataAtual() {
    String temp = "";
    RtcDateTime momentoAtual = this->rtc->GetDateTime();
    temp.concat(momentoAtual.Day());
    temp.concat("/");
    temp.concat(momentoAtual.Month());
    temp.concat("/");
    temp.concat(momentoAtual.Year());
    return temp;
  }

  int dia() {
    RtcDateTime momentoAtual = this->rtc->GetDateTime();
    return momentoAtual.Day();
  }
  
  int mes() {
    RtcDateTime momentoAtual = this->rtc->GetDateTime();
    return momentoAtual.Month();
  }
  
  int ano() {
    RtcDateTime momentoAtual = this->rtc->GetDateTime();
    return momentoAtual.Year();
  }
  
  int hora() {
    RtcDateTime momentoAtual = this->rtc->GetDateTime();
    return momentoAtual.Hour();
  }
  
  int minuto() {
    RtcDateTime momentoAtual = this->rtc->GetDateTime();
    return momentoAtual.Minute();
  }
  
  int segundo() {
    RtcDateTime momentoAtual = this->rtc->GetDateTime();
    return momentoAtual.Second();
  }
  
  int diaDaSemana() {
    int ano = this->ano();
    int mes = this->mes();
    int dia = this->dia();
    
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    ano -= mes < 3;
    
    return (((ano + ano/4 - ano/100 + ano/400 + t[mes-1] + dia) % 7) + 1);
  }

  void atualizaRtc(String data, String hora) {
    RtcDateTime momento = RtcDateTime(data.c_str(), hora.c_str());
    this->rtc->SetDateTime(momento);
  }
  
};

#endif
