#include <Wire.h> 
#include <DS3232RTC.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3232RTC myRTC;

#include <dht11.h>
#define SENSOR_PIN 2

const int soglia_critica = 300; //Soglia
const int PIN_Terreno = A3;

const int PIN_Motore = 3;
const int PIN_Ventola = 4;
const int PIN_Led_V = 6;
const int PIN_VentolaCalda = 5;

const int button_Pin_Motore = 8;
const int button_Pin_Ventola = 9;
const int button_Pin_Led_Viola = 10;
const int button_Pin_VentolaCalda = 11;//Modalità automatica

int sensorValue = 0;
int ledb = 0; 
int ledv = 0; 
int ledr = 0; 
const int PIN_Livello_Acqua = A0;
int misurazione = 0;


  int buttonState0 = 0;
  int buttonState1 = 0;
  int buttonState2 = 0;
  int buttonState3 = 0;

  int temperatura = 0;

  const int PIN_LEDVERDE = A1;
  const int PIN_LEDROSSO = A2;
  const int piezoPin = 7;
  const int ORA= "10";
  const int MINUTI = 00;
  const int SECONDI = 00;
void setup()
{
  pinMode(PIN_Motore,OUTPUT);
  pinMode(PIN_Ventola,OUTPUT);
  pinMode(PIN_Led_V,OUTPUT);
  pinMode(PIN_VentolaCalda,OUTPUT);

  pinMode(button_Pin_Motore, INPUT);
  pinMode(button_Pin_Ventola, INPUT);
  pinMode(button_Pin_Led_Viola, INPUT);
  pinMode(button_Pin_VentolaCalda, INPUT);

   
  Serial.begin(9600);
  
  myRTC.begin();

  THSensor.begin(SENSOR_PIN);
  
  lcd.init();                     // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Serra Domotica");
  lcd.setCursor(3,1);
  lcd.print("Ingeimaks ");
  delay(5000);
  lcd.clear();
  digitalWrite(PIN_Motore,LOW);
  digitalWrite(PIN_Ventola,LOW);
  digitalWrite(PIN_Led_V,LOW);
  digitalWrite(PIN_VentolaCalda,LOW);
  digitalWrite(PIN_LEDVERDE,LOW);
  digitalWrite(PIN_LEDROSSO,HIGH);
  
}


