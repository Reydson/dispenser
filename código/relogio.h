#ifndef RELOGIO
#define RELOGIO
#include <Arduino.h>
#include <string.h>

class TRelogio {

public:

  virtual String pegaHoraAtual();
  virtual String pegaDataAtual();

  virtual int dia();
  virtual int mes();
  virtual int ano();
  virtual int hora();
  virtual int minuto();
  virtual int segundo();
  virtual int diaDaSemana();

  virtual void atualizaRtc(String Data, String Hora);
  
};

#endif
