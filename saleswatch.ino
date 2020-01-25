#include <ArduinoJson.h>
#include <ESP8266WiFi.h> 
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

//Program will not run without following library
//https://github.com/adafruit/Adafruit-GFX-Library.git
//https://github.com/markruys/arduino-Max72xxPanel.git
//https://github.com/bblanchon/ArduinoJson.git


const char* ssid = "";//ssid of wifi
const char* password = "";//password of wifi


String data;

//Matrix

//LED Matrix          ESP8266
//Vcc                 3V (3V on NodeMCU 3V3 on WEMOS)
//GND                 GND (G on NodeMCU)
//DIN                 D7 (Same Pin for WEMOS)
//CS                  D4 (Same Pin for WEMOS)
//CLK                 D5 (Same Pin for WEMOS)


int pinCS = D4; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
//int numberOfHorizontalDisplays = 4;
//int numberOfVerticalDisplays   = 1;
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays   = 4;
char time_value[20];



int max_order=0;
int min_order=999;


Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
int wait   = 75; // In milliseconds between scroll movements
int spacer = 1;
int width  = 5 + spacer; // The font width is 5 pixels
String SITE_WIDTH =  "1000";
String message;

int check=0;

void display_message(String message) {
  for ( int i = 0 ; i < width * message.length() + matrix.width() - spacer; i++ ) {
    //matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < message.length() ) {
        matrix.drawChar(x, y, message[letter], HIGH, LOW, 1); // HIGH LOW means foreground ON, background OFF, reverse these to invert the display!
      }
      letter--;
      x -= width;
    }
    matrix.write(); // Send bitmap to display
    delay(wait / 2);
  }
}

 
void setup () {

  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
   // Serial.println("Connecting..");
   
    data=" ";
    
  }

  matrix.setIntensity(10);    // Use a value between 0 and 15 for brightness
//  matrix.setRotation(0, 1);  // The first display is position upside down
//  matrix.setRotation(1, 1);  // The first display is position upside down
//  matrix.setRotation(2, 1);  // The first display is position upside down
//  matrix.setRotation(3, 1);  // The first display is position upside down


  matrix.setRotation(1);  // The first display is position upside down
  wait    = 100;
  message = "IOTWEBPLANET.COM";
  display_message(message); // Display the message
  display_message(String(1.8e+9));
  
  
  
}
 
void loop() {
 StaticJsonDocument<40> doc;
 StaticJsonDocument<200> sales_data;
  wait    = 100;
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    
 
  HTTPClient http;
  Serial.println("Sending data");
  http.begin("http://192.168.1.102/saleswatch/report.php"); //url of your webservice
  http.addHeader("Content-Type", "application/json");
  
  
if(check%2==0)
  doc["find"] = "today";
else  
  doc["find"] = "month";

check++;

  //covert to json
  serializeJson(doc,data);
  
  
  String cst=doc["find"];
  
  
  
  display_message("Fetching Report by :"+cst); // Display the message
  
  int httpCode = http.POST(data);  //http code
  
  String payload = http.getString(); //output

  Serial.println();  
  Serial.print(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
    
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(sales_data, payload);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    display_message("Error :"+String(error.c_str())); // Display the message
    return;
  }
  else{
     float net_sales = sales_data["net_sales"];
     int total_orders = sales_data["total_orders"];
     String message = sales_data["message"];

     Serial.println();  
     Serial.print("net_sales :"); 
     Serial.println(net_sales); 
     
    
     display_message("Sales: "+String(net_sales)); // Display the message
     
     Serial.print("total_orders :"); 
     Serial.println(total_orders); 
     
     
     display_message("Orders :"+String(total_orders)); // Display the message
    
      
     Serial.print("message :"); 
     Serial.println(message); 
     
     display_message(message); // Display the message
     wait=80;
      
   }
  
  Serial.print("Data is...");
  Serial.println(data);
  http.end();   //Close connection
  data=" ";
  }
 
  delay(20000);    //Send a request every 20 seconds
 
  Serial.print("Refresing...");
  display_message("Refresing..."+String(check)); // Display the message
  delay(1000); 
 
}