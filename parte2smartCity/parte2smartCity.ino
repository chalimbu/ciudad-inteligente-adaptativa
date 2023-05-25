#include <LiquidCrystal_I2C.h>  //Library for LCD display via I2C


//Library definition
#include <Wire.h> //Library required for I2C comms (LCD)
#include <LiquidCrystal_I2C.h>  //Library for LCD display via I2C
#include <math.h> //Mathematics library for pow function (CO2 computation)

//I/O pin labeling
#define LDR1 0 //LDR Light sensor from traffic light 1 connected in pin A0
#define LDR2 1  //LDR Light sensor from traffic light 2 connected in pin A1
#define CO2 2  //CO2 sensor connected in pin A2
#define P1 37  //Traffic light 1 button connected in pin 37
#define P2 36  //Traffic light 2 button connected in pin 36
#define CNY1 35 //Infrared sensor 1 in traffic light 1 connected in pin 35
#define CNY2 34 //Infrared sensor 2 in traffic light 1 connected in pin 34
#define CNY3 33 //Infrared sensor 3 in traffic light 1 connected in pin 33
#define CNY4 32 //Infrared sensor 4 in traffic light 2 connected in pin 32
#define CNY5 31 //Infrared sensor 5 in traffic light 2 connected in pin 31
#define CNY6 30 //Infrared sensor 6 in traffic light 2 connected in pin 30
#define LR1 22 //Red traffic light 1 connected in pin 22
#define LY1 23 //Yellow traffic light 1 connected in pin 23
#define LG1 24 //Green traffic light 1 connected in pin 24
#define LR2 25 //Red traffic light 2 connected in pin 25
#define LG2 27 //Green traffic light 2 connected in pin 27
#define LY2 26 //Yellow traffic light 2 connected in pin 26

//Library definitions
LiquidCrystal_I2C lcd(0x27, 16, 4); //Set the LCD address to 0x27 for a 16 chars and 4 line display


//Configuration
void setupConnections() {
   //Pin config
  //Pin config
  pinMode(P1, INPUT); //Traffic light 1 button as Input
  pinMode(P2, INPUT); //Traffic light 2 button as Input
  pinMode(CNY1, INPUT); //Infrared sensor 1 in traffic light 1 as Input
  pinMode(CNY2, INPUT); //Infrared sensor 2 in traffic light 1 as Input
  pinMode(CNY3, INPUT); //Infrared sensor 3 in traffic light 1 as Input
  pinMode(CNY4, INPUT); //Infrared sensor 4 in traffic light 2 as Input
  pinMode(CNY5, INPUT); //Infrared sensor 5 in traffic light 2 as Input
  pinMode(CNY6, INPUT); //Infrared sensor 6 in traffic light 2 as Input
  pinMode(LR1, OUTPUT); //Red traffic light 1 as Output
  pinMode(LY1, OUTPUT); //Yellow traffic light 1 as Output
  pinMode(LG1, OUTPUT); //Green traffic light 1 as Output
  pinMode(LR2, OUTPUT); //Red traffic light 2 as Output
  pinMode(LY2, OUTPUT); //Yellow traffic light 2 as Output
  pinMode(LG2, OUTPUT); //Green traffic light 2 as Output

  //Output cleaning
  digitalWrite(LR1, LOW); //Turn Off Red traffic light 1
  digitalWrite(LY1, LOW); //Turn Off Yellow traffic light 1
  digitalWrite(LG1, LOW); //Turn Off Green traffic light 1
  digitalWrite(LR2, LOW); //Turn Off Red traffic light 2
  digitalWrite(LY2, LOW); //Turn Off Yellow traffic light 2
  digitalWrite(LG2, LOW); //Turn Off Green traffic light 2

  lcd.begin();  //Start communications with LCD display
  lcd.backlight();  //Turn on LCD backlight
  
}



/////////////////////////////////////////////////// MEF DIA_NOCHE

#define DIA 0
#define NOCHE  1

int eDiaNoche = DIA;
bool luz = 1;


void MEF_DiaNoche() {
  switch (eDiaNoche) {
    case DIA:
      luz=1;
      if(analogRead(LDR1)<=270 && analogRead(LDR2)<=270){
        eDiaNoche=NOCHE;
        Serial.println("Cambio noche");
      }
      break;
    case NOCHE:
      luz=0;
      if(analogRead(LDR1)>270 || analogRead(LDR2)>270){
        eDiaNoche=DIA;
        Serial.println("Cambio dia");
      }
      break;
  }
}


/////////////////////////////////////////////////// MEF SALUDABLE

#define SALUDABLE 0
#define NIVEL_0 2
#define NO_SALUDABLE  1

