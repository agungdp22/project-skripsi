/*
 *  Mikon -> Arduino MEGA
 */

#include <Wire.h>

#define SLAVE_ADDRESS 0x04

int dataDiterima = 0;
int dataTerkirim = 0;

double temp;
int sens;
int kirimData;

// init pin multiplexer output
int mul[4][4] = {{22,23,24,25},{26,27,28,29},{30,31,32,33},{34,35,36,37}}; // pin s0,s1,s2,s3 all multiplexer
int ENMul[4] = {38,39,40,41};    // port EN all multiplexer

float volt = 0.0;

void setup() {
    Serial.begin(9600);
    for(int k=0;k<4;k++){
        for(int l=0;l<4;l++){
            pinMode(mul[k][l], OUTPUT);
        }
        digitalWrite(ENMul[k],LOW);
    }

    // Communication dgn Raspi
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);

    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
}

void loop(){
//    scanEIT();
// masih ambil data per baris
    for (int i = 2; i < 15; i++) { // i = kutub positif tegangan
        delay(2000);
        int j=i+1;                 // j = kutub negatif tegangan
        multiplex(0, i);           // multiplexer 1 handle kutub positif tegangan
        multiplex(1, j);           // multiplexer 2 handle kutub negatif tegangan
        volt = getVoltage();
        //Serial.print(j);Serial.print(" ");Serial.println(i);
        Serial.println(volt,5);
    }
    Serial.println("end data\n");
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

// -------------------------------------- EIT SECTION ---------------------------------------------

// get data EIT (matrix 16x13), metode tetangga
void scanEIT(){
    // init elektroda pertama
    int aPos = 1; // kutub positif arus injeksi
    int aNeg = 0; // kutub negatif arus injeksi
    int vPos = 2; // kutub positif ukur tegangan
    int vNeg = 3; // kutub negatif ukur tegangan
    
    for(aNeg, aPos; aNeg<16; aNeg++, aPos++){ // 16 baris 
        if(aPos==16) aPos=0;
        Serial.print("Baris(Arus): ");Serial.print(aPos);Serial.print(" ");Serial.println(aNeg);
        
        multiplex(2, aPos); // multiplexer 2 handle kutub positif arus injeksi
        multiplex(3, aNeg); // multiplexer 3 handle kutub negatif arus injeksi
        
        for (int i = 0; i < 13; i++) { // get 13 data voltage (13 kolom matrix)
            if(vPos==16) vPos=0;
            if(vNeg==16) vNeg=0;
//            Serial.print(vPos);Serial.print(" ");Serial.println(vNeg);
            delay(3000);

            multiplex(0, vPos); // multiplexer 0 handle kutub positif tegangan
            multiplex(1, vNeg); // multiplexer 1 handle kutub negatif tegangan
            
            volt = getVoltage();
            delay(3000);

            Serial.println(volt,5);
            
            vPos++;
            vNeg++;
        }
        vPos=aPos+1;
        vNeg=vPos+1;
        Serial.println("\n");
    }
}

// get voltage measurement
float getVoltage(){
    analogReference(INTERNAL1V1); // voltage reference 1.1 volt buat MEGA
    int value = analogRead(A0);
    float voltage = (value*1.1)/1023.0;
    return (voltage);
}

// control multiplexer, change decimal to biner
void multiplex(int tipe, int chanel){
    int bits;
    for(int i=0;i<4;i++){
        bits = chanel%2;
        chanel =(int) chanel/2;

        digitalWrite(mul[tipe][i], bits);
    }
    delay(10);
}

/*
 * Agung Dwi Prasetyo
 * G64130073
 * cs.ipb.ac.id
 */
