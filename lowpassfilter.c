#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <avr/interrupt.h>
#include <math.h>
/*
   timer = 500 us
   fs = 2khz
*/
uint8_t dataADC = 0;
const int orde = 50;
unsigned char channel = 0;
unsigned char datakirim;

long long y;
static int8_t b[orde] = {
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  0,
  -1,
  -2,
  -1,
  0,
  2,
  3,
  3,
  0,
  -3,
  -6,
  -6,
  -2,
  5,
  15,
  24,
  30,
  30,
  24,
  15,
  5,
  -2,
  -6,
  -6,
  -3,
  0,
  3,
  3,
  2,
  0,
  -1,
  -2,
  -1,
  0,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0
};
uint8_t x[orde];

ISR(TIMER0_COMPA_vect)
{
  konversi();
}

void TimerInit(void)
{
  TCCR0A |= _BV(WGM01);
  TCCR0B = (1 << CS01);

  TIMSK0 |= (1 << OCIE0A);
  OCR0A = 0x3E8;
}
void ADCInit(void)
{
  ADCSRA |= (1 << ADATE);
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
  ADCSRB &= ~(_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0));
  ADMUX |= ( 1 << ADLAR ) ;
  ADMUX |= _BV(REFS0) | channel;
  ADCSRA |= (1 << ADEN);
  ADCSRA |= (1 << ADSC);
}
int main(void)
{
  int dataADC = 0;
  ADCInit();
  TimerInit();
  sei();
  Serial.begin(115200);
  //konversi();
  while (1)
  {
  }
}

void konversi()
{
  dataADC = ADCH;
  uint8_t hasil = dataADC;
  datakirim = (unsigned char)fir(hasil);
  Serial.println(datakirim);
}

uint8_t fir(uint8_t data_ADC)
{
  y = 0;

  for (int i = (orde-1); i > 0; i--) {
    x[i] = x[i - 1];
  }
  x[0] = data_ADC;

  for (int i = 1; i < orde; i++) {
    y += b[i] * x[i];
  }

  if (y < 0) y=0;
  return y >> 8;
}
