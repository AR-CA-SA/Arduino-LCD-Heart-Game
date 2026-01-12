#include <LiquidCrystal.h>
int x = 8, y = 1;
const int rs = 7, e = 8, D4 = 9, D5 = 10, D6 = 11, D7 =12 , pin_4  = 4 , pin_2 = 2,  pin_6  = 6, pin_5  = 5;
LiquidCrystal lcd(rs, e , D4, D5, D6, D7);

unsigned long lastBulletUpdate = 0;
const unsigned long bulletInterval = 400;

unsigned long int lastTime = 0;
unsigned long int moveInterval = 200;

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000

};

byte broken_heart_1[8] = {
  0b00000,
  0b00010,
  0b10101,
  0b11011,
  0b01100,
  0b01010,
  0b00100,
  0b00000

};

byte broken_heart_2[8] = {
  0b00000,
  0b00000,
  0b10101,
  0b10011,
  0b01100,
  0b01010,
  0b00000,
  0b00000

};
byte broken_heart_3[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b10001,
  0b01000,
  0b00010,
  0b00000,
  0b00000

};
byte broken_heart_4[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000

};

struct Bullet{
  int col;
  int row ;
  int direction;
  bool active;
};
Bullet bullet;

byte bullet_charac[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
};

byte empty[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};


int heartUp (int current_x_pos,int current_y_pos){
  int new_y_pos{current_y_pos};
  if (current_y_pos >=0){
      lcd.setCursor(current_x_pos, current_y_pos);

      lcd.write(byte(2));
      lcd.setCursor(current_x_pos, --new_y_pos);
      lcd.write(byte(0));
  }
  return new_y_pos;
}

int  heartDown (int current_x_pos,int current_y_pos){
  int new_y_pos{current_y_pos};
  if (current_y_pos < 1){
      lcd.setCursor(current_x_pos, current_y_pos);
;
      lcd.write(byte(2));
      lcd.setCursor(current_x_pos, ++new_y_pos);
      lcd.write(byte(0));

  }
  return new_y_pos;
  
}

int heartRight (int current_x_pos,int current_y_pos){
  int new_x_pos{current_x_pos};
  if (current_x_pos < 15){
      lcd.setCursor(current_x_pos, current_y_pos);

      lcd.write(byte(2));
      lcd.setCursor(++new_x_pos, current_y_pos);
      lcd.write(byte(0));


  }
  return new_x_pos;
  
}

int heartLeft (int current_x_pos,int current_y_pos){
  int new_x_pos{current_x_pos};
  if (current_x_pos >=0 ){
      lcd.setCursor(current_x_pos, current_y_pos);
      lcd.write(byte(2));
      lcd.setCursor(--new_x_pos, current_y_pos);
      lcd.write(byte(0));

  }
  return new_x_pos;
}

void generateBullet(){
  int choice[2] = {0,15};
  bullet.row = random(0,2);
  bullet.col = choice[random(0,2)];
  if (bullet.col == 0 ){
    bullet.direction = 1;

  }else if (bullet.col == 15){
    bullet.direction = -1;
  }
  bullet.active = true;

}





void updateBullet(){
  if(bullet.active == false){
    return;
  }      
  if (millis() - lastBulletUpdate >= bulletInterval){
      lastBulletUpdate = millis();
      lcd.setCursor(bullet.col,bullet.row);
      lcd.write(byte(2));
      bullet.col += bullet.direction;
      if(bullet.col < 0 or bullet.col > 15){
        bullet.active = false;
        generateBullet();
        return;
    }

  }
  lcd.setCursor(bullet.col,bullet.row);
  lcd.write(byte(1));
}

void playerInput(){
  
  if (millis() - lastTime < moveInterval){
    return;
  }
  if (digitalRead(pin_4) == LOW){
        // Serial.println("UP");
        y = heartUp(x,y);
        lastTime = millis();
      }else if (digitalRead(pin_2) == LOW){
        // Serial.println("DOWN");
        y = heartDown(x,y);
        lastTime = millis();
      }else if(digitalRead(pin_5) == LOW){
          // Serial.println("RIGHT");
          x = heartRight(x,y);
          lastTime = millis();
      }else if(digitalRead(pin_6) == LOW){
          // Serial.println("LEFT");
          x = heartLeft(x,y);
          lastTime = millis();
    }
}


void startGame(){
    while (!(digitalRead(pin_4) == LOW || digitalRead(pin_2) == LOW || digitalRead(pin_5) == LOW || digitalRead(pin_6) == LOW)){
      lcd.setCursor(0, 0);
      lcd.write("Press any button");
    }
    lcd.clear();
    return;
    
}


void checkCollision(){

  if (bullet.col == x && bullet.row == y){
    lcd.clear();
    bullet.active = false;
    lcd.setCursor(3,0);
    lcd.write("Game  over");        
    lcd.setCursor(8,1);
    lcd.write(byte(3));
    lcd.setCursor(8,1);
    delay(500);
    lcd.setCursor(8,1);
    lcd.write(byte(4));
    lcd.setCursor(8,1);
    delay(500);
    lcd.write(byte(5));
    lcd.setCursor(8,1);
    delay(500);
    lcd.write(byte(6));
    while(1){
        if (digitalRead(pin_4) == LOW || digitalRead(pin_2) == LOW || digitalRead(pin_5) == LOW || digitalRead(pin_6) == LOW){
          generateBullet();
          lcd.clear();
          lcd.setCursor(x,y);
          lcd.write(byte(0));
          return;
        }
      }
  }   
  
}
void setup() {

  randomSeed(analogRead(0));
  Serial.begin(9600);
  pinMode(pin_2, INPUT_PULLUP);
  pinMode(pin_4, INPUT_PULLUP);
  pinMode(pin_5, INPUT_PULLUP);
  pinMode(pin_6, INPUT_PULLUP);
  lcd.begin(16, 2);
  
  lcd.createChar(0, heart);
  lcd.createChar(1, bullet_charac);
  lcd.createChar(2, empty);
  lcd.createChar(3, broken_heart_1);
  lcd.createChar(4, broken_heart_2);
  lcd.createChar(5, broken_heart_3);
  lcd.createChar(6, broken_heart_4);
  lcd.setCursor(x,y);
  lcd.write(byte(0));
  startGame();
  lcd.setCursor(x,y);
  lcd.write(byte(0));
  generateBullet();
}

void loop() {
    playerInput();
    updateBullet();
    checkCollision();
  
    
}
