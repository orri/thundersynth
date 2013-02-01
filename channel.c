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
      run_oscillator(chan->osc, chan->channel_block + block_offset, BLOCK_LEN);
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

u16 notes[] = { 65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902, 0};
