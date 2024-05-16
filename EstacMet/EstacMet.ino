//Bibliotecas Adafruit (BMP280 e DHT22)
#include <Adafruit_Sensor.h>
#include <DHT.h> 
#include <Adafruit_BMP280.h> 

//Bibliotecas para manipular o LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// "Renomeando" portas
#define  LM A0
#define LDR A1
#define Botao 3

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
float IncLDR = 0.0021; //incerteza padrão de medição de altitude do ldr (lx)

//Variáveis para manipulação do BMP280
Adafruit_BMP280 bmp; //OBJETO DO TIPO Adafruit_BMP280 (I2C)
float pressao,alt; // declaracao de variaveis

//variaveis LDR
int bin;
float Vout, RLDR, lum;

// Variável para modo do botão 
int Bt = 0;

int teladelay = 550; //Delay de atualização das informações na tela em ms

void setup() { 
  dht.begin(); //Inicia a comunicação da biblioteca DHT
  bmp.begin(0x76); // Inicia o BMP no endereco 0x76
  analogReference(INTERNAL); // Conversao A/D com fundo de escala de 1.1V para o LM35
  pinMode(Botao , INPUT_PULLUP); //Inicia o pino do botão utilizando o pull up interno do arduino
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);




 //Inicia LCD
 lcd.init(); // inicializa o lcd
  lcd.backlight(); // liga o backlight

  byte m_m[] = {
  B00100,
  B00100,
  B11111,
  B00100,
  B00100,
  B00000,
  B11111,
  B00000
};
  byte cels[] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte til[] = {
  B01110,
  B00000,
  B01110,
  B00001,
  B01111,
  B10001,
  B01111,
  B00000
};
byte cir[] = {
  B00100,
  B01010,
  B01110,
  B00001,
  B01111,
  B10001,
  B01111,
  B00000
};

lcd.createChar(0, m_m);
lcd.createChar(1, cels);
lcd.createChar(2,til);
lcd.createChar(3,cir);
 //info grupo e disciplina
  lcd.setCursor(0,0);  // posiciona o cursor
  lcd.print("Instrumed");  // Escreve no LCD
  lcd.setCursor(0,1);  // posiciona o cursor
  lcd.print("2024");  // Escreve no LCD
  delay(2000);
  lcd.clear();
  lcd.setCursor(5,0);  // posiciona o cursor
  lcd.print("Grupo ");  // Escreve no LCD
  lcd.setCursor(6,1);  // posiciona o cursor
  lcd.print("FMC");  // Escreve no LCD
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);  // posiciona o cursor
  lcd.print("Felipe, ");  // Escreve no LCD
  lcd.setCursor(8,0);  // posiciona o cursor
  lcd.print("Matheus");  // Escreve no LCD
  lcd.setCursor(4,1);  // posiciona o cursor
  lcd.print("E Camila");  // Escreve no LCD
  delay(1000);
  lcd.clear();


 //info estação
  lcd.setCursor(1,0);  // posiciona o cursor
  lcd.print("Meteorological");  // Escreve no LCD
  lcd.setCursor(3,1);  // posiciona o cursor
  lcd.print("Station");  // Escreve no LCD
  delay(1000);
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
  Ad_temp = 1.0095*temp + 0.8695; //LM35 com temperatura ajustada

// BMP280
  pressao=bmp.readPressure(); // retorna a pressao em Pascal (Pa)
  pressao= pressao /100; // Converte a pressão de Pa para hPa
  alt= bmp.readAltitude(1013.25); // retorna a altitude em metros
                                    // 1 atm = 1013,25 hPa (hectopascal)


