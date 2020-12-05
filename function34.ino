#include <DS1302.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#define DHT11_PIN 13

dht DHT;
unsigned long button_millis = 0;
int waterSwitch = 12;
int redPin = 11;
int greenPin = 10;
int bluePin = 9;
int count2 = 0;
// 분갈이 모듈 변수
int SW_POT = 4; // 분갈이 스위치 8 번
DS1302 rtc (5, 6, 7); // DS1302
LiquidCrystal_I2C lcd (0x27,16,2);

// play_music모듈 변수
int BUZZER = 8; //스피커가 연결된 디지털핀 설정
int note[] = {2093,2349,2637,2793,3136,3520,3951,4186}; //도레미파솔라시도
int music[] = {3136,3136,2637,2093,2093,3136,3136,2793,2793,2349,2349,2349,2349,2349,2349,
2793,2793,2349,1975,1975,2793,2793,2637,2637,2093,2093,2093,2093,2093,2093,
1760,1760,3520,3520,3136,2793,2637,2793,3136,3136,3136,2093,2093,2093,2093,2093,
2793,2637,2093,2793,2637,2093,3520,3136,3136,3136,3136,3136,3136,
1567,1567,1567,2793,2637,2349,2093,2093,2093};
int count=0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(waterSwitch, INPUT_PULLUP);
  // 새로운 날짜와 시간 정보를 칩에 저장
  rtc.writeProtect (false);
  rtc.halt(false);
  Time t(2020, 11, 28, 22, 30, 00, Time :: kSaturday);
  rtc.time(t);
  lcd.begin (16, 2);
  pinMode (SW_POT, INPUT_PULLUP);
  button_millis = millis();
}

void loop() {

  // lcd.clear();
  lcd.setCursor(0,0);

  Time t = rtc.time (); // 저장된 시간 읽어 오기

  if (digitalRead (SW_POT) == LOW) {
    count++;
  }

  if(count>0){
    Serial.println("on");
    printTime ();
    lcd.backlight ();
    delay(1000);
  }

  if(digitalRead (waterSwitch) == LOW){
    count2++;
  }

  if(count2>0){
    button_millis = millis();
    lcd.setCursor (12, 1);
    lcd.print ((millis()-button_millis)/1000);
    delay(1000);
    play_music();
  }

  Serial.println("start");
  //온습도**//
  int chk = DHT.read11(DHT11_PIN);
  if (digitalRead (SW_POT) == HIGH) {
    lcd.setCursor(0,0);
    lcd.print("Temp."); // "온도=" 라고 출력한다.
    lcd.setCursor(5,0);
    lcd.print(DHT.temperature); // 온도를 알려준다.
    lcd.setCursor(8,0);
    lcd.print("Humi."); // "습도=" 라고 출력한다
    lcd.setCursor(13,0);
    lcd.print(DHT.humidity); // 습도를 알려준다.
    delay(1000);
  }
//전체 점수**//

setColor(0, 0, 255); // blue로 set
  if(DHT.humidity<50||DHT.temperature>27||DHT.temperature<17){ //온도나 습도 안맞으면
    setColor(0, 255, 0); //blue에서 green으로 변화
    // delay(1000);

    if(DHT.humidity<35||DHT.temperature>30||DHT.temperature<13){ //온도나 습도 과도하게 안맞으면
      setColor(255, 0, 0);
      // delay(1000);
    }
  }
}

// 음악재생 모듈
void play_music(){

  int elementCount = sizeof(music) / sizeof(int);
  for (int i=0; i < elementCount; i++){
    tone(BUZZER,music[i]/12,500);
    delay(350);
  }
  count2 = 0;
}

void printTime () {
  Time t = rtc.time (); // 저장된 시간 읽어 오기
  // 날짜 출력
  char repot [12];
  snprintf (repot, sizeof (repot), "%04d-%02d-%02d", t.yr, t.mon, t.date);
  lcd.setCursor (0, 1);
  lcd.print (repot);
  count = 0;
}

void setColor(int red, int green, int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}