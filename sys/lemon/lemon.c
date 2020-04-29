#include "pcm.h"

struct pcm pcm;

void pcm_submit(){}
void pcm_init(){
    pcm.hz = pcm.len = pcm.stereo = pcm.pos = 0;
    pcm.buf = 0;
}
void pcm_close(){}