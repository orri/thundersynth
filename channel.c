/** 
*  @file channel.c
*  @author Orri Tómasson
*/


#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "channel.h"


#define dprintf if(0)printf

/** @brief creates one period of a sine wave
    @param length of the sine period in samples
*/
void sine_period(u8 *period, u16 period_len){
  u16 i;
  for (i=0; i < period_len; i++)
    period[i] = round(128+127*sin(i*2*M_PI/period_len));
};

void square_period(u8 *period, u16 period_len){
  u16 i;
  for (i=0; i <period_len/2; i++)
    period[i] = 0;
  for (i=period_len/2; i<period_len; i++)
    period[i] = 255;
}
void square_period_duty(u8 *period, u16 period_len, u8 duty)
{
  u16 i, breakpoint;
  breakpoint = period_len*duty>>8; // duty is in Q0.8
  for (i=0; i < breakpoint; i++)
    period[i] = 0;
  for (i=breakpoint; i<period_len; i++)
    period[i] = 255;
} 

/** @brief creates one period of a trig wave
*   @param length of the sine period in samples
*/
void trig_period(u8 *period, u16 period_len){
   
  u16 i;
  u16 half_period_len = (period_len >> 1);
  u8 *sa = period;
  
  // up flank
  for (i=0; i < half_period_len; i++)
    *(sa++) = 255 * i/ (half_period_len);
    
  // create down flank
  // period_len - half_period_len is to deal with odd period_len
  for (i= 0; i < period_len - half_period_len; i++) 
    *(sa++) = 255 -  (255 * i/ (period_len-half_period_len));
};

void random_period(u8 *period, u16 period_len){
  u16 i;
  srandom(0);
  printf("calling random period\n");
  for (i = 0; i < period_len; i++)
    period[i] = (u8)random();
  
}

/** @brief updates the oscillator's period block
 *  @param osc the oscilator instance
 *  @param frequency the new frequency to set the oscilator to
*/
void get_new_period(oscillator* osc, u16 frequency){
  u8 phase; // Q0.8
   
  // calculate phase the oscilator was in 
  if(osc->period_len)
    phase = osc->period_index * 0x100 / osc->period_len;
  else
    phase = 0;
  osc->period_len = SAMPLING_FREQ / frequency;  
  osc->period_index = phase*osc->period_len / 0x100;
  
  switch (osc->wavetype){
  case TRIG:
    trig_period(osc->period, osc->period_len);
    break;
  case SINE:
    sine_period(osc->period, osc->period_len);
    break;
  case SQUARE:
    square_period(osc->period, osc->period_len);
    
    /*   default:
    if (osc->wavetype & 0x80) 
    // when the wavetype is SQUARE the MSB indicates it is square and the next bits indicate its dutycycle (in Q0.8, after one left shift)
    square_period_duty(osc->period, osc->period_len, (osc->wavetype<<1)&0xff);*/
    break;  
  case RANDOM:
    random_period(osc->period, osc->period_len);
    break;
  }
  

}

// needs to be tested
void get_new_rest(oscillator* osc){
  osc->period[0] = 128;
  osc->period_len = 1;
  osc->period_index = 0;
}



/** 
 *   @brief outputs data from the oscillator
 *   must be called after get_new_period has initialized the oscillator
 *   at least once
 *   @param oscillator object
 *   @param buffer the buffer to update the data to
 *   @param len the amount of samples to output
 */
// must be called after get_new_period
void run_oscillator(oscillator* osc, u8* buffer, u16 len){
  int i;
  
  if (osc->wavetype==RANDOM){
    for (i = 0; i < len; i++)
      buffer[i] = random();

  }
  else {
    for (i = 0; i < len; i++){
      buffer[i] = osc->period[osc->period_index];
      osc->period_index = (osc->period_index + 1) % osc->period_len;
    }
  }
}

u16 duration_to_samples(u8 duration)
{
  return SAMPLES_PER_BEAT*duration>>4; // duration is in Q4.4
}


void calc_release_time(adsr *a, u16 note_length){
  a->until_release =  note_length*START_RELEASE >> 8; // start_release is Q0.8
}

