#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const char* ssid="TX2RX";           //Wifi SSID and Password to connect
const char* password="34567890";

#define pulsein 12  //D6
#define led 14      //D5
const uint8_t controlLed[4]={05,04,00};  //D1 D2 D3
String  ledStat[4] = {"OFF","OFF","OFF"};
String theme="bootstrap";

double cost =0;
double unit=0;
float unitPrice=10;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);
  
  EEPROM.begin(512);
  EEPROM.get(0,unit);
  
  delay(10);
  Serial.println('\n');
  pinMode(pulsein,INPUT);
  pinMode(led,OUTPUT);
  digitalWrite(pulsein,HIGH);
  
  for(int x=0;x<4;x++){
    pinMode(controlLed[x],OUTPUT);
  }
  
  WiFi.begin(ssid,password);
  Serial.println("Connecting to");
  Serial.println(ssid);
  Serial.print("...");

  int i=0;
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
  
}

void loop() {
  server.handleClient();
  if(!(digitalRead(pulsein))){
    digitalWrite(led,HIGH);
    unit += 1/32;     //should be 3200
    Serial.print("Unit: \t");
    Serial.println(unit);
    
    while(!(digitalRead(pulsein))){
      digitalWrite(led,LOW);
    }
    cost=unit*unitPrice;

    EEPROM.put(0,unit);
    EEPROM.commit();
  }
}

String getPage(){
  String page = "<html charset=UTF-8><head><meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1'/>";
  page += "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  if ( theme == "bootstrap" ) {
    page += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>";
    } 
  else {
    page += "<link href='https://maxcdn.bootstrapcdn.com/bootswatch/3.3.7/";
    page += theme;
    page += "/bootstrap.min.css' rel='stylesheet'>";
  }
  page += "<title>EE 3200 Project</title></head><body>";
  page += "<div class='container-fluid'>";
  page +=   "<div class='row'>";
  page +=     "<div class='col-md-12'>";
  page +=       "<h1><center><b>Project On EE 3200</b></h1>";
  page +=       "<h1><center><b>Project Title: IoT Smart Energy Management</b></h1>";
  page +=        "<br>";
  page +=       "<h3><center><b>Energy Meter Data</b></h3>";
  page +=        "<br>";
  page +=        "<center> <ul class='nav nav-pills nav-stacked' style = 'max-width: 360px; '>";
  page +=         "<li class='active'>";
  page +=           "<a href='#'> <span class='badge pull-right'>";
  page +=           unit;
  page +=           " KWH";
  page +=           "</span><b> Consumed Unit Electricity </b> </a>";
  page +=         "</li><li>";
  page +=       "</ul>";
  page +=        " <ul class='nav nav-pills nav-stacked' style = 'max-width: 360px; '>";
  page +=         "<li class='active'>";
  page +=           "<a href='#'> <span class='badge pull-right'>";
  page +=           cost;
  page +=           " BDT";
  page +=           "</span><b> Cost in this month</b> </a>";
  page +=         "</li><li>";
  page +=       "</ul>";
  page +=        "<br>";
  page +=        "<br>";
  page +=       "<h3><center><b>Control LEDs<b></h3>";
  page +=        "<br>";
  page +=       "<div class='row'>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>GREEN ";
  page +=           "<span class='badge'>";
  page +=           ledStat[0];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='GREEN' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='GREEN' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>YELLOW ";
  page +=           "<span class='badge'>";
  page +=           ledStat[1];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='YELLOW' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='YELLOW' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>RED ";
  page +=           "<span class='badge'>";
  page +=           ledStat[2];
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='RED' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='RED' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=       "</div>";
  page +=        "<br>";
  page +=        "<br>";
  page +=       "<h2><b>Project Submitted To:</b></h2>";
  page +=       "<h3>Md. Rezaur Raihan</h3>";
  page +=       "<h4>Assistant Professor,</h4>";
  page +=       "<h4>Dept. of EEE, KUET.</h4>";
  page +=       "<h2><b>Project Submitted By:</b></h2>";
  page +=       "<h3>Md. Rakibul Hasan</h3>";
  page +=       "<h4>Roll: 1403015</h4>";
  page +=        "<br>";
  page +=   "<div class='row'>";
  page +=     "<div class='col-md-4'>";
  page +=       "<form method='POST' name='selecttheme' id='selecttheme'/>"; 
  page +=       "<input class='span' id='choosetheme' name='theme' type='hidden'>";
  page +=       "<div class='btn-group'>";
  page +=         "<button class='btn btn-default'>Choose a theme</button>";
  page +=         "<button data-toggle='dropdown' class='btn btn-default dropdown-toggle'><span class='caret'></span></button>";
  page +=         "<ul class='dropdown-menu'>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"bootstrap\"); $(\"#selecttheme\").submit()'><a href='#'>Boostrap</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"cerulean\"); $(\"#selecttheme\").submit()'><a href='#'>Cerulean</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"cosmo\"); $(\"#selecttheme\").submit()'><a href='#'>Cosmo</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"cyborg\"); $(\"#selecttheme\").submit()'><a href='#'>Cyborg</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"darkly\"); $(\"#selecttheme\").submit()'><a href='#'>Darkly</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"flatly\"); $(\"#selecttheme\").submit()'><a href='#'>Flatly</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"journal\"); $(\"#selecttheme\").submit()'><a href='#'>Journal</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"lumen\"); $(\"#selecttheme\").submit()'><a href='#'>Lumen</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"paper\"); $(\"#selecttheme\").submit()'><a href='#'>Paper</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"readable\"); $(\"#selecttheme\").submit()'><a href='#'>Readable</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"sandstone\"); $(\"#selecttheme\").submit()'><a href='#'>Sandstone</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"simplex\"); $(\"#selecttheme\").submit()'><a href='#'>Simplex</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"slate\"); $(\"#selecttheme\").submit()'><a href='#'>Slate</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"spacelab\"); $(\"#selecttheme\").submit()'><a href='#'>Spacelab</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"superhero\"); $(\"#selecttheme\").submit()'><a href='#'>Superhero</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"united\"); $(\"#selecttheme\").submit()'><a href='#'>United</a></li>";
  page +=           "<li onclick='$(\"#choosetheme\").val(\"yeti\"); $(\"#selecttheme\").submit()'><a href='#'>Yeti</a></li>";
  page +=         "</ul>";
  page +=       "</div>";
  page +=       "</form></div>";
  page +=       "<div class='col-md-8'>";
  page +=         "<p><a href='https://github.com/rakibul-eeekuet/IoT-Smart-Energy-Management'>Click to See full project at <b>GitHub</b></p>";
  page +=       "</div>";
  page +=   "</div>"; 
  page += "</div></div></div>";
  page += "</body></html>";
  return page;
}

