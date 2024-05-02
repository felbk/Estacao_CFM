/*Bibliotecas e variáveis para utilização do DHT22 */
#include <Adafruit_Sensor.h>
#include <DHT.h>
DHT dht(2, DHT22);
float umid, temp;


void setup() { 
  dht.begin(); /*Inicia a comunicação da biblioteca DHT*/

}
void loop(){
  umid = dht.readHumidity();
  temp = dht.readTemperature();
}
