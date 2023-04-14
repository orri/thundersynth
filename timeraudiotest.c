#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <alsa/asoundlib.h>


#define BUFSIZE 1000
#define FREQ_RATE 44100


void timer_interupt_handler(){
 
};

void auwritehead(FILE *f){
  char headerinfo[] =  {'.', 's', 'n', 'd', 
			0x00, 0x00, 0x00, 24,
			0xff, 0xff, 0xff, 0xff,
			0, 0, 0, 10,
                        0, 0, FREQ_RATE>>8, FREQ_RATE&0xff,
			0, 0, 0, 1};
  
		      		       
  fwrite(headerinfo, 1, 24, f);
}

void playback(snd_pcm_t *playback_handle, unsigned char *buf, FILE *audiooutfile){

  int i, written;
  snd_pcm_sframes_t delay;
  snd_pcm_delay(playback_handle, &delay); 
  printf("playing back. available now: %d, delay: %d \n", 
	 (int)snd_pcm_avail_update(playback_handle), (int)delay);

  // deal with underrun
  if (snd_pcm_state(playback_handle) == SND_PCM_STATE_XRUN || 
      snd_pcm_avail_update(playback_handle) > 3000)
    memset(buf, 128, BUFSIZE);

  while (snd_pcm_state(playback_handle) == SND_PCM_STATE_XRUN){
    snd_pcm_prepare(playback_handle);
    snd_pcm_writei(playback_handle, buf, BUFSIZE);
    printf("underrun detected, filling buffer with 0 samples\n");
  }
  /* while (snd_pcm_avail_update(playback_handle) > 3000){
    written = snd_pcm_writei(playback_handle, buf, BUFSIZE);
    printf("not enough avail, writing: %d, avail, %d", 
	   (int)written , (int)snd_pcm_avail_update(playback_handle));
	   }*/

  
  for (i = 0; i<BUFSIZE; i++)
    buf[i] = random()&0xff;
  written = snd_pcm_writei(playback_handle, buf, BUFSIZE);
  if( written < BUFSIZE)
    printf("wrote less: %d %d\n", written, BUFSIZE);  
  printf("pcm_state: %d, wrote: %d, BUFSIZE: %d, avail: snd_pcm_avail: %d\n", snd_pcm_state(playback_handle), written, BUFSIZE, (int)snd_pcm_avail_update(playback_handle));
  fwrite(buf, 1, written, audiooutfile);
}

// taken from http://alsamodular.sourceforge.net/seqdemo.c
snd_pcm_t *open_pcm(char *pcm_name) {

    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    unsigned int freq_rate = FREQ_RATE;
 
    if (snd_pcm_open (&playback_handle, pcm_name, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        fprintf (stderr, "cannot open audio device %s\n", pcm_name);
        exit (1);
    }
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(playback_handle, hw_params);
    snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_U8);
    snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &freq_rate, 0);
    snd_pcm_hw_params_set_channels(playback_handle, hw_params, 1); // was 2
    snd_pcm_hw_params_set_periods(playback_handle, hw_params, 1, 0); // was 2
    int rassgat = 0;
    snd_pcm_uframes_t tippi = 100;
    snd_pcm_hw_params_set_period_size_near(playback_handle, hw_params, &tippi, &rassgat);
    //snd_pcm_hw_params_set_period_size_integer(playback_handle, hw_params);
    snd_pcm_hw_params(playback_handle, hw_params);
    snd_pcm_sw_params_alloca(&sw_params);
    snd_pcm_sw_params_current(playback_handle, sw_params);
    snd_pcm_sw_params_set_avail_min(playback_handle, sw_params, 256);
    snd_pcm_sw_params(playback_handle, sw_params);
    if ((snd_pcm_prepare (playback_handle)) < 0) {
      fprintf (stderr, "cannot prepare audio interface for use \n" );
      exit (1);
    }
    
    return(playback_handle);
}