void loop()
{
  
buttonState0 = digitalRead(button_Pin_Motore);
buttonState1 = digitalRead(button_Pin_Ventola);
buttonState2 = digitalRead(button_Pin_Led_Viola);
buttonState3 = digitalRead(button_Pin_VentolaCalda);

    if (buttonState0 == HIGH) {    
    digitalWrite(PIN_Motore, HIGH);    
    delay(7000);
    digitalWrite(PIN_Motore, LOW);    
    Schermo();
  } else {    
    digitalWrite(PIN_Motore, LOW);
  }

if (buttonState1 == HIGH)ledb=ledb+1;
delay(70);
  if (ledb == 1) {    
    digitalWrite(PIN_Ventola, HIGH);           
  } else if(ledb == 2) {   
    digitalWrite(PIN_Ventola, LOW);
    ledb = 0;
    delay(500);
  } else if (ledb > 2)ledb = 0;


if (buttonState2 == HIGH)ledv=ledv+1;
delay(70);
  if (ledv == 1) {   
    digitalWrite(PIN_Led_V, HIGH);                   
  } else if(ledv == 2) {
    // turn LED off:
    digitalWrite(PIN_Led_V, LOW);
    ledv = 0;
    delay(500);
  } else if (ledv > 2)ledv = 0;







if (buttonState3 == HIGH)ledr=ledr+1;
delay(70);
  if (ledr == 1) {
    digitalWrite(PIN_LEDVERDE,HIGH);
  digitalWrite(PIN_LEDROSSO,LOW);
    // Modalità ON
    Serial.println("MOD");
    if (temperatura > 26){
    digitalWrite(PIN_Ventola, HIGH);
    digitalWrite(PIN_VentolaCalda, LOW);
    Serial.println("Temp NON OK");
    }else if (temperatura <= 19){
    digitalWrite(PIN_VentolaCalda, HIGH); 
    digitalWrite(PIN_Ventola, HIGH);    
    }else if (temperatura >=20 || temperatura <=26){
     Serial.println("Temp OK");
    digitalWrite(PIN_Ventola, LOW);
    digitalWrite(PIN_VentolaCalda, LOW);
    }
      
         
              
  } else if(ledr == 2) {
    // Modalità OFF
    digitalWrite(PIN_VentolaCalda, LOW);
    digitalWrite(PIN_Ventola, LOW);
    ledr = 0;
    delay(500);
    digitalWrite(PIN_LEDVERDE,LOW);
  digitalWrite(PIN_LEDROSSO,HIGH);
  
  } else if (ledr > 2)ledr = 0;


  misurazione = analogRead(PIN_Livello_Acqua);  
  if(misurazione<100){
  tone(piezoPin, 1000, 500);
  delay(500);
  tone(piezoPin, 500, 500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    RIEMPIRE");
  lcd.setCursor(0,1);
  lcd.print("   SERBATORIO");
  delay(3000);
  lcd.clear();
  
  }

  sensorValue = analogRead(PIN_Terreno);  
  int valoreUT = map(sensorValue,0,1023,0,100);

  if(valoreUT > 85){
    digitalWrite(PIN_Motore, HIGH);    
    delay(7000);
    digitalWrite(PIN_Motore, LOW);    
  }




    char buf[40];
    time_t t = myRTC.get();
    sprintf(buf, "%.2d:%.2d:%.2d %.2d %s ",hour(t), minute(t), second(t), day(t), monthShortStr(month(t)));
    lcd.setCursor(0,0);
    lcd.print(buf);
    //Serial.println(buf);

    if (ORA == hour(t) && MINUTI == minute(t) && SECONDI == second(t)){
      digitalWrite(PIN_Motore, HIGH);    
    delay(7000);
    digitalWrite(PIN_Motore, LOW);
      }

  int result;
  result = THSensor.read(); //Leggo i dati
  //Mostro risultato
  switch (result){
    case DHTLIB_OK:
        //Serial.print(THSensor.humidity, 1);
        //Serial.print(",\t\t");
        //Serial.print(THSensor.temperature, 1);  
        //Serial.println(",\t\tOK"); 
    lcd.setCursor(0,1);
    lcd.print("T=");
    temperatura = (int)THSensor.temperature;
    lcd.print(THSensor.temperature, 1);  
    lcd.print("C");  
    lcd.setCursor(8,1);
    lcd.print("U.TE=");
    sensorValue = analogRead(PIN_Terreno);
    lcd.print(map(sensorValue,0,1023,0,100));
    lcd.print("%");
       
      
        break;
    case DHTLIB_ERROR_CHECKSUM: 
        Serial.println("?,\t\t?,\t\tErrore Checksum"); 
        break;
    case DHTLIB_ERROR_TIMEOUT: 
        Serial.println("?,\t\t?,\t\tErrore Timeout"); 
        break;
    default: 
        Serial.println("?,\t\t?,\t\tErrore Sconosciuto"); 
        break;}
        //end swich
      
  
}

void Schermo (){
  ledr = 0  ;     
  lcd.clear();
  
  sensorValue = analogRead(PIN_Terreno);
  lcd.setCursor(0,0);
  lcd.print("Um.DHT11=");
  //lcd.print(map(sensorValue,0,1023,0,100));
  //lcd.print("%");
  lcd.print(THSensor.humidity, 1);
  lcd.print("%");    
  //Serial.println(sensorValue);
  lcd.setCursor(0,1);
  lcd.print("Pompa=");
  lcd.print("ON");
  lcd.setCursor(9,1);
  //misurazione = analogRead(PIN_Livello_Acqua);
  //lcd.print("A.=");
  //lcd.print(misurazione);
  
  delay(6000);  
  lcd.clear();  

  loop();
 }
