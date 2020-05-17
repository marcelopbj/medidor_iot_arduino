#include "SPI.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64

#define OLED_RESET     LED_BUILTIN
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10

#define LOGO_HEIGHT 16
#define LOGO_WIDTH  16

const unsigned char PROGMEM ladee_logo [] = { 
0x00, 0x00, 0x00, 0x3F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x01, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x1C, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x04, 0x00, 0x00,
0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x30, 0x00, 0xC0, 0x00,
0x00, 0x04, 0x00, 0x00, 0xF0, 0x00, 0x20, 0x00, 0x00, 0x18, 0x01, 0x80, 0xE0, 0x00, 0x10, 0x00,
0x00, 0x20, 0x03, 0x80, 0xE0, 0x00, 0x0C, 0x00, 0x00, 0x40, 0x01, 0xF0, 0xE0, 0x00, 0x02, 0x00,
0x00, 0x80, 0x00, 0xFC, 0xE0, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x01, 0x00,
0x01, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x02, 0xF8, 0x00, 0x00, 0x40,
0x04, 0x00, 0x20, 0x0D, 0xFF, 0x30, 0x00, 0x20, 0x00, 0x00, 0x00, 0x05, 0xF3, 0xF0, 0x00, 0x20,
0x08, 0x00, 0x01, 0x84, 0xE0, 0x70, 0x00, 0x10, 0x10, 0x00, 0x01, 0xC0, 0xE0, 0x00, 0x00, 0x10,
0x10, 0x00, 0x01, 0x84, 0xF0, 0x00, 0x00, 0x08, 0x20, 0x00, 0x03, 0x8C, 0xE0, 0x00, 0x00, 0x08,
0x20, 0x00, 0x07, 0xDC, 0xE0, 0x00, 0x00, 0x04, 0x20, 0x00, 0x07, 0xF6, 0xE0, 0x00, 0x00, 0x04,
0x40, 0x00, 0x07, 0xE4, 0xE0, 0x00, 0x00, 0x00, 0x40, 0x00, 0x07, 0xF0, 0xE0, 0x00, 0x00, 0x02,
0x40, 0x00, 0x07, 0xFF, 0xE0, 0x00, 0x00, 0x02, 0x00, 0x00, 0x07, 0xFF, 0xE0, 0x00, 0x00, 0x02,
0x80, 0x00, 0x03, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x80, 0x00, 0x03, 0xFC, 0xE0, 0x00, 0x00, 0x01,
0x80, 0x00, 0x03, 0xFE, 0xE0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x3E, 0xE0, 0x00, 0x00, 0x01,
0x80, 0x00, 0x00, 0x1E, 0xE0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x1E, 0xE0, 0x00, 0x00, 0x01,
0x80, 0x00, 0x00, 0x0E, 0xE0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x01,
0x80, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x01,
0x80, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
0x40, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,
0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,
0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8,
0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0xF9, 0xFC, 0x78, 0x18, 0x08, 0x1F, 0xF0,
0x07, 0xF9, 0xFC, 0x38, 0x08, 0x98, 0x1F, 0xE0, 0x07, 0xF9, 0xF8, 0x39, 0xC8, 0xF8, 0xFF, 0xC0,
0x03, 0xF9, 0xF9, 0x39, 0xC8, 0x18, 0x1F, 0xC0, 0x01, 0xF9, 0xF0, 0x19, 0x88, 0xF9, 0xFF, 0x80,
0x00, 0xF8, 0x10, 0x18, 0x08, 0x18, 0x1F, 0x00, 0x00, 0xF8, 0x03, 0x88, 0x18, 0x08, 0x1E, 0x00,
0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00,
0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00,
0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM teste_32x32[] = {
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000111,B10000000,B00000000,
  B00000000,B01111111,B11111000,B00000000,
  B00000001,B11111000,B01111100,B00000011,
  B00000011,B11000000,B00001110,B00000111,
  B00001111,B00000000,B00000000,B00001110,
  B00001100,B00000000,B00000000,B00011100,
  B00011000,B00000000,B00000000,B00111000,
  B00111000,B00000000,B00000000,B01110000,
  B00110000,B00000000,B00000000,B11100000,
  B01100000,B00000000,B00000001,B11000000,
  B01100000,B00100000,B00000011,B10000000,
  B11100000,B01110000,B00000111,B00000000,
  B11000000,B00111000,B00001110,B00000000,
  B11000000,B00011100,B00011100,B00011000,
  B11000000,B00001110,B00111000,B00011000,
  B11000000,B00000111,B01110000,B00011000,
  B11000000,B00000011,B11100000,B00011000,
  B11000000,B00000001,B11000000,B00011000,
  B11100000,B00000000,B00000000,B00011000,
  B01100000,B00000000,B00000000,B00011000,
  B01100000,B00000000,B00000000,B00110000,
  B00110000,B00000000,B00000000,B00110000,
  B00110000,B00000000,B00000000,B01100000,
  B00011000,B00000000,B00000000,B11100000,
  B00001100,B00000000,B00000001,B11000000,
  B00001111,B00000000,B00000011,B10000000,
  B00000011,B11000000,B00001111,B00000000,
  B00000001,B11111000,B11111100,B00000000,
  B00000000,B01111111,B11110000,B00000000,
  B00000000,B00000111,B10000000,B00000000,
  B00000000,B00000000,B00000000,B00000000
};

const unsigned char PROGMEM curva_32x32[] = {
  B00000000,B00000000,B00000000,B00111100,
  B00000000,B00000000,B00000000,B00111100,
  B00000000,B00000000,B00000000,B01100110,
  B00000000,B00000000,B00000000,B01100110,
  B00000000,B00000000,B00000000,B01111100,
  B00000000,B00011000,B00000000,B11000000,
  B00000000,B00111100,B00000001,B10000000,
  B00000000,B01100110,B00000011,B00000000,
  B00000000,B01100111,B00011110,B00000000,
  B00000000,B01111111,B11111110,B00000000,
  B00000000,B11111000,B01100110,B00000000,
  B00000001,B10000000,B01100100,B01111110,
  B00111111,B00000000,B00111100,B01100110,
  B00110110,B00000000,B00000000,B01100110,
  B01100110,B00000000,B00000000,B01100110,
  B00100110,B00000000,B00000000,B01100110,
  B00111100,B00111110,B00000000,B01100110,
  B00000000,B01100110,B00000000,B01100110,
  B00000000,B01100110,B00000000,B01100110,
  B00000000,B01100110,B01111110,B01100110,
  B00000000,B01100110,B01100110,B01100110,
  B00000000,B01100110,B01100110,B01100110,
  B00000000,B01100110,B01100110,B01100110,
  B01111110,B01100110,B01100110,B01100110,
  B01100110,B01100110,B01100110,B01100110,
  B01100110,B01100110,B01100110,B01100110,
  B01100110,B01100110,B01100110,B01100110,
  B01100110,B01100110,B01100110,B01100110,
  B01100110,B01100110,B01100110,B01100110,
  B01100110,B01100110,B01100110,B01100110,
  B01100110,B01100110,B01100110,B01100110,
  B11111111,B11111111,B11111111,B11111111
};

const unsigned char PROGMEM wifi_48x48[] = {
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,
  B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,
  B00000000,B00001111,B11111111,B11111111,B11110000,B00000000,
  B00000000,B00111111,B11111111,B11111111,B11111100,B00000000,
  B00000000,B11111111,B11000000,B00000011,B11111111,B00000000,
  B00000001,B11111110,B00000000,B00000000,B01111111,B10000000,
  B00000111,B11110000,B00000000,B00000000,B00001111,B11100000,
  B00001111,B11100000,B00000000,B00000000,B00000111,B11110000,
  B00011111,B10000000,B00011111,B11111000,B00000001,B11111000,
  B00111111,B00000000,B11111111,B11111111,B00000000,B11111100,
  B01111100,B00000011,B11111111,B11111111,B11000000,B00111110,
  B11111000,B00001111,B11111111,B11111111,B11110000,B00011111,
  B11110000,B00111111,B11100000,B00000111,B11111100,B00001111,
  B11100000,B01111111,B00000000,B00000000,B11111110,B00000111,
  B01000000,B11111100,B00000000,B00000000,B00111111,B00000010,
  B00000001,B11111000,B00000000,B00000000,B00011111,B10000000,
  B00000011,B11100000,B00000111,B11100000,B00000111,B11000000,
  B00000011,B11000000,B00111111,B11111100,B00000011,B11000000,
  B00000011,B10000000,B11111111,B11111111,B00000001,B11000000,
  B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,
  B00000000,B00000011,B11111000,B00011111,B11000000,B00000000,
  B00000000,B00000111,B11000000,B00000011,B11100000,B00000000,
  B00000000,B00000011,B10000000,B00000001,B11000000,B00000000,
  B00000000,B00000001,B00000000,B00000000,B10000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000011,B11000000,B00000000,B00000000,
  B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,
  B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,
  B00000000,B00000000,B00011111,B11111000,B00000000,B00000000,
  B00000000,B00000000,B00011110,B01111000,B00000000,B00000000,
  B00000000,B00000000,B00011100,B00111000,B00000000,B00000000,
  B00000000,B00000000,B00011110,B01111000,B00000000,B00000000,
  B00000000,B00000000,B00011111,B11111000,B00000000,B00000000,
  B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,
  B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,
  B00000000,B00000000,B00000011,B11000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};

void display_init(void)
{ 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {  
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

void init_logo(void)
{    
  display.clearDisplay();
  display.drawBitmap(32, 0, ladee_logo, 64, 64, 1);
  display.display();
  delay(2000);
}

void print_menu(void)
{
  display.clearDisplay();
  display.drawBitmap(17, 0, teste_32x32, 32, 32, 1);
  display.setCursor(55, 6);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("TESTE DE");
  display.setCursor(55, 18);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("PRECISAO");
  display.drawBitmap(17, 32, curva_32x32, 32, 32, 1);
  display.setCursor(55, 41);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("CURVA");
  display.setCursor(55, 54);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("DE CARGA");
  display.display();
}

void print_teste_precisao(void)
{
  display.clearDisplay();
  display.drawBitmap(17, 16, teste_32x32, 32, 32, 1);
  display.setCursor(55, 22);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("TESTE DE");
  display.setCursor(55, 34);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("PRECISAO");
  display.display();
}


void print_curva_carga(void)
{
  display.clearDisplay();
  display.drawBitmap(17, 16, curva_32x32, 32, 32, 1);
  display.setCursor(55, 22);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("CURVA");
  display.setCursor(55, 34);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("DE CARGA");
  display.display();
}

void print_conectado_wifi(String nome_wifi)
{
  display.clearDisplay();
  display.drawBitmap(40, 0, wifi_48x48, 48, 48, 1);
  display.setCursor(28, 48);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Conectado em:");
  display.setCursor(40, 56);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(nome_wifi);
  display.display();
  delay(2000); 
}

void print_conectando_wifi()
{
  display.clearDisplay();
  display.drawBitmap(40, 0, wifi_48x48, 48, 48, 1);
  display.setCursor(28, 52);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Conectando...");
  display.display();
}

void print_dados_teste(int pulsos, float constante)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Constante:");
  display.setCursor(0, 10);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(constante);
  display.println("Wh");
  display.setCursor(0, 32);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Pulsos de precisao:");
  display.setCursor(0, 42);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(pulsos);
  display.println(" pulsos");
  display.display();
}

void print_dados_curva(String nome_curva, float constante)// float potencia_media
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Curva:");
  display.println(nome_curva);
  display.print("Constante:");
  display.print(constante);
  display.println("Wh");
  display.println("Intervalo: ");//inserir periodo
  display.println("Periodo: ");//inserir periodo

  display.println("Potencia media:");
  display.setCursor(0, 42);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("9999");//print(potencia_media)
  display.println("W");
  display.display();
}

void print_calibrando()
{
  display.clearDisplay();
  display.setCursor(3, 20);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("Calibrando");
  display.setCursor(25, 25);
  display.setTextSize(4);
  display.setTextColor(WHITE);

  if (color_sense_get_pulses() == 0)
    display.println("...");

  else if (color_sense_get_pulses() == 1)
    display.println("..");
    
  else if (color_sense_get_pulses() == 2)
    display.println(".");
  
  display.display(); 
}

void print_contagem(int numero)
{
  display.clearDisplay();
  display.setCursor(20, 0);
  display.setTextSize(9);
  display.setTextColor(WHITE);
  display.println(numero);
  display.display(); 
}

void print_potencia(float potencia)
{
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Potencia media:");
  display.setCursor(0, 32);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(potencia);
  display.print("W");
  display.display(); 
}

/*
void print_display (uint32_t pulses, float potencia){
  
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Pulsos:");
  display.println(pulses);
  display.println(" ");
  display.println("Potencia:");
  display.print(potencia);
  display.println("W");
  
  display.display();
  
  Serial.print("Pulsos:");
  Serial.println(pulses);
  Serial.println("Potencia média:");
  Serial.println(potencia);
  
  }
*/
