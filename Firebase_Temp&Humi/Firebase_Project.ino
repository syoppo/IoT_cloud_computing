#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "time.h"     //시계 라이브러리

#define DHTPIN D3
#define DHTTYPE DHT11
#include <DHT.h>

const char* ntpServer = "pool.ntp.org"; //NTP서버
uint8_t timeZone = 9; //한국 타임존 설정
uint8_t summerTime = 0; //3600, 썸머타임 시간

int s_hh = 12;  //시간 설정용 시 변수 및 초기값, <0 조건 위해 자료형 int
int s_mm = 59; // 시간 설정용 분 변수 및 초기값
uint8_t s_ss = 45; // 시간 설정용 초 변수 및 초기값
uint16_t s_yy = 2020; // 시간 설정용 년 변수 및 초기값
uint8_t s_MM = 07; // 시간 설정용 월 변수 및 초기값
uint8_t s_dd = 05; // 시간 설정용 일 변수 및 초기값

time_t now; // 현재 시간 변수
time_t prevEpoch; // 이전 UTC 시간 변수
struct tm * timeinfo; // 로컬 시간 반영용 포인터 변수 선언


DHT dht(DHTPIN, DHTTYPE);

#define FIREBASE_HOST "fir-project-1bcac.firebaseio.com"
#define FIREBASE_AUTH "qpRpKoDryJFr1uRUkqgWIhO5HvqRyv1UYbL918bJ"
#define WIFI_SSID "abcdefg"
#define WIFI_PASSWORD "69719602"

void get_NTP() { // NTP 서버 시간 동기화
configTime(3600 * timeZone, 3600 * summerTime, ntpServer); // NTP 서버 접속 및 동기화 함수
timeinfo = localtime(&now); // 로컬 시간값 확인
while (timeinfo->tm_year +1900 == 1970) { // 시간 동기화 안되어 있으면, 1970년
Serial.println("Failed to obtain time"); // 메시지 표시
set_time(); // set time // 초기값으로 시간 설정
localtime(&now); // 로컬 시간 반영 재실행
    return;
}
}

void set_time() { // 사용자 시간 설정 함수
  struct tm tm_in; // 사용자 시간 설정용 구조체 변수 선언
  tm_in.tm_year = s_yy - 1900;
  tm_in.tm_mon = s_MM - 1;
  tm_in.tm_mday = s_dd;
  tm_in.tm_hour = s_hh;
  tm_in.tm_min = s_mm;
  tm_in.tm_sec = s_ss;
  time_t ts = mktime(&tm_in); // UTC 시간 값으로 변환
  printf("Setting time: %s", asctime(&tm_in)); // 설정 시간을 문자열로 출력
  struct timeval now = { .tv_sec = ts }; // 설정시간을 현재 시간에 반영
    settimeofday(&now, NULL);
}

void printLocalTime( ){
  if(time(&now) != prevEpoch){ // 현재 UTC 시간 값과 이전 UTC 시간 값이 다르면
    Serial.println(time(&now)); // 현재 UTC 시간 값 출력
    timeinfo = localtime(&now); // 로컬 변경함수이용 UTC 시간값 변경
    int dd = timeinfo->tm_mday; // 구조체 내 해당값 가져오기
    int MM = timeinfo->tm_mon + 1;
    int yy = timeinfo->tm_year +1900;
    int ss = timeinfo->tm_sec;
    int mm = timeinfo->tm_min;
    int hh = timeinfo->tm_hour;
    int week = timeinfo->tm_wday;
    Serial.print(week); Serial.print(". "); 
    Serial.print(yy); Serial.print(". "); 
    Serial.print(MM); Serial.print(". "); 
    Serial.print(dd); Serial.print(" "); 
    Serial.print(hh); Serial.print(": "); 
    Serial.print(mm); Serial.print(": "); 
    Serial.println(ss);

    prevEpoch = time(&now);// 현재 UTC 시간 값을 저장하여 1초마다 실행되도록 함.
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  dht.begin(9600);

  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  get_NTP();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temper = dht.readTemperature();
  float humi = dht.readHumidity();

  if(Serial.available() >0){
    String temp = Serial.readStringUntil('\n');
    if(temp == "1") set_time();// set time
    else if (temp =="2") get_NTP();// NTP Sync
  }
  printLocalTime();// 로컬 시간 출력

  StaticJsonBuffer<200> jsonbuffer;
  JsonObject& root = jsonbuffer.createObject();
  root["temperature"] = temper;
  root["humidity"] = humi;
  root["time"] = ctime(&now); //캐릭터 문자열로 시간값 출력해주기

  String name = Firebase.push("logDHT", root);

  Serial.println("pushed: /logDHT/");
  delay(5000);
}
