/** 
*  @file alsacrap.h
*  @brief ALSA functionality for thundersynth
*  A stripped version of an example: 
*  http://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_8c-example.html 
*  @author Orri Tómasson
*  
*/

#ifndef ALSACRAP_H
#define ALSACRAP_H


#include <alsa/asoundlib.h>




int set_hwparams(snd_pcm_t *handle,
		 snd_pcm_hw_params_t *params,
		 snd_pcm_access_t access,
		 snd_pcm_sframes_t *period_s);

int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams);

int xrun_recovery(snd_pcm_t *handle, int err);

int wait_for_poll(snd_pcm_t *handle, struct pollfd *ufds, unsigned int count);

int write_and_poll_loop(snd_pcm_t *handle,
			unsigned char *samples,
			void*(*main_loop)(void*),
			void *mainloop_args);
#endif // ALSACRAP_H
