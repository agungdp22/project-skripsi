/*
 * I2C yaitu media untuk komunikasi antara arduino pada pin A4 dan A5, dengan raspberry pi pada GPIO 02(pin 03) dan GPIO 03(pin 05)
 */

#include <Wire.h>
 
#define SLAVE_ADDRESS 0x04

int dataDiterima = 0;
int dataTerkirim = 0;
int LED = 12;
 
double temp;
int sens;
int kirimData;
int s[4] = {2,3,4,5};
int aktifMultiplex=6;
 
void setup() {
 Serial.begin(9600);
 pinMode(LED, OUTPUT);
 for(int k=0;k<4;k++){
  pinMode(s[k], OUTPUT);
 }
 pinMode(aktifMultiplex, OUTPUT);
 digitalWrite(aktifMultiplex,LOW);
 
 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);
 
 // define callbacks for i2c communication
 Wire.onReceive(receiveData);
 Wire.onRequest(sendData);
}
 
void loop(){
  temp = GetTemp(); // get suhu internal arduino
  
  if(dataDiterima == 1){
    sens = GetSensor1(); // baca data sensor LDR
    Serial.print("Sensor 1 = ");Serial.println(sens);
    if(sens < 900){
      digitalWrite(13,HIGH);
    }else{
      digitalWrite(13,LOW);
    }
  }else if(dataDiterima == 2){
    sens = GetSensor2(); // baca data resistivitas yang sedang diukur
    Serial.print("Sensor 2 = ");Serial.println(sens);
  }else if(dataDiterima == 3){
    sens = GetSensor3(); // null
    Serial.print("Sensor 3 = ");Serial.println(sens);
  }
  
  aksi(dataDiterima);
  multiplex(dataDiterima);
  delay(1000);
}
 
// menerima data dari raspberry pi
void receiveData(int byteCount){
 while(Wire.available()) {
  dataDiterima = Wire.read();
  Serial.print("Data yang diterima dari raspi = ");Serial.print(dataDiterima);
 
  if (dataDiterima == 1){
    kirimData = GetSensor1();
  }
  else if(dataDiterima == 2) {
    kirimData = GetSensor2();
  }
  else if(dataDiterima == 3){
    kirimData = dataDiterima*1000;
  }
  
  Serial.print("\t Data yang dikirim ke raspi = ");Serial.println(kirimData);
  dataTerkirim = kirimData;
 }
}
 
// mengirim data ke raspberry pi
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

int GetSensor1(){
  int sensor1 = analogRead(A0); // pin A0 terhubung pada sensor LDR
  return (sensor1);
}
int GetSensor2(){
  int sensor2 = analogRead(A1); // pin A1 terhubung ke modul pembaca resistivitas
  return (sensor2);
}
int GetSensor3(){
  int sensor3 = analogRead(A1);
  return (sensor3);
}
void aksi(int data){
  for(int i=0;i<data;i++){
    digitalWrite(LED,HIGH);
    delay(200);
    digitalWrite(LED,LOW);
    delay(200);
  }
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

