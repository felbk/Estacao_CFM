#include <Adafruit_Sensor.h>
#include <DHT.h>

void setup() { 

}
void loop(){
    umid = dht.readHumidity();
  temp = dht.readTemperature();
}
