#include <stdio.h>
#include <stdlib.h>

int main ()
{
  FILE * f = fopen("/dev/midi3", "rb");
  fpos_t *p;
  long lSize;
  int seekres;
  do {
  seekres = fseek (f , 1 , SEEK_SET);
  lSize = ftell (f);
  //  printf("seekres: %d\n", seekres);
  }while (seekres != 0);
//rewind (f);
  printf("size %ld\n", lSize);
  //  fgetpos(f, p);
  unsigned char *c = (char*)malloc(3);
  fread(c, 1, 3, f);
  printf("pos %d %d %d\n", c[0], c[1], c[2]);

}
