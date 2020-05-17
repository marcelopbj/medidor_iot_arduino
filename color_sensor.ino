#include "SparkFun_APDS9960.h"
#include "Wire.h"
#include "SPI.h"

#define COLOR_LAG       50
#define COLOR_WIN       10
#define COLOR_THRESHOLD 600
#define PULSE_DURATION  2

#define COLOR_LAST_IDX(x) (x == 0 ? COLOR_LAG - 1 : x - 1)
#define COLOR_IDX(x,n)    ((x-n) < 0 ? COLOR_LAG + (x-n) : x - n)

uint16_t data;
uint16_t data_amb;
int8_t pulse;
int8_t last_pulse;
uint16_t lag;
int32_t tmf;
int32_t psi;
uint32_t pulse_time;
int32_t color_avg[COLOR_LAG];
int32_t color_data[COLOR_LAG];
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint32_t color_sense_total_pulses;

int color_sense_get_pulses(void)
{  
  return color_sense_total_pulses; 
}

void color_sensor_init(void)
{  
  memset(color_avg,0,sizeof(color_avg));
  memset(color_data,0,sizeof(color_data));

  lag = 0;
  last_pulse = pulse_time = 0;
  pulse = 0;
  color_sense_total_pulses = 0; // TODO: reload from persistence

  apds.init();
  apds.enableLightSensor(false);

  delay(500); // calibration 
}

void color_sensor_tick(void)
{  
  uint16_t n;
  int32_t tm;

  apds.readAmbientLight(data_amb);
  apds.readRedLight(data);

  color_data[lag] = data;

  tm = 0;
  for(n = 0 ; n < COLOR_WIN ; n++)
    tm += color_data[COLOR_IDX(lag,n)];

  tm = tm/COLOR_WIN;
  tmf = color_avg[lag] = tm;

  // y[n] = x[n-1]^2 - x[n]*x[n-2]
  int32_t f0 = (color_avg[COLOR_IDX(lag,0)] + color_avg[COLOR_IDX(lag,1)] + color_avg[COLOR_IDX(lag,2)])/3;
  int32_t f1 = (color_avg[COLOR_IDX(lag,3)] + color_avg[COLOR_IDX(lag,4)] + color_avg[COLOR_IDX(lag,5)])/3;
  int32_t f2 = (color_avg[COLOR_IDX(lag,6)] + color_avg[COLOR_IDX(lag,7)] + color_avg[COLOR_IDX(lag,8)])/3;

  psi = f1*f1 - f0*f2;

  if(psi > COLOR_THRESHOLD)
  {  
    pulse_time = 0;
    pulse = 1; 
  }
  
  else
  {  
    if(++pulse_time > PULSE_DURATION)
    {
      pulse = 0;
      pulse_time = 0;
    }
  }

  if(pulse == 0 && last_pulse == 1)
    color_sense_total_pulses++;

  last_pulse = pulse;

  if(++lag >= COLOR_LAG)
    lag = 0;
}