u8 get_next_note(channel* chan){
  notelist * l = chan->nlist;
  u8 new_note;
  if (l->next_note == l->length){ // no more notes in list
    printf("gen_next_note returning 0\n");
    return 0;
  }
  new_note = l->list[l->next_note];
  l->curr_note_length = duration_to_samples(l->durations[l->next_note++]);

  if (new_note == r)
    get_new_rest(chan->osc);
  else
    get_new_period(chan->osc, notes[new_note]);
  
  // reset ADSR
  chan->a->current_amp_scale = 0x0400; 
  chan->a->current_phase = ATTACK;
  calc_release_time(chan->a, l->curr_note_length);

  l->amount_done = 0;
  return 1;

}



u8 next_chan_block(channel* chan){
  u16 left_in_note = chan->nlist->curr_note_length - chan->nlist->amount_done;
  u16 left_in_block = BLOCK_LEN;
  u16 block_offset = 0;
  while (left_in_block){
    if (left_in_note > BLOCK_LEN){
      run_oscillator(chan->osc, chan->channel_block + block_offset, BLOCK_LEN-block_offset);
      chan->nlist->amount_done += BLOCK_LEN;
      left_in_block = 0;
    }
    else { // left_note is less or equal than block len
      // run oscilator for rest of note
      run_oscillator(chan->osc, chan->channel_block + block_offset, left_in_note); 
      block_offset += left_in_note;
      left_in_block -= left_in_note;
      if (get_next_note(chan) == 0){
	// fill rest of block with silence;
	memset(chan->channel_block+block_offset, 128, left_in_block);
	return 0;
      }
      left_in_note = chan->nlist->curr_note_length - chan->nlist->amount_done;
    }
  }
  return 1;
}

u8 scale_amplitude(u8 in, u8 scale){
  u8 amp = in - 128; // 128 is the "zero" in unsigned 8 bit format
  amp = ((((s8)amp*scale))>>8)+128; // scale is in Q0.8

  return amp;
}

void adsr_block(adsr *a, u8* block, u16 len){
  
  u16 i, decay_ampl;
  u8 scale;

  for (i = 0; i < len; i++){
    switch (a->current_phase){
    case ATTACK:
      // Q0.16 = Q0.16 * Q4.12 
      a->current_amp_scale = (a->current_amp_scale * ATTACK_COEFF) >> 12;
      scale = a->current_amp_scale >> 8; // Q0.16 to Q0.8 
      
      if(a->until_release-- == 0)
	a->current_phase = RELEASE;
      else if (a->current_amp_scale > ATTACK_LEVEL){
	a->current_phase = DECAY;
	scale=0xfc;
	a->current_amp_scale = ATTACK_LEVEL;
      }
      block[i] = scale_amplitude(block[i], scale);// Q8.0 = Q8.0 * Q0.8;
      dprintf("attack 1: %x, scale: %x, coeff: %x \n", a->current_amp_scale, scale, ATTACK_COEFF);
      
      break;
    case DECAY:
      // Q0.16 = Q0.16 * Q4.12
      decay_ampl = a->current_amp_scale - SUSTAIN_LEVEL;
      decay_ampl = decay_ampl*DECAY_COEFF >> 12;
      a->current_amp_scale = decay_ampl + SUSTAIN_LEVEL;
      scale = a->current_amp_scale >> 8; // Q0.16 to Q0.8 
   
      if(a->until_release-- == 0)
	a->current_phase = RELEASE;
      else if (scale < SUSTAIN_LEVEL+1){
	a->current_phase = SUSTAIN;
	dprintf("switching to sustain\n"); 
	scale = SUSTAIN_LEVEL;
	a->current_amp_scale = SUSTAIN_LEVEL<<8;
      }
      block[i] = scale_amplitude(block[i], scale); // Q8.0 = Q8.0 * Q0.8;
      dprintf("decay 1: %x, scale: %x, coeff: %x: val: %d\n", a->current_amp_scale, scale, DECAY_COEFF, block[i]);
      break;
      
    case SUSTAIN:
      dprintf("sustain old: %d, ", block[i]);
	block[i] = scale_amplitude(block[i],SUSTAIN_LEVEL);
      if (a->until_release-- == 0)
	a->current_phase = RELEASE;
      dprintf("new: %d, until_release: %d \n", block[i], a->until_release);
      break;

    case RELEASE:
      
      a->current_amp_scale = (a->current_amp_scale * RELEASE_COEFF) >> 12;
      scale = a->current_amp_scale >> 8; // Q0.16 to Q0.8 
      block[i] = scale_amplitude(block[i], scale); // Q8.0 = Q8.0 * Q0.8;
      dprintf("release 1: %x, scale: %x, coeff: %x \n", a->current_amp_scale, scale, RELEASE_COEFF);
      break;
	
    }
  }

}


