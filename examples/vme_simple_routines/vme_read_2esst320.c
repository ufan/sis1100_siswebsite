#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>


#include "dev/pci/sis1100_var.h"
#include "sis3100_vme_calls.h"


/*
int soll_data ;
int max_lword_cnt ;
int loop_cnt ;
*/


/****************************************************************************/
int main(int argc, char* argv[])
{

#define MAX_NUMBER_LWORDS 0x100000 /* 256MB */

int p;
int mod_base;
int no_of_lwords ;
int i, k;

u_int32_t blt_data[MAX_NUMBER_LWORDS] ;
u_int32_t get_lwords ;
int return_code ;
int loop_counter ;
   struct sis1100_vme_block_req block_req;
   struct timeval tstart, tend;
   double tdiff, bandwidth;

if (argc<4)
  {
  fprintf(stderr, "usage: %s PATH  VME_BASE_ADDR  NO_OF_LWORDS [LOOP_COUNTER]  \n", argv[0]);
  return 1;
  }

if ((p=open(argv[1], O_RDWR, 0))<0) {
      printf("error opening device\n");
      return 1;
   }

/* open VME */
/*
   if ((p=open("/tmp/sis1100", O_RDWR, 0))<0) {
     printf("error on opening VME environment\n");
     return -1;
   }
*/
mod_base     = strtoul(argv[2],NULL,0) ;
no_of_lwords = strtoul(argv[3],NULL,0) ;

loop_counter  = 1 ;
if (argc>4) loop_counter  = strtoul(argv[4],NULL,0) ;


if (no_of_lwords > MAX_NUMBER_LWORDS)
  {
     printf("no_of_lwords (0x%08x) must be lower then MAX_NUMBER_LWORDS (0x%08x)\n",no_of_lwords, MAX_NUMBER_LWORDS);
     return -1;
   }

   printf( "adr: %08X\n", mod_base );
   printf( "req: %08X\n", no_of_lwords );
   
  for( k = 0;k < loop_counter;k++ ){
   
    gettimeofday( &tstart, NULL );
    for (i=0; i < loop_counter; i++) {
      /* return_code =   vme_A32BLT32_read(p, mod_base, blt_data, no_of_lwords, &get_lwords) ; */

      block_req.num=no_of_lwords   ; /*  */
      block_req.fifo=1;
      block_req.size=4;
      block_req.am=0x260;
      block_req.addr=mod_base ;
      block_req.data = (u_int8_t*)blt_data ;
      if (ioctl(p, SIS3100_VME_BLOCK_READ, &block_req)<0){
	printf( "ioctl != 0\n" );
      }
      get_lwords = block_req.num;
      return_code = block_req.error;
      if( return_code != 0 ){
	printf( "return_code: %d\n", return_code );
	return -1;
      }
    }
    gettimeofday( &tend, NULL );
    tdiff = (tend.tv_sec - tstart.tv_sec) * 1000000;
    tdiff += tend.tv_usec - tstart.tv_usec;
    bandwidth = loop_counter * no_of_lwords * 4; /* loops * 4bytes * lword_count */
    bandwidth /= tdiff;

    printf( "%d bytes in %.2f secs = %.2f MB/s\n", loop_counter * no_of_lwords * 4, tdiff / 1000000, bandwidth );

    printf("vme_A32_2ESST320_read:   return_code = 0x%08x\n", return_code );
    printf("vme_A32_2ESST320_read:   get_lwords  = 0x%08x\n", get_lwords );
    
  }

   for (i=0;(i)<get_lwords;i++)
    {
      /* printf("i = 0x%08x     Data = 0x%08x\n",i, blt_data[i]); */
    }




close(p);
return 0;
}





 





























