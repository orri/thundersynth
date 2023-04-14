#include "channel.h"
#include <stdio.h>

unsigned int tobin(u8 p)
{
  unsigned int result = 0;
  int i, j;
  for (i = 0x80, j = 10000000; i > 0; i= i>>1, j = j/10)
    result += ((p&i)>0) * j;
  return result;

}

int main (){
  linealg la;
  short prevval, newval;
  unsigned short i;
  linealg l;
  step_sine_osc o;
  o.la = &l;
  /*  step_osc o;
  o = (step_osc){(void*)&child_o, SINE, next_sine_sample, new_sine_per};
  o.osc = &child_o;
  o.wave_form = SINE;
  o.next_sample = next_sine_sample;
  o.new_per = new_sine_per;

 
  o.new_per((void*)&child_o, 300);
  */
  new_sine_per(&o, 119);

  for (i = 0; i < 44100; i++)
    printf("%0d %d\n", i, next_sine_sample(&o));
  new_sine_per(&o, 200);
  /*
  prevval = 0;
  for (i = 400; i < 800; i++){ 
    newval = next_sine_sample(&o);
    printf("%d %d\n", i, newval);
    prevval = newval;
    }  */
}
