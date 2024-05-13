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
float Ad_umid;

//Variáveis para manipulação do LM35
int adc; 
float tensao;
float temp ;
float Ad_temp;

//Incertezas
float IncLM35 = 0.50; // incerteza padrão de medição do LM35 (°C)
float IncDHT22 = 2; //incerteza padrão de medição do DHT22 (%)
float IncBMP280Press = 0.12; //incerteza padrão de medição de pressão do BMP (hPa)
float IncBMP280Alt = 1; //incerteza padrão de medição de altitude do BMP (m)

//Variáveis para manipulação do BMP280
Adafruit_BMP280 bmp; //OBJETO DO TIPO Adafruit_BMP280 (I2C)
float pressao,alt; // declaracao de variaveis

//variaveis LDR
int bin;
float Vout, RLDR, lum;

// Variável para modo do botão 
int Bt = 0;

#define  LM A0
#define LDR A1
#define Botao 2


void setup() { 
  dht.begin(); //Inicia a comunicação da biblioteca DHT
  bmp.begin(0x76); // Inicia o BMP no endereco 0x76
  analogReference(INTERNAL); // Conversao A/D com fundo de escala de 1.1V para o LM35
  pinMode(LM,INPUT); // Inicia o pino do LM35 como entrada
  pinMode(LDR,INPUT); // Inicia o pino do LDR como entrada
  pinMode(Botao , INPUT_PULLUP); //Inicia o pino do botão utilizando o pull up interno do arduino
  

 //Inicia LCD
 lcd.init(); // inicializa o lcd
  lcd.backlight(); // liga o backlight

 //info grupo e disciplina
  lcd.setCursor(0,0);  // posiciona o cursor
  lcd.print("Instrumentacao");  // Escreve no LCD
  lcd.setCursor(0,1);  // posiciona o cursor
  lcd.print("e Medicao");  // Escreve no LCD
  delay(5000);
  lcd.clear();
  lcd.setCursor(5,0);  // posiciona o cursor
  lcd.print("Grupo ");  // Escreve no LCD
  lcd.setCursor(6,1);  // posiciona o cursor
  lcd.print("FMC");  // Escreve no LCD
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);  // posiciona o cursor
  lcd.print("Felipe, ");  // Escreve no LCD
  lcd.setCursor(8,0);  // posiciona o cursor
  lcd.print("Matheus");  // Escreve no LCD
  lcd.setCursor(4,1);  // posiciona o cursor
  lcd.print("E Camila");  // Escreve no LCD
  delay(5000);
  lcd.clear();


 //info estação
  lcd.setCursor(0,0);  // posiciona o cursor
  lcd.print("Estacao");  // Escreve no LCD
  lcd.setCursor(0,1);  // posiciona o cursor
  lcd.print(" Meteorologica");  // Escreve no LCD
  delay(5000);
  lcd.clear();
}
void loop(){
 
 
 //Inicia coleta e conversão de dados:
  umid = dht.readHumidity(); // salva umidade sem ajuste
  Ad_umid = 1.2*umid - 11.82;

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


//LDR
bin = analogRead(LDR);
Vout = bin*1.1/1024.0;
RLDR = (3.3-Vout)/Vout * 49820;
lum = pow(10,-1.2*log10(RLDR+21451.94));


  // Analisa modo do botão
  if (digitalRead(Botao) == 0) {
    Bt = Bt +1;  // Avança um modo
    delay(1400);
  }
  if (Bt == 6){ Bt = 0;} //volta para o primeiro modo

  // se o modo for 0 ele exibe todas informações, os demais modos representam respectivamente :
  // Temperatura, Umidade, Pressão, Altitude, Iluminância

  //EXIBE INFORMAÇÕES

  if (Bt == 0 || Bt == 1) {
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
  if (Bt == 0 || Bt == 2) {
    //Exibe umidade
      lcd.setCursor(0,0);  // posiciona o cursor
    lcd.print("Umidade:");  // Escreve no LCD
    lcd.setCursor(9,0);  // posiciona o cursor
    lcd.print(Ad_umid);  // Escreve no LCD
    lcd.setCursor(15,0);  // posiciona o cursor
    lcd.print("%");  // Escreve no LCD

    //Exibe a incerteza da umidade
    lcd.setCursor(0,1);  // posiciona o cursor
    lcd.print("Incert:+-");  // Escreve no LCD
    lcd.setCursor(10,1);  // posiciona o cursor
    lcd.print(IncDHT22,2);  // Escreve no LCD
    lcd.setCursor(15,1);  // posiciona o cursor
    lcd.print("%");  // Escreve no LCD
    delay(3000);
    lcd.clear();
  }
  if (Bt == 0 || Bt == 3) {
  // Exibe pressão
    lcd.setCursor(0,0);  // posiciona o cursor
    lcd.print("Pressao:");  // Escreve no LCD
    lcd.setCursor(8,0);  // posiciona o cursor
    lcd.print(pressao);  // Escreve no LCD
    lcd.setCursor(13,0);  // posiciona o cursor
    lcd.print("hPa");  // Escreve no LCD
    //Exibe a incerteza da pressão
    lcd.setCursor(0,1);  // posiciona o cursor
    lcd.print("Incert:+-");  // Escreve no LCD
    lcd.setCursor(10,1);  // posiciona o cursor
    lcd.print(IncBMP280Press,2);  // Escreve no LCD
    lcd.setCursor(13,1);  // posiciona o cursor
    lcd.print("hPa");  // Escreve no LCD
    delay(3000);
    lcd.clear();
  }

  if (Bt == 0 || Bt == 4) {
  // Exibe altitude
    lcd.setCursor(0,0);  // posiciona o cursor
    lcd.print("Altitude:");  // Escreve no LCD
    lcd.setCursor(10,0);  // posiciona o cursor
    lcd.print(alt);  // Escreve no LCD
    lcd.setCursor(15,0);  // posiciona o cursor
    lcd.print("m");  // Escreve no LCD
  //Exibe a incerteza da altitude
    lcd.setCursor(0,1);  // posiciona o cursor
    lcd.print("Incert:+-");  // Escreve no LCD
    lcd.setCursor(10,1);  // posiciona o cursor
    lcd.print(IncBMP280Alt,2);  // Escreve no LCD
    lcd.setCursor(15,1);  // posiciona o cursor
    lcd.print("m");  // Escreve no LCD
    delay(3000);
    lcd.clear();
  }
  if (Bt == 0 || Bt == 5) {
  // Exibe Iluminância






  }


}
