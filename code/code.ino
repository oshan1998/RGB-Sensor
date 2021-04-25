#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char deciKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {3,4,5,6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7,12,13}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(deciKeys), rowPins, colPins, ROWS, COLS); 
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
int green = 10;
int red = 9;
int blue = 2;

int green_ =A2;
int red_ = A1;
int blue_ = A3;

void setup() {
 Serial.begin(9600);
 lcd.init();
 lcd.backlight();
 lcd.setCursor(0,0);
 lcd.print("1)Mode1");
 lcd.setCursor(8,0);
 lcd.print("2)Mode2");
 lcd.setCursor(4,1);
 lcd.print("3)Mode3");
 
// if (mode=='1'){
//  lcd.clear();
//  lcd.backlight();}
} 

 String get_3_digit(){
    int count=0;
    String val;
    while(count<3){
    char digit = customKeypad.getKey();
    if(digit){
    
    lcd.setCursor(9+count,0);
    lcd.print(digit);
    val+=digit;
    count+=1;}
   }
    return val;
  }
 void temp(){
   char mode = customKeypad.getKey();
 //mode 1
 if(mode=='1'){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Black");
  lcd.setCursor(7,0);
  lcd.print("R-");
  
  lcd.setCursor(0,1);
  lcd.print("*-clear");
  lcd.setCursor(9,1);
  lcd.print("Enter-#");
  String red_b=get_3_digit();
  char stat = customKeypad.getKey();
  Serial.println(stat);
  if(stat=='#'){
    lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Black");
  lcd.setCursor(7,0);
  lcd.print("G-");
  
  lcd.setCursor(0,1);
  lcd.print("*-clear");
  lcd.setCursor(9,1);
  lcd.print("Enter-#");
  String red_b=get_3_digit();
    }
  
  
 
 }
 //mode2
 else if(mode=='2'){
  Serial.println(mode);
  }
  //mode3
 else if(mode=='3'){
  Serial.println(mode);
  }
 }

void loop(){
  temp();
  }

