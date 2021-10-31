#ifndef TDISPENSER
#define TDISPENSER
#include <Arduino.h>
#include <string.h>

#include "FS.h"

#include "rede.h";
#include "configuracoes.h";
#include "armazenamentoPermanenteEmEeprom.h";
#include "sensorRacaoComLaserELdr.h";
#include "alertaFaltaRacaoComLed.h"
#include "relogioComDs3231.h"
#include "gerenciadorDeAlarmes.h"
#include "motorDePasso23lmk25503.h"

class TDispenser {
private:
  TRede * rede;
  TSensorRacao * sensorRacao;
  TArmazenamentoPermanente * armazenamentoPermanente;
  TAlertaFaltaRacao * alertaFaltaRacao;
  TRelogio * relogio;
  TGerenciadorDeAlarmes * gerenciadorDeAlarmes;
  TMotor * motor;

public:

  TDispenser() {
    this->armazenamentoPermanente = new TArmazenamentoPermanenteEmEeprom(); //instancia armazenamento permanente em EEPROM
    String ssidStr = this->armazenamentoPermanente->lerSsid(); //lê o SSID
    char ssid[ssidStr.length()+1]; //cria array para armazenar o SSID
    int i;
    for(i = 0; i < ssidStr.length(); i++) { //para cada caractere
      ssid[i] = ssidStr[i]; //copia para o array
    }
    ssid[i] = 0; //fecha o array, '\n' não funciona
    
    String senhaStr = this->armazenamentoPermanente->lerSenhaWifi(); //lê a senha
    if (senhaStr != "") {
      char senha[senhaStr.length()+1]; //cria array para armazenar a senha
      for(i = 0; i < senhaStr.length(); i++) {//para cada caractere
        senha[i] = senhaStr[i]; //copia para o array
      }
      senha[i] = 0; //fecha o array, '\n' não funciona
      this->rede = new TRede(PORTAHTTP, IPDISPENSER, MASCARADEREDEDISPENSER, ssid, senha); //instancia rede com senha
    } else {
      this->rede = new TRede(PORTAHTTP, IPDISPENSER, MASCARADEREDEDISPENSER, ssid); //instancia rede sem senha
    }
  
    if(!SPIFFS.begin()){  // inicializa o SPIFFS
      ESP.restart(); //reinicia o ESP
    };
  
    this->sensorRacao = new TSensorRacaoComLaserELdr(PINOLASER, PINOLDR);
    this->alertaFaltaRacao =  new TAlertaFaltaRacaoComLed(PINOLED);
    this->relogio = new TRelogioComDs3231();
    this->motor = new TMotorDePasso23lmk25503(PINOENABLE, PINOSTEP);
    this->gerenciadorDeAlarmes = new TGerenciadorDeAlarmes(armazenamentoPermanente, this->motor);
    pinMode(PINOBOTAO, INPUT);
  }

  void processar() {
    this->rede->processarRequisicoes();
    this->sensorRacao->processar();
    this->alertaFaltaRacao->processar(sensorRacao->temRacao());
    this->gerenciadorDeAlarmes->processar(relogio->diaDaSemana(), this->relogio->hora(), this->relogio->minuto());
    this->motor->processar();
    if(digitalRead(PINOBOTAO)) {
      while(digitalRead(PINOBOTAO)) {
      }
        this->armazenamentoPermanente->gravarSsid("");
        this->armazenamentoPermanente->gravarSenhaWifi("");
        ESP.restart();
    }
  }

  void onNotFound(ESP8266WebServer::THandlerFunction funcao) {
    this->rede->onNotFound(funcao);
  }

  void on(const char* uri, ESP8266WebServer::THandlerFunction funcao){
    this->rede->on(uri, funcao);
  }

  void processaReq404() {
    this->rede->enviaTexto("<script language='JavaScript'>window.location='/status.html'; </script>"); //redireciona todas as requisições inválidas para a página de status
  }
  
  void processaReqStatusHtml() {
    this->rede->enviaArquivoHtml("/status.html");
  }

  void processaReqStatusJson() {
    sensorRacao->processar(true);
    String temp = "{";
    temp.concat("\"dataDispenser\":\"");
    temp.concat(relogio->pegaDataAtual());
    temp.concat("\", \"horaDispenser\":\"");
    temp.concat(relogio->pegaHoraAtual());
    temp.concat("\", \"versaoDoFirmware\":\"");
    temp.concat(VERSAO_DO_FIRMWARE);
    temp.concat("\", \"qtdHorariosAtivos\":\"");
    temp.concat(gerenciadorDeAlarmes->quantidadeDeAlarmesAtivos());
    temp.concat("/");
    temp.concat(QUANTIDADEDEALARMES);
    temp.concat("\", \"possuiRacao\":");
    temp.concat(sensorRacao->temRacao());
    temp.concat(", \"nomeRede\":\"");
    temp.concat(armazenamentoPermanente->lerSsid());
    temp.concat("\", \"quantidade\":\"");
    temp.concat(armazenamentoPermanente->lerQuantidade());
    temp.concat("\", \"unidade\":\"");
    temp.concat(armazenamentoPermanente->lerUnidade());
    temp.concat("\", \"unidades\": ");
    temp.concat(UNIDADES);
    temp.concat(", \"valorLido\":\"");
    temp.concat(sensorRacao->getUltimoValorLido());
    temp.concat("\"}");
    this->rede->enviaTexto(temp);
  }
  