int eSaludable = SALUDABLE;
bool saludable = 1;
const unsigned long TCO2 = 2000; // Constante para el tiempo de apagado   1000 ms
const unsigned long LIMITE_CO2 = 9; //TODO cambiar a 7 PARA DEMOSTRACIONES
unsigned long trsalud;

void MEF_Saludable() {
  
  switch (eSaludable) {
    case SALUDABLE:
      
      saludable=1;
      if(analogRead(CO2)>LIMITE_CO2 && (millis() - trsalud > TCO2) ){
        eSaludable=NIVEL_0;
        trsalud=millis();
        Serial.println("NIVEL_0");
        Serial.println(String(analogRead(CO2)));
      }
      break;
    case NIVEL_0:
      saludable=1;
      if(analogRead(CO2)<=LIMITE_CO2 && (millis() - trsalud > TCO2)){
        eSaludable=SALUDABLE;
        trsalud=millis();
      }else if(analogRead(CO2)>LIMITE_CO2&&(millis() - trsalud > TCO2)  ){
        eSaludable=NO_SALUDABLE;
        Serial.println("NO_SALUDABLE");
        trsalud=millis();
      }
      break;
    case NO_SALUDABLE:
      saludable=0;
      if(analogRead(CO2)<=LIMITE_CO2){
        eSaludable=SALUDABLE;
        Serial.println("SALUDABLE");
        trsalud=millis();
      }
      break;
  }
}

/////////////////////////////////////////////////// MEF Boton1

#define SUELTO 0
#define PRESIONADO 1

int eBoton1 = SUELTO;
bool b1 = 0;
const unsigned long trb = 10000; // constante para el holding del boton

unsigned long trb1;

void MEF_Boton1() {
  
  switch (eBoton1) {
    case SUELTO:
      b1=0;
      if (digitalRead(P1) == HIGH) {
        eBoton1=PRESIONADO;
        trb1=millis();
        while (digitalRead(P1) == HIGH) {} //Debouncing
        Serial.println("PRESIONADO"); 
      }
      break;
    case PRESIONADO:
      b1=1;
      if( (millis() - trb1 > trb)){
         trb1=millis();
         eBoton1=SUELTO;
         Serial.println("SUELTO");
      }
      break;
  }
}

/////////////////////////////////////////////////// MEF Boton2

int eBoton2 = SUELTO;
unsigned long trb2;
bool b2 = 0;

void MEF_Boton2() {
  
  switch (eBoton2) {
    case SUELTO:
      b2=0;
      if (digitalRead(P2) == HIGH) {
        eBoton2=PRESIONADO;
        trb2=millis();
        while (digitalRead(P2) == HIGH) {} //Debouncing
        Serial.println("PRESIONADO2"); 
      }
      break;
    case PRESIONADO:
      b2=1;
      if( (millis() - trb2 > trb)){
         trb2=millis();
         eBoton2=SUELTO;
         Serial.println("SUELTO2");
      }
      break;
  }
}

/////////////////////////////////////////////////// MEF CNYS
//////////////////////////////lecturas de los sensores de proximidad
const unsigned long trbCNY = 4000; // constante para el holding del CNY
unsigned long CNYSEMAFORO1=0;
unsigned long CNYREAD1=0;
unsigned long CNYREAD2=0;
unsigned long CNYREAD3=0;
unsigned long CNYREAD4=0;
unsigned long CNYREAD5=0;
unsigned long CNYREAD6=0;

/////////////////////////////////////////////////// MEF CNY1

int eCNY1 = SUELTO;
unsigned long trsCNY1;

void MEF_CNY1() {
  
  switch (eCNY1) {
    case SUELTO:
      CNYREAD1=0;
      if (digitalRead(CNY1)== LOW) {
        eCNY1=PRESIONADO;
        trsCNY1=millis();
        Serial.println("CNY1 PRESIONADO"); 
      }
      break;
    case PRESIONADO:
      CNYREAD1=1;
      if( (millis() - trsCNY1 > trbCNY )){
         trsCNY1=millis();
         eCNY1=SUELTO;
         Serial.println("CNY1 SUELTO");
      }
      break;
  }
}
/////////////////////////////////////////////////// MEF CNY2

int eCNY2 = SUELTO;
unsigned long trsCNY2;

void MEF_CNY2() {
  switch (eCNY2) {
    case SUELTO:
      CNYREAD2 = 0;
      if (digitalRead(CNY2) == LOW) {
        eCNY2 = PRESIONADO;
        trsCNY2 = millis();
        Serial.println("CNY2 PRESIONADO");
      }
      break;
    case PRESIONADO:
      CNYREAD2 = 1;
      if (millis() - trsCNY2 > trbCNY) {
        trsCNY2 = millis();
        eCNY2 = SUELTO;
        Serial.println("CNY2 SUELTO");
      }
      break;
  }
}


