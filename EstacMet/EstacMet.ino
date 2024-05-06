//Bibliotecas Adafruit (BMP280 e DHT22)
#include <Adafruit_Sensor.h>
#include <DHT.h> 
#include <Adafruit_BMP280.h> 

//Bibliotecas para manipular o LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Funcao para definir endereco e tamanho do LCD
// LiquidCrystal_I2C lcd(endereco,colunas,linhas);
LiquidCrystal_I2C lcd(0x27,16,2);

//Variável e objeto para manipular DHT22
DHT dht(2, DHT22);
float umid;

//Variáveis para manipulação do LM35
int adc; 
float tensao;
float temp ;
float Ad_temp;

//Incertezas
float IncLM35 = 0.61; // incerteza padrão de medição do LM35 (°C)
float IncDHT22 = 0; //incerteza padrão de medição do DHT22 (%)

//Variáveis para manipulação do BMP280
Adafruit_BMP280 bmp; //OBJETO DO TIPO Adafruit_BMP280 (I2C)
float pressao,alt; // declaracao de variaveis




void setup() { 
  dht.begin(); //Inicia a comunicação da biblioteca DHT
  bmp.begin(0x76); // Inicia o BMP no endereco 0x76
  analogReference(INTERNAL); // Conversao A/D com fundo de escala de 1.1V para o LM35
  pinMode(A0,INPUT); // Inicia o pino A0 como entrada

 //Inicia LCD
 lcd.init(); // inicializa o lcd
  lcd.backlight(); // liga o backlight
  lcd.setCursor(0,0);  // posiciona o cursor
  lcd.print("Estação");  // Escreve no LCD
  lcd.setCursor(0,1);  // posiciona o cursor
  lcd.print(" Meteorologica");  // Escreve no LCD
  delay(5000);
  lcd.clear();
}
void loop(){

  umid = dht.readHumidity(); // salva umidade sem ajuste
  //Ad_umid = 

//LM35
  adc = analogRead(A0);       //Le a entrada analogica A0
  tensao = adc*(1.1/1024);    // converte em tensao
  temp = tensao*100;// converte em temperatura (oC)
  Ad_temp = 1.0101*temp + 0,8406; //LM35 com temperatura ajustada

// BMP280
  pressao=bmp.readPressure(); // retorna a pressao em Pascal (Pa)
  pressao= pressao /100; // Converte a pressão de Pa para hPa
  alt= bmp.readAltitude(1013.25); // retorna a altitude em metros
                                    // 1 atm = 1013,25 hPa (hectopascal)


//EXIBE INFORMAÇÕES
  lcd.setCursor(0,0);  // posiciona o cursor
  lcd.print("Pessao:");  // Escreve no LCD
  lcd.setCursor(8,0);  // posiciona o cursor
  lcd.print(pressao);  // Escreve no LCD
  lcd.setCursor(13,0);  // posiciona o cursor
  lcd.print("hPa");  // Escreve no LCD
  delay(3000);
  lcd.clear();


   lcd.setCursor(0,0);  // posiciona o cursor
  lcd.print("Altitude:");  // Escreve no LCD
  lcd.setCursor(10,0);  // posiciona o cursor
  lcd.print(alt);  // Escreve no LCD
  lcd.setCursor(15,0);  // posiciona o cursor
  lcd.print("m");  // Escreve no LCD
  delay(3000);
  lcd.clear();

  // Exibe temperatura
  lcd.setCursor(0,0);  // posiciona o cursor
  lcd.print("Temp:");  // Escreve no LCD
  lcd.setCursor(6,0);  // posiciona o cursor
  lcd.print(Ad_temp);  // Escreve no LCD
  lcd.setCursor(14,0);  // posiciona o cursor
  lcd.print("oC");  // Escreve no LCD
  //Exibe a incerteza da temperatura
  lcd.setCursor(0,1);  // posiciona o cursor
  lcd.print("Incert:+-");  // Escreve no LCD
  lcd.setCursor(10,1);  // posiciona o cursor
  lcd.print(IncLM35,2);  // Escreve no LCD
  lcd.setCursor(14,1);  // posiciona o cursor
  lcd.print("oC");  // Escreve no LCD
  delay(3000);
  lcd.clear();


}
