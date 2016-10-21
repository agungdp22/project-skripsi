//Import library dan deklarasi program.
#include <arduino/Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
 
// Settingan network(IP Address).
byte mac[]         = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte ip[]          = { 192, 168, 100, 2 };
byte dns[]         = {8,8,8,8};
byte gateway[]     = { 192, 168, 100, 1 };
 
char pubschar [20];
char message_buff[250];
char message_buff_call[200];
int ledPin = 13;
EthernetClient ethClient;
 
// Fungsi ini yang akan dipanggil oleh MQTT jika menerima data
// Pesan yang dikirim melalui MQTT tersimpan pada payload
// Oleh karena itupayload lah yang akan menampung perintah
// untuk device tersebut
 
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
 
// Tambahkan PubSubClient object
PubSubClient client("geeknesia.com", 1883, callback, ethClient);
 
//Dibagian ini adalah bagian setup pada program, setup baud rate 
// serial,setupEthernet,atau menambahkan serial print debugging 
// program.
 
void setup() {
    system("ifdown eth0");
    system("ifup eth0");
    delay(10000); //wait for accessing sensor
    Serial.begin(9600);
 
  	// Koneksi DHCP
  	if (Ethernet.begin(mac) != 1) {
    	// Jika gagal, maka koneksi manual
    	Ethernet.begin(mac, ip, dns, gateway);
  	}    
 
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
    pinMode(ledPin,OUTPUT);
    digitalWrite(ledPin,LOW);
}
 
// Jika mqtt client belum terkoneksi maka harus menjalankan fungsi
// connect dengan parameter device id yang ada pada detail device
// dan melakukan subscribe pada topic khusus untuk menerima pesan
// perintah untuk device tersebut
// perintah client.loop digunakan untuk memeriksa koneksi mqtt secara
// berkala dan mengubah return value dari fungsi client.connected
 
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

/*
Alhamdulillahirabbil'alamiin
Agung Dwi Prasetyo
G64130073
*/