#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <WiFi101.h>

//L298N,
int IN1 = 3;
int IN2 = 4;
int IN3 = 5;
int IN4 = 6;

char ssid[] = "Atlas5";      
char pass[] = "Futureistech";   
int keyIndex = 0;                 
int ledpin = 7;
bool val = true;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.print(" LHC - Rover IoT ");
  Serial.print("Start Serial ");
  pinMode(ledpin, OUTPUT);      // set the LED pin mode
  Serial.print("WiFi101 shield: ");
  
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("NOT PRESENT");
    return; // don't continue
  }
  Serial.println("DETECTED");
  
  
  while ( status != WL_CONNECTED) {
    digitalWrite(ledpin, LOW);
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   
    digitalWrite(ledpin, HIGH);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection
    delay(10000);
  }
  server.begin();                           // start the web server 
  printWifiStatus();                        // you r connected now toh print the status
  digitalWrite(ledpin, HIGH);
  
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // client code mushak
    Serial.println("new client");           
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {                    


          if (currentLine.length() == 0) {
            // a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("Click <a href=\"/F\">here</a> GET FRONT <br>");
            client.print("Click <a href=\"/S\">here</a> GET STOPING <br>");
            client.print("Click <a href=\"/L\">here</a> GET LEFT <br>");
            client.print("Click <a href=\"/R\">here</a> GET RIGHT <br>");
            client.print("Click <a href=\"/B\">here</a> GET BACK <br>");
            client.println();
            // break out of the while loop:
            break;
          }
          else {      
            currentLine = "";
          }
        }
        else if (c != '\r') {    
          currentLine += c;      
        }

        if (currentLine.endsWith("GET /F")) {
          front();               // go front
        }

        if (currentLine.endsWith("GET /S")) {
          stoping();               // stop
        }

        if (currentLine.endsWith("GET /L")) {
          left();               // go left
        }
        
        if (currentLine.endsWith("GET /R")) {
          right();               // go right
        }

        if (currentLine.endsWith("GET /B")) {
          back();               // go Back
        }
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void  front(){
  
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void  stoping (){
    //Gira o Motor A no sentido horario
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
}

void  left (){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);    
}

void  right (){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);    
}

void  back (){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // wiFi shield's IP address daalo

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  
  Serial.print("User yout firefox http://");
  Serial.print(ip);
}