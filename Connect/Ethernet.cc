#include <arduino/Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
 
byte mac[]         = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte ip[]          = { 192, 168, 100, 2 };
byte dns[]         = {8,8,8,8};
byte gateway[]     = { 192, 168, 100, 1 };
 
char pubschar [20];
char message_buff[250];
char message_buff_call[200];
int ledPin = 13;
EthernetClient ethClient;

 
void callback(char* topic, byte* payload, unsigned int length) { 
    int i = 0; for(i=0; i<length; i++) 
    {
        message_buff_call[i] = payload[i];
    }
    message_buff_call[i] = '\0';
    String msgString = String(message_buff_call);
    Serial.println(msgString);
 
    if(msgString.equals("on")) 
    { 
        Serial.println("ON");
        digitalWrite(ledPin,HIGH);
    } 
    if(msgString.equals("off")) 
    {
        Serial.println("OFF");
        digitalWrite(ledPin,LOW);
    }
}
 

PubSubClient client("geeknesia.com", 1883, callback, ethClient);
 
 
void setup() {
    system("ifdown eth0");
    system("ifup eth0");
    delay(10000);
    Serial.begin(9600);
 
  	  	if (Ethernet.begin(mac) != 1) {
    	Ethernet.begin(mac, ip, dns, gateway);
  	}    
 
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
    pinMode(ledPin,OUTPUT);
    digitalWrite(ledPin,LOW);
}
 
 
void loop() {
    if (!client.connected())
    {
        client.connect("DEVICE-ID");
        client.subscribe("PUBLISHED TOPIC");
     }
    client.loop();
}

int main(){
    init();
    setup();
    while(true){
        loop();
    }
}