const u16 notes[84] = { 65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902};

const u16 notesperiod[84] = {678,  639,  604,  565,  538,  507,  479,  450,  424,  401,  377,  359, 337,  317,  300,  283,  267,  252,  238,  225,  212,  200,  189,  179,  168,  159,  150,  142,  134,  126,  119,  113,  106,  100,  95,  89,  84,  80,  75,  71,  67,  63,  60,  56,  53,  50,  47,  45,  42,  40,  38,  35,  33,  32,  30,  28,  27,  25,  24,  22,  21,  20,  19,  18,  17,  16,  15,  14,  13,  13,  12,  11,  11,  10,  9,  9,  8,  8, 7,  7,  7,  6,  6,  6};

const u8 sine512[512] = {128, 129, 131, 132, 134, 135, 137, 138, 140, 142, 143, 145, 146, 148, 149, 151, 152, 154, 155, 157, 158, 160, 162, 163, 165, 166, 167, 169, 170, 172, 173, 175, 176, 178, 179, 181, 182, 183, 185, 186, 188, 189, 190, 192, 193, 194, 196, 197, 198, 200, 201, 202, 203, 205, 206, 207, 208, 210, 211, 212, 213, 214, 215, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 234, 235, 236, 237, 238, 238, 239, 240, 241, 241, 242, 243, 243, 244, 245, 245, 246, 246, 247, 248, 248, 249, 249, 250, 250, 250, 251, 251, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 253, 253, 253, 253, 252, 252, 252, 251, 251, 250, 250, 250, 249, 249, 248, 248, 247, 246, 246, 245, 245, 244, 243, 243, 242, 241, 241, 240, 239, 238, 238, 237, 236, 235, 234, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 215, 214, 213, 212, 211, 210, 208, 207, 206, 205, 203, 202, 201, 200, 198, 197, 196, 194, 193, 192, 190, 189, 188, 186, 185, 183, 182, 181, 179, 178, 176, 175, 173, 172, 170, 169, 167, 166, 165, 163, 162, 160, 158, 157, 155, 154, 152, 151, 149, 148, 146, 145, 143, 142, 140, 138, 137, 135, 134, 132, 131, 129, 128, 126, 124, 123, 121, 120, 118, 117, 115, 113, 112, 110, 109, 107, 106, 104, 103, 101, 100, 98, 97, 95, 93, 92, 90, 89, 88, 86, 85, 83, 82, 80, 79, 77, 76, 74, 73, 72, 70, 69, 67, 66, 65, 63, 62, 61, 59, 58, 57, 55, 54, 53, 52, 50, 49, 48, 47, 45, 44, 43, 42, 41, 40, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 21, 20, 19, 18, 17, 17, 16, 15, 14, 14, 13, 12, 12, 11, 10, 10, 9, 9, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 14, 15, 16, 17, 17, 18, 19, 20, 21, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 45, 47, 48, 49, 50, 52, 53, 54, 55, 57, 58, 59, 61, 62, 63, 65, 66, 67, 69, 70, 72, 73, 74, 76, 77, 79, 80, 82, 83, 85, 86, 88, 89, 90, 92, 93, 95, 97, 98, 100, 101, 103, 104, 106, 107, 109, 110, 112, 113, 115, 117, 118, 120, 121, 123, 124, 126};

