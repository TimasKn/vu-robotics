#include <EEPROM.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define SENSOR_R A0
#define SENSOR_L A1
#define BUTTON 2
#define TIMER_DELAY 100 //in ms (1-250)

int prev_option = 1;

struct Settings {
  int magic_num;
  volatile int option;
  volatile int sensor_avg;
  volatile int sensor_right;
  volatile int sensor_left;
};

Settings settings = {
  0, //Magic Num
  0, //Option number
  0, //Average sensor value
  0, //Right sensor value
  0, //Left sensor value
};

int timer_ticks = 25000;
ISR(TIMER1_COMPA_vect) {
  OCR1A += timer_ticks;
  settings.sensor_right = map(analogRead(SENSOR_R), 0, 930, 1, 10);
  settings.sensor_left = map(analogRead(SENSOR_L), 0, 930, 1, 10);
  settings.sensor_avg = (settings.sensor_right + settings.sensor_left) / 2;
}

void button_press() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) {
    settings.option += 1;
    if (settings.option > 3) settings.option = 1;
  }
 
  last_interrupt_time = interrupt_time;
  EEPROM.put(0, settings);
}

void setup() {
  lcd.begin(16, 2, LCD_5x10DOTS); 
  pinMode(SENSOR_R, INPUT);
  pinMode(SENSOR_L, INPUT);

  EEPROM.get(0, settings);
  if (settings.magic_num == 35) {
      if (settings.option == 1) display_value(settings.sensor_avg, "Average");
      else if (settings.option == 2) display_value(settings.sensor_left, "Left sensor");
      else display_value(settings.sensor_right, "Right sensor");
  } else {
    settings.magic_num = 35;
    settings.option = 1;
    settings.sensor_avg = 0;
    settings.sensor_right = 0;
    settings.sensor_left = 0;
  }

  attachInterrupt(digitalPinToInterrupt(BUTTON), button_press, FALLING);

  timer_ticks = 250000/(1000/TIMER_DELAY);
  TCCR1A = 0;           // Init Timer1
  TCCR1B = 0;           // Init Timer1
  TCCR1B |= B00000011;  // Prescalar = 64
  OCR1A = timer_ticks;  // Timer CompareA Register
  TIMSK1 |= B00000010;  // Enable Timer COMPA Interrupt
}

void loop() {
  if (settings.option != prev_option) {
    lcd.clear();
    prev_option = settings.option;
  }

  if (settings.option == 1) display_value(settings.sensor_avg, "Average");
  else if (settings.option == 2) display_value(settings.sensor_left, "Left sensor");
  else display_value(settings.sensor_right, "Right sensor");
}

void display_value(int value, String value_name) {
  lcd.setCursor(0,0); 
  lcd.print("Light level");
  lcd.print(" ");
  lcd.print(settings.option);
  lcd.print("/3");

  lcd.setCursor(0,1); 
  lcd.print(value_name);
  lcd.print(": ");
  if (value < 10) lcd.print(0);
  lcd.print(value);
}