int main(){
  int i, err;
  struct sigaction sa; 
  struct itimerval timer; 
  unsigned char *buf;
 
  buf = malloc(BUFSIZE);
  
  /* Install timer_handler as the signal handler for SIGVTALRM.  */ 
  signal(SIGALRM, &timer_interupt_handler);
  int timerval = (int)(1000000*(((double)BUFSIZE/FREQ_RATE)));
  printf("timerval: %d", timerval);
  /* Configure the timer to expire after 250 msec...  */ 
  timer.it_value.tv_sec = 0; 
  timer.it_value.tv_usec = timerval; 
  /* ... and every 250 msec after that.  */ 
  timer.it_interval.tv_sec = 0; 
  timer.it_interval.tv_usec = timerval;
  /* Start a virtual timer. It counts down whenever this process is 
     executing.  */ 
  setitimer (ITIMER_REAL, &timer, 0); 
 
  snd_pcm_t *playback_handle = open_pcm("default");
  FILE *auoutfile = fopen("audioout.au", "w");
  //auwritehead(auoutfile);

  if (snd_pcm_wait(playback_handle, 1000) == 0){
    fprintf(stderr, "not ready for audio\n");
  }
  //playback(playback_handle);
  /* playback(playback_handle, buf, auoutfile);
  playback(playback_handle, buf, auoutfile);
  playback(playback_handle, buf, auoutfile);*/
  //playback(playback_handle, buf, auoutfile);
  /* Do busy work.  */ 
  for (i=0; i<2*FREQ_RATE/BUFSIZE; i++){
    playback(playback_handle, buf, auoutfile);
    printf("here I am\n");
    pause(); 
  }
  
  for (i=0; i < 10; i++){// i<2*FREQ_RATE/BUFSIZE; i++){
    //  playback(playback_handle, buf);
    printf("here I am in wait loop, avail: %d\n", (int)snd_pcm_avail_update(playback_handle));
    pause(); 
  }
  //snd_pcm_recover(playback_handle, 4,0);
  //snd_pcm_prepare (playback_handle);
  /*if (snd_pcm_wait(playback_handle, 1000) == 0){
    fprintf(stderr, "not ready for audio\n");
    }*/

  //playback(playback_handle, buf);
  //playback(playback_handle, buf);
  printf("\nstate: %d, %d\n", (int)snd_pcm_state(playback_handle), (int)SND_PCM_STATE_XRUN  );
  /*  while(snd_pcm_state(playback_handle) == SND_PCM_STATE_XRUN){
    memset(buf,128, BUFSIZE);
    if ((err = snd_pcm_writei (playback_handle, buf, BUFSIZE)) != BUFSIZE) {
      fprintf (stderr, "write to audio interface failed (%s)\n",
	       snd_strerror (err));
      exit (1);
    }
    printf("trying to catch up on undrrun %d", (int)err);
    }*/
  
  for (i=0; i<2*FREQ_RATE/BUFSIZE; i++){
    playback(playback_handle, buf, auoutfile);
    printf("here I am\n");
    pause(); 
  }

  printf("SND_PCM_STATE_OPEN %d\n", SND_PCM_STATE_OPEN);
  printf("SND_PCM_STATE_SETUP %d\n", SND_PCM_STATE_SETUP);
  printf("SND_PCM_STATE_PREPARED %d\n", SND_PCM_STATE_PREPARED);
  printf("SND_PCM_STATE_RUNNING %d\n", SND_PCM_STATE_RUNNING);
  printf("SND_PCM_STATE_XRUN %d\n", SND_PCM_STATE_XRUN);
  printf("SND_PCM_STATE_DRAINING %d\n", SND_PCM_STATE_DRAINING);
  printf("SND_PCM_STATE_PAUSED %d\n", SND_PCM_STATE_PAUSED);

  printf("SND_PCM_STATE_SUSPENDED %d\n", SND_PCM_STATE_SUSPENDED);
  printf("SND_PCM_STATE_DISCONNECTED %d\n", SND_PCM_STATE_DISCONNECTED);


  fclose(auoutfile);
  snd_pcm_close(playback_handle);
  free(buf);
}
