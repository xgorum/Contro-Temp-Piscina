#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

DeviceAddress sondaEntrada = {0x28, 0x11, 0xF7, 0x76, 0xE0, 0x01, 0x3C, 0x3A}; 
DeviceAddress sondaSalida = {0x28,0x99, 0xB8, 0x76, 0xE0, 0x01, 0x3C, 0xF7};   
const byte releBomba = 3;
const byte pinDatosDQ = 9;
const int tempDeseadaPiscina = 35;
const byte pinSeguridad1 = 4;
const byte pinSeguridad2 = 5;

//LiquidCrystal_I2C lcd(0x3F,16,2); // (0x27,16,2)                                

OneWire oneWireObjeto(pinDatosDQ);                                              
DallasTemperature sensorDS18B20(&oneWireObjeto);                                

void setup(){
    
    pinMode(releBomba, OUTPUT);                                                 
    pinMode(pinSeguridad1, OUTPUT);    
    pinMode(pinSeguridad2, OUTPUT);
    //attachInterrupt(digitalPinToInterrupt(2), paro_de_emergencia, RISING);
    //lcd.init();                                                                 
    //lcd.backlight();                                                            
    Serial.begin(9600);                                                        
    sensorDS18B20.begin();                                                     
    

    digitalWrite(releBomba, LOW);
    digitalWrite(pinSeguridad1, LOW);                                           
    digitalWrite(pinSeguridad2, LOW);

    //lcd.setCursor(0,0);                                                         
    //lcd.print("Buscando Sondas...");                                             
    Serial.println("Buscando dispositivos...");
    //lcd.print("Encontrados: ");
    Serial.print("Encontrados: ");
    int numeroSondasConectadas = sensorDS18B20.getDeviceCount();
    Serial.print(numeroSondasConectadas);                                         
    //lcd.print(numeroSondasConectadas);
    Serial.println(" Sondas");
    //lcd.println( " Sondas"); 
    if (numeroSondasConectadas != 2);
    {
    espera_sondas_correctas();
    }
    
}

void loop()
{                                                         
    do
    {            
        //lcd.setCursor(0,0);
        Serial.print("tº Sonda de salida: ");
        Serial.print(lectura_temp_salida());
        Serial.println(" ºC"); 
        //lcd.print("Piscina: ");
        //lcd.print(lectura_temp_salida());                                                  
        //lcd.print("ºC");

    } while (lectura_temp_salida() > tempDeseadaPiscina);                                                  

    //lcd.clear();
    
    while(lectura_temp_entrada() > lectura_temp_salida()){
        Serial.println("La temperatura de entrada es mayor a temperatura salida");                                      
        digitalWrite(pinSeguridad1, HIGH);
        digitalWrite(pinSeguridad2, HIGH);
        if (digitalRead(pinSeguridad1) == HIGH && digitalRead(pinSeguridad2) == HIGH)
        {
            digitalWrite(releBomba, HIGH);                                               
            //lcd.setCursor(0,0);
            //lcd.print("Retorno: ");
            Serial.print("tº Sonda de Entrada: ");
            //lcd.print(tempSondaEntrada);
            Serial.print(lectura_temp_entrada());
            Serial.println(" ºC");                                                
            //lcd.print(" ºC");
            //lcd.setCursor(1,1);
            Serial.println ("Bomba Activada");
            //lcd.print("Bomba activada");                                                 
        }
        
                                                                                    
    }
    Serial.println("La temperatura de entrada es menor a la temperatura salida");
    Serial.print("Temperatura de Entrada: ");
    Serial.print(lectura_temp_entrada());
    Serial.println(" ºC");
    digitalWrite(releBomba, LOW);
                                                      
}

float lectura_temp_salida()
{
    float media = 0;
    for (int i = 0; i < 5; i++)
    {
    sensorDS18B20.requestTemperaturesByAddress(sondaSalida);                     
    float tempSondaSalida = sensorDS18B20.getTempC(sondaSalida);
    media = tempSondaSalida + media;
    delay(500);
    
    }
    return media/5;
    
}

float lectura_temp_entrada()
{
    float media = 0;
    for (int i = 0; i < 5; i++)
    {
    sensorDS18B20.requestTemperaturesByAddress(sondaEntrada);                     
    float tempSondaEntrada = sensorDS18B20.getTempC(sondaEntrada);
    media = tempSondaEntrada + media;
    delay(500);
    
    }
    return media/5;
}

void espera_sondas_correctas()
{
  int numeroSondasConectadas = sensorDS18B20.getDeviceCount();
  if (numeroSondasConectadas==2)
  {
  return;
  }
   else
   {
       espera_sondas_correctas();
   }
   
}
/*void paro_de_emergencia()
{
 digitalWrite(pinSeguridad1, LOW);
 digitalWrite(releBomba, LOW);
}*/
/*

2.- Poner un led o bombilla  encendido cuando se active la bomba. 
3.- Seta paro segurdad a traves de interrupcion.
4.- Incluir modulo <Bluetooth HC-05 para ver y controlar la temp de la piscina. 
5.- Mirar posibilidad de cambiar arduino uno por ESP32


*/

