#ifndef TMOTOR
#define TMOTOR
#include <Arduino.h>
#include <string.h>

class TMotor {

public:

  virtual void darVoltas(int numeroDeVoltas);

  virtual void processar();
  
};

#endif
