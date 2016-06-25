///

// This FILE is for the client that drives the motors
// Hardware: Arduino Uno + Seeed Motor Shield

///

#include "tank_shared.h"
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>

//

bool driveMotors = true;

///

// PIN 9 is led on ethernet arduino
int led = 9;


int pinI1=8;		//define I1 interface
int pinI2=11;		//define I2 interface 
int speedpinA=9;	//enable motor A
int pinI3=12;		//define I3 interface 
int pinI4=13;		//define I4 interface 
int speedpinB=10;	//enable motor B
int spead =127;		//define the spead of motor

///

int curMode = MODE_DEFAULT;
int lastRequestedMode = MODE_DEFAULT;


// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long MODE_UPDATE_INTERVAL = 1000;           // interval at which to blink (milliseconds)


byte last_received_byte = MODE_DEFAULT;

///

void setup_leds() {
    pinMode(led, OUTPUT);
}

void setup_motors() {
  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(speedpinA,OUTPUT);
  pinMode(pinI3,OUTPUT);
  pinMode(pinI4,OUTPUT);
  pinMode(speedpinB,OUTPUT);

  Serial.println("Setup: Motors are ready");
}

void setup_serial() {
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
   }
}

void wire_receive_client(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.println("wire_receive_byte: ignoring char ");
    Serial.print(c);         // print the character
  }

  Serial.println("wire_receive_byte: new mode received ");
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer

  last_received_byte = x;
}

void setup_wire_client() {
  Wire.begin(WIRE_CLIENT_ADDR);         // join i2c bus with given address
  Wire.onReceive(wire_receive_client); // register event
}
 
// the setup routine runs once when you press reset
void setup() {
  Serial.println("client: hello world");
  setup_serial();
  //setup_leds();
  setup_wire_client();
  if (driveMotors) {
    setup_motors();
  }
}

///

void forward()
{
     analogWrite(speedpinA,spead);//input a simulation value to set the speed
     analogWrite(speedpinB,spead);
     digitalWrite(pinI4,HIGH);//turn DC Motor B move clockwise
     digitalWrite(pinI3,LOW);
     digitalWrite(pinI2,LOW);//turn DC Motor A move anticlockwise
     digitalWrite(pinI1,HIGH);
}
void backward()//
{
     analogWrite(speedpinA,spead);//input a simulation value to set the speed
     analogWrite(speedpinB,spead);
     digitalWrite(pinI4,LOW);//turn DC Motor B move anticlockwise
     digitalWrite(pinI3,HIGH);
     digitalWrite(pinI2,HIGH);//turn DC Motor A move clockwise
     digitalWrite(pinI1,LOW);
}
void left()//
{
     analogWrite(speedpinA,spead);//input a simulation value to set the speed
     analogWrite(speedpinB,spead);
     digitalWrite(pinI4,HIGH);//turn DC Motor B move clockwise
     digitalWrite(pinI3,LOW);
     digitalWrite(pinI2,HIGH);//turn DC Motor A move clockwise
     digitalWrite(pinI1,LOW);
}
void right()//
{
     analogWrite(speedpinA,spead);//input a simulation value to set the speed
     analogWrite(speedpinB,spead);
     digitalWrite(pinI4,LOW);//turn DC Motor B move anticlockwise
     digitalWrite(pinI3,HIGH);
     digitalWrite(pinI2,LOW);//turn DC Motor A move clockwise
     digitalWrite(pinI1,HIGH);
}
void stop()//
{
     digitalWrite(speedpinA,LOW);// Unenble the pin, to stop the motor. this should be done to avid damaging the motor. 
     digitalWrite(speedpinB,LOW);
     delay(1000);
 
}

///
// LED 

void blink_me() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}

///

void loop_client() {

  int newMode = curMode;

  lastRequestedMode = last_received_byte;
 
  unsigned long currentMillis = millis();


//  if (currentMillis - previousMillis >= MODE_UPDATE_INTERVAL) 
 //   Serial.println("check one:");

 // if (lastRequestedMode != MODE_NO_CHANGE)
 //   Serial.println("check two");

  if (curMode != lastRequestedMode)
	Serial.println("check three");

  //if (driveMotors)
//	Serial.println("check four");

  if ( (currentMillis - previousMillis >= MODE_UPDATE_INTERVAL) &&
      lastRequestedMode != MODE_NO_CHANGE &&
//      curMode != lastRequestedMode &&
      driveMotors)
  {
   
    curMode = lastRequestedMode;

    {
      Serial.println("CLIENT: CHANGING MODE: ");
      Serial.println(curMode);
    }
   
    switch (curMode) {
      case MODE_STOP:
      stop();
      break;

      case MODE_FORWARD:
	Serial.println("FORWARD");
      forward();
      break;

      case MODE_BACKWARD:
      backward();
      break;

      case MODE_LEFT:
      left();
      break;

      case MODE_RIGHT:
      right();
      break;
    }

  delay(2000);
  stop();
  }
}


void loop() {
  loop_client();
}

