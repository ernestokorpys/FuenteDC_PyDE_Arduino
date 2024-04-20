#include <Key.h>
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128    // Ancho del display OLED, en píxeles
#define SCREEN_HEIGHT 32    // Alto del display OLED, en píxeles
#define OLED_RESET -1       // Pin de reset (o -1 si se comparte el pin de reset del Arduino)
#define SCREEN_ADDRESS 0x3C // Dirección del display OLED
#define ADS1115_ADDRESS_ADDR_GND 0x48

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_ADS1115 ads; // Creamos un objeto ADS1115

unsigned long lastScreenChangeTime = 0; // Variable para almacenar el tiempo del último cambio de pantalla
int SCREEN = 0; // Variable que indica la pantalla actual
int menuSelection = 1; // Variable que indica la selección actual del menú y ayuda a moverse
const float MAX_TENSION = 30.00;
const float MAX_CORRIENTE = 3.00;
float Tension = 0.00;
float Corriente = 0.000;
int Tiempo = 0;
int TiempoTranscurrido = 0;
char tensString[5] = "    "; // Almacenar la tensión como una cadena de caracteres de 4 dígitos
char corrienteString[5] = "    "; // Almacenar la corriente como una cadena de caracteres de 4 dígitos
bool edit_values = false;
int Mode=0; //0.Sin Modo; 1. Tensión; 2. Corriente; 3. Rampa Define el modo de funcionamiento
const byte ROWS = 4;
const byte COLUMNS = 4;
char keys[ROWS][COLUMNS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {22, 23, 24, 25};
byte columnPins[COLUMNS] = {26, 27, 28, 29};
Keypad teclado = Keypad(makeKeymap(keys), rowPins, columnPins, ROWS, COLUMNS);
char key;
volatile char lastKeyPress = ' '; // Variable para almacenar la última tecla pulsada
int listValue[8] = {0}; // Lista para almacenar los valores de tensión y corriente
int listRamp[6] = {0}; //Almacena valores tension y tiempo 
int index=0;
bool state=false;
void setup() {
  Serial.begin(9600);
  // Inicializamos el display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hola Erne");
  display.setCursor(0, 10);
  display.println("Presione un N°");
  display.display();
  delay(1000);
  // Inicializamos el ADS1115 con la dirección especificada
  ads.begin(ADS1115_ADDRESS_ADDR_GND); // Dirección del ADS1115 configurada en 0x48
}

void loop() {
  // Actualizar el estado de la pantalla cada 10 segundos
  key = teclado.getKey();
  if (key) {
    Serial.println(key);
    Keyboard(key);
    // Mostrar la tecla pulsada en el display OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    Serial.print("Tiempo: ");
    Serial.println(Tiempo);
    switch (SCREEN) {
      case 0:
        SCREEN0();
        break;
      case 1:
        SCREEN1();
        break;
      case 2:
        SCREEN2();
        break;
      case 3:
        SCREEN3();
        break;
      case 4:
        SCREEN4();
        break;
      case 5:
        SCREEN5();
        break;
    }
    display.display();
  }
}
void Keyboard(int key) {
  if (edit_values == true) {
    if (key >= '0' && key <= '9') {
      if(menuSelection!=3){
      updateValues(key);
      if (index == 8) { // Si se han ingresado todos los valores
        edit_values = false; // Finalizar la edición de valores
        if(updateFloatValues()){
          switch (menuSelection){
            case 1:
              SCREEN=1;
              break;
            case 2:
              SCREEN=2;
              break;
          }
        }else{
          SCREEN=4;
        } // Actualizar los valores de tensión y corriente
      }
      } else {
        updateRampValues(key);
         if (index == 6) { // Si se han ingresado todos los valores
          edit_values = false; // Finalizar la edición de valores
          if(updateRampFloatValues()){
            SCREEN=3;
          } else{
            SCREEN=4;
          }
        }
        //Funcion para cargar el valor de tension y el tiempo en seg
      }
    }
  }
  if (key == 'A' && SCREEN != 0) {
    edit_values = true;
    SCREEN = 5; // Cambiar a la pantalla de visualización de valores
    switch (menuSelection) {
      case 1: 
      index=0;
      break;
      case 2:
      index=4;
      break;
      case 3:
      index=0;
    }
    memset(tensString, ' ', sizeof(tensString)); // Limpiar el arreglo de la tensión
    memset(corrienteString, ' ', sizeof(corrienteString)); // Limpiar el arreglo de la corriente
    memset(listValue, 0, sizeof(listValue)); // Reiniciar la lista de valores
  }
  if (key == 'B' && SCREEN == 0) {
    menuSelection++;
    if (menuSelection > 3) {
      menuSelection = 1;
    }
  }
  if (key == 'C') {
    SCREEN = menuSelection;
  }
  if (key == 'D') {
    if (SCREEN==0){
      state=false;
      Mode=0;
    }
    SCREEN = 0;
  }
  if (key == '#' && SCREEN!=0) {
    state = true;
    switch (menuSelection){
      case 0:
        //No hace nada
        break;
      case 1:
        Mode=1;
        //Limita tension y corriente.
        break;
      case 2:
        Mode=2;
        //Limita tensión.
        break;
      case 3:
        Mode=3;
        //Hace rampa
        break;
      }
  }
}
// Displays mostrando cosas
void SCREEN0() {
  // Mostrar las opciones del menú
  display.setCursor(0, 0);
  if (menuSelection == 1) {
    display.print("* ");
  }
  display.println("1. Tension");
  display.setCursor(0, 10);
  if (menuSelection == 2) {
    display.print("* ");
  }
  display.println("2. Corriente");
  display.setCursor(0, 20);
  if (menuSelection == 3) {
    display.print("* ");
  }
  display.print("3. Rampa   ");
  switch (Mode) {
    case 0:
    display.print("OFF");
    break;
    case 1:
    display.print("T");
    break;
    case 2:
    display.print("C");
    break;
    case 3:
    display.print("R");
    break;
  }
}
void SCREEN1() {
  display.setCursor(0, 0);
  display.print("T. R:15.55 ");
  display.print("D: ");
  display.print(Tension); // Mostrar los dígitos ingresados para la tensión
  display.setCursor(0, 10);
  display.print("C. R:1.65 ");
  display.print("D: ");
  display.print(Corriente); // Mostrar los dígitos ingresados para la corriente
  display.setCursor(0, 20);
  display.print("MODO TENSION  #ON");
}

void SCREEN2() {
  display.setCursor(0, 0);
  display.print("Real: 1.5");
  display.setCursor(0, 10);
  display.print("Deseada: ");
  display.print(Corriente); // Mostrar los dígitos ingresados para la tensión
  display.setCursor(0, 20);
  display.print("MODO CORRIENTE  #ON");
}

void SCREEN3() {
  display.setCursor(0, 0);
  display.print("T. R:15.55 ");
  display.print("  D: ");
  display.print(Tension); // Mostrar los dígitos ingresados para la tensión
  display.setCursor(0, 10);
  display.print("t. R:");
  display.print(TiempoTranscurrido);
  display.print("  D:");
  display.print(Tiempo);
  display.setCursor(0, 20);
  display.print("MODO RAMPA");
}

void SCREEN4() {
  display.setCursor(0, 0);
  display.println("Valores Invalidos");
  display.setCursor(0, 10);
  display.println("Presione Tecla");
  switch (menuSelection) {
      case 1: 
      SCREEN=1;
      break;
      case 2:
      SCREEN=2;
      break;
    }
}

void SCREEN5() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  switch (menuSelection) {
    case 1:
      display.print("TENSION: ");
      for (int i = 0; i < 4; i++) {   // Mostrar los dígitos ingresados para la tensión
        display.print(tensString[i]);
      }
      // Mostrar el separador decimal
      display.print(".");
      display.print(tensString[2]); // Mostrar el tercer dígito como el decimal
      // Mostrar los dígitos ingresados para la corriente
      display.setCursor(0, 10);
      display.print("CORRIENTE: ");
      for (int i = 0; i < 4; i++) {
        display.print(corrienteString[i]);
      }
      // Mostrar el separador decimal
      display.print(".");
      display.print(corrienteString[1]); // Mostrar el segundo dígito como el decimal
      display.setCursor(0, 20);
      display.print("EDITAR VALORES: ");
      break;
      
    case 2:
      display.print("Corriente MAX: ");
      for (int i = 0; i < 4; i++) {
        display.print(corrienteString[i]);
      }
      break;
      
    case 3:
      display.print("TENSION: ");
      for (int i = 0; i < 4; i++) {
        display.print(tensString[i]);
      }
      // Mostrar el separador decimal
      display.print(".");
      display.print(tensString[2]); // Mostrar el tercer dígito como el decimal
      // Mostrar los dígitos ingresados para el tiempo
      display.setCursor(0, 10);
      display.print("TIEMPO: ");
      for (int i = 4; i < 6; i++) {
        display.print(corrienteString[i - 4]);
      }
      display.print(" s");
      break;
  }
}