/////////////////////////////////////////////////// MEF CNY3
int eCNY3 = SUELTO;
unsigned long trsCNY3;

void MEF_CNY3() {
  switch (eCNY3) {
    case SUELTO:
      CNYREAD3 = 0;
      if (digitalRead(CNY3) == LOW) {
        eCNY3 = PRESIONADO;
        trsCNY3 = millis();
        Serial.println("CNY3 PRESIONADO");
      }
      break;
    case PRESIONADO:
      CNYREAD3 = 1;
      if (millis() - trsCNY3 > trbCNY) {
        trsCNY3 = millis();
        eCNY3 = SUELTO;
        Serial.println("CNY3 SUELTO");
      }
      break;
  }
}

/////////////////////////////////////////////////// MEF CNY4
int eCNY4 = SUELTO;
unsigned long trsCNY4;

void MEF_CNY4() {
  switch (eCNY4) {
    case SUELTO:
      CNYREAD4 = 0;
      if (digitalRead(CNY4) == LOW) {
        eCNY4 = PRESIONADO;
        trsCNY4 = millis();
        Serial.println("CNY4 PRESIONADO");
      }
      break;
    case PRESIONADO:
      CNYREAD4 = 1;
      if (millis() - trsCNY4 > trbCNY) {
        trsCNY4 = millis();
        eCNY4 = SUELTO;
        Serial.println("CNY4 SUELTO");
      }
      break;
  }
}

/////////////////////////////////////////////////// MEF CNY5
int eCNY5 = SUELTO;
unsigned long trsCNY5;

void MEF_CNY5() {
  switch (eCNY5) {
    case SUELTO:
      CNYREAD5 = 0;
      if (digitalRead(CNY5) == LOW) {
        eCNY5 = PRESIONADO;
        trsCNY5 = millis();
        Serial.println("CNY5 PRESIONADO");
      }
      break;
    case PRESIONADO:
      CNYREAD5 = 1;
      if (millis() - trsCNY5 > trbCNY) {
        trsCNY5 = millis();
        eCNY5 = SUELTO;
        Serial.println("CNY5 SUELTO");
      }
      break;
  }
}

/////////////////////////////////////////////////// MEF CNY6

int eCNY6 = SUELTO;
unsigned long trsCNY6;

void MEF_CNY6() {
  switch (eCNY6) {
    case SUELTO:
      CNYREAD6 = 0;
      if (digitalRead(CNY6) == LOW) {
        eCNY6 = PRESIONADO;
        trsCNY6 = millis();
        Serial.println("CNY6 PRESIONADO");
      }
      break;
    case PRESIONADO:
      CNYREAD6 = 1;
      if (millis() - trsCNY6 > trbCNY) {
        trsCNY6 = millis();
        eCNY6 = SUELTO;
        Serial.println("CNY6 SUELTO");
      }
      break;
  }
}




unsigned long t1; //tiempo para el cambio de estado1 a estado2 del semaforo
unsigned long t3; //tiempo para el cambio de estado3 a estado4 del semaforo
const unsigned long TIEMPO_CAMBIO_SEMAFORO = 10000; // constante para tiempo cambio semaforos
const unsigned long AJUSTE_POR_CONGESTION=1000;
const unsigned long MODIFICACION_POR_PRESION_BOTON=6000;
void actualizacionVariablesTiempo(){
   CNYSEMAFORO1 = CNYREAD1 +CNYREAD2+CNYREAD3;

   t1 = TIEMPO_CAMBIO_SEMAFORO + (AJUSTE_POR_CONGESTION*(CNYREAD1)) + (AJUSTE_POR_CONGESTION*(CNYREAD2)) + (AJUSTE_POR_CONGESTION*(CNYREAD3))- b1*MODIFICACION_POR_PRESION_BOTON + b2*MODIFICACION_POR_PRESION_BOTON ;
   t3 = TIEMPO_CAMBIO_SEMAFORO + (AJUSTE_POR_CONGESTION*(CNYREAD4)) + (AJUSTE_POR_CONGESTION*(CNYREAD5)) + (AJUSTE_POR_CONGESTION*(CNYREAD6))+ b1*MODIFICACION_POR_PRESION_BOTON - b2*MODIFICACION_POR_PRESION_BOTON ;
 
}


/////////////////////////////////////////////////// MEF Semaforos

void setSemaforo1(int rojo,int amarillo,int verde){
      digitalWrite(LR1, rojo);
      digitalWrite(LY1, amarillo);
      digitalWrite(LG1, verde);
}

void setSemaforo2(int rojo,int amarillo,int verde){
      digitalWrite(LR2, rojo);
      digitalWrite(LY2, amarillo);
      digitalWrite(LG2, verde);
}


