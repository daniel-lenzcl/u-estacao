/* 

Programa para armazenamento de dados Temperatura (Celsius) e Umidade Relativa do ar (%) em cartão de memória tipo SD - Versão 1.0 de 27/08/2015. 

Autores: Alipson Santos e Helio S. Amorim (MPEF – Mestrado Profissional em Ensino de Física – IF/UFRJ) 

Baseado nos códigos, 
‘SD dataloggermemorycard 
created 24 Nov 2010 
modified 9 Apr 2012 
by Tom Igoe’
e 
‘Example testing sketch for various 
DHT humidity/temperature sensors 
Written by ladyada, public domain’ 

O circuito: 
* sensor digital DHT22 
* Shield SD card ligado ao SPI bus como se segue: 

** MOSI - pino 11 
** MISO - pino 12 
** CLK - pino 13 
** CS - pino 4 
*/

#include <SPI.h>
#include <SD.h> 
#include <DHT.h>
#include <Wire.h>
#include "RTClib.h"
const int chipSelect = 10; //cs or the save select pin from the sd shield is connected to 10.
RTC_DS1307 RTC;


#define MQ_analog A0
#define MQ_dig 6
#define DHTPIN 7 
#define DHTTYPE DHT22 // DHT 22 

DHT dht(DHTPIN, DHTTYPE);

File dataFile;
DateTime now;

int ppm;
bool gas;
String dataleitura = ("data inicio");
String horaleitura = ("hora inicio");
String leitura = ("inicio");

void setup(){ 
  pinMode(MQ_analog, INPUT);
  pinMode(MQ_dig, INPUT);


// Habilita a comunicação serial; 
Serial.begin(9600); 
  Wire.begin();
  RTC.begin();

//check or the Real Time Clock is on
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    RTC.adjust(DateTime(__DATE__, __TIME__));
  } else {
    Serial.println("RTC is running!");
  }

Serial.print("Inicializando o cartão SD..."); 
// se cartão está presente ou pode ser inicializado: 
  if (!SD.begin(chipSelect)){ 
    Serial.println("Falha no cartão ou cartão não presente"); 
    // não faça mais nada: 
    return;
  }else {
    Serial.println("cartão inicializado."); 
  }


Serial.println(); 
Serial.println("   data    |   hora   |  u(%) | T oC  | ppm | gas"); 

// Abre o arquivo “datalog.txt”; você pode trocar o nome deste 
// arquivo segundo as suas necessidades; 
File dataFile = SD.open("datalog.txt", FILE_WRITE); 

//Inicia escrevendo informações de controle; altere essas 
//informações segundo suas necessidades; 
dataFile.println("ABRIGO: 1"); // indique o abrigo usado; 
dataFile.println("SENSOR: DHT22"); 
dataFile.println("PROGRAMA: Datalogger_tu_rolando"); 

//indique sempre a versão do programa que esta gerando os dados; 
dataFile.println("ULTIMA REVISAO DO SISTEMA: 20/06/2018"); 
dataFile.println("   data    |   hora   |  u(%) | T oC  | ppm | gas");  
dataFile.close(); 
dht.begin(); 

} 


void loop(){ 

//FAZER LEITURAS
  now = RTC.now();

if (now.day()<10) {
  dataleitura += "0" ;
  dataleitura += now.day();
} else dataleitura = now.day();      // LETURA DO DIA
dataleitura += "/";
if (now.month()<10) {
  dataleitura += "0" ;
  dataleitura += now.month();
} else dataleitura += now.month();      // LETURA DO DIA
//dataleitura += now.month();
dataleitura += "/";
dataleitura += now.year();
//Serial.println(dataleitura); 

leitura = dataleitura;
leitura += " | ";
//Serial.println(leitura); 

  
if (now.hour()<10) {
  horaleitura = "0" ;
  horaleitura += now.hour();
} else horaleitura = now.hour();      // LETURA DA HORA

horaleitura += ":";

if (now.minute()<10) {
  horaleitura += "0" ;
  horaleitura += now.minute();
} else horaleitura += now.minute();      // LETURA DA HORA

horaleitura += ":";

if (now.second()<10) {
  horaleitura += "0" ;
  horaleitura += now.second();
} else horaleitura += now.second();      // LETURA DA HORA

//Serial.println(horaleitura); 

leitura += horaleitura;
leitura += " | ";
//Serial.println(leitura); 

//Lê a temperature e a umidade relative(a leitura do sensor 
//leva em torno de 250 milisegundos); 
float h = dht.readHumidity(); 
float t = dht.readTemperature();

leitura += h;           //LEITURA UMIDADE
leitura += " | ";
leitura += t;           //LEITURA TEMPERATURA
leitura += " | ";
//Serial.println(leitura); 

//Lê ppm e presença de gás
ppm = analogRead(MQ_analog); 
gas = digitalRead(MQ_dig);

leitura += ppm;           //LEITURA TEMPERATURA
leitura += " | ";
  if (gas == 0)
     leitura += "GAS DETECTADO !!!";
   else 
     leitura += "GAS AUSENTE !!!";

Serial.print(leitura); 

//Abra o arquivo “datalog.txt” e escreva os valores obtidos; 
File dataFile = SD.open("datalog.txt", FILE_WRITE); 

//dataFile.println("data  | hora  | umidade(%) | temp (°C)  | ppm | gás"); 

//se o arquivo está aberto, escreva; 
if (dataFile) { 

  dataFile.println(leitura); 
  dataFile.close(); 
  Serial.println(" dados registrados");

}
//Se o arquivo não abre dê uma mensagem de erro; 
else { 
Serial.println("erro ao abrir datalog.txt"); 
} 
  
  
//período de amostragem (20s). Mude esse valor segundo suas 
//necessidades; 
delay(300000); } //5 minutos (milisegundos 300 mil) 2 minutos (120 mil)




