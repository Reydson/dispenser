#include "dispenser.h"

TDispenser * dispenser;

void setup() {
  dispenser = new TDispenser();

  /* Páginas web */
  dispenser->onNotFound([](){dispenser->processaReq404();}); //caso uma página inválida seja acessada

  dispenser->on("/status.html", [](){dispenser->processaReqStatusHtml();}); //ao acessar a página status.html a função processaReqStatusHtml é executada
  dispenser->on("/status.json", [](){dispenser->processaReqStatusJson();}); //ao acessar a página status.json a função processaReqStatusJson é executada
  
  dispenser->on("/horarios.html", [](){dispenser->processaReqHorariosHtml();}); //ao acessar a página horarios.html a função processaReqHorariosHtml é executada
  dispenser->on("/horarios.json", [](){dispenser->processaReqHorariosJson();}); //ao acessar a página horarios.html a função processaReqHorariosJson é executada
  dispenser->on("/horarios.salvaAlarme", [](){dispenser->processaReqHorariosSalvaAlarme();}); //ao acessar a página horarios.html a função processaReqHorariosSalvaAlarme é executada
  
  dispenser->on("/ajustes.html", [](){dispenser->processaReqAjustesHtml();}); //ao acessar a página ajustes.html a função processaReqAjustesHtml é executada
  dispenser->on("/ajustes.json", [](){dispenser->processaReqAjustesJson();}); //ao acessar a página ajustes.json a função processaReqAjustesJson é executada
  dispenser->on("/ajustes.ajustaHora", [](){dispenser->processaReqAjustesHoraEData();}); //ao acessar a página ajustes.json a função processaReqAjustesHoraEData é executada
  dispenser->on("/ajustes.ajustaRede", [](){dispenser->processaReqAjustesRede();}); //ao acessar a página ajustes.json a função processaReqAjustesRede é executada
  dispenser->on("/ajustes.restauraHorarios", [](){dispenser->processaReqAjustesRestauraHorarios();}); //ao acessar a página ajustes.json a função processaReqAjustesRestauraHorarios é executada
  dispenser->on("/ajustes.restauraTudo", [](){dispenser->processaReqAjustesRestauraTudo();}); //ao acessar a página ajustes.json a função processaReqAjustesRestauraTudo é executada
  dispenser->on("/ajustes.dispensarManualmente", [](){dispenser->processaReqAjustesDispensarManualmente();}); //ao acessar a página ajustes.json a função processaReqAjustesDispensarManualmente é executada
}

void loop() {
  dispenser->processar();
}
