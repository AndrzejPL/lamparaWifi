#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

//Datos de conexion a la red
const char* ssid = "NombreDeTuRed";
const char* password = "Contraseña";

#define PIN 6
#define NUMPIXELS 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

String R,G,B;
int Ri, Gi, Bi;

ESP8266WebServer server(80);
WiFiClient client;

const int led = 13;

String web = "<!DOCTYPE HTML><html><head><title>Lampara</title></head><body><h1> <FONT SIZE=\"5\" COLOR=\"red\"> Colores </h1>Selecciona un color: <form action=\"\"><input name=\"color\" type=\"color\" style='width:300px; height:300px'/><br><input type=\"submit\" value=\"Cambiar\" style='width:100px; height:50px'/></form></body></html>";

//Convertir Hexadecimal a Decimal
//Funcion extraida de https://github.com/benrugg/Arduino-Hex-Decimal-Conversion/blob/master/hex_dec.ino
unsigned int hexToDec(String hexString) {

  unsigned int decValue = 0;
  int nextInt;

  for (int i = 0; i < hexString.length(); i++) {

    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);

    decValue = (decValue * 16) + nextInt;
  }

  return decValue;
}

void iluminacion(int R, int G, int B){
   for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(R,G,B));
    pixels.show();
    delay(200);
  }
}

void handleRoot() {
  String colores;
  server.method() == HTTP_GET;
  digitalWrite(led, 1);
  server.send(200, "html", web);
  digitalWrite(led, 0);
  Serial.print("Argumentos:");
  Serial.println(server.args());
  Serial.print("URI:");
  Serial.println(server.uri());
  for (uint8_t i=0; i<server.args(); i++){
    colores += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  Serial.println(colores);

  //Busca el comienzo del codigo hexadecimal de color
  int firstHastag = colores.indexOf('#');
  Serial.print("Primera respuesta: ");Serial.println(firstHastag);

  //Extrae los colores codificados en Hexadecimal en R-G-B
  R = colores.substring(firstHastag+1, firstHastag+3);
  G = colores.substring(firstHastag+3, firstHastag+5);
  B = colores.substring(firstHastag+5, firstHastag+7);
  Serial.print(R);Serial.print("-"); Serial.print(G);Serial.print("-");Serial.println(B);

  //Convierte los valores de color hexadecimales en decimales
  R = hexToDec(R);
  G = hexToDec(G);
  B = hexToDec(B);
  Serial.print(R);Serial.print("-"); Serial.print(G);Serial.print("-");Serial.println(B);

  //Transforma el String en valores integer
  Ri = R.toInt();
  Gi = G.toInt();
  Bi = B.toInt();

  //Encendemos la luces en el color seleccionado
  iluminacion(Ri,Gi,Bi);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pixels.begin();
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();

  WiFiClient client;
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);

  client.flush();

}