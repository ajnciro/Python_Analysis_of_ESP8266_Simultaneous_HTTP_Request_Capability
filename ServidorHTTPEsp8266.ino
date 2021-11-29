//Biliotecas necessárias
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "imagenzinha.h"

String imagem = ImgBase64;		//Imagem convertida para texto Base64 está contida em "imgenzinha.h"

#ifndef STASSID
#define STASSID "nome_do_ponto_acesso"	//Definir o SSDI da rede ao qual o módulo se conectará
#define STAPSK "senha"			//senha
#endif
const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);		//Escolher uma porta de comunicação <<80 para padrão>>


//Função resposável por montar e enviar as páginas ao cliente
void handleRoot(void)
{
    int leitura = analogRead(A0);  	//Um GPIP (pino) para leitura de dados quaisquer
    String dir = server.uri();
    
    String pg = "<html><head><title>Saudacoes</title></head>\
                 <body style='text-align: center;'><h1>";
    pg += "Ola para a pagina: ";
    pg += dir;
    pg += "</h1><div><img src =";
    pg += imagem;			//Local em que a imagem será exibida dentro da página web
    pg += "/><p>Leitura do sensor 1: ";
    pg += String(leitura);		//Local em que a leitura feita será exibida dentro da página
    pg += "</div></body></html>";
    
    server.send(200,"text/html", pg);
    Serial.println("OK - p: " + dir);	//Mensagem de controle enviada para porta de comunicação serial
}

//Função resposável por montar e enviar a página de erro 404 (página não encontrada)
void handleNotFound()
{
    String msg = "404 - \tA pagina ", dir = server.uri();
    msg += dir;
    msg += " nao eh lugar para voce!\n\tTente outra. =]\n\n";
    server.send(404, "text/plain", msg);
    Serial.println(dir + ": Not found");
}

void setup(void)
{
    pinMode(A0,INPUT);				//Escolha do GPIO
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    while (WiFi.status() != WL_CONNECTED) {	 //Aguarda Conexão
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Conectado a ");
    Serial.println(ssid);
    Serial.print("IP: ");			//O endereço do servidor será enviado pela porta serial.
    Serial.println(WiFi.localIP());		//'Normalmente' a escolha será do tipo "   .   .   .70"

    if (MDNS.begin("meuesp")) {			//resolve um nome para o endereço IP, <<meuesp>> no caso
        Serial.println("MDNS responder iniciado");
    }

//Contrução das páginas web: {"/", "/0", "/1", "2", ..., "/99"}
    server.on("/", handleRoot);
    for (int num = 0; num < 100; server.on("/" + String(num), handleRoot), num++)
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("Servidor HTTP iniciado.");
}

void loop(void)
{
    server.handleClient();			//Operação de servidor
    MDNS.update();
}