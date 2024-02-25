# 에어깨긋해
🏫 본 프로젝트는 `전주대학교` `정보통신공학과` `캡스톤디자인` 프로젝트입니다.

|이름|담당|Github|
|---|---|---|
|유종환|Backend, Client, 아두이노 통신|https://github.com/Potato-Y|
|한태희|아두이노 설계 및 제어|-|
|박철규|케이스 제작|-|
|이용현|케이스 제작|-|

## 저장소 안내
- Backend https://github.com/Potato-Y/airclean_server
- Client https://github.com/Potato-Y/airclean_client
- Arduino https://github.com/Potato-Y/airclean_device

프로젝트의 전체 설명은 [Backend](https://github.com/Potato-Y/airclean_server) 문서를 확인해 주세요.

## 사용 기술 스택
<img src="https://img.shields.io/badge/arduino-00878F?style=for-the-badge&logo=arduino&logoColor=white">
<img src="https://img.shields.io/badge/socketdot.io-010101?style=for-the-badge&logo=socketdotio&logoColor=white">

## 주요 기능
- 공기질 실시간 모니터링
- 공기질에 맞게 팬 속도 조절
- 서버와 Socket.IO를 통해 실시간 통신
  - 서버에 실시간 공기질 정보 전송
  - 사용자의 제어 명령어를 전송 받음

## 사용 소자
- Arduino D1 R1
- DHT-22 (온습도 센서)
- GP2Y1010AU0F (미세먼지 센서)
- 초음파 가습기
- DC 팬
- LCD 디스플레이 (20 * 4)

## 설계도
![image](https://github.com/Potato-Y/airclean_device/assets/68105481/68d62c03-9e72-41f5-959d-52b64f995c0c)
![image](https://github.com/Potato-Y/airclean_device/assets/68105481/88acaeb0-4611-4ac1-a46f-4908abbd1601)
![image](https://github.com/Potato-Y/airclean_device/assets/68105481/d9ee5525-f59b-4df2-86b0-fbfd2ae2bff2)
