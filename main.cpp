#include <stdio.h>  //FILE
#include <stdlib.h> //exit
#include <unistd.h> //getopt

#include "PageTable.h"
#include "tracereader.h"
#include "output_mode_helpers.h"

void AddressDecoder(p2AddrTr *addr_ptr, FILE *out);

int main(int argc, char **argv)
{
  FILE *ifp;	        /* trace file */
  unsigned long i = 0;  /* instructions processed */
  p2AddrTr trace;	/* traced address */

  /* check usage */
  if(argc != 2) {
    fprintf(stderr,"usage: %s input_byutr_file\n", argv[0]);
    exit(1);
  }
  
  /* attempt to open trace file */
  if ((ifp = fopen(argv[1],"rb")) == NULL) {
    fprintf(stderr,"cannot open %s for reading\n",argv[1]);
    exit(1);
  }
	
  while (!feof(ifp)) {
    /* get next address and process */
    if (NextAddress(ifp, &trace)) {
      AddressDecoder(&trace, stdout);
      i++;
      if ((i % 100000) == 0)
	fprintf(stderr,"%dK samples processed\r", i/100000);
    }
  }	

  /* clean up and return success */
  fclose(ifp);
  return (0);
}