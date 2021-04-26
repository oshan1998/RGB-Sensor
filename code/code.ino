#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
String Red_b, Green_b, Blue_b = "0";
String Green_w, Red_w, Blue_w = "1023";
boolean calibrated = false;
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char deciKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {3, 4, 5, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 12, 13}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(deciKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x3F for a 16 chars and 2 line display
int green = 10;
int red = 9;
int blue = 2;

int green_ = A2;
int red_ = A1;
int blue_ = A3;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(red_, OUTPUT);
  pinMode(green_, OUTPUT);
  pinMode(blue_, OUTPUT);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("1)Mode1");
  lcd.setCursor(8, 0);
  lcd.print("2)Mode2");
  lcd.setCursor(4, 1);
  lcd.print("3)Mode3");

}

String get_3_digit() {
  int count = 0;
  String val;

  while (count < 3) {
    char digit = customKeypad.getKey();
    if (digit) {

      lcd.setCursor(9 + count, 0);
      lcd.print(digit);
      val += digit;
      count += 1;
    }
  }
  return val;
}
void temp() {
  analogWrite(red_, 255);
  analogWrite(green_, 255 );
  analogWrite(blue_, 255);
  Serial.println("temp");
  char mode = customKeypad.getKey();
  //mode 1
  boolean flag = false;
  if (mode) {
    flag = true;
  }
  else {
    flag = false;
  }
  while (flag) {
    if (mode == '1') {
      Red_b = displayWrite("Black", "R-");
      if (Red_b) {
        Green_b = displayWrite("Black", "G-");
        if (Green_b) {
          Blue_b = displayWrite("Black", "B-");
          if (Blue_b) {
            Red_w = displayWrite("White", "R-");
            if (Red_w) {
              Green_w = displayWrite("White", "G-");
              if (Green_w) {
                Blue_w = displayWrite("White", "B-");
                if (Blue_w) {
                  flag = false;
                  calibrated = true;
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Calibration");
                  lcd.setCursor(6, 1);
                  lcd.print("Done!");
                  delay(1000);
                  setup();
                }
              }
            }
          }

        }
      }


    }

    //mode2
    else if (mode == '2') {
      boolean finish = false;
      while (not finish) {
        lcd.clear();
        digitalWrite(green, HIGH);
        delay(300);
        int gr_avg = average(get_reading(A0));
        if (calibrated) {
          gr_avg = int_to_byte(map(gr_avg, int (Green_b.toInt()), int (Green_w.toInt()), 0, 255));
        }



        digitalWrite(green, LOW);

        digitalWrite(red, HIGH);
        delay(300);
        int rd_avg = average(get_reading(A0));
        if (calibrated) {
          rd_avg = int_to_byte(map(rd_avg, int (Red_b.toInt()), int (Red_w.toInt()), 0, 255));
        }


        digitalWrite(red, LOW);

        digitalWrite(blue, HIGH);
        delay(300);
        int bl_avg = average(get_reading(A0));
        if (calibrated) {
          bl_avg = int_to_byte( map(bl_avg, int (Blue_b.toInt()), int (Blue_w.toInt()), 0, 255));
        }


        digitalWrite(blue, LOW);

        //byte rd_avg_new,gr_avg_new, bl_avg_new = byte(develop(rd_avg,gr_avg,bl_avg)); //if want some pushup for values...!
        lcd.setCursor(0, 0);
        lcd.print("R-");
        lcd.setCursor(2, 0);
        lcd.print(rd_avg);

        lcd.setCursor(10, 0);
        lcd.print("G-");
        lcd.setCursor(12, 0);
        lcd.print(gr_avg);

        lcd.setCursor(6, 1);
        lcd.print("B-");
        lcd.setCursor(8, 1);
        lcd.print(bl_avg);


        Serial.print("Green :");
        Serial.print(gr_avg);
        Serial.print(" RED :");
        Serial.print(rd_avg);
        Serial.print(" Blue :");
        Serial.print(bl_avg);
        Serial.println("");
        if (calibrated) {
          analogWrite(red_, 255 - rd_avg);
          analogWrite(green_, 255 - gr_avg);
          analogWrite(blue_, 255 - bl_avg);
        }
        

        delay(1000);
      }
    }
    //mode3
    else if (mode == '3') {
      lcd.clear();
      lcd.setCursor(0, 0);
      String R = displayWrite("RGB", "R-");
      if (R) {
        String G = displayWrite("RGB", "G-");
        if (G) {
          String B = displayWrite("RGB", "B-");
          if (B) {
            lcd.clear();
            lcd.setCursor(6, 0);
            lcd.print("Done!");
            flag = false;
            analogWrite(red_, 255 - int (R.toInt()));
            analogWrite(green_,  255 - int (G.toInt()));
            analogWrite(blue_, 255 - int (B.toInt()));

            delay(900);
            setup();
          }
        }
      }

    }

  }
}

void loop() {
  temp();
  char chr = customKeypad.getKey();
        if(chr=='#'){
        setup();}
}


String displayWrite(String clr_mode, String clr ) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(clr_mode);
  lcd.setCursor(7, 0);
  lcd.print(clr);
  lcd.setCursor(0, 1);
  lcd.print("*-clear");
  lcd.setCursor(9, 1);
  lcd.print("Enter-#");
  String val = get_3_digit();
  if (val) {
    boolean flag1 = true;
    while (flag1) {
      char stat = customKeypad.getKey();
      if (stat == '#') {
        return  val;
      }
      else if (stat == '*') {
        displayWrite(clr_mode, clr );
      }
    }
  }
}
int average(int list[]) {
  int sum = 0;
  for (int i = 0; i < sizeof(list); i++) {
    sum += list[i];
  }
  return sum / sizeof(list);
}
int* get_reading(int pin) {
  int list[10];
  for (int i = 0; i < 10; i++) {
    list[i] = analogRead(pin);
    delay(25);
  }
  return list;
}
//int* develop(byte r, byte g, byte b) {
//  if (r < g and b < g) {
//    r = r - 50;
//    b = b - 50;
//    g = g + 50;
//
//  }
//  else if (b < r and g < r) {
//    r = r + 50;
//    b = b - 50;
//    g = g - 50;
//
//  }
//  else if (r < b and g < b) {
//    r = r - 50;
//    b = b + 50;
//    g = g - 50;
//
//  }
//  return r, g, b;
//}
byte  int_to_byte(int val) {
  if (val < 255) {
    return byte(val);
  }
  else {
    return 255;
  }
}
