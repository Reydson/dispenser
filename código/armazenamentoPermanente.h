#ifndef TARMAZENAMENTOPERMANENTE
#define TARMAZENAMENTOPERMANENTE
#include <Arduino.h>
#include <string.h>

#include "alarme.h"

class TArmazenamentoPermanente {

public:

  virtual void gravarSsid(String ssid);
  virtual String lerSsid();

  virtual void gravarSenhaWifi(String senha);
  virtual String lerSenhaWifi();

  virtual void gravarQuantidade(int quantidade);
  virtual int lerQuantidade();

  virtual void gravarUnidade(int unidade);
  virtual int lerUnidade();
  
  virtual void gravarAlarme(TAlarme alarme);
  virtual TAlarme lerAlarme(int numero);

  virtual void salvar();
  
};

#endif
