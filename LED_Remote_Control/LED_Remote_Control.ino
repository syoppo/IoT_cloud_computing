#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "led-remote-control-f8fc5.firebaseio.com"
#define FIREBASE_AUTH "PcEOcDzXy1szlv6rxTsnGYGFi7YpbQMd83vnYjyc"
#define WIFI_SSID "abcdefg"
#define WIFI_PASSWORD "69719602"

String led_fireStatus = "";

int led = D4;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  pinMode(led, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("LED_STATUS", "OFF");
}

void loop() {
  // put your main code here, to run repeatedly:
  led_fireStatus = Firebase.getString("LED_STATUS");  //LED_STATUS의 값을 가져온다.
  if(led_fireStatus == "on"){ //위에서 가져온 값이 on이면 아래의 코드 실행
    Serial.println("LED Turned ON");
    digitalWrite(led, HIGH);
  } else if(led_fireStatus == "off"){ //off이면 아래의 코드 실행
    Serial.println("lED Turned OFF");
    digitalWrite(led, LOW);
  } else{
    Serial.println("Command Error! Please send ON/OFF");
  }
  delay(2000);
}
