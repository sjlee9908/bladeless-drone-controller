#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//각 제어정보의 최대값, 최소값
#define THROTTLE_MAX 1000
#define THROTTLE_MIN 700

#define ROLL_MAX 30
#define ROLL_MIN -30

#define PITCH_MAX 30
#define PITCH_MIN -30

#define YAW_MAX 30
#define YAW_MIN -30

//전송할 제어 정보
int throttle;
int pitch;
int roll;
double yaw;

RF24 radio(7, 8); // RF을 위한 (CE, CSN)핀 선언
const byte address[6] = "00001"; // 송신기와 수신기를 동일한 값으로 주소 설정

//throttle값 읽기
void check_joystick_throttle(){
  //throttle 조이스틱을 읽어, 평탄화
  throttle = analogRead(A1);
  throttle = map(throttle, 0,1023, THROTTLE_MIN,THROTTLE_MAX);
}

//roll값 읽기
void check_joystick_roll(){
  roll = analogRead(A2);
  if(roll <= 525 && roll >=524){
    roll = 0;
    return;
  }
  roll = map(roll, 0,1023, ROLL_MIN,ROLL_MAX);
}

//pitch값 읽기
void check_joystick_pitch(){
  pitch = analogRead(A3);
  if(pitch <= 480 && pitch >=460){ 
    pitch = 0;
    return;
  }
  pitch = map(pitch, 0,1023, PITCH_MIN,PITCH_MAX);
}

//yaw값 읽기 -> 누적
void check_yaw_button(){
  if(digitalRead(3) == LOW){
    yaw -= 0.1;
    if(yaw <= YAW_MIN)yaw = YAW_MIN;
  }
  if(digitalRead(2) == LOW){
    yaw += 0.1;
    if(yaw >= YAW_MAX)yaw = YAW_MAX;
  }
  if(digitalRead(2) == digitalRead(3)) yaw = 0; 
}

//긴급정지 버튼
void check_stop_button(){
  if(digitalRead(4) == LOW){
    while(1){
      Serial.println("STOP");
      radio.write(&" 0 0 0 0", sizeof(" 0 0 0 0"));
    }
  } 
}

//통신 연결 여부 확인
void sending_dummy(){
  while(1){
    Serial.println("Sending Dummy");
    char charText = " d u m m y";
    int res = radio.write(&charText, sizeof(charText));
    Serial.println(res+"\n");
    if(res==1) break;
  }
}

//통신 연결 후, 드론 초기화
void init_drone(){
    while(1){
    Serial.println("press the button");
    if(digitalRead(5) == LOW){
      Serial.println("1");
      while(!radio.write(&"1", sizeof(&"1")));
      delay(6000);
      break;
    }
  }
}


void setup() {
  radio.begin();  //rf 통신 init
  Serial.begin(9600);
  radio.openWritingPipe(address); // 데이터를 보낼 수신 주소를 설정
  radio.setPALevel(RF24_PA_HIGH); // 송신거리에 따른, 전원공급 파워레벨 설정
  radio.stopListening(); // 모듈을 송신기로 설정

  //digitalPin setup
  pinMode(2, INPUT_PULLUP); //right
  pinMode(3, INPUT_PULLUP); //left
  pinMode(4, INPUT_PULLUP);  //stop
  pinMode(5, INPUT_PULLUP);  //init
  pinMode(9, OUTPUT);  //led
  pinMode(10, OUTPUT); //buzzer
  
  //led on
  digitalWrite(9, HIGH);
  
  //buzzer off
  digitalWrite(10, LOW);
  
  sending_dummy();  //통신 연결 확인
  init_drone();  //드론 초기화

  //led off
  digitalWrite(9, LOW);
  
}

void loop() {
  //조이스틱, 버튼 등 제어정보 check
  check_joystick_throttle();
  check_joystick_roll();
  check_joystick_pitch();
  check_yaw_button();
  check_stop_button();

  String stringText2 = (String)" "+pitch + "  " + roll +  "  "+ throttle +"  " + (int)yaw;  //
  char charText[200];

  stringText2.toCharArray(charText, 200);
  Serial.println(charText);
  int res = radio.write(&charText, sizeof(charText));  //Send to Drone
  Serial.println(res); 
  
}
