#ifndef TSENSORRACAO
#define TSENSORRACAO
#include <Arduino.h>
#include <string.h>

class TSensorRacao {

public:

virtual void processar(bool forcarLeitura = false);

virtual bool temRacao();

virtual int getUltimoValorLido();
 
};

#endif