#define ESEM1 0
#define ESEM2 1
#define ESEM3 2
#define ESEM4 3
#define ESEM5 4

long tt = 2000; // tiempos entre trancisiones intermedias
int eSemaforos = ESEM1;
unsigned long trsSemaforo;

void MEF_Semaforos() {
  switch (eSemaforos) {
    case ESEM1:
      setSemaforo1(0,0,1);
      setSemaforo2(1,0,0);
      if (millis() - trsSemaforo > t1) {
        trsSemaforo = millis();
        eSemaforos = ESEM2;
        Serial.println("ESEM2");
      }else if(saludable==0){
        trsSemaforo = millis();
        eSemaforos = ESEM3;
        Serial.println("ESEM3");
      }else if(luz==0 && saludable==1){
        trsSemaforo = millis();
        eSemaforos = ESEM5;
        Serial.println("ESEM5"); 
      }
      break;
      
    case ESEM2:
      setSemaforo1(0,1,0);
      setSemaforo2(1,0,0);

      if (millis() - trsSemaforo > tt) {
        trsSemaforo = millis();
        eSemaforos = ESEM3;
        Serial.println("ESEM3");
      }else if(luz==0&&saludable==1){
        trsSemaforo = millis();
        eSemaforos = ESEM5;
        Serial.println("ESEM5");
      }
      break;
      
    case ESEM3:
      setSemaforo1(1,0,0);
      setSemaforo2(0,0,1);
      if (millis() - trsSemaforo > t3) {
        trsSemaforo = millis();
        eSemaforos = ESEM4;
        Serial.println("ESEM4");
      }else if(luz==0&&saludable==1){
        trsSemaforo = millis();
        eSemaforos = ESEM5;
        Serial.println("ESEM5");
      }
      break;
      
    case ESEM4:
      setSemaforo1(1,0,0);
      setSemaforo2(0,1,0);
      if (millis() - trsSemaforo > tt) {
        trsSemaforo = millis();
        eSemaforos = ESEM1;
        Serial.println("ESEM1");
      }else if(luz==0&&saludable==1){
        trsSemaforo = millis();
        eSemaforos = ESEM5;
        Serial.println("ESEM5");
      }
      break;
      
    case ESEM5:
      setSemaforo1(0,1,0);
      setSemaforo2(0,1,0);
      if (saludable==0 || luz ==1 ) {
        trsSemaforo = millis();
        eSemaforos = ESEM3;
        Serial.println("ESEM3");
      }
      break;
  }
}


void displayInfo(){

      lcd.setCursor(0, 0);
      long int carrosActuales=CNYREAD1+CNYREAD2+CNYREAD3+CNYREAD4+CNYREAD5+CNYREAD6;
      lcd.print("Congestion Actual "+String(carrosActuales));
      lcd.setCursor(0, 1);
      lcd.print("Calle libre "+String(CNYREAD1+CNYREAD2+CNYREAD3));
      lcd.setCursor(0, 2);
      lcd.print("Calle tunel "+String(CNYREAD4+CNYREAD5+CNYREAD6));
      //lcd.setCursor(0, 1);
      //lcd.print("Hello World2");
      //delay(2000);  //Wait 2 seconds
      //lcd.clear();
        
  
}


byte clima = 0; //0 para despejado, 1 para lloviendo
void serialInput(){
  if (Serial.available() > 0) {    
    clima = Serial.read(); // read the incoming byte:
    if (clima != -1) { // -1 means no data is available
      lcd.setCursor(0, 3); // set cursor to first row
      lcd.print(clima); // print out to LCD
      tt=2000+(clima*4000);
    }
  }
}


 

void setup() {
  setupConnections();

   //Communications
  Serial.begin(9600); //Start Serial communications with computer via Serial0 (TX0 RX0) at 9600 bauds
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  
  Serial.println("In order to test Analog Sensors and Actuators, send the following commands through the Serial Monitor:");
  Serial.println("A: Tests LDR1 Sensor");
  Serial.println("B: Tests LDR2 Sensor");
  Serial.println("C: Test CO2 Sensor");
  Serial.println("D: Tests Traffic Light 1 (LR1, LY1 and LG1)");
  Serial.println("E: Tests Traffic Light 2 (LR2, LY2 and LG2)");
  Serial.println("F: Tests LCD");
  trsalud=millis();
  trsSemaforo=millis();
  
}

void loop() {
  MEF_DiaNoche();
  MEF_Saludable();
  MEF_Boton1();
  MEF_Boton2();
  actualizacionVariablesTiempo();
  MEF_CNY1();
  MEF_CNY2();
  MEF_CNY3();
  MEF_CNY4();
  MEF_CNY5();
  MEF_CNY6();
  MEF_Semaforos();
  displayInfo();
  serialInput();
}
