// 지혜님은 이 코드에 지혜님 모듈들을 넣어주세요

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>

int sw1 = 8; // 분갈이 스위치 8 번
DS1302 rtc (5, 6, 7); // DS1302
LiquidCrystal_I2C lcd (0x27,16,2);


// play_music모듈 변수
int BUZZER = 12; //스피커가 연결된 디지털핀 설정
int note[] = {2093,2349,2637,2793,3136,3520,3951,4186}; //도레미파솔라시도
int music[] = {3136,3136,2637,2093,2093,3136,3136,2793,2793,2349,2349,2349,2349,2349,2349,
              2793,2793,2349,1975,1975,2793,2793,2637,2637,2093,2093,2093,2093,2093,2093,
              1760,1760,3520,3520,3136,2793,2637,2793,3136,3136,3136,2093,2093,2093,2093,2093,
              2793,2637,2093,2793,2637,2093,3520,3136,3136,3136,3136,3136,3136,
              1567,1567,1567,2793,2637,2349,2093,2093,2093};



void setup() {
  Serial.begin(9600);
  // 새로운 날짜와 시간 정보를 칩에 저장
  // rtc.writeProtect (false);
  //rtc.halt(false);
  // 시간 t (2020, 11, 28, 22, 30, 00, 시간 :: kSaturday);
  //rtc.time(t);

  lcd.begin (16, 2);
  pinMode (sw1, INPUT_PULLUP);
  play_music();
}
 
void loop() {
  if (digitalRead (sw1) == LOW) {
printTime ();
lcd.backlight ();
}
   
}

// 음악재생 모듈
void play_music(){
  int elementCount = sizeof(music) / sizeof(int);
  for (int i=0; i < elementCount; i++)  
  {
    tone(BUZZER,music[i]/12,500);
    delay(350);
  }
}

void printTime () {
Time t = rtc.time (); // 저장된 시간 읽어 오기
// 날짜 출력
char repot [12];
snprintf (repot, sizeof (repot), "%04d-%02d-%02d", t.yr, t.mon, t.date);
lcd.setCursor (0, 0);
lcd.print (repot);
}
