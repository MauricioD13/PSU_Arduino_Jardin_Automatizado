//Librerias necesarias para los sensores: Usar el manejador de librerias para conseguirlas
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//Declaración las variables de los sensores
int humedad_suelo = 0;
float lect_norm = 0;
float temp = 0;
float h = 0;


//Declaracion de los pines a usar
short int pin_humedad_suelo =  A0;
short int pin_bomba = 7;
short int pin_humedad = 2;


//Declaracion de las variables para el uso del tiempo
long t_actual_bomba;
long t_actual_lcd;
long t_diff_lcd;
long t_diff_bomba=0;

//Bandera
bool bomba_activada = false; 

//Objetos de las librerias necesarias para sensor y pantalla LCD
DHT dht(pin_humedad,DHT11);
LiquidCrystal_I2C lcd(0x3F,16,2);


void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(pin_bomba, OUTPUT);
  digitalWrite(pin_bomba, LOW);
  lcd.init();
  lcd.backlight();
  t_actual_lcd = micros();
  
}


void loop() {
  //Lectura en humedad del suelo
  humedad_suelo = analogRead(pin_humedad_suelo);
  lect_norm = (humedad_suelo * (0.0976));
  
  //Lectura humedad en el aire y temperatura
  h = dht.readHumidity();
  temp = dht.readTemperature();
  
  //Imprimir valores en la pantalla LCD
  lcd.print("H:");
  lcd.print(h);
  lcd.setCursor(9,0);
  
  lcd.print(lect_norm);
  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(temp);
  lcd.setCursor(9,1);
  lcd.print("AGUA:");
  lcd.print("SI");

  //Según la humedad del suelo se determina si prender la bomba de agua
  if(humedad_suelo < 30){
    digitalWrite(pin_bomba, HIGH);
    bomba_activada = true;
    t_actual_bomba = micros();
  }
  
  //Determina cuanto tiempo estar prendida la bomba y luego se apaga el pin digital
  t_diff_bomba =micros() - t_actual_bomba;
  if((t_diff_bomba >= 5000000) && (bomba_activada == true)){
    bomba_activada = false;
    digitalWrite(pin_bomba, LOW);
  }
  
  //Determina el tiempo de refresco de la pantalla LCD para que se vea bien 
  t_diff_lcd = micros() - t_actual_lcd;
  if(t_diff_lcd >= 500000){
  
    lcd.clear();
    t_actual_lcd = micros();
  }
}
