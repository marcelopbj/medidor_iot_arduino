#include "SparkFun_APDS9960.h"
#include "SPI.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "ESP8266WiFi.h"
#include "FirebaseArduino.h"

#define APDS9960_SDA    D2
#define APDS9960_SDL    D1

#define FIREBASE_HOST "medidor-iot.firebaseio.com"
#define FIREBASE_AUTH "ndObRyKsvh2jidKaoZhn8LaqJgP3JIFfTD8Yj9y7"
#define WIFI_SSID "J.A.R.V.I.S."
#define WIFI_PASSWORD "05111970"

int init_time = 0;
int flag_1 = 0;
int flag_2 = 0; 
int total_time = 0;
float pot_total = 0;
int numero_pulsos = 0;
float valor_constante = 0;
String status_menu;
String status_teste;
String status_curva;
int periodo = 0;
float intervalo = 0;
int init_pulse;
String nome_curva;
int ultimo_pulse;
int ultimo_pulse_time;
int cod_set;
float xvalue = 0;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  display_init();
  init_logo();
  color_sensor_init();
  inicia_wifi();
  inicia_firebase();

}

void loop()
{  
  status_menu = Firebase.getString("/StatusMenu");

  if (status_menu == "MenuPrincipal")
    print_menu();

  else if (status_menu == "CurvaCarga")
  {
      status_curva = Firebase.getString("/CurvaCarga/Status");

      if (status_curva == "ObtendoValores")
        print_curva_carga();

      else if (status_curva == "ObtendoCurva")
      {
        valor_constante = Firebase.getInt("/CurvaCarga/ValorConstante");
        periodo = Firebase.getInt("/CurvaCarga/Periodo");
        nome_curva = Firebase.getString("/CurvaCarga/NomeCurva");
        print_dados_curva(Firebase.getString("/CurvaCarga/NomeCurva"), valor_constante); // potencia_media
        delay(2000);        
      }

      while (status_curva == "ObtendoCurva")
      {
        color_sensor_tick();

        if (init_time == 0)
        {
          init_time = millis();
        }
        
        else if (ultimo_pulse != color_sense_get_pulses() && (millis() - init_time) < (intervalo*60000))// transformar intervalo em milisegundos
        {
         ultimo_pulse = color_sense_get_pulses(); 
         ultimo_pulse_time = millis();
        }

        else if ((millis() - init_time) > (intervalo*60000))
        {
          total_time = ultimo_pulse_time - init_time;
          numero_pulsos = ultimo_pulse - init_pulse;
          pot_total = ((numero_pulsos + 1)*valor_constante*3600000)/(total_time);
         
          cod_set++;
          xvalue = xvalue+intervalo;
          Firebase.setFloat("/" + nome_curva + "/" + (String)cod_set + "/xValue", cod_set);
          Firebase.setFloat("/" + nome_curva + "/" + (String)cod_set + "/yValue", pot_total);
          intervalo = Firebase.getFloat("/CurvaCarga/Intervalo");
          init_time = 0;
          ultimo_pulse = color_sense_get_pulses();
         
        }
      }     
  }

  else if (status_menu == "TestePrecisao")
  {
    
    status_teste = Firebase.getString("/TestePrecisao/Status");       
    
    if (status_teste == "ObtendoValores")
      print_teste_precisao();

    else if (status_teste == "Proximo")
    {
      numero_pulsos = Firebase.getInt("/TestePrecisao/NumeroPulsos");
      valor_constante = Firebase.getFloat("/TestePrecisao/ValorConstante");
      print_dados_teste(numero_pulsos, valor_constante);
    }
      
    while (status_teste == "Iniciar")//mudar para realizando teste
    {
      color_sensor_tick();
  
      if (color_sense_get_pulses() < 3)
      {
        print_calibrando();
        Firebase.setString("/TestePrecisao/PulsosStatus", "Calibrando");
      } 

     // colocar color_sensor_tick(); antes de todo color_sense_get_pulses() aumenta a precisao?
      else if (color_sense_get_pulses() == 3 && flag_1 == 0)
      {
        init_time = millis(); 
        flag_1 = 1;
        print_contagem((color_sense_get_pulses() - 3));
        Firebase.setInt("/TestePrecisao/PulsosStatus", (color_sense_get_pulses() - 3));
      }
      else if (color_sense_get_pulses() > 3 && color_sense_get_pulses() < (numero_pulsos + 4))
      {
        print_contagem((color_sense_get_pulses() - 3));
        Firebase.setInt("/TestePrecisao/PulsosStatus", (color_sense_get_pulses() - 3));
      }
      else if (color_sense_get_pulses() == (numero_pulsos + 4) && flag_2 == 0)
      {
        flag_2 = 1;
        total_time = millis() - init_time;
        pot_total = ((numero_pulsos + 1)*valor_constante*3600000)/(total_time); 
        print_potencia(pot_total);
        Firebase.setString("/TestePrecisao/Status", "TesteFinalizado");
        Firebase.setFloat("/TestePrecisao/Potencia", pot_total);
      }
      reconecta_wifi();  
    }  
  }

  reconecta_wifi();
    
  /*
  else if (status == "proximo")
    print_dados(Firebase.getInt("/teste/pulsos"), Firebase.getFloat("/teste/constante")); 

  else if (status == "iniciar")
  {
    color_sensor_tick();
    Serial.print("Pulsos: ");
    Serial.println(color_sense_get_pulses());
    
    if (color_sense_get_pulses() < 3)
    {
      print_calibrando();
      Serial.print("Calibrando: ");
      Serial.println(color_sense_get_pulses());
      Firebase.setString("/teste/mensagem", "Calibrando...");
    }
  
    else if (color_sense_get_pulses() == 3 && flag_1 == 0)
    {
      init_time = millis(); 
      flag_1 = 1;
      Serial.print("Iniciando contagem");
      print_contagem((color_sense_get_pulses() - 3));
      Firebase.setInt("/teste/mensagem", (color_sense_get_pulses() - 3));
      
    }

    else if (color_sense_get_pulses() > 3 && color_sense_get_pulses() < (Firebase.getInt("/teste/pulsos") + 4))
    {
      Serial.print("Contando: ");
      Serial.println(color_sense_get_pulses() - 3);
      print_contagem((color_sense_get_pulses() - 3));
      Firebase.setInt("/teste/mensagem", (color_sense_get_pulses() - 3));
    }

    else if (color_sense_get_pulses() == (Firebase.getInt("/teste/pulsos") + 4) && flag_2 == 0)
    {
      flag_2 = 1;
      total_time = millis() - init_time;
      pot_total = ((Firebase.getInt("/teste/pulsos") + 1)*Firebase.getInt("/teste/constante")*3600000)/(total_time);

      Serial.print("Potencia total:");
      Serial.println(pot_total); 
      print_potencia(pot_total);
      Firebase.setFloat("/teste/mensagem", pot_total);
    }
  }
  */
  
}

void inicia_wifi(void)
{
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando...");
  print_conectando_wifi();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Conectando: ");
  print_conectado_wifi(WIFI_SSID);
  Serial.println(WiFi.localIP());  
}

void reconecta_wifi(void)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();
  }
}

void inicia_firebase()
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  Firebase.setFloat("/TestePrecisao/ValorConstante", 0);
  Firebase.setString("/TestePrecisao/Mensagem", "");
  Firebase.setString("/TestePrecisao/Potencia", "");
  Firebase.setInt("/TestePrecisao/NumeroPulsos", 0);
  Firebase.setString("/TestePrecisao/PulsosStatus", "Calibrando");
  Firebase.setString("/TestePrecisao/Status", "Aguardando");
}
