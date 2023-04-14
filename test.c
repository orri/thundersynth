#include <stdio.h>
#include <stdlib.h>

int get_delay()
{
  char example[] =  "mmm\ndelay: 24";
  int delay;
  char *r = malloc(20);
  
  sscanf(example, "%*s\ndelay: %d",  &delay);
  printf("%s\n", r);
  return delay;
 
}

int main(){
  printf("delay is: %d", get_delay());
}
