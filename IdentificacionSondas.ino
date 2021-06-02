#include <OneWire.h>
#include <DallasTemperature.h>

const int pinDatosDQ = 9;
int numeroSondasConectadas;
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

void setup()
{
	sensorDS18B20.begin();
  Serial.begin (9600);
    
  Serial.println("Buscando dispositivos...");
  Serial.print("Encontrados: ");
    
  numeroSondasConectadas = sensorDS18B20.getDeviceCount();
  Serial.print(numeroSondasConectadas);
  Serial.println(" Sondas");

 if(numeroSondasConectadas==1)
 {
        
    // Tipo definido como una array de 8 bytes (uint8_t)
    DeviceAddress sensorTemperatura;
    // Obtenemos dirección
    sensorDS18B20.getAddress(sensorTemperatura, 1);

    // Mostamos por el monitor serie
    Serial.print("Sensor encontrado: ");

    // Recorremos los 8 bytes del identificador único
    for (uint8_t i = 0; i < 8; i++)
    {
      // Si solo tiene un dígito rellenamos con un cero a la izquierda
      if (sensorTemperatura[i] < 16) Serial.print("0");

        // Mostramos los datos que van en HEXADECIMAL
        Serial.print(sensorTemperatura[i], HEX);
    }
  } 
}

void loop()
{

}
