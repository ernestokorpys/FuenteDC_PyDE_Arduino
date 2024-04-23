#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definiciones de pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define ADS1115_ADDRESS_ADDR_GND 0x48

// Objetos y variables globales
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_ADS1115 ads;
Adafruit_MCP4725 dac;

// Variables para almacenar valores del ADC
int16_t adc0, adc1;

// Direcciones de los dispositivos I2C
#define ADS1115_ADDRESS 0x48
#define MCP4725_ADDRESS 0x60
#define DAC_RESOLUTION 4095.0
#define ADC_RESOLUTION 26666.0   // Valor Practico diferente a valor real que es 2^16 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Inicialización de display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  initDisplay();
  
  // Inicialización de ADC y DAC
  ads.begin(ADS1115_ADDRESS);
  dac.begin(MCP4725_ADDRESS);
}

void loop() {
  // Leer valores del ADC
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);

  // Convertir valores a voltaje (referencia de 5V)
  float voltage0 = adc0 * (5.0 / ADC_RESOLUTION);   // Convierte el valor del ADC0 a voltaje
  float voltage1 = adc1 * (5.0 / ADC_RESOLUTION);   // Convierte el valor del ADC1 a voltaje
  float aux= adc1*(DAC_RESOLUTION/ADC_RESOLUTION);  //Calculo auxiliar debe hacerse el calculo con floats
  int dacValue = aux;                               // Valor de bits enviados al dac en int
  float volt_ref= dacValue*(5.0 / DAC_RESOLUTION);
  // Mostrar valores en el monitor serial OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("ADC0: ");
  display.print(adc0);
  display.print("  V: ");
  display.println(voltage0, 3); // Mostrar voltaje con 3 decimales
  display.setCursor(0, 10);
  display.print("ADC1: ");
  display.print(adc1);
  display.print("  V: ");
  display.println(voltage1, 3); // Mostrar voltaje con 3 decimales
  display.setCursor(0, 20);
  display.print("DAC: ");
  display.print(dacValue);
  display.print("  V:");
  display.println(volt_ref, 3); // Mostrar voltaje con 3 decimales
  display.display();
  dac.setVoltage(dacValue, false); // Enviar valor al DAC
  
  delay(1000); // Esperar un segundo antes de la próxima lectura
}

void initDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hola Erne");
  display.setCursor(0, 10);
  display.println("Bienvenido");
  display.display();
  delay(100);
}
