/*
 * I2C yaitu media untuk komunikasi antara arduino pada pin A4 dan A5, dengan raspberry pi pada GPIO 02(pin 03) dan GPIO 03(pin 05)
 */

#include <Wire.h>
 
#define SLAVE_ADDRESS 0x04

int dataDiterima = 0;
int state = 0;
int dataTerkirim = 0;
int sensor = 0;
 
double temp;
int sens;
int kirimData;
 
void setup() {
 pinMode(13, OUTPUT);
 Serial.begin(9600);
 
 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);
 
 // define callbacks for i2c communication
 Wire.onReceive(receiveData);
 Wire.onRequest(sendData);
}
 
void loop(){
 delay(100);
 temp = GetTemp(); // get suhu internal arduino
}
 
// menerima data dari raspberry pi
void receiveData(int byteCount){
 while(Wire.available()) {
  dataDiterima = Wire.read();
  Serial.print("Data yang diterima dari raspi = ");Serial.print(dataDiterima);
 
  if (dataDiterima == 1){
    kirimData = 255;
    Serial.print("\t Data yang dikirim ke raspi = ");Serial.println(kirimData);
    if (state == 0){
      digitalWrite(13, HIGH);
      state = 1;
    } else{
      digitalWrite(13, LOW);
      state = 0;
    }
  }
  else if(dataDiterima == 2) {
   kirimData = GetSensorLDR();
   Serial.print("\t Data yang dikirim ke raspi = ");Serial.println(kirimData);
   if(kirimData < 900){
      digitalWrite(13,HIGH);
    }else{
      digitalWrite(13,LOW);
    }
  }
  else if(dataDiterima == 3){
    kirimData = dataDiterima*1000;
    Serial.print("\t Data yang dikirim ke raspi = ");Serial.println(kirimData);
  }
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

int GetSensorLDR(){
  sensor = analogRead(A0);
  return (sensor);
}
void nomor3(){
  dataDiterima = dataDiterima + 9;
  for(int i=0;i<dataDiterima;i++){
    digitalWrite(13,HIGH);
    delay(200);
    digitalWrite(13,LOW);
    delay(200);
  }
}
/*
 * Agung Dwi Prasetyo
 * G64130073
 */

