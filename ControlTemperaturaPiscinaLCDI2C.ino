#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

DeviceAddress sondaEntrada = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  //Asignamos la direccion de sonda entrada a la variable sondaEntrada
DeviceAddress sondaSalida = {0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};   //Asignamos la direccion de sonda salida a la variable sondaSalida
const int releBomba = 3;
const int pinDatosDQ = 9;

LiquidCrystal_I2C lcd(0x3F,16,2); // (0x27,16,2)                                //definimos el LCD en la direccion 0x27 y lo definimos de 16/2 columnas/filas

OneWire oneWireObjeto(pinDatosDQ);                                              //Definimos pinDatosDQ como objeto de OneWire
DallasTemperature sensorDS18B20(&oneWireObjeto);                                //Definimos sensorDS18B20 a la direccion del objeto pinDatosDQ

void setup(){

    pinMode(releBomba, OUTPUT);                                                 // Pin Rele como Salida
    lcd.init();                                                                 //iniciamos lcd
    lcd.backlight();                                                            //encendemos luz de fondo lcd
    Serial.begin(9600);                                                         //iniciamos puerto serie
    sensorDS18B20.begin();                                                      //iniciamos puerto OneWire
    

    digitalWrite(releBomba, HIGH);                                               // Por seguridad ponemos a nivel alto el relé para que la bomba empiece apagada. 

    lcd.setCursor(0,0);                                                          // ponemos el cursor en 0,0
    lcd.print("Buscando Sondas...");                                             // escribimos ....
    Serial.println("Buscando dispositivos...");
    delay(1000);                                                                    
    lcd.print("Encontrados: ");
    Serial.println("Encontrados: ");
    delay(1000);
    int numeroSondasConectadas = sensorDS18B20.getDeviceCount();                  // ejecutamos una busqueda de dispositivos conectados guardandola en variable numeroSondaConectadas
    Serial.print(numeroSondasConectadas);                                         // Mostramos el numero de sondas detectadas.
    lcd.print(numeroSondasConectadas);
    Serial.println(" Sondas");
    lcd.println( " Sondas");
    
    /*
  ------------------------------------------------------------------------
    - Bucle para asignar el numero de serie en hex a cada sonda encontrada -
    ------------------------------------------------------------------------
    */
    for (int in = 0; in <= numeroSondasConectadas; in++){                           // bucle que se repite por cada sonda encontrada                       
               
        DeviceAddress sensorTemperatura;                                            // Tipo definido como una array de 8 bytes                                          
        sensorDS18B20.getAddress(sensorTemperatura, 1);                             // Obtenemos dirección
      
        Serial.print("Sonda encontrada: ");                                         // Escribimos....
        lcd.print("Sonda encontrada: ");
        
        for (uint8_t i = 0; i < 8; i++)                                             // Recorremos los 8 bytes del identificador único
        {
          if (sensorTemperatura[i] < 16) Serial.print("0");                         // Si solo tiene un dígito rellenamos con un cero a la izquierda
         
        Serial.print(sensorTemperatura[i], HEX);                                    // Mostramos los datos que van en HEXADECIMAL
        lcd.print(sensorTemperatura[i], HEX);
        delay(5000);
        lcd.clear();                                                                // Borramos todo el texto del lcd
        }

    }

}

void loop(){
    
    float tempSondaSalida;                                                           // Definimos la valiable que vamos a usar dentro de do/while
    
    do
    {
        sensorDS18B20.requestTemperaturesByAddress(sondaSalida);                     // Mandamos comando para toma de temp de sensor sonda salida
        tempSondaSalida = sensorDS18B20.getTempC(sondaSalida);                       // Consegimos la temp de sonda Salida y la guardamos en la variable tempSondaSalida
        lcd.setCursor(0,0);
        lcd.print("Piscina: ");
        lcd.print(tempSondaSalida);                                                  // Mostramos la temp de la sonda de salida 
        lcd.print("ºC");

    } while (tempSondaSalida > 24);                                                  // Si la temp de sonda salida en mayor que 24ºC repetimos el bucle anterior. 

    lcd.clear();                                                                     // Borramos pantalla LCD.
    sensorDS18B20.requestTemperaturesByAddress(sondaEntrada);                        // Mandamos comando para toma de temp de sensor sonda entrada
    float tempSondaEntrada = sensorDS18B20.getTempC(sondaEntrada);                   // Consegimos la temp de sonda entrada y la guardamos en la variable tempSondaEntrada
    
    while(tempSondaEntrada > tempSondaSalida){                                       // Si la temp de entrada en mayor que la de salida ejecutamos el bucle   
        digitalWrite(releBomba, LOW);                                                // Activamos bomba poniendo a LOW la salida del rele
        lcd.setCursor(0,0);
        lcd.print("Retorno: ");
        lcd.print(tempSondaEntrada);                                                 // Mostramos por LCD la temperatura de enmtrada
        lcd.print("ºC");
        lcd.setCursor(1,1);
        lcd.print("Bomba activada");                                                 // Informamos que la bomba esta activa
    }
    digitalWrite(releBomba, LOW);                                                    // Si la temp de entrada es menor o igual a temp salida apagamos la bomba. 
}

/*

1.- Terminar el bucle para que asigne la direccion unica de cada sensor a su variable.
2.- Poner un led o bombilla  encendido cuando se active la bomba. 
3.- Seta paro segurdad a traves de interrupcion.
4.- Incluir modulo Bluetooth HC-05 para ver y controlar la temp de la piscina. 
5.- Mirar posibilidad de cambiar arduino uno por ESP32
6.- Incluir un for para tomar una media de temp (5 medidas, una cada 0.5 seg)

*/