void handleRoot(){ 
  if ( server.hasArg("theme") ) {
    handleTheme();
  } else if ( server.hasArg("GREEN") ) {
    handleGREEN();
  } else if ( server.hasArg("YELLOW") ) {
    handleYELLOW();
  } else if ( server.hasArg("RED") ) {
    handleRED();
  } else {
    server.send ( 200, "text/html", getPage() );
  }  
}

void handleTheme(){
  theme = server.arg("theme");
  Serial.println("Update theme : "); 
  Serial.print(theme);  
  server.send ( 200, "text/html", getPage() );
}
 
void handleGREEN() {
  String D1Value; 
  updateGPIO(0,server.arg("GREEN")); 
}
 
void handleYELLOW() {
  String D2Value; 
  updateGPIO(1,server.arg("YELLOW")); 
}
 
void handleRED() {
  String D3Value; 
  updateGPIO(2,server.arg("RED")); 
}
 
void updateGPIO(int gpio, String DxValue) {
  Serial.println("");
  Serial.println("Update GPIO "); 
  Serial.print(controlLed[gpio]); 
  Serial.print(" -> "); 
  Serial.println(DxValue);
  
  if ( DxValue == "1" ) {
    digitalWrite(controlLed[gpio], HIGH);
    ledStat[gpio] = "On";
    server.send ( 200, "text/html", getPage() );
  } else if ( DxValue == "0" ) {
    digitalWrite(controlLed[gpio], LOW);
    ledStat[gpio] = "Off";
    server.send ( 200, "text/html", getPage() );
  } else {
    Serial.println("Err LED Value");
  }  
}
