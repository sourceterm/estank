///

#include <SPI.h>
#include <Ethernet.h>

///

// PIN 9 is led on ethernet arduino
int led = 9;


int pinI1=8;//define I1 interface
int pinI2=11;//define I2 interface 
int speedpinA=9;//enable motor A
int pinI3=12;//define I3 interface 
int pinI4=13;//define I4 interface 
int speedpinB=10;//enable motor B
int spead =127;//define the spead of motor

byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
  
EthernetServer server(80);

///

const int MODE_NO_CHANGE = -1;
const int MODE_DEFAULT = 0;
const int MODE_STOP = 0;
const int MODE_FORWARD = 1;
const int MODE_BACKWARD = 2;
const int MODE_LEFT = 3;
const int MODE_RIGHT = 4;

int curMode = MODE_DEFAULT;
int lastRequestedMode = MODE_DEFAULT;



// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long MODE_UPDATE_INTERVAL = 1000;           // interval at which to blink (milliseconds)

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
}

void setup_serial() {
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
   }
}

void setup_server() {
    // start the Ethernet connection and the server:
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
  
// the setup routine runs once when you press reset:
void setup() {
  Serial.println("hello world");
  setup_serial();
  setup_leds();
  setup_server();
  //setup_motors();
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

char page_header[] = ""
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta charset=utf-8>"
  "<title>shake it</title>"
  "</head>"
  "<body>";

char page_footer[] = ""
  "</body>"
  "</html>";
  
char nbsp[] = "&nbsp;";
String circle_html = "&#9679;";
String up_html = "&#8593;";
String down_html = "&#8595;";
String left_html = "&#8592;";


String right_html = "&#8594;";

String emp_color = "red";
String reg_color = "black";

String link_open_part = "<a href='";
String link_close = "</a>";


void serve_page(EthernetClient& client, int mode) {
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  
  client.println(page_header);
  
  client.println("<table>");
  
  //String linkOpen = link_open_part; // + "http://192.168.0.3/"; // Ethernet.localIP() //HACK!
  
  String linkOpenUp = link_open_part + "1" + "'>";
  String linkOpenDown = link_open_part + "2" + "'>";
  //String linkOpenLeft = link_open_part + "3" + "'>";
  //String linkOpenRight = link_open_part + "4" + "'>";
  //String linkOpenStop = link_open_part + "0" + "'>";
  
  String upAndEmphasis = "<font color=" + (mode == MODE_FORWARD? emp_color : reg_color) + ">" + linkOpenUp + up_html + link_close + "</font>";
  //String leftAndEmphasis =  "<font color=" + (mode == MODE_LEFT? emp_color : reg_color) + ">" + linkOpenLeft + left_html + link_close + "</font>";
  //String rightAndEmphasis =  "<font color=" + (mode == MODE_RIGHT? emp_color : reg_color) + ">" + linkOpenRight + right_html + link_close + "</font>";
  String downAndEmphasis =  "<font color=" + (mode == MODE_BACKWARD? emp_color : reg_color) + ">" + linkOpenDown + down_html + link_close + "</font>";
  //String stopAndEmphasis =  "<font color=" + (mode == MODE_STOP? emp_color : reg_color) + ">" + linkOpenStop + circle_html + link_close + "</font>";
  
  String row0 = "<tr><td>&nbsp;</td><td>" + upAndEmphasis + "</td></tr>";
  //String row1 = "<tr><td>" + leftAndEmphasis + "</td><td>" + stopAndEmphasis + "</td><td>" + rightAndEmphasis + "</td></tr>";
  String row2 = "<tr><td>&nbsp;</td><td>" + downAndEmphasis + "</td></tr>"; // + downAndEmphasis
  
  client.println(row0);
  //client.println(row1);
  client.println(row2);
  
  client.println("</table>");
  
  client.println(page_footer);
  
}

// returns desired new mode, or -1 if no new mode requested
int update_server() {
  int requestedMode = -1;
  
    // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        boolean incoming = 0;
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        
        if (c == '\n' && currentLineIsBlank) {
          serve_page(client, curMode);
          break;
        } 
        
        //reads URL string from $ to first blank space
        if(incoming && c == ' '){ 
          incoming = 0;
        }
        if(c == '$'){ 
          incoming = 1; 
        }
        
        //Checks for the URL string $1 or $2
        if(incoming == 1){
          
          if(c == '0'){
            requestedMode = 0;
            Serial.println("REQUESTED 0");
          }
          
          if(c == '1'){
            requestedMode = 1;
            Serial.println("REQUESTED 1");
          }
          
          if(c == '2'){
            requestedMode = 2;
            Serial.println("REQUESTED 2");
          }
          
          if(c == '3'){
            requestedMode = 3;
            Serial.println("REQUESTED 3");
          }
          
          if(c == '4'){
            requestedMode = 4;
            Serial.println("REQUESTED 4");
          }
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
  
  return requestedMode;
}

void blink_me() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}


void loop() {

  
  int newMode = curMode;
  
  lastRequestedMode = update_server();
  
  
  unsigned long currentMillis = millis();

  if ( (currentMillis - previousMillis >= MODE_UPDATE_INTERVAL) &&
      lastRequestedMode != MODE_NO_CHANGE &&
      curMode != lastRequestedMode &&
      0 != 0)
  {
    
    curMode = lastRequestedMode;
    
    switch (curMode) {
      case 0:
      stop();
      break;
      
      case 1:
      forward();
      break;
      
      case 2:
      backward();
      break;
      
      case 3:
      left();
      break;
      
      case 4:
      right();
      break;
    }
  }
  
  
}



///
// credits, etc

// portions derived from:

/*
  Web Server Demo
  thrown together by Randy Sarafan
 
 Allows you to turn on and off an LED by entering different urls.
 
 To turn it on:
 http://your-IP-address/$1
 
 To turn it off:
 http://your-IP-address/$2
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Connect an LED to pin D2 and put it in series with a 220 ohm resistor to ground
 
 Based almost entirely upon Web Server by Tom Igoe and David Mellis
 
 Edit history: 
 created 18 Dec 2009
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe
 
 */
