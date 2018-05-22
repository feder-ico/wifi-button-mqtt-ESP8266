#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_AP "xxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxx"

char mqttServer[] = "xxx.xxx.xxx.xxx";
#define userName "xxxxxxxx"
#define passWord "xxxxxxx"
// Para conectar a thingsBoard poner el TOKEN en el userName y NULL en passWord
// For using with thingsBoars use TOKEN as userName ans NULL in passWord 



WiFiClient wifiClient;

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
int ButtonPIN = 2;
unsigned long lastRead;
bool  pulsadorEnviado = false;

void setup()
{
  Serial.begin(115200);
  delay(10);
  InitWiFi();
  client.setServer( mqttServer, 1883 );

//Define el GPIO2 como entrada
//Define GPIO2 as input 
pinMode(ButtonPIN, INPUT);
  
  lastRead = 0;

}

void loop()
{
  if ( !client.connected() ) {
    reconnect();
  }


 if ( millis() - lastRead > 200 ) { // Verifica el boton cada 1 tercio de segundo... Check button every 1/3 second
    readPin();
    lastRead = millis();
 }

  client.loop();
}

void readPin()    //Aca hacer la magia del timbre
{
  Serial.println("Leyendo el pin del pulsador");
/*  if (digitalRead(ButtonPIN) == LOW) {
      // Prepare a JSON payload string
      String payload_boton = "1";    
      // Send payload
      char attributes[100];
      payload_boton.toCharArray( attributes, 100 );
      client.publish( "frente/pulsador1", attributes );
      Serial.println( attributes );
      lastSend = millis();  // para que si el boton se apreto no vuelva a chequear en 3 segundos.
  }
  */ 

  Serial.println("Leyendo el pin del pulsador");
  if (digitalRead(ButtonPIN) == LOW && !pulsadorEnviado) {
    pulsadorEnviado = true;
    client.publish( "caja001/pulsador", "1");
  } else if (digitalRead(ButtonPIN) == HIGH && pulsadorEnviado) {
    pulsadorEnviado = false;
  }

}


void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to mqtt broker ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("Timbre-Frente", userName, passWord) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}

