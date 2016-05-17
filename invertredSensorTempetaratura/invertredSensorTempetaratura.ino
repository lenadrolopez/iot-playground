// Libreria para Sensores DHT
#include "DHT.h"


#define LEDREADINGPIN 3    // Pin del led que se enciende cuando lee tª
#define DHTPIN 4           // Pin del Arduino al cual esta conectado el pin 4 del sensor
#define LEDALERTPIN 5      // Pin del led que se enciende cuando la Tª excedel el umbral
#define SIRENPIN 6         // Pin del transistor para la sirena
// Descomentar segun el tipo de sensor DHT usado
#define DHTTYPE DHT11     // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Diagrama
// Pin 1 Sensor a +5V de Arduino
// Pin 4 Sensor a HDTPIN (en este sketch es el pin 4)
// Pin 4 Sensor a GROUND de  Arduino
// Resistencia de 10K desde Pin 2 de sensor a Pin 1 de Sensor o +5V
int umbral  = 23;
// Inicializa el sensor
DHT dht(DHTPIN, DHTTYPE);

// Configura Arduino
void setup() {
  Serial.begin(9600);
  Serial.println("Invertred - Detector de exceso de Tª!");
  Serial.print("Umbral de Tª: ");
  Serial.print(umbral);
  Serial.println("");
  pinMode(LEDREADINGPIN,OUTPUT);
  pinMode(SIRENPIN,OUTPUT);
  pinMode(LEDALERTPIN,OUTPUT);
  dht.begin();
  
}

void loop() {
   digitalWrite(LEDREADINGPIN,LOW);
   
  // Espera dos segundos para realizar la primera medición.
  delay(5000);
  
  digitalWrite(LEDREADINGPIN,HIGH);
  // Lee los datos entregados por el sensor, cada lectura demora 250 milisegundos
  // El sensor muestrea la temperatura cada 2 segundos}

  // Obtiene la Humedad
  float h = dht.readHumidity();
  // Obtiene la Temperatura en Celsius
  float t = dht.readTemperature();

  // Control de errores, valida que se obtuvieron valores para los datos medidos
  if (isnan(h) || isnan(t)) {
    Serial.println("Falla al leer el sensor DHT!");
    return;
  }

  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C ");
  if(t>umbral){
    Serial.println("Excede el umbral!!!!");

    digitalWrite(LEDALERTPIN,HIGH);
    digitalWrite(SIRENPIN,HIGH);
  }else{
    Serial.println("Tª OK");
    digitalWrite(LEDALERTPIN,LOW);
    digitalWrite(SIRENPIN,LOW);
  }
  
}
