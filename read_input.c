#include "read_input.h"

void read_input(void *params){
  u8 read_data[3];
  u8 amount_read;
  input_params *p = (input_params*)params;
  printf("opening device: %s\n", p->filename);
  FILE * f = fopen(p->filename, "rb");
  midi_t *curr_midi;
  
  if (f == NULL)
    printf("ERROR: opening device unsuccessful\n");

  do {
    amount_read       = fread(&read_data, 3, 1, f);
    curr_midi         = p->midi_inputs[p->amount_read];
    curr_midi->type   = read_data[0];
    curr_midi->value1 = read_data[1];
    curr_midi->value2 = read_data[2];
    //    printf("amount_read: %d\n", amount_read);
    //printf("read loop\n");
    switch (read_data[0]){
     case 0x90:
       // printf("Note press received: note: %x, velocity: %x\n", read_data[1], read_data[2]);
       break;
     case 0x80:
       //printf("Note release received: note: %x\n", read_data[1]);
       break;
    
    }
    //printf("  read bytes: %x %x %x \n", read_data[0], read_data[1], read_data[2]);
    p->amount_read++;
  } while (amount_read);

  //printf("exiting read loop\n");

}

u8 get_new_input(input_params *params){
  
  u8 roll_over = (params->amount_read < 0x10 && params->amount_consumed > 0xf0);
  if ((params->amount_read > params->amount_consumed) || roll_over){
    params->last_input = params->midi_inputs[params->amount_consumed];
    params->amount_consumed++;
    //  printf("new input_gotten. amount_read: %x, amount_consumed: %x\n ", 
    //  params->amount_read, params->amount_consumed);
    
    return 1;
  }
  return 0;
}
