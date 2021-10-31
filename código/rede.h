#ifndef TREDE
#define TREDE
#include <Arduino.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

class TRede {

private:

DNSServer * servidorDns;
ESP8266WebServer  * servidorHttp;
ESP8266HTTPUpdateServer httpUpdater;

public:

TRede(int portaHttp, IPAddress ipEquipamento, IPAddress mascaraDeRede, char * ssid, char * senha = NULL){
  this->servidorDns = new DNSServer();
  this->servidorHttp = new ESP8266WebServer(portaHttp);
  WiFi.mode(WIFI_AP); //define o modo de trabalho do ESP8266
  WiFi.softAPConfig(ipEquipamento, ipEquipamento, mascaraDeRede); //define os parâmetros de rede
  if (senha == NULL) {
    WiFi.softAP(ssid, NULL, 13, 0); //define o SSID
  } else {
    WiFi.softAP(ssid, senha, 13, 0); //define SSID e senha
  }
  this->servidorDns->start(53, "*", ipEquipamento);
  MDNS.begin("dispenser");
  this->httpUpdater.setup(servidorHttp);
  this->servidorHttp->begin();
  MDNS.addService("http", "tcp", 80);
}

void processarRequisicoes() {
  this->servidorDns->processNextRequest();
  this->servidorHttp->handleClient();
  MDNS.update();
}

void on(const char* uri, ESP8266WebServer::THandlerFunction funcao){
   this->servidorHttp->on(uri, funcao);
}

void onNotFound(ESP8266WebServer::THandlerFunction funcao) {
  this->servidorHttp->onNotFound(funcao);
}

String arg(String argName) {
  return this->servidorHttp->arg(argName);
}

void enviaTexto(String texto){
  this->servidorHttp->send(200, "text/html", texto); //envia a resposta para o navegador
}

void enviaArquivoHtml(String nome){
  fs::File arquivo=SPIFFS.open(nome,"r"); //abre o arquivo em modo de leitura
  this->servidorHttp->setContentLength(CONTENT_LENGTH_UNKNOWN); //define o tamanho da resposta como desconhecido
  this->servidorHttp->streamFile(arquivo,"text/html"); //envia o arquivo para o navegador text/css text/html
  arquivo.close(); //fecha o arquivo
}

~TRede() {
  free(this->servidorDns);
  free(this->servidorHttp);
}
 
};

#endif