//LDR
bin = analogRead(A1);
Vout = bin*1.1/1024.0;   //Conversor ADC
RLDR = (Vout/(3.3-Vout)) * 49820;
lum = pow(10,(-1.2)*log10(RLDR)+6.69); //iluminância 


  // Analisa modo do botão
  if (digitalRead(Botao) == 0) {
    Bt = Bt +1;  // Avança um modo
    lcd.clear();
    delay(700);
  }
  if (Bt == 6){ Bt = 0;} //volta para o primeiro modo

  // se o modo for 0 ele exibe todas informações, os demais modos representam respectivamente :
  // Temperatura, Umidade, Pressão, Altitude, Iluminância

  //EXIBE INFORMAÇÕES
  if (Bt == 0) {
    lcd.setCursor(0,0);  // posiciona o cursor
    lcd.print("Aperte e mude a");  // Escreve no LCD
    lcd.setCursor(0,1);  // posiciona o cursor
    lcd.print("grandeza medida");  // Escreve no LCD
    delay(teladelay);
    
  }
  if ( Bt == 1) {
    // Exibe temperatura
    lcd.setCursor(0,0);  // posiciona o cursor
    lcd.print("Temperatura ( C)");  // Escreve no LCD
    lcd.setCursor(13,0);
    lcd.write(1);
    lcd.setCursor(0,1);  // posiciona o cursor
    lcd.print(Ad_temp,2);  // Escreve no LCD
    lcd.setCursor(14,0);  // posiciona o cursor
   
    //Exibe a incerteza da temperatura
    lcd.setCursor(7,1);  // posiciona o cursor
    lcd.write(0);  // Escreve no LCD
    lcd.setCursor(9,1 );
    lcd.print(IncLM35,2);  // Escreve no LCD
     lcd.setCursor(0,1);
    delay(teladelay);
    lcd.print("               ");
     
    }
  if ( Bt == 2) {
    //Exibe umidade
   lcd.setCursor(0,0);  // posiciona o cursor
    lcd.print("Umidade (%)");  // Escreve no LCD
    lcd.setCursor(0,1);  // posiciona o cursor
    lcd.print(Ad_umid,1);  // Escreve no LCD
    lcd.setCursor(14,0);  // posiciona o cursor
   
    //Exibe a incerteza da temperatura
    lcd.setCursor(7,1);  // posiciona o cursor
    lcd.write(0);  // Escreve no LCD
    lcd.setCursor(9,1 );
    lcd.print(IncDHT22,1);  // Escreve no LCD
     lcd.setCursor(0,1);
    delay(teladelay);
    lcd.print("               ");
    
  }
  if ( Bt == 3) {
  // Exibe pressão
      lcd.setCursor(0,0);  // posiciona o cursor
    lcd.print("Press o (hPa)");  // Escreve no LCD
    lcd.setCursor(5,0);
    lcd.write(2);
    lcd.setCursor(4,1);  // posiciona o cursor
    lcd.print(pressao,2);  // Escreve no LCD
   
    //Exibe a incerteza da temperatura
     lcd.setCursor(0,1);
    delay(teladelay);
    lcd.print("               ");
    
  }

  if ( Bt == 4) {
  // Exibe altitude
         lcd.setCursor(0,0);  // posiciona o cursor
    lcd.print("Altitude (m)");  // Escreve no LCD
    lcd.setCursor(0,1);  // posiciona o cursor
    lcd.print(alt,1);  // Escreve no LCD
   
    //Exibe a incerteza da temperatura
    lcd.setCursor(8,1);  // posiciona o cursor
    lcd.write(0);  // Escreve no LCD
    lcd.setCursor(10,1 );
    lcd.print(IncBMP280Alt,1);  // Escreve no LCD
     lcd.setCursor(0,1);
    delay(teladelay);
    lcd.print("               ");
    
  }
  if ( Bt == 5) {
  // Exibe Iluminância
             lcd.setCursor(0,0);  // posiciona o cursor
    lcd.print("Ilumin ncia (lx)");  // Escreve no LCD
    lcd.setCursor(6,0);
    lcd.write(3);
    lcd.setCursor(4,1);  // posiciona o cursor
    lcd.print(lum,2);  // Escreve no LCD
   
    //Exibe a incerteza da temperatura
    lcd.setCursor(0,1);
    delay(teladelay);
    lcd.print("               ");





  }


}
