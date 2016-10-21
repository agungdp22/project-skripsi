int s0 = 0;
int s1 = 0;
int s2 = 0;
int s3 = 0;
int count = 0;
 
void setup(){
  pinMode(2, OUTPUT); // hubungin pin s0 pada multiplex ke pin nomor 2 pada arduino
  pinMode(3, OUTPUT); // hubungin pin s1 pada multiplex ke pin nomor 3 pada arduino
  pinMode(4, OUTPUT); // hubungin pin s2 pada multiplex ke pin nomor 4 pada arduino
  pinMode(5, OUTPUT); // hubungin pin s3 pada multiplex ke pin nomor 5 pada arduino
}

void loop () {
  for (count; count<=15; count++) {
    // select the bit  
    s0 = bitRead(count,0);
    s1 = bitRead(count,1);
    s2 = bitRead(count,2);
    s3 = bitRead(count,3);

    digitalWrite(2, s0);
    digitalWrite(3, s1);
    digitalWrite(4, s2);
    digitalWrite(5, s3);
  }  

}