const u8 sine1024[1024] = {128, 128, 129, 130, 131, 131, 132, 133, 134, 135, 135, 136, 137, 138, 138, 139, 140, 141, 142, 142, 143, 144, 145, 145, 146, 147, 148, 149, 149, 150, 151, 152, 152, 153, 154, 155, 155, 156, 157, 158, 158, 159, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 167, 168, 169, 170, 170, 171, 172, 173, 173, 174, 175, 176, 176, 177, 178, 178, 179, 180, 181, 181, 182, 183, 183, 184, 185, 186, 186, 187, 188, 188, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 196, 196, 197, 198, 198, 199, 200, 200, 201, 202, 202, 203, 203, 204, 205, 205, 206, 207, 207, 208, 208, 209, 210, 210, 211, 211, 212, 213, 213, 214, 214, 215, 215, 216, 217, 217, 218, 218, 219, 219, 220, 220, 221, 221, 222, 222, 223, 224, 224, 225, 225, 226, 226, 227, 227, 228, 228, 228, 229, 229, 230, 230, 231, 231, 232, 232, 233, 233, 234, 234, 234, 235, 235, 236, 236, 236, 237, 237, 238, 238, 238, 239, 239, 240, 240, 240, 241, 241, 241, 242, 242, 242, 243, 243, 243, 244, 244, 244, 245, 245, 245, 246, 246, 246, 246, 247, 247, 247, 248, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 253, 253, 253, 252, 252, 252, 252, 252, 252, 251, 251, 251, 251, 251, 250, 250, 250, 250, 250, 249, 249, 249, 249, 248, 248, 248, 248, 247, 247, 247, 246, 246, 246, 246, 245, 245, 245, 244, 244, 244, 243, 243, 243, 242, 242, 242, 241, 241, 241, 240, 240, 240, 239, 239, 238, 238, 238, 237, 237, 236, 236, 236, 235, 235, 234, 234, 234, 233, 233, 232, 232, 231, 231, 230, 230, 229, 229, 228, 228, 228, 227, 227, 226, 226, 225, 225, 224, 224, 223, 222, 222, 221, 221, 220, 220, 219, 219, 218, 218, 217, 217, 216, 215, 215, 214, 214, 213, 213, 212, 211, 211, 210, 210, 209, 208, 208, 207, 207, 206, 205, 205, 204, 203, 203, 202, 202, 201, 200, 200, 199, 198, 198, 197, 196, 196, 195, 194, 194, 193, 192, 192, 191, 190, 190, 189, 188, 188, 187, 186, 186, 185, 184, 183, 183, 182, 181, 181, 180, 179, 178, 178, 177, 176, 176, 175, 174, 173, 173, 172, 171, 170, 170, 169, 168, 167, 167, 166, 165, 165, 164, 163, 162, 162, 161, 160, 159, 158, 158, 157, 156, 155, 155, 154, 153, 152, 152, 151, 150, 149, 149, 148, 147, 146, 145, 145, 144, 143, 142, 142, 141, 140, 139, 138, 138, 137, 136, 135, 135, 134, 133, 132, 131, 131, 130, 129, 128, 128, 127, 126, 125, 124, 124, 123, 122, 121, 120, 120, 119, 118, 117, 117, 116, 115, 114, 113, 113, 112, 111, 110, 110, 109, 108, 107, 106, 106, 105, 104, 103, 103, 102, 101, 100, 100, 99, 98, 97, 97, 96, 95, 94, 93, 93, 92, 91, 90, 90, 89, 88, 88, 87, 86, 85, 85, 84, 83, 82, 82, 81, 80, 79, 79, 78, 77, 77, 76, 75, 74, 74, 73, 72, 72, 71, 70, 69, 69, 68, 67, 67, 66, 65, 65, 64, 63, 63, 62, 61, 61, 60, 59, 59, 58, 57, 57, 56, 55, 55, 54, 53, 53, 52, 52, 51, 50, 50, 49, 48, 48, 47, 47, 46, 45, 45, 44, 44, 43, 42, 42, 41, 41, 40, 40, 39, 38, 38, 37, 37, 36, 36, 35, 35, 34, 34, 33, 33, 32, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 21, 21, 21, 20, 20, 19, 19, 19, 18, 18, 17, 17, 17, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 17, 17, 17, 18, 18, 19, 19, 19, 20, 20, 21, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 40, 40, 41, 41, 42, 42, 43, 44, 44, 45, 45, 46, 47, 47, 48, 48, 49, 50, 50, 51, 52, 52, 53, 53, 54, 55, 55, 56, 57, 57, 58, 59, 59, 60, 61, 61, 62, 63, 63, 64, 65, 65, 66, 67, 67, 68, 69, 69, 70, 71, 72, 72, 73, 74, 74, 75, 76, 77, 77, 78, 79, 79, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 88, 88, 89, 90, 90, 91, 92, 93, 93, 94, 95, 96, 97, 97, 98, 99, 100, 100, 101, 102, 103, 103, 104, 105, 106, 106, 107, 108, 109, 110, 110, 111, 112, 113, 113, 114, 115, 116, 117, 117, 118, 119, 120, 120, 121, 122, 123, 124, 124, 125, 126, 127};
