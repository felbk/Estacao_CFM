//Bibliotecas e variáveis para utilização do DHT22 
#include <Adafruit_Sensor.h>
#include <DHT.h>
DHT dht(2, DHT22);
float umid;
//Variáveis para manipulação do LM35
int adc;
float tensao;
float temp;


void setup() { 
  dht.begin(); //Inicia a comunicação da biblioteca DHT

}
void loop(){
  umid = dht.readHumidity();
  Ad_umid = 
  adc = analogRead(A0);       //Le a entrada analogica A0
  tensao = adc*(1.1/1024);    // converte em tensao
  temp = tensao*100;// converte em temperatura (oC)
  Ad_temp = 1,0101*temp + 0,8406
 


}
