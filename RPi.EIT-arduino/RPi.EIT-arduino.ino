#include <Wire.h>

#define SLAVE_ADDRESS 0x04

int dataDiterima = 0;
int dataTerkirim = 0;

double temp;
int sens;
int kirimData;

// init pin multiplexer output
int mul1[4] = {2,3,4,5};     // pin s0,s1,s2,s3 multiplexer 1
int mul2[4] = {8,9,10,11};   // pin s0,s1,s2,s3 multiplexer 2
int mul3[4] = {20,21,22,23}; // pin s0,s1,s2,s3 multiplexer 3
int mul4[4] = {24,25,26,27}; // pin s0,s1,s2,s3 multiplexer 4
int aktifMultiplex1 = 6;     // port EN multiplexer 1
int aktifMultiplex2 = 7;     // port EN multiplexer 2
int aktifMultiplex3 = 28;    // port EN multiplexer 3
int aktifMultiplex4 = 29;    // port EN multiplexer 4

float volt = 0.0;

void setup() {
    Serial.begin(9600);
    for(int k=0;k<4;k++){
        pinMode(mul1[k], OUTPUT);
        pinMode(mul2[k], OUTPUT);
    }
    pinMode(aktifMultiplex1, OUTPUT);
    digitalWrite(aktifMultiplex1,LOW);
    pinMode(aktifMultiplex2, OUTPUT);
    digitalWrite(aktifMultiplex2,LOW);

    // Communication dgn Raspi
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);

    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
}

void loop(){
    scanEIT();
    Serial.println("\n");
    delay(4000);
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

// get data EIT (matrix 16x13)
void scanEIT(){
    int aPos=14; // kutub positif arus injeksi
    int aNeg=15; // kutub negatif arus injeksi
    for(int x=0; x<16; x++){
        if(aPos==16) aPos=0;
        if(aNeg==16) aNeg=0;
        
        multiplex(3, aPos); // multiplexer 3 handle kutub positif arus injeksi
        delay(10);
        multiplex(4, aNeg); // multiplexer 4 handle kutub negatif arus injeksi
        
        for (int i = 0; i < 13; i++) { // i = kutub positif tegangan
            delay(1000);
            int j=i+1;                 // j = kutub negatif tegangan
            multiplex(1, i);           // multiplexer 1 handle kutub positif tegangan
            delay(10);
            multiplex(2, j);           // multiplexer 2 handle kutub negatif tegangan
            volt = getVoltage();
            //Serial.print(j);Serial.print(" ");Serial.println(i);
            Serial.println(volt,5);
        }
        aPos+=1;
        aNeg+=1;
    }
}

// get voltage measurement
float getVoltage(){
    analogReference(INTERNAL);
    int value = analogRead(A0);
    float voltage = (value*1.1)/1023.0;
    return (voltage);
}

// mengontrol sinyal yang lewat pada multiplexer, channel decimal to biner
void multiplex(int tipe, int chanel){
  int bits;
  for(int i=0;i<4;i++){
    bits = chanel%2;
    chanel =(int) chanel/2;
    
    if(tipe==1){
      digitalWrite(mul1[i],bits);
    }else if(tipe==2){
      digitalWrite(mul2[i],bits);
    }else if(tipe==3){
      digitalWrite(mul3[i],bits);
    }else if(tipe==4){
      digitalWrite(mul4[i],bits);
    }
  }
}

/*
 * Agung Dwi Prasetyo
 * G64130073
 * cs.ipb.ac.id
 */
