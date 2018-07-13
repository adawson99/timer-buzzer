#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "microgreen-timer.firebaseio.com"
#define FIREBASE_AUTH "YOUR KEY"
#define WIFI_SSID   "WIFI" 
#define WIFI_PASSWORD "PASSWORD"
#define buzzerPin D3
#define BUTTON D4
#define ESP8266_SSID_PREFIX "NETWORK NAME"
ESP8266WebServer server;
//password for direct ESP8266 login must be at least 8 characters
const char WiFiAPPSK[] = "PASSWORD";

const char INDEX_HTML[] =
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>ESP8266</title>"
  "<style>"
    "body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: Maroon; }"
  "</style>"
  "</head>"
  "<body>"
  "<h1>ESP8266 Direct Access Demo</h1>"
  "<button onclick='toggleLED()'>Toggle LED</button>"
  "<button onclick='buzzBuzzer()'>Buzz Buzzer</button>"
  "<script>"
    "function toggleLED(){"
      "fetch('/toggleLED').then(stream=>stream.text()).then(text=>console.log(text))"
    "}"
    "function buzzBuzzer(){"
      "fetch('/buzzBuzzer').then(stream=>stream.text()).then(text=>console.log(text))"
    "}"
  "</script>"
  "</body>"
  "</html>";

void setup() 
{
  Serial.begin(115200);
  setupWifi();

  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN,LOW);
  
  server.on("/",sendIndex);
  server.on("/buzzBuzzer", buzzBuzzer);
  server.begin();
  Serial.println("");
  Serial.println("");
  Serial.print("Server running on http://192.168.4.1/");
  Serial.println("");

  pinMode(buzzerPin, OUTPUT);
  pinMode(BUTTON, INPUT);  
  //define the type of data a pin is responsible for

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setBool("share/buzz", false);
  
  if(Firebase.failed()){
    Serial.print("Firebase connection failed!");
    Serial.println("");
    Serial.print(Firebase.error());
  } else {
    Serial.print("Firebase connection success!");
  }
  Serial.println("");
}

int timePassed (int time) {
  int diff = 0;
  if (millis() <= time) {
    diff = (69666 - time) + millis();
  } else {
    diff = millis() - time;
  }
  return diff;
}

int checkFirebaseTime = 0;
bool buzz = false;
bool buttonState = false;
bool timeIsUp = false;

void loop()
{
  if (timePassed (checkFirebaseTime) >= 1000) {
    setButtonState();
    Serial.println(buttonState);
    buzz = Firebase.getBool("share/buzz");
    timeIsUp = Firebase.getBool("share/timeIsUp");
    if(buzz && timeIsUp) {
      //so need && statement here
      buzzBuzzer();
    }
    checkFirebaseTime = millis();
  }
  server.handleClient();
}

void sendIndex(){
  server.send(200,"text/html",INDEX_HTML);  
}

void buzzBuzzer(){
  //server.send(204,"");
  //Firebase.setBool("share/buzz", false); //doesn't set back to false when buzz
  if(Firebase.failed()){
    Serial.println("Firebase buzz set false failed!");    
    Serial.println(Firebase.error());
  } 
  server.send(200,"text/plain","Buzz Buzzer!\n");
  //tone( pin number, frequency in hertz, duration in milliseconds);
  tone(buzzerPin,1300,500);
  delay(500);
  tone(buzzerPin,0);
}

//sets the state of the button in firebase so that PLC can determine
// whether or not to buzz, if button isn't pressed, buzz = true
void setButtonState(){
  delay(10);
  buttonState = digitalRead(BUTTON);
  Firebase.setBool("share/buttonState",!buttonState);
  if(timeIsUp) {
    if(buttonState){
     Firebase.setBool("share/buzz",true);
  }
  else {
    Firebase.setInt("share/startTime", 0);
    Firebase.setBool("share/timeIsUp", false);
    Firebase.setBool("share/buzz", false);
  }
  }//if you press the button when the time is up, the time resets to false in firebase 
}

void setupWifi()
{
  Serial.println("SetupWifi()");
  WiFi.mode(WIFI_STA);
  //Set up Access Point
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.println();
  Serial.print("Local IP: ");
  Serial.println();
  Serial.println(WiFi.localIP());
}









  
