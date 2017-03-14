#include <Wire.h>

#define SLAVE_ADDRESS 0x04

int dataDiterima = 0;
int dataTerkirim = 0;
int LED = 12;

double temp;
int sens;
int kirimData;
int s[4] = {2,3,4,5}; // s0,s,s2,s3 multiplexer
int aktifMultiplex=6;

void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    for(int k=0;k<4;k++){
        pinMode(s[k], OUTPUT);
    }
    pinMode(aktifMultiplex, OUTPUT);
    digitalWrite(aktifMultiplex,LOW); //port EN multiplexer

    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);

    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
}

void loop(){
    temp = GetTemp(); // get suhu internal arduino

    getVoltage();

    aksi(dataDiterima);
    for (int i = 0; i < 16; i++) {
        multiplex(i); // auto switch multiplexer
        delay(100);
    }
}

// receive data dari raspberry pi
void receiveData(int byteCount){
    while(Wire.available()) {
        dataDiterima = Wire.read();
        Serial.print("Data yang diterima dari raspi = ");Serial.print(dataDiterima);

        if (dataDiterima == 1){
            kirimData = getVoltage();
        }
        else if(dataDiterima == 2){
            kirimData = dataDiterima*1000;
        }

        Serial.print("\t Data yang dikirim ke raspi = ");Serial.println(kirimData);
        dataTerkirim = kirimData;
    }
}

// send data ke raspberry pi
void sendData(){
    Wire.write(dataTerkirim);
}

// Get the internal temperature of the arduino
double GetTemp(void){
    unsigned int wADC;
    double t;
    ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
    ADCSRA |= _BV(ADEN); // enable the ADC
    delay(20); // wait for voltages to become stable.
    ADCSRA |= _BV(ADSC); // Start the ADC
    while (bit_is_set(ADCSRA,ADSC));
    wADC = ADCW;
    t = (wADC - 324.31 ) / 1.22;
    return (t);
}

// EIT SECTION
// get voltage
float getVoltage(){
    int analogInput = A0;
    float vout = 0.0;
    float vin = 0.0;
    float R1 = 30000.0;
    float R2 = 7500.0;
    int value = 0;

    value = analogRead(analogInput);
    vout = (value * 5.0)/ 1024.0;
    vin = vout / (R2/(R1+R2));

    return (vin);
}

// mengontrol sinyal yang lewat pada multiplexer
void multiplex(int chanel){
  int bits;
  for(int i=0;i<4;i++){
    bits = chanel%2;
    chanel =(int) chanel/2;
    digitalWrite(s[i],bits);
  }
}

/*
 * Agung Dwi Prasetyo
 * G64130073
 * cs.ipb.ac.id
 */
