/** 
*  @file channel.h
*  @brief defines major building blocks for Thundersynth
*  @author Orri Tómasson
*/

#ifndef CHANNEL_H
#define CHANNEL_H


#include <stdint.h>
#include <stdio.h>
#include "notes.h"

#define SAMPLING_FREQ 44100
#define BLOCKS_PER_SEC 1000
#define BLOCK_LEN SAMPLING_FREQ/BLOCKS_PER_SEC
#define BPM 120
#define SAMPLES_PER_BEAT SAMPLING_FREQ*60/BPM

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef uint32_t u32;

// ADSR coefficientes are in Q4.12
#define ATTACK_COEFF 0x1010 
#define DECAY_COEFF 0x0ff8
#define RELEASE_COEFF 0x0fff
// ADSR times are simmulated using adsrtest.py
#define ATTACK_TIME 100//2260//2840
#define DECAY_TIME 271//4950
#define RELEASE_TIME 4209
#define ADR_TIME (ATTACK_TIME+DECAY_TIME+RELEASE_TIME)
// ADSR phases enums
#define ATTACK 0
#define DECAY 1
#define SUSTAIN 2
#define RELEASE 3

#define ATTACK_LEVEL 0xfe00
#define SUSTAIN_LEVEL 0x80
#define START_RELEASE 0x80

#define TRIG 0
#define SINE 1
#define SQUARE 0xc0
#define SQUARE75 0xe0
#define SQUARE25 0xa0
#define RANDOM 7



typedef struct {
  u8 *list;
  u8 *durations;
  u16 length; // length of notelist 
  u16 curr_note_length,  amount_done; // samples
  u16 next_note; // index to list and durations
} notelist;

typedef struct {
  u8 wavetype; // TRIG, SQUARE, sine ...
  // u8 phase;    // the phase the oscillator was in.
  u8 *period;
  u16 period_len; // number of allocated bytes for period.
  u16 period_index; 
} oscillator;

typedef struct {
  u16 current_amp_scale; // Q0.16
  u16 until_release;
  u8 current_phase;
} adsr;

typedef struct {
  u8 *channel_block; // [BLOCK_LEN];
  notelist *nlist;
  oscillator *osc;
  adsr *a;
} channel;



void get_new_period(oscillator* osc, u16 frequency);
void run_oscillator(oscillator* osc, u8* buffer, u16 len);
u8 get_next_note(channel* chan);
u8 next_chan_block(channel* chan);
void adsr_block(adsr *a, u8* block, u16 len);
u8 scale_amplitude(u8 in, u8 scale); // scales 
void calc_release_time(adsr *a, u16 note_length);

extern u16 notes[];


#endif // CHANNEL_H
