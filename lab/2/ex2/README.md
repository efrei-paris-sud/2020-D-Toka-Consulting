

# Exercise 2 
Where are the I2C pins (SDA and SCL) in our devices such as micro controller, sensors, actuators.

We reffered to the arduino schema that we had in the lab 1. 

## I2C Protocol
With **Inter-integrated Circuit(I2C)**, you can connect multiple slaves to a single master and you can have multiple masters controlling single, or multiple slaves.
Like UART communication, I2C only uses two wires to transmit data between devices. Those two wires are : 

**Serial Data Line(SDA)** - The line for the master and slave to send and receive data.

**Serial Clock Line(SCL)** - The line that carries the clock signal.

## Schematic 
![Arduino_schema](Capture_arduino_lab2.PNG?raw=true)

The I2C pins (SDA and SCL) are the analogs A4 and A5(see at the bottom left of the schema).


