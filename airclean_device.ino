/*
 * WebSocketClientSocketIO.ino
 *
 *  Created on: 06.06.2016
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

#define USE_SERIAL Serial1

// 기기 현 상태
int mode = 1;
int windSpeed = 1;
bool uv = true;
bool humidifier = false;
bool petier = false;
String temperature = "30.3";
int humidity = 30;
float pm1_0 = 5;
float pm2_5 = 5;
float pm10_0 = 9;
int gas = 1;
String debug = "";
String debug2 = "";

void socketIOEvent(socketIOmessageType_t type, uint8_t* payload, size_t length) {
  String input = "";

  switch (type) {
    case sIOtype_DISCONNECT:
      USE_SERIAL.printf("[IOc] Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      USE_SERIAL.printf("[IOc] get event: %s\n", payload);
      input = String((char*)payload);

      if (input.indexOf("mode_change") >= 0) {
        int inputStart = input.indexOf("{");                            // find the start of the input data
        int inputEnd = input.lastIndexOf("}");                          // find the end of the input data
        String jsonString = input.substring(inputStart, inputEnd + 1);  // extract the input data as a substring
        jsonString.replace("\\\"", "\"");

        const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(4);  // JSON 객체의 크기에 따라 조정합니다.
        StaticJsonDocument<JSON_BUFFER_SIZE> jsonDocument;

        DeserializationError error = deserializeJson(jsonDocument, jsonString);

        if (error) {
          // JSON 파싱 오류 처리
          debug = error.c_str();
          debug2 = jsonString;
        } else {
          // JSON 파싱이 성공한 경우
          int modeValue = jsonDocument["mode"].as<int>();
          int humidityModeValue = jsonDocument["humidityMode"].as<int>();

          // mode = modeValue;
          mode = modeValue;
          // 모드 변경 코드

          // 가습기 변경 코드
          if(humidityModeValue == 2){
            humidifier = true;
          }if(humidityModeValue == 1){
            humidifier = false; 
          }
        }
      }

      break;
    case sIOtype_ACK:
      USE_SERIAL.printf("[IOc] get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      USE_SERIAL.printf("[IOc] get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      USE_SERIAL.printf("[IOc] get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}

void setup() {
  // USE_SERIAL.begin(921600);
  USE_SERIAL.begin(115200);


  //Serial.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  // disable AP
  if (WiFi.getMode() & WIFI_AP) {
    WiFi.softAPdisconnect(true);
  }

  WiFiMulti.addAP("sharkIoT", "potato1234!");

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  String ip = WiFi.localIP().toString();
  USE_SERIAL.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

  // server address, port and URL
  socketIO.begin("ariclean.duckdns.org", 3000, "/socket.io/?EIO=4");

  // event handler
  socketIO.onEvent(socketIOEvent);
}

unsigned long messageTimestamp = 0;
void loop() {
  socketIO.loop();

  uint64_t now = millis();

  if (now - messageTimestamp > 2000) {
    messageTimestamp = now;

    // Socket.IO(이벤트)에 대한 JSON 메시지 생성
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();

    // 이벤트 이름 추가
    // Hint: socket.on('event_name', ....
    array.add("data");

    // 이벤트에 대한 페이로드(매개변수) 추가
    JsonObject param1 = array.createNestedObject();
    param1["state"] = true;
    param1["mode"] = mode;
    param1["windSpeed"] = windSpeed;
    param1["uv"] = uv;
    param1["humidifier"] = humidifier;
    param1["petier"] = petier;
    param1["temperature"] = temperature;
    param1["humidity"] = humidity;
    param1["gas"] = gas;


    JsonObject param2 = array.createNestedObject();
    param2["1.0"] = pm1_0;
    param2["2.5"] = pm2_5;
    param2["10.0"] = pm10_0;

    param1["dust"] = param2;
    // param1["debug"] = debug;
    // param1["debug2"] = debug2;


    // JSON을 문자열로(직렬화)
    String output;
    serializeJson(doc, output);

    // 이벤트 보내기
    socketIO.sendEVENT(output);

    // 디버깅을 위해 JSON 인쇄
    // USE_SERIAL.println(output);
  }
}
