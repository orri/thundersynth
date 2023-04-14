
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sched.h>
#include <errno.h>
//#include <getopt.h>
//#include <alsa/asoundlib.h>
#include <sys/time.h>
#include <math.h>

#include <pthread.h>
#include "read_input.h"
#include "channel.h"
#include "alsastuff.h"

static char *mididevice = "/dev/snd/midiC3D0";
static char *device = "default";                     /* playback device */
//static snd_pcm_format_t format = SND_PCM_FORMAT_U8;    /* sample format */
//static unsigned int rate = 44100;                       /* stream rate */

//static double freq = 1;                               /* sinusoidal wave frequency in Hz */
static int verbose = 0;                                 /* verbose flag */



void playback(u8 *samples, short count, channel **c, u8 num_channels){
  short i, j;
  u8 * curr_sample;

  for (i = 0; i < count; i++){
    curr_sample = samples++;
    *curr_sample = 0;
    for (j=0; j < num_channels; j++){
      *curr_sample += next_channel_sample(c[j]);
    }
  }
}

const u8 SPLIT_VAL = 0x3e;

void process_input(midi_t *m, channel **c, u8 num_channels){
  
  unsigned short control_knob = (m->type << 8) + m->value1;
  channel *ch1, *ch2;
  ch1 = c[0];
  ch2 = c[1];
  
  if (m->type == 0x90){ // press note
    if ( m->value1 < SPLIT_VAL )
      note_press(ch1, m->value1, m->value2);
    else
      note_press(ch2, m->value1, m->value2);
  } 
  else if (m->type == 0x80) {
    if ( m->value1 < SPLIT_VAL)
      note_release(ch1, m->value1);
    else 
      note_release(ch2, m->value1);
  }
  else { // check control buttons
    switch (control_knob){
    case KB_ATTACK_C: // attack time
      ch1->ap->attack_coeff = m->value2 +1;//0x8000 + (m->value2<<2) ;
      //      printf("new attack coeff: %x\n", ch1->ap->attack_coeff);
      break;
    case KB_DECAY_C:
      ch1->ap->decay_coeff = 0x8000 - ((m->value2<<2) +1);
      break;
    case KB_RELEASE_C: 
      ch1->ap->release_coeff = 0x8000 - ((m->value2) +1);
      break;
    case KB_SUSTAIN_L:
      ch1->ap->sustain_level = m->value2<<1;
      break;
    case KB_WAVEFORM:
      ch1->wavetype = m->value2>>5; // ust only 2 MSBs of 7
      break;
    case KB_DUTY:
      ch1->duty_cycle = m->value2<<1;
      break;
   
    case KB_ATTACK_C_2: // attack time
      ch2->ap->attack_coeff = m->value2 +1;//0x8000 + (m->value2<<2) ;
      printf("new attack coeff: %x\n", ch1->ap->attack_coeff);
      break;
    case KB_DECAY_C_2:
      ch2->ap->decay_coeff = 0x8000 - ((m->value2<<2) +1);
      break;
    case KB_RELEASE_C_2: 
      ch2->ap->release_coeff = 0x8000 - ((m->value2) +1);
      break;
    case KB_SUSTAIN_L_2:
      ch2->ap->sustain_level = m->value2<<1;
      break;
    case KB_WAVEFORM_2:
      ch2->wavetype = m->value2>>5;
      break;
    case KB_DUTY_2:
      ch2->duty_cycle = m->value2<<1;
      break;





    case KB_STOP:
      exit(0);
    }
      
  }
  
}

typedef struct {
  u8 *samples;
  input_params *iparams;
  //  double *phase;
  int period_s;
  channel **channels;
  u8 num_chan;
} mainloop_args_t;
 
// called from inside 
void main_loop(void *args)
{
  mainloop_args_t *a = (mainloop_args_t*)args; 
  while(get_new_input(a->iparams)){
    process_input(a->iparams->last_input, a->channels, a->num_chan);
  }
  playback(a->samples, a->period_s, a->channels, a->num_chan);
  //  generate_trig(a->samples, a->period_s, a->phase);
}


