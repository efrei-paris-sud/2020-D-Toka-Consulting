

# Exercise 1 
We had to develop an arduino program which can read a byte from serial and adujst the passive buzzer frequency with that.
We also had to write a response to say that our buzzer frequency has changed.

This exercice helped us to comprehend how the UART protocol works. 

## UART Prtocol
UART stands for Universal Asynchronous Receiver/Transmitter. A UART’s main purpose is to transmit and receive serial data.
One of the best things about UART is that it only uses two wires to transmit data between devices.

## Code
 ```Arduino
 int buzzer = 5;
 
void setup(){
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
}

void loop(){
  while(Serial.available() > 0) {
  int i = Serial.parseInt();
  byte b = Serial.read();
  
  if( i != 0)
   {
    tone(buzzer, i);
    delay(1000);
    Serial.println("the code has changed");
   }
  }
}
```
  