void updateValues(int key) {
  if (index < 4) { // Si todavía estamos ingresando los valores de tensión
    tensString[index] = key; // Almacenar el valor en el arreglo de la tensión
  } else { // Si ya hemos terminado de ingresar los valores de tensión
    corrienteString[index - 4] = key; // Almacenar el valor en el arreglo de la corriente
  }
  listValue[index] = key - '0'; // Almacenar el valor en la lista
  index++;
}

bool updateFloatValues() {
  // Calcular los valores de tensión y corriente a partir de los valores ingresados
  float Tension2, Corriente2;
  Tension2 = (listValue[0] * 10 + listValue[1]) + (listValue[2] * 0.1) + (listValue[3] * 0.01);
  Corriente2 = (listValue[4] + (listValue[5] * 0.1) + (listValue[6] * 0.01) + (listValue[7] * 0.001));
  if (Tension2<=MAX_TENSION && Corriente2<=MAX_CORRIENTE){
    Tension=Tension2;
    Corriente=Corriente2;
    return true;
  }
  return false;
}

void updateRampValues(int key) {
  if (index < 4) { // Si todavía estamos ingresando los valores de tensión
    tensString[index] = key; // Almacenar el valor en el arreglo de la tensión
    listRamp[index] = key - '0'; // Almacenar el valor en la lista de rampa
  } else if (index >= 4 && index < 6) { // Si estamos ingresando los valores de tiempo
    corrienteString[index - 4] = key; // Almacenar el valor en el arreglo de tiempo
    listRamp[index] = key - '0'; // Almacenar el valor en la lista de rampa
  }
  index++;
}

bool updateRampFloatValues() {
  // Calcular los valores de tensión y tiempo a partir de los valores ingresados en listRamp
  float Tension2;
  int Tiempo2;
  Tension2 = (listRamp[0] * 10 + listRamp[1]) + (listRamp[2] * 0.1) + (listRamp[3] * 0.01);
  Tiempo2 = (listRamp[4] * 10 + listRamp[5]);
  
  // Verificar si los valores están en rango
  if (Tension2 <= MAX_TENSION && Tiempo2 >= 0) {
    Tension = Tension2;
    Tiempo = Tiempo2;
    Serial.print("TiempoCargado: ");
    Serial.println(Tiempo);
    return true; // Los valores están en rango y se cargaron a las variables
  }
  return false; // Al menos uno de los valores está fuera de rango
}


