
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#include "channel.h"
#include "constantarrays.h"
#include "read_input.h"

void auwritehead(FILE *f){
  char headerinfo[] =  {'.', 's', 'n', 'd', 
			0x00, 0x00, 0x00, 24,
			0xff, 0xff, 0xff, 0xff,
			0, 0, 0, 10,
                        0, 0, SAMPLING_FREQ>>8, SAMPLING_FREQ&0xff,
			0, 0, 0, 1};
  
		      		       
  fwrite(headerinfo, 1, 24, f);
}

void write_for_plot(FILE *f, u8 *buf, u16 len, u16 *index, u8 adsrphase){
  int i;
  for (i=0; i < len; i++){
    fprintf(f, "%d\t%d\t%d\n", (*index)++, buf[i], adsrphase); 
    
  }

}

int main(){
  u16 i;
  oscillator osc = {SINE, 0, 0, 0};
  osc.period = (u8*)malloc(SAMPLING_FREQ / notes[0] ); // use length of longest note
  

  
  notelist nl = {0, 0, 8, 0,0,0};
  u8 *song = (u8*)malloc(8);//{a2, b2, c3, r, e3, f3, g3};
  u8 *durs = (u8*)malloc(8);//{q, q, q, q, q, e, e};
  printf("making song\n");
  for (i = 0; i< 8; i++){
    song[i] = i % 8;
    durs[i] = e;
  }
  printf("song ready\n");


  nl.list = song;
  nl.durations = durs;
    
  channel chan = {0, &nl, &osc, 0};
  adsr adsr = {0,0,0};
  chan.a = &adsr;
  get_next_note(&chan);
  
  chan.channel_block = (u8*)malloc(BLOCK_LEN);

  printf("setting up main loop\n");
  FILE * ofile = fopen("auout.au", "w");
  FILE * plotfile = fopen("plotfile.dat", "w");
  //  FILE * input_file = fopen("/dev/midi5", "r");
  // setvbuf(input, NULL, _IONBF, 0);
  //auwritehead(ofile);
  // main loop:
  u8 cont = 1;
  u16 blocks = 0xff;

  // read_input();
  
  input_params i_params;
  i_params.filename = "/dev/midi1";
  i_params.amount_read = 0;
  i_params.amount_consumed = 0;
  printf("1");
  for (blocks = 0; blocks < 256; blocks++)
    i_params.midi_inputs[blocks] = malloc(sizeof(midi_t));
  printf("2");

  pthread_t input_thread;
  if(pthread_create(&input_thread, NULL, (void*)(&read_input), (void*)(&i_params))) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
    
  }
  
  //  u16 index = 0;
  //  u16 block_count = 0;
  printf("starting mainloop\n");
  midi_t *m;
  
  while (cont){
    printf("block nr: \n");
    while (get_new_input(&i_params)){
      m = i_params.last_input;
      //      c = i_params.amount_consumed-1;
      printf("main loop got input: %x %x %x\n", 
	     m->type, m->value1, m->value2 );
    }  
    //cont = next_chan_block(&chan);
    // adsr_block(chan.a, chan.channel_block, BLOCK_LEN);
    //fwrite(chan.channel_block, 1, BLOCK_LEN, ofile);
    // write_for_plot(plotfile, chan.channel_block, BLOCK_LEN, &index, chan.a->current_phase);
    //fprintf(plotfile, "%d\n", chan.a->current_phase); 
    // blocks--;
    //  printf("block %d, ", index++);
    usleep(500000);
  }
  
  /* int i, j, k;
  k = 0;
  u8 am;
  for (j = 0; j < 0x100; j += 0x10){
    for(i = 0; i < 0x100; i++){
       am = scale_amplitude(i, j);
       fprintf(plotfile, "%d\t%d\n", k++, am);
    }
  }
  printf("testing scale_amplitude %x %x\n", 
	 scale_amplitude(0xc0, 0xf0), 
	 scale_amplitude(0x40, 0xf0)); 
  */
  printf("closing\n");
  fclose(ofile);
  fclose(plotfile);
  free(osc.period);
  free(chan.channel_block);
  free(song);
  free(durs);
  

  return 0;

}
