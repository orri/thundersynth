
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

#include "read_input.h"


unsigned int get_mtime(){
  struct timeval t;
  gettimeofday(&t);
  return (unsigned int)(t.tv_sec)*1000 + (unsigned int)(t.tv_usec) / 1000;
}

int main () {
  input_params i_params;
  i_params.filename = "/dev/midi4";
  i_params.amount_read = 0;
  i_params.amount_consumed = 0;
 
 short i;
  for (i = 0; i < 256; i++)
    i_params.midi_inputs[i] = malloc(sizeof(midi_t));
  printf("2");

  pthread_t input_thread;
  if(pthread_create(&input_thread, NULL, (void*)(&read_input), (void*)(&i_params))) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }
  struct timeval t1;
  int new_time, last_time;
  last_time = 0;
  /*while (1){
    while (get_new_input(&i_params)){
      if (i_params.last_input->type == 0x90){
	gettimeofday(&t1, NULL);
	new_time = (unsigned int)(t1.tv_usec) +  ((unsigned int)(t1.tv_sec)*1000000);
	printf("got input %x %d\n", i_params.last_input->type, new_time-last_time);
	last_time = new_time;
      }
    }
    }*/

}
