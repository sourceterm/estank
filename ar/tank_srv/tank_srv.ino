///

// This FILE is for the server that controls I2C and Ethernet/Wifi communication
// Hardware: Ethernet Arduino

///

#include "tank_shared.h"
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>

///

// PIN 13 is led on arduino uno
int led = 13;


byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
  
EthernetServer server(80);

///

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

void setup_serial() {
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
   }
}

void setup_eth_server() {
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

void setup_wire_server() {
  Wire.begin();  // join i2c bus (address optional for master)
}

void wire_send_byte(byte toSend)
{
  Wire.beginTransmission(WIRE_CLIENT_ADDR);
  Wire.write(toSend);
  Wire.endTransmission();

}
 
// the setup routine runs once when you press reset
void setup() {
  Serial.println("server: hello world");
  setup_serial();
  setup_leds();
  setup_eth_server();
  setup_wire_server();
}

///

// TODO - hardcoded modes in this static html

char page_static[] = ""
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta charset=utf-8>"
  "<title>shake it</title>"
  "<style type='text/css'>a {text-decoration: none; }</style>"
  "</head>"
  "<body>"
  "<table>"
  "<tr><td>&nbsp;</td><td><a href='$2'>&#8593;</a></td></tr>" // up

  "<tr><td>"
	"<a href='$4'>&#8592;</a>" // left 
  "</td><td>" 
	"<a href='$0'>&#9679;</a>" // stop / circle
  "</td><td>"
	"<a href='$3'>&#8594;</a>" // right 
  "</td></tr>"
  "<tr><td>&nbsp;</td><td><a href='$1'>&#8595;</a></td></tr>" // down
  "</table>"
  "</body>"
  "</html>";
  
//char nbsp[] = "&nbsp;";
//String circle_html = "&#9679;";
//String up_html = "&#8593;";
//String down_html = "&#8595;";
//String left_html = "&#8592;";
String right_html = "&#8594;";

//String emp_color = "red";
//String reg_color = "black";

//String link_open_part = "<a href='";
//String link_close = "</a>";

/*
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
*/

void serve_page_static(EthernetClient& client)
{
    // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
 
 client.println(page_static); 
}

void serve_page_redirect(EthernetClient& client)
{
  client.println("HTTP/1.1 303 See Other");
  client.println("Location: /");
  client.println();
}

// returns desired new mode, or MODE_NO_CHANGE if no new mode requested
int update_server() {
  int requestedMode = MODE_NO_CHANGE;
  
    // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean directionRequest = false;

    while (client.connected()) {
      if (client.available()) {
        boolean incoming = 0;
        char c = client.read();
	//Serial.println(c);
        
	// if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        
        if (c == '\n' && currentLineIsBlank) {
          if (directionRequest) {
            serve_page_redirect(client);
          } else {
            serve_page_static(client);
          }
          break;
        } 
        
        //reads URL string from $ to first blank space
        if(incoming && c == ' '){
	Serial.println("incoming and SPACE");
          incoming = 0;
        }
        if(c == '$'){ 
	  Serial.println("DOLLAR SIGN");
          incoming = 1; 
          c = client.read();
        }
        
        //Checks for the URL string $1 or $2
        if(incoming == 1){
          
          if(c == '0'){
            requestedMode = 0;
            directionRequest = true;
            Serial.println("REQUESTED 0");
          }
          
          if(c == '1'){
            requestedMode = 1;
            directionRequest = true;
            Serial.println("REQUESTED 1");
          }
          
          if(c == '2'){
            requestedMode = 2;
            directionRequest = true;
            Serial.println("REQUESTED 2");
          }
          
          if(c == '3'){
            requestedMode = 3;
            directionRequest = true;
            Serial.println("REQUESTED 3");
          }
          
          if(c == '4'){
            requestedMode = 4;
            directionRequest = true;
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

  if (requestedMode != MODE_NO_CHANGE)
  { 
    Serial.println("FINAL REQUESTED MODE: ");
    Serial.println(requestedMode); 
  }

  return requestedMode;
}

void blink_me() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}


void loop_server() {
  
  int newMode = curMode;
  
  lastRequestedMode = update_server();
  
  unsigned long currentMillis = millis();

  if ( (currentMillis - previousMillis >= MODE_UPDATE_INTERVAL) &&
      lastRequestedMode != MODE_NO_CHANGE &&
      curMode != lastRequestedMode)
  {
    
    curMode = lastRequestedMode;

    { 
      Serial.println("SERVER: CHANGING MODE: ");
      Serial.println(curMode); 
    }

    wire_send_byte(curMode);
  }
}

void loop() {
  loop_server();
}

