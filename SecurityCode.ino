Skip to content
 
Search or jump to…

Pull requests
Issues
Marketplace
Explore
 
@agabriel1 
1
0 0 ixAndy1/Security_System
 Code  Issues 0  Pull requests 0  Projects 0  Wiki  Security  Insights
Security_System/Security_System.ino
@ixAndy1 ixAndy1 Updated Security_System.ino
7d72671 on Sep 20, 2018
202 lines (159 sloc)  7.11 KB
    
//This project is for an Arduino-based home security system.
//Items needed include an Arduino and a corresponding Ethernet shield, PIR motion sensors, and wires.
//The Arduino will output an IP address in the Serial Monitor.
//Typing in this IP address in a browser window will lead to the Arduino Home Security Website, where
//information about the current state of the "home" will be shown/updated while the program runs.


#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 137, 196);
EthernetServer server(80);

int kitchenPin = 7; 
int bedRoomPin = 9;    //Input pins for the PIR Motion Sensors in arduino pins 7, 9, and 11
int livingRoomPin = 11; 

int pirKitchen = LOW; 
int pirBedRoom = LOW;      //State of all sensors start LOW (no motion detected) 
int pirLivingRoom = LOW; 

int statusKitchen = 0;
int statusBedRoom = 0;       //Variables to compare to the state of the motion sensors
int statusLivingRoom = 0;

String defaultTitle = "<h1>Arduino Home Security System</h1>";
String kitchenMotion = "<h1>Motion Detected in the Kitchen</h1>";
String bedRoomMotion = "<h1>Motion Detected In The Bed Room!</h1>";
String livingRoomMotion = "<h1>Motion Detected In The Living Room!</h1>";
String redBackgroundColor = "background-color: red";
String blueBackgroundColor = "background-color: blue";


void createWebpage(String x, String y) //HTML webpage function
{
      EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
              client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 2");  // refresh the page automatically every 2 sec
          client.println(); 

client.println("<! DOCTYPE html>");
client.println("<html>");
    client.println("<head>");
    
        client.println("<title>Arduino Home Security</title>");
        client.println("<style type='text/css'>");
        
            client.println("*");
            client.println("{");
            client.println("font-family: arial, verdana, sans-serif;"); // Specifies font
            client.println(y);
            client.println("}");
            
        client.println("header img ");
        client.println("{");
            client.println("background-size: cover;");
           client.println(" background-position:center center;"); // Positions image
        client.println("} ");
        
       client.println("h1 ");
       client.println("{ ");
            client.println("font-size: 40pt;");
            client.println("color: black;");
           client.println(" text-align: center;");
          client.println("  position: absolute; ");   //Positions Text
           client.println(" top: 400px; ");
           client.println(" left: 100px; ");
           client.println(" width: 100%;");
           client.println(" color: white;   ");
        client.println("}");

       client.println(" article h2");
        client.println("{");
           client.println(" font-style: italics;");       // Positions sub text
       client.println(" } ");
        
       client.println(" p ");
        client.println("{");
          client.println("  font-size: 14pt;");
           client.println(" color: #6B6BD7;");
        client.println("}");
        client.println("</style>");
        
    client.println("</head>");
client.println("<body>");
    client.println("<header>");

         client.println("<img id='pic' src='https://arduinohomesecuritysystem.files.wordpress.com/2016/04/security-system.jpg' alt='' />");
        client.println(x);
   client.println("</header>");
 
client.println("<section>");
    client.println(" <article>");
           client.println("<h2> ");
                client.println("  Welcome to the Arduino Home Security System project by Andrew Nguyen.");
         client.println(" </h2> ");                                                                                 
       client.println(" <p>This project uses an Arduino to monitor the environment using sensors and sends a message via text and website to the homeowner when tripped.</p>");
        
        
       client.println("</article>");
 client.println("</section>");
        
client.println("    </body>");
client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
} //End of HTML

void setup()
{
    pinMode(kitchenPin, INPUT);
    pinMode(bedRoomPin, INPUT);    //Declares three motion sensors as input
    pinMode(livingRoomPin, INPUT);
   
    Serial.begin(9600);
    Serial.println("It's quiet in the house. Too quiet...");

    Ethernet.begin(mac, ip);
    server.begin();
    Serial.print("Server is at ");
    
    Serial.println(Ethernet.localIP() );   //IP Address shown in the Serial Monitor
    createWebpage(defaultTitle, blueBackgroundColor); 
}
    
    
void loop()
{
      statusKitchen = digitalRead(kitchenPin);  // Reads input value (7)
      statusBedRoom = digitalRead(bedRoomPin);  // Reads input value (9)
      statusLivingRoom = digitalRead(livingRoomPin);  // Reads input value (11)
 
 
  if (statusKitchen == HIGH) //Checks if kitchen input is HIGH
  {          
      Serial.println("A wild human has appeared in the Kitchen!");

      createWebpage(kitchenMotion, redBackgroundColor); // Creates Kitchen Webpage
    
      delay(1000);  
        Serial.println("It's quiet in Kitchen. Too quiet...");
        createWebpage(defaultTitle, blueBackgroundColor); 
   }

 
  if (statusBedRoom == HIGH) //Checks if bedroom input is HIGH
  {           
      Serial.println("A wild human has appeared in the Bedroom!"); 
      
      createWebpage(bedRoomMotion, redBackgroundColor); // Creates Bed Room Webpage

      delay(1000);   
        Serial.println("It's quiet in the Bedroom. Too quiet...");
        createWebpage(defaultTitle, blueBackgroundColor); 
   }
 
 
  if (statusLivingRoom == HIGH) //Checks if living room input is HIGH
  {           
      Serial.println("A wild human has appeared in the Living Room!");

      createWebpage(livingRoomMotion, redBackgroundColor); // Creates Living Room Webpage

      delay(1000);   
    
        Serial.println("It's quiet in the Living Room. Too quiet...");
     
        createWebpage(defaultTitle, blueBackgroundColor);
   }
}
© 2019 GitHub, Inc.
Terms
Privacy
Security
Status
Help
Contact GitHub
Pricing
API
Training
Blog
About
