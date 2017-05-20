#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include "confi.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

<<<<<<< HEAD

=======
const char* ssid = "TuSSDI";
const char* password = "PASSWORDdelaRED";
>>>>>>> 8728117a3949f84c9150cab987f45a54c56b346d

#define PIN 4
#define NUMPIXELS 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

String R,G,B;
int Ri, Gi, Bi;

String intensidad;
int Ii;

ESP8266WebServer server(80);
WiFiClient client;

const int led = 13;

String web = "<!DOCTYPE HTML><html><head><title>Lampara</title></head><body><h1> <FONT SIZE=\"5\" COLOR=\"red\"> Colores </h1>Selecciona un color: <form action=\"\"><input name=\"color\" type=\"color\" style='width:300px; height:300px'/><input name=\"intensidad\" type=\"range\" style='width:80%; height:300px'/><br><input type=\"submit\" value=\"Cambiar\" style='width:100px; height:50px'/></form></body></html>";

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

void iluminacion(int R, int G, int B, int I){
   for(int i=0;i<NUMPIXELS;i++){   
    pixels.setPixelColor(i, pixels.Color(R,G,B));
    pixels.setBrightness(I);
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
  Serial.print("Color en Hex: ");Serial.print(R);Serial.print("-"); Serial.print(G);Serial.print("-");Serial.println(B);
  
  //Convierte los valores de color hexadecimales en decimales
  R = hexToDec(R);
  G = hexToDec(G);
  B = hexToDec(B);  
  Serial.print("Color en RGB: ");Serial.print(R);Serial.print("-"); Serial.print(G);Serial.print("-");Serial.println(B);
  
  //Transforma el String en valores integer
  Ri = R.toInt();
  Gi = G.toInt();
  Bi = B.toInt();

  //Obtenemos la intensidad
  int findIntensidad = colores.indexOf('intensidad');
  int ultPos = colores.length();
  //int ultPos = colores.lastIndexOf('/');
  Serial.print("Comienza la cadena");Serial.println(findIntensidad);Serial.print("Posicion");Serial.println(ultPos);
  intensidad = colores.substring(findIntensidad+4,ultPos-1);
  Ii = intensidad.toInt();
  Serial.print("Intensidad: ");Serial.println(intensidad);
  Serial.print("I: ");Serial.println(Ii);
  
  //mapeamos los valores del range HTML5 (0 a 100) a brightness de Neopixel (0 a 255)
  Ii = map(Ii, 0, 100, 0, 255);
  Serial.print("Intensidad mapeada: ");Serial.println(Ii);
  
  //Encendemos la luces en el color seleccionado
  iluminacion(Ri,Gi,Bi,Ii);
 
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
  
  WiFi.mode(WIFI_AP_STA); 
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pixels.begin();
  Serial.println("");

  for(int i=0;i<NUMPIXELS;i++){   
    pixels.setPixelColor(i, pixels.Color(0,0,255));
    pixels.setBrightness(100);
    pixels.show(); 
    delay(200);
  }
  for(int i=0;i<NUMPIXELS;i++){   
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show(); 
    delay(200);
  } 
  // Wait for connection
  if (WiFi.status() == WL_CONNECTED) {
      pixels.setPixelColor(3, pixels.Color(0,255,0));
      pixels.show(); 
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      pixels.setPixelColor(3, pixels.Color(0,0,0));
      pixels.show();
  }else{
    WiFi.mode(WIFI_AP);
    pixels.setPixelColor(3, pixels.Color(255,0,0));
    pixels.show(); 
    Serial.println("Configuring access point...");  
    WiFi.softAP(nombreRed, claveRed); 
    delay(500);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(nombreRed);
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.show(); 
  }
  


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
