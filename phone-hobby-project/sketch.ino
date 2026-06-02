#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <string>
#include <map>


#define I2C_ADDR 0x27

int COLUMS = 16;

LiquidCrystal_I2C lcd(I2C_ADDR, COLUMS, 2);

const uint8_t ROWS = 4;
const uint8_t COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

uint8_t colPins[COLS] = { 18, 17, 16}; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = { 9, 46, 3, 8 }; // Pins connected to R1, R2, R3, R4


Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int wait = 1;
int index_num = 0;
std::string number = "";

std::map<std::string, std::string> phone_book;


void clearRow(int row) {
  lcd.setCursor(0, row);
  lcd.print("                ");
}

void EnterNumberTask(void *p) {
  while (true) {
    if (wait == 1) {
      if (number.length() == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Enter number:   ");
      }
      else {
        lcd.setCursor(0, 0);
        lcd.print("Pres # to call. ");

        vTaskDelay(400 / portTICK_PERIOD_MS);

        lcd.setCursor(0, 0);
        lcd.print("Pres * to clean.");

      }
    }

    else if (wait == 2) {
      //calling
      lcd.setCursor(0, 0);
      lcd.print("Calling to:     ");
    }

    else if(wait == 3){
      lcd.setCursor(0, 0);
      lcd.print("colling stoped! ");
    }
    vTaskDelay(400 / portTICK_PERIOD_MS);
  }

}

void InputTask(void *p) {
  Serial.println("task input");

  while (true) {
    char key = keypad.getKey();

    if (key != NO_KEY) {
      if (key != '*' && key != '#' && wait == 1) {
        number += key;
        if (number.length() <= 16) {
          lcd.setCursor(0, 1);
          lcd.print(number.c_str());
        }
        else {
          lcd.setCursor(0, 1);
          std::string text =
            "..." + number.substr(number.length() - COLUMS + 3, COLUMS - 3);
          lcd.print(text.c_str());
        }
      }
      else if (key == '*') {
        if (wait == 2) {
          wait = 3;
          vTaskDelay(1000 / portTICK_PERIOD_MS);

        }
        wait = 1;
        number = "";
        clearRow(1);
      }
      else if (wait == 1) {
        //calling...
        wait = 2;
        if (number.length() > COLUMS) {
          lcd.setCursor(0, 1);
          lcd.print("Un Valid Number!");
          vTaskDelay(1000 / portTICK_PERIOD_MS);
          wait = 1;
          number = "";
          clearRow(1);
        }
        else {
          if (phone_book.find(number) != phone_book.end()) {
            clearRow(1);
            lcd.setCursor(0, 1);
            lcd.print(phone_book[number].c_str());
          }
        }
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}



void setup() {

  phone_book["0527164909"] = "Yaffi Yud Bea''m";
  phone_book["0527651909"] = "Yudi Yud Hhamud!";
  phone_book["0548441445"] = "My Lovly Daddi";
  phone_book["0504137458"] = "My Lovly Mommy";
  phone_book["0527616169"] = "Shverrrrr";
  phone_book["0527605321"] = "Shvigerrrrr";
  phone_book["029922900"] = "Friedman";
  phone_book["029916321"] = "Yud (the Big...)";
  phone_book["0556733854"] = "My Best Friend!!!";
  phone_book["0556776443"] = "My sister-in-law";


  Wire.begin(14, 13);

  lcd.init();
  lcd.backlight();

  xTaskCreate(EnterNumberTask, "EnterNumber", 5000, NULL, 1, NULL);
  xTaskCreate(InputTask, "Input", 5000, NULL, 1, NULL);
}

void loop() {

}










