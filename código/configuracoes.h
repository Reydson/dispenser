//rede
#define SSIDPADRAO "dispenser" //SSID definido quando o equipamento é resetado
IPAddress IPDISPENSER(10, 0, 0, 1); //ip do dispenser na rede propria
IPAddress MASCARADEREDEDISPENSER(255, 255, 255, 0); //mascara de rede do dispenser na rede propria
#define PORTAHTTP 80 //porta da página de gerenciamento

//sensor de ração com laser e LDR
#define PINOLASER D3 //pino para acionamento do laser
#define PINOLDR A0 //pino para leitura do LDR
#define INTERVALOLEITURAS 30 //intervalo entre leituras em segundos
#define VALORACIONAMENTO 90 //valor na porta analógica para que seja considerado que está sem ração

//alerta de falta de ração com LED
#define PINOLED D5 //pino para acionamento do LED

//gerenciador de alarmes
#define QUANTIDADEDEALARMES 8 //quantidade de alarmes que podem ser programados

//motor de passo 23lm-k255-03
#define PINOENABLE D6
#define PINOSTEP D7
#define PASSOSPORVOLTA 5305
#define TEMPOENTREPASSOS 10 //tempo entre cada passo em milisegundos (deve ser um valor par)

//botão reset rede
#define PINOBOTAO D4