int main(int argc, char *argv[])
{

  snd_pcm_t *handle;
  int err;
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;
  unsigned char *samples;
  snd_pcm_sframes_t period_s;
  snd_output_t *output = NULL;
  
  snd_pcm_hw_params_alloca(&hwparams);
  snd_pcm_sw_params_alloca(&swparams);
 
     
  err = snd_output_stdio_attach(&output, stdout, 0);
  if (err < 0) {
    printf("Output failed: %s\n", snd_strerror(err));
    return 0;
  }
  printf("Playback device is %s\n", device);

  if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    printf("Playback open error: %s\n", snd_strerror(err));
    return 0;
  }
        
  if ((err = set_hwparams(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED, &period_s)) < 0) {
    printf("Setting of hwparams failed: %s\n", snd_strerror(err));
    exit(EXIT_FAILURE);
  }
  if ((err = set_swparams(handle, swparams)) < 0) {
    printf("Setting of swparams failed: %s\n", snd_strerror(err));
    exit(EXIT_FAILURE);
  }
  if (verbose > 0)
    snd_pcm_dump(handle, output);
  samples = malloc(period_s);
  if (samples == NULL) {
    printf("Not enough memory\n");
    exit(EXIT_FAILURE);
  }
        


  // setup read_input
  input_params i_params;
  i_params.filename = mididevice;
  i_params.amount_read = 0;
  i_params.amount_consumed = 0;
  printf("1");
  short i;
  for (i = 0; i < 256; i++)
    i_params.midi_inputs[i] = malloc(sizeof(midi_t));
  printf("2");

  pthread_t input_thread;
  if(pthread_create(&input_thread, NULL, (void*)(&read_input), (void*)(&i_params))) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
    
  }

  // set up channels and oscillators
  
  // channel 1
  u8 num_osc = 4;

  step_osc **opp = malloc(num_osc*sizeof(step_osc*));
  adsr **app = malloc(num_osc*sizeof(adsr*));
  linealg **lpp = malloc(num_osc*sizeof(linealg*));
  for (i=0; i<num_osc; i++){
    opp[i] = malloc(sizeof(step_osc));
    app[i] = malloc(sizeof(adsr));
    lpp[i] = malloc(sizeof(linealg));
    *(opp[i]) = (step_osc){1, lpp[i]};
    *(app[i]) = (adsr){0x100, 0, REST};
  }
  
  adsr_params ap = {0xf0, 0x7f00, 0x7f00, 0xff, 0xf8, 1};
  channel ch1 = {opp, app, num_osc, TRIG, &ap, 0, 0x80};

  
  // channel 2
  u8 num_osc_c2 = 4;

  step_osc **opp_c2 = malloc(num_osc*sizeof(step_osc*));
  adsr **app_c2 = malloc(num_osc*sizeof(adsr*));
  linealg **lpp_c2 = malloc(num_osc*sizeof(linealg*));
  for (i=0; i<num_osc_c2; i++){
    opp_c2[i] = malloc(sizeof(step_osc));
    app_c2[i] = malloc(sizeof(adsr));
    lpp_c2[i] = malloc(sizeof(linealg));
    *(opp_c2[i]) = (step_osc){1, lpp_c2[i]};
    *(app_c2[i]) = (adsr){0x100, 0, REST};
  }
  
  adsr_params ap_c2 = {0xf0, 0x7f00, 0x7f00, 0xff, 0xf8, 1};
  channel ch2 = {opp_c2, app_c2, num_osc, TRIG, &ap_c2, 0, 0x80};

  channel **c = malloc(2*sizeof(channel*));
  c[0] = &ch1;
  c[1] = &ch2;
  
  // setting mainloop args
  mainloop_args_t mainloop_args;
  mainloop_args.samples =samples;
  mainloop_args.iparams = &i_params;
  //  *(mainloop_args.phase) = 0;
  mainloop_args.period_s = period_s;
  mainloop_args.channels = c;
  mainloop_args.num_chan = 2;
;
  printf("period_s: %d\n", (int)period_s);
  

  err = write_and_poll_loop(handle, samples, (void*)(&main_loop), 
			    (void*)(&mainloop_args));
  
  if (err < 0)
    printf("Transfer failed: %s\n", snd_strerror(err));

  free(samples);
  free(opp);
  free(opp_c2);
  free(app);
  free(app_c2);
  free(lpp);
  free(lpp_c2);
 

  snd_pcm_close(handle);
  return 0;
}