  void processaReqHorariosHtml() {
    this->rede->enviaArquivoHtml("/horarios.html");
  }
  
  void processaReqHorariosJson() {
    String json = gerenciadorDeAlarmes->jsonEncode().substring(1);
    String temp = "";
    temp.concat( "{ \"quantidade\":\"");
    temp.concat(armazenamentoPermanente->lerQuantidade());
    temp.concat("\", \"unidade\":\"");
    temp.concat(armazenamentoPermanente->lerUnidade());
    temp.concat("\", \"unidades\": ");
    temp.concat(UNIDADES);
    temp.concat( ", ");
    temp.concat(json);
    this->rede->enviaTexto(temp);
  }
  
  void processaReqHorariosSalvaAlarme() {
    int numero = this->rede->arg("numero").toInt();
    bool ativo = this->rede->arg("ativo") == "1";
    String diasDaSemana = this->rede->arg("diasDaSemana");
    int hora = this->rede->arg("hora").toInt();
    int minuto = this->rede->arg("minuto").toInt();
    int valor = this->rede->arg("valor").toInt();
    
    TAlarme alarme = TAlarme(numero, ativo, diasDaSemana, hora, minuto, valor);
    this->gerenciadorDeAlarmes->atualizaAlarme(alarme);
    this->rede->enviaTexto("{\"resposta\": \"OK\"}");
  }
  
  void processaReqAjustesHtml() {
    this->rede->enviaArquivoHtml("/ajustes.html");
  }
  
  void processaReqAjustesJson() {
    String temp = "{";
    temp.concat("\"quantidade\":\"");
    temp.concat(armazenamentoPermanente->lerQuantidade());
    temp.concat("\", \"quantidadePadrao\":\"");
    temp.concat(QUANTIDADEPADRAO);
    temp.concat("\", \"unidade\":\"");
    temp.concat(armazenamentoPermanente->lerUnidade());
    temp.concat("\", \"unidadePadrao\":\"");
    temp.concat(UNIDADEPADRAO);
    temp.concat("\", \"unidades\": ");
    temp.concat(UNIDADES);
    temp.concat(", \"dataDispenser\":\"");
    temp.concat(relogio->pegaDataAtual());
    temp.concat("\", \"horaDispenser\":\"");
    temp.concat(relogio->pegaHoraAtual());
    temp.concat("\", \"nomeRede\":\"");
    temp.concat(armazenamentoPermanente->lerSsid());
    temp.concat("\", \"senhaRede\":\"");
    temp.concat(armazenamentoPermanente->lerSenhaWifi());
    temp.concat("\", \"versaoDoFirmware\":\"");
    temp.concat(VERSAO_DO_FIRMWARE);
    temp.concat("\"}");
    this->rede->enviaTexto(temp);
  }
  
  void processaReqAjustesHoraEData() {
    String data = this->rede->arg("data");
    String hora = this->rede->arg("hora");
    if(data != "" && hora != "") {
      this->relogio->atualizaRtc(data, hora);
    }
    this->rede->enviaTexto("{\"resposta\": \"OK\"}");
  }

  void processaReqAjustesQuantidade() {
    int quantidade = this->rede->arg("quantidade").toInt();
    this->armazenamentoPermanente->gravarQuantidade(quantidade);
    int unidade = this->rede->arg("unidade").toInt();
    this->armazenamentoPermanente->gravarUnidade(unidade);
    this->rede->enviaTexto("{\"resposta\": \"OK\"}");
  }
  
  void processaReqAjustesRede() {
    String nomeRede = this->rede->arg("nomeRede");
    String senhaRede = this->rede->arg("senhaRede");
    this->armazenamentoPermanente->gravarSsid(nomeRede);
    this->armazenamentoPermanente->gravarSenhaWifi(senhaRede);
    this->rede->enviaTexto("{\"resposta\": \"OK\"}");
    delay(100);
    ESP.restart();
  }
  
  void processaReqAjustesRestauraHorarios() {
    this->gerenciadorDeAlarmes->resetaTodosOsAlarmes();
    this->rede->enviaTexto("{\"resposta\": \"OK\"}");
  }
  
  void processaReqAjustesRestauraTudo() {
    this->gerenciadorDeAlarmes->resetaTodosOsAlarmes();
    this->armazenamentoPermanente->gravarQuantidade(0);
    this->armazenamentoPermanente->gravarUnidade(0);
    this->armazenamentoPermanente->gravarSsid("");
    this->armazenamentoPermanente->gravarSenhaWifi("");
    this->rede->enviaTexto("{\"resposta\": \"OK\"}");
    delay(100);
    ESP.restart();
  }
  
  void processaReqAjustesDispensarManualmente() {
    this->motor->darVoltas(1);
    this->rede->enviaTexto("{\"resposta\": \"OK\"}");
  }

  ~TDispenser() {
    delete(this->rede);
    delete(this->sensorRacao);
    delete(this->armazenamentoPermanente);
    delete(this->alertaFaltaRacao);
    delete(this->relogio);
    delete(this->gerenciadorDeAlarmes);
    delete(this->motor);
  }
};

#endif
