### README (English)

#### Arduino-Based Digital Control Strategy for DC Power Supply

This repository contains Arduino (.ino) files implementing a digital control strategy for a DC power supply. Originally analog, the power supply's control system has been digitalized using Arduino. The control strategy employs a Proportional-Integral (PI) algorithm, allowing precise regulation. Additionally, a keypad interface facilitates parameter adjustment, while an OLED display, DAC, and opto-isolated ADC provide real-time monitoring and control.

#### Dependencies:
- **Libraries:**
  - `Key.h`
  - `Keypad.h`
  - `Wire.h`
  - `Adafruit_ADS1X15.h`
  - `Adafruit_GFX.h`
  - `Adafruit_SSD1306.h`
  - `Adafruit_MCP4725.h`

#### Components Used:
- Arduino Board
- Keypad
- OLED Display
- DAC (Digital-to-Analog Converter)
- ADC (Analog-to-Digital Converter)

#### Usage:
1. Connect the Arduino board to the components accordingly.
2. Upload the provided .ino files to the Arduino.
3. Adjust control parameters via the keypad interface.
4. Monitor and control the power supply via the OLED display.

### README (Español)

#### Estrategia de Control Digital para Fuente de Alimentación DC basada en Arduino

Este repositorio contiene archivos Arduino (.ino) que implementan una estrategia de control digital para una fuente de alimentación DC. Originalmente analógica, el sistema de control de la fuente de alimentación se ha digitalizado utilizando Arduino. La estrategia de control emplea un algoritmo Proporcional-Integral (PI), permitiendo una regulación precisa. Además, una interfaz de teclado facilita el ajuste de parámetros, mientras que una pantalla OLED, DAC y ADC optoaislados proporcionan monitoreo y control en tiempo real.

#### Dependencias:
- **Librerías:**
  - `Key.h`
  - `Keypad.h`
  - `Wire.h`
  - `Adafruit_ADS1X15.h`
  - `Adafruit_GFX.h`
  - `Adafruit_SSD1306.h`
  - `Adafruit_MCP4725.h`

#### Componentes Utilizados:
- Placa Arduino
- Teclado
- Pantalla OLED
- DAC (Conversor Digital-Analógico)
- ADC (Conversor Analógico-Digital)

#### Uso:
1. Conecta la placa Arduino a los componentes correspondientes.
2. Sube los archivos .ino proporcionados a Arduino.
3. Ajusta los parámetros de control a través de la interfaz de teclado.
4. Monitorea y controla la fuente de alimentación a través de la pantalla OLED.
