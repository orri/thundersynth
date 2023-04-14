#ifndef READ_INPUT_H
#define READ_INPUT_H

#include "channel.h" // to get u8

#define KB_NOTE_PRESS 0x90
#define KB_NOTE_RELEASE 0x80
#define KEY_OFFSET 0x24

#define KB_ATTACK_C 0xb024
#define KB_DECAY_C 0xb025
#define KB_RELEASE_C 0xb01c
#define KB_SUSTAIN_L 0xb021

#define KB_WAVEFORM 0xb010
#define KB_DUTY 0xb011

#define KB_WAVEFORM_2 0xb012
#define KB_DUTY_2 0xb013

#define KB_ATTACK_C_2 0xb026
#define KB_DECAY_C_2 0xb032
#define KB_RELEASE_C_2 0xb037
#define KB_SUSTAIN_L_2 0xb034



#define KB_STOP 0xbf74

typedef struct {
  u8 type;
  u8 value1;
  u8 value2;
} midi_t;

typedef struct{
  const char* filename;
  midi_t* midi_inputs[256];
  u8 amount_read;
  u8 amount_consumed;
  midi_t *last_input;
} input_params;



void read_input(void* params);// run by pthread

u8 get_new_input(input_params* params);



#endif // READ_INPUT_H
