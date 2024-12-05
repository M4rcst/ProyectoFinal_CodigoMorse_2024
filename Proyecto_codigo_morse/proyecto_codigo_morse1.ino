#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define PIN_BOTON_MORSE 2
#define PIN_BOTON_CONFIRMAR 13
#define PIN_BUZZER 6
#define PIN_LED_CONFIRMAR 5
#define DURACION_PUNTO 200
#define DURACION_RAYA 600
#define DURACION_ESPACIO 1000
#define TIEMPO_ELIMINAR 1000

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

SoftwareSerial bluetooth(10, 11); // RX, TX

String entradaMorse = "";  
String palabraActual = ""; 

String codigosMorse[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", 
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", 
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----",
  ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." 
};

String letras[] = {
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", 
  "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

void setup() {
  pinMode(PIN_BOTON_MORSE, INPUT_PULLUP);
  pinMode(PIN_BOTON_CONFIRMAR, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_CONFIRMAR, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Codigo Morse");
  delay(2000);
  lcd.clear();
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  Serial.println("Bluetooth listo para enviar.");
}

void loop() {
  manejarEntradaMorse();
  manejarConfirmarEliminarOEspacio();
  if (Serial.available()) {
    char data = Serial.read(); // Lee un carácter del monitor serie
	bluetooth.print(data); // Envia el carácter al módulo Bluetooth
	Serial.print("Enviado: "); // Muestra en el monitor serie el dato
	enviado
	Serial.println(data);
   }
	
}

void manejarEntradaMorse() {
  if (digitalRead(PIN_BOTON_MORSE) == LOW) {
    delay(20); 
    unsigned long tiempoInicio = millis();
    
    while (digitalRead(PIN_BOTON_MORSE) == LOW); 
    unsigned long duracionPulsacion = millis() - tiempoInicio;

    if (duracionPulsacion < DURACION_PUNTO) {
      entradaMorse += ".";  
      tone(PIN_BUZZER, 1000, 100); 
    } else if (duracionPulsacion >= DURACION_PUNTO && duracionPulsacion <= DURACION_RAYA) {
      entradaMorse += "-"; 
      tone(PIN_BUZZER, 1000, 300); 
    }
    delay(300);  
  }
}

void manejarConfirmarEliminarOEspacio() {
  if (digitalRead(PIN_BOTON_CONFIRMAR) == LOW) {
    delay(20);  
    unsigned long tiempoInicio = millis();

    while (digitalRead(PIN_BOTON_CONFIRMAR) == LOW) {
      if (millis() - tiempoInicio > TIEMPO_ELIMINAR) {
        if (palabraActual.length() > 0) { 
          palabraActual.remove(palabraActual.length() - 1); 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Palabra:");
          lcd.setCursor(0, 1);
          lcd.print(palabraActual); 
        }
        delay(500);  
        return; 
      }
    }

    if (entradaMorse == "") {
      palabraActual += " "; 
    } else {
      String letra = morseACaracter(entradaMorse);
      if (letra != "?") {
        palabraActual += letra;  
      }
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Palabra:");
    lcd.setCursor(0, 1);
    lcd.print(palabraActual); 

    digitalWrite(PIN_LED_CONFIRMAR, HIGH);
    delay(500);
    digitalWrite(PIN_LED_CONFIRMAR, LOW);

    entradaMorse = "";  
    delay(500);
  }
}

String morseACaracter(String morse) {
  String resultado = "?";
  int tamanoAlfabeto = sizeof(codigosMorse) / sizeof(codigosMorse[0]);
  for (int i = 0; i < tamanoAlfabeto; i++) {
    if (codigosMorse[i] == morse) {
      resultado = letras[i];
      break;
    }
  }
  
  if (resultado == "?") {
    if (entradaMorse == "") {
      resultado = " ";  
    }
  }
  
  return resultado;
}
