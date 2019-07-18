//DEVELOPED BY @CHECHESWAP JESUS JOSE NAVARRETE BACA
#include <Keypad.h> 
#include <LiquidCrystal.h> 
#include "pt.h" 
#include <Wire.h>

#define pinact 2 //PIN LED
#define pinledadvice 13 // PIN LED
#define lcdCols 20 //CONSTANTE NUMERO DE COLUMNAS LCD
#define pwdLength 20 //CONSTANTE MAXIMO DIGITOS PARA PASSWORD
#define pinSound 100 // PIN ACTIVAR BOCINA
#define pinSync 0 // PIN LED SINCRONIZADO


static int pthDef(struct pt *, int, void(*)(void)); 
void lcdinit(void); 
void alert(void);
void readPass(void); 
void textTitle(void); 
void ledadvise(void); 
void lcdpwincorrect(void); 
void stopAlarm(void); 
void listenFromJava(void); 
void welcomeMsg(void); 
void clsPass(void); 
void deniedAccess(void); 
void printUnavailable(void); 
void clsUnavailable(); 

void i2c_control(); 

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = { 
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte ROWSPINS [] = {14, 15, 16, 17}; 
byte COLSPINS [] = {9, 10, 11, 12}; 
Keypad mykeypad = Keypad(makeKeymap(keys), ROWSPINS, COLSPINS, ROWS, COLS); 
//               rs,en,d4,d5,d6,d7
LiquidCrystal lcd(7, 8, 3, 4, 5, 6);
const char arrtitleLCD [lcdCols] = "  >>> S.G.B.S <<<  "; 
const char* msg = "  => Bienvenido <= "; 
const char* deniedmsg = " =>Acceso Denegado! "; 
const char * rowCls = "                    "; 
const char * closedsystem = "Sistema Desconectado";
const char keyClear = '#'; 
const char keyStopAlarm = 'D'; 
const char keyAccept = '*'; 


static struct pt pth1; //NODO DE HILO #1
static struct pt pth2; //NODO DE HILO #2
static struct pt pth3; //NODO DE HILO #3
static struct pt pth4; //NODO DE HILO #4
static struct pt pth5; //NODO DE HILO #5
static struct pt pth6; //NODO DE HILO #6
static struct pt pth7; //NODO DE HILO #7
boolean flagBlink = false; 
boolean alertStatus = false;  
boolean flagledadvise = false; 
boolean listenalert = false; 
boolean isScrolling = false; 
boolean daemonMsg = true; //FOR WELCOME MSG IN A SUB THREAD MANAGMENT
boolean killDenied = true; // FOR DENIED MSG IN A SUB THREAD MANAGMENT
boolean isScrollingDenied = false;
boolean systemStatus = true; //STAUS DEL SISTEMA (1 = ACCEDIDO AL SISTEMA, 0 = EN ESPERA DE ACCESO)
boolean synchronized = false; //STATUS DEL SISTEMA (1 = SINCRONIZADO CON SOFTWARE APLICACION 0=DESCONECTADO)
boolean unvst = false; 
boolean goliat = false; 
int passwordCount = 0; 
char arrPass[pwdLength + 1] = {}; 
int tmpcol = 0; 


//PUNTEROS A FUNCIONES EMPLEADOS COMO DESENCADENADORES DE LAS SUB-RUTINAS QUE SE EJECUTAN EN LOS SUB-THREADS
static int (*blinklcd)(struct pt*, int, void(*)(void)); 
static int (*bipalert)(struct pt*, int, void(*)(void));
static int (*lcdnormal)(struct pt*, int, void(*)(void));
static int (*lcdpwbad)(struct pt*, int, void(*)(void));
static int (*listener)(struct pt*, int, void(*)(void));
static int (*TH_welcomeMsg)(struct pt*, int, void(*)(void));
static int (*TH_deniedMsg)(struct pt*, int, void(*)(void));
static int (*TH_unavailable)(struct pt*, int, void(*)(void));

void setup() {  
  Serial.begin(9600);
  pinMode(pinact, OUTPUT);
  pinMode(pinSound, OUTPUT);
  pinMode(pinledadvice, OUTPUT);
  pinMode(pinSync, OUTPUT);
  Wire.begin();
  lcdinit();

  blinklcd = pthDef;
  bipalert = pthDef2;
  lcdnormal = pthDef3;
  listener = pthDef4;
  TH_welcomeMsg = pthDef5;
  TH_deniedMsg = pthDef6;
  TH_unavailable = pthDef7;
}

void loop() {

  blinklcd(&pth1, 500, textTitle);
  
  listener(&pth4, 10, listenFromJava);

  if (!synchronized) {
    
    TH_unavailable(&pth7, 180, printUnavailable);    
  }
  else {

    if(goliat){
      clsUnavailable();
      goliat = false;
    }
    
    if (synchronized && systemStatus) {
      readPass();
    }
        
    if (!listenalert) {
      lcdnormal(&pth3, 500, ledadvice);
    }
    if (!daemonMsg) {
      TH_welcomeMsg(&pth5, 350, welcomeMsg);
    }
    if (!killDenied) {
      TH_deniedMsg(&pth6, 150, deniedMsg);
    }
  }

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void i2c_control(char value){
  Wire.beginTransmission(8);
  Wire.write(value);
  Wire.endTransmission();
}
void readPass() {

  char xchar = mykeypad.getKey();

  bool flag = false;

  if (xchar) {

    if ((!(xchar == keyClear)) && (!(xchar == keyStopAlarm)) && (!(xchar == keyAccept))) {

      ++passwordCount;
      lcd.setCursor(passwordCount - 1, 2);
      lcd.print('*');

    }

    if (passwordCount == pwdLength) {
      flag = true;
    }

    else {

      Serial.println(xchar);

    }

    switch (xchar) {
      case ('1'):
      case ('2'):
      case ('3'):
      case ('4'):
      case ('5'):
      case ('6'):
      case ('7'):
      case ('8'):
      case ('9'):
      case ('0'):
      case ('A'):
      case ('B'):
      case ('C'): {

          arrPass[passwordCount] = xchar;
          daemonMsg = true;
          killDenied = true;
          i2c_control('1');
          
          break;
        }
      case (keyStopAlarm): {
          stopAlarm();
          break;
        }
      case (keyClear): {

          if (passwordCount >= 1) {

            --passwordCount;

            lcd.setCursor(passwordCount, 2);

            lcd.print(" ");
          }
          break;
        }
    }
  }

  if (flag || listenalert) {

    deniedAccess();

    if (flag) {
      clsPass();
    }
  }

}

void listenFromJava() {

  if (0 < Serial.available()) {
    int incoming = Serial.read();

    switch (incoming) {
      case ('0'): {
          killDenied = false;
          alert();
          break;
        }
      case ('1'): {
          daemonMsg = false;
          systemStatus = false;
          break;
        }
      case ('D'): {
          stopAlarm();
          break;
        }
      case ('@'): {
          systemStatus = true;
          synchronized = true;

          goliat = true;
          break;
        }

        case('z'):{
          systemStatus = false;
          synchronized = false;

          break;
        }
    }
  }
}

void clsUnavailable(){
    lcdinit();
    digitalWrite(pinSync, HIGH);
}

void printUnavailable() {

  unvst ^= 1;

  if (unvst) {
    lcd.setCursor(0, 2);
    lcd.print(closedsystem);
    lcd.setCursor(0, 3);
    lcd.print(closedsystem);    
  }
  else {
    lcd.setCursor(0, 2);
    lcd.print(rowCls);
    lcd.setCursor(0, 3);
    lcd.print(rowCls);
  }

  digitalWrite(pinSync, unvst);
}
void textTitle() {

  flagBlink ^= 1;

  if (flagBlink) {
    lcd.setCursor(0, 0);
    lcd.print(arrtitleLCD);
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print(rowCls);
  }
}
void lcdinit() {
  lcd.clear();
  lcd.begin(20, 4);
  lcd.setCursor(0, 1);
  lcd.print("--Ingrese Password--");
}

void alert() {
  listenalert = true;
  alertStatus ^= 1;
  digitalWrite(pinact, alertStatus);
  digitalWrite(pinSound, HIGH);
}

void ledadvice() {
  flagledadvise ^= 1;
  digitalWrite(pinledadvice, flagledadvise);
}

void stopAlarm() {
  alertStatus = true;
  bipalert(&pth2, 1, alert);
  listenalert = false;
  digitalWrite(pinSound, LOW);
  lcd.setCursor(0, 3);
  lcd.print(rowCls);
  killDenied = true;
}

void deniedAccess() {
  bipalert(&pth2, 50, alert);
  digitalWrite(pinledadvice, LOW);
}

void welcomeMsg() {

  isScrolling ^= 1;

  lcd.setCursor(0, 3);

  if (isScrolling) {
    lcd.print(msg);
  }
  else {
    lcd.print(rowCls);
  }

  clsPass();
}

void deniedMsg() {

  isScrollingDenied ^= 1;

  lcd.setCursor(0, 3);

  if (isScrollingDenied) {
    lcd.print(deniedmsg);
  }
  else {
    lcd.print(rowCls);
  }

  clsPass();
}
void clsPass() {
  passwordCount = 0;
  lcd.setCursor(0, 2);
  lcd.print(rowCls);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int pthDef(struct pt * pt, int interval, void(*func)(void)) {

  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  while (true) {

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );

    timestamp = millis();

    func();
  }

  PT_END(pt);
}

static int pthDef2(struct pt * pt, int interval, void(*func)(void)) {

  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  while (true) {

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );

    timestamp = millis();

    func();
  }

  PT_END(pt);
}

static int pthDef3(struct pt * pt, int interval, void(*func)(void)) {

  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  while (true) {

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );

    timestamp = millis();

    func();
  }

  PT_END(pt);
}

static int pthDef4(struct pt * pt, int interval, void(*func)(void)) {

  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  while (true) {

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );

    timestamp = millis();

    func();
  }

  PT_END(pt);
}

static int pthDef5(struct pt * pt, int interval, void(*func)(void)) {

  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  while (true) {

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );

    timestamp = millis();

    func();
  }

  PT_END(pt);
}

static int pthDef6(struct pt * pt, int interval, void(*func)(void)) {

  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  while (true) {

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );

    timestamp = millis();

    func();
  }

  PT_END(pt);
}

static int pthDef7(struct pt * pt, int interval, void(*func)(void)) {

  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  while (true) {

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );

    timestamp = millis();

    func();
  }

  PT_END(pt);
}

