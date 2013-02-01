
#include <stdio.h>
#include <stdlib.h>

#include "channel.h"

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
  for (i=0; i < len; i++)
    fprintf(f, "%d\t%d\t%d\n", (*index)++, buf[i], adsrphase); 

}


int main(){
  u8 i;
  oscillator osc = {SQUARE, 0, 0, 0};
  osc.period = (u8*)malloc(SAMPLING_FREQ / notes[0] ); // use length of longest note
  
  notelist nl = {0, 0, 72, 0,0,0};
  u8 *song = malloc(83);//{a2, b2, c3, r, e3, f3, g3};
  u8 *durs = malloc(83);//{q, q, q, q, q, e, e};
  for (i = 0; i< 84; i++){
    song[i] = i;
    durs[i] = e;
  }
    


  nl.list = song;
  nl.durations = durs;
    
  channel chan = {(u8*)malloc(BLOCK_LEN), &nl, &osc, 
		  (adsr*)malloc(sizeof(adsr))};
  get_next_note(&chan);
  


  printf("setting up main loop\n");
  FILE * ofile = fopen("auout.au", "w");
  FILE * plotfile = fopen("plotfile.dat", "w");
  //auwritehead(ofile);
  // main loop:
  u8 cont = 1;
  u8 blocks = 0xff;
  u16 index;
  //  u16 block_count = 0;
  while (cont){
    //   printf("block nr: %d\n", block_count++);
    
    cont = next_chan_block(&chan);
    adsr_block(chan.a, chan.channel_block, BLOCK_LEN);
    
    fwrite(chan.channel_block, 1, BLOCK_LEN, ofile);
    write_for_plot(plotfile, chan.channel_block, BLOCK_LEN, &index, chan.a->current_phase);
    //fprintf(plotfile, "%d\n", chan.a->current_phase); 
    blocks--;
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

  return 0;

}
