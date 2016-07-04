#include <Ethernet.h>
#include <SPI.h>
#define LED_RED 7
// Definimos el pin del LED rojo
#define LED_GREEN 8
// Definimos el pin del LED verde
#define TEMP_SENSOR 0
// Definimos el pin analogo del sensor

// Definimos la MAC Address de nuestro Ethernet Shield
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x98, 0x91};
// Definimos la Dirección IP del Servidor Node.js
byte server[] = {192, 168, 169, 100};
// Definimos la Dirección IP del Arduino
byte ip[] = {192, 168, 169, 120};

// Instanciamos el cliente Ethernet con el que nos conectaremos al servidor
EthernetClient client;

// Definimos la variable donde almacenaremos la respuesta del servidor.
String response = String(10);
// Definimos el separador del mensaje
char separator = '|';
// Utilizaremos esta variable para verificar si la lectura de la respuesta ha comenzado
boolean started = false;


void setup() {
  Ethernet.begin(mac, ip);
  // Iniciamos el Ethernet Shield
  Serial.begin(9600);
  // Utilizaremos el puerto Serial como debug
  pinMode(LED_RED, OUTPUT);
  // Definimos el pin del LED rojo como salida
  pinMode(LED_GREEN, OUTPUT);
  // Definimos el pin del LED verde como salida

}

void loop() {
  // Utilizamos la misma lógica de lectura del sensor vista en el artículo
  // anterior.
  int voltage = analogRead(TEMP_SENSOR);
  int temp = (5 * voltage * 100) / 1024;
  // Realizamos la conexión al servidor Node.js en el puerto 8080
  if (client.connect(server, 8080)) { // Enviamos la peticion GET utilizando el protocolo HTTP
    client.println("GET /temp?value=" + String(temp) + " HTTP/1.0");
    client.println(); // A continuación realizaremos la lectura de la respuesta desde el servidor
    // En este caso solo vamos a hacer lectura mientras estemos conectados
    // Luego de leer la respuesta completa desconectamos el cliente
    while (client.connected()) {
      // Solo realizaremos la lectura de la respuesta si el cliente tiene
      // algo para nosotros
      if (client.available()) {
        // Debemos leer caracter por caracter
        char c = client.read();
        // En el siguiente bloque de código utilizaremos el separador `|` para
        // verificar cuando empieza y termina el mensaje que necesitamos
        if (c != separator && started) {
          response += c;
        } else if (c == separator && !started) {
          started = true;
        } else if (c == separator && started) {
          started = false;
          client.stop();

        }
      }
    }
    // Luego de recibir la respuesta del servidor podemos decidir cual LED encender
    if (response == "normal") {
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
    } else if (response == "warning") {
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
    } response = "";
    // Limpiamos la variable para la siguiente petición
  } else {
    Serial.print("connection failed");
  }

  delay(5000); // Esperamos 5s antes de empezar la siguiente peticion.

}