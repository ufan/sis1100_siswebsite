#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "dev/pci/sis1100_var.h"

int p;

/* das sind die CAEN-Module, die ich zufaellig habe, beliebig durcheinander-
   gewuerfelt und vervielfacht */
struct sis1100_pipelist list[]={
    {0x0F010400, 0x09, 0x38000000, 1},
    {0x0F010400, 0x09, 0x38000000, 0x10000},
    {0x0F010400, 0x09, 0x38000000, 1},
    {0x0F010400, 0x09, 0x38000000, 0x10000},
    {0x0c010000, 0x09, 0x300fa, 0},
    {0x0c010000, 0x09, 0x100fa, 0},
    {0x03010000, 0x09, 0x300fc, 0},
    {0x0c010000, 0x09, 0x100fe, 0},
    {0x0c010000, 0x09, 0x300fe, 0},
    {0x0c010000, 0x09, 0x400fa, 0},
    {0x03010000, 0x09, 0x400fc, 0},
    {0x0c010000, 0x09, 0x400fe, 0},
    {0x0c010000, 0x09, 0x500fa, 0},
    {0x03010000, 0x09, 0x500fc, 0},
    {0x03010000, 0x09, 0x100fc, 0},
    {0x0c010000, 0x09, 0x200fa, 0},
    {0x03010000, 0x09, 0x200fc, 0},
    {0x0c010000, 0x09, 0x200fe, 0},
    {0x0c010000, 0x09, 0x300fa, 0},
    {0x0c010000, 0x09, 0x100fa, 0},
    {0x03010000, 0x09, 0x300fc, 0},
    {0x0c010000, 0x09, 0x100fe, 0},
    {0x0c010000, 0x09, 0x300fe, 0},
    {0x0c010000, 0x09, 0x400fa, 0},
    {0x03010000, 0x09, 0x100fc, 0},
    {0x0c010000, 0x09, 0x200fa, 0},
    {0x03010000, 0x09, 0x200fc, 0},
    {0x0c010000, 0x09, 0x200fe, 0},
    {0x03010000, 0x09, 0x100fc, 0},
    {0x0c010000, 0x09, 0x200fa, 0},
    {0x03010000, 0x09, 0x200fc, 0},
    {0x0c010000, 0x09, 0x200fe, 0},
    {0x0c010000, 0x09, 0x300fa, 0},
    {0x0c010000, 0x09, 0x100fa, 0},
    {0x03010000, 0x09, 0x300fc, 0},
    {0x0c010000, 0x09, 0x100fe, 0},
    {0x0c010000, 0x09, 0x300fe, 0},
    {0x0c010000, 0x09, 0x400fa, 0},
    {0x03010000, 0x09, 0x400fc, 0},
    {0x0c010000, 0x09, 0x400fe, 0},
    {0x0c010000, 0x09, 0x500fa, 0},
    {0x03010000, 0x09, 0x500fc, 0},
    {0x03010000, 0x09, 0x100fc, 0},
    {0x0c010000, 0x09, 0x200fa, 0},
    {0x03010000, 0x09, 0x200fc, 0},
    {0x0c010000, 0x09, 0x200fe, 0},
    {0x0c010000, 0x09, 0x300fa, 0},
    {0x0c010000, 0x09, 0x100fa, 0},
    {0x03010000, 0x09, 0x300fc, 0},
    {0x0c010000, 0x09, 0x100fe, 0},
    {0x0c010000, 0x09, 0x300fe, 0},
    {0x0c010000, 0x09, 0x400fa, 0},
    {0x0c010000, 0x09, 0x300fa, 0},
    {0x0c010000, 0x09, 0x100fa, 0},
    {0x03010000, 0x09, 0x300fc, 0},
    {0x0c010000, 0x09, 0x100fe, 0},
    {0x0c010000, 0x09, 0x300fe, 0},
    {0x0c010000, 0x09, 0x400fa, 0},
    {0x03010000, 0x09, 0x400fc, 0},
    {0x0c010000, 0x09, 0x400fe, 0},
    {0x0c010000, 0x09, 0x500fa, 0},
    {0x03010000, 0x09, 0x500fc, 0},
    {0x03010000, 0x09, 0x100fc, 0},
    {0x0c010000, 0x09, 0x200fa, 0},
    {0x03010000, 0x09, 0x200fc, 0},
    {0x0c010000, 0x09, 0x200fe, 0},
    {0x0c010000, 0x09, 0x300fa, 0},
    {0x0c010000, 0x09, 0x100fa, 0},
    {0x03010000, 0x09, 0x300fc, 0},
    {0x0c010000, 0x09, 0x100fe, 0},
    {0x0c010000, 0x09, 0x300fe, 0},
    {0x0c010000, 0x09, 0x400fa, 0},
    {0x03010000, 0x09, 0x400fc, 0},
    {0x0c010000, 0x09, 0x400fe, 0},
    {0x0c010000, 0x09, 0x500fa, 0},
    {0x03010000, 0x09, 0x500fc, 0},
    {0x03010000, 0x09, 0x100fc, 0},
    {0x0c010000, 0x09, 0x200fa, 0},
    {0x03010000, 0x09, 0x200fc, 0},
    {0x0c010000, 0x09, 0x200fe, 0},
    {0x0c010000, 0x09, 0x300fa, 0},
    {0x0c010000, 0x09, 0x100fa, 0},
    {0x03010000, 0x09, 0x300fc, 0},
    {0x0c010000, 0x09, 0x100fe, 0},
    {0x0c010000, 0x09, 0x300fe, 0},
    {0x0c010000, 0x09, 0x400fa, 0},
    {0x03010000, 0x09, 0x400fc, 0},
    {0x0c010000, 0x09, 0x400fe, 0},
    {0x0c010000, 0x09, 0x500fa, 0},
    {0x03010000, 0x09, 0x500fc, 0},
    {0x03010000, 0x09, 0x300fc, 0},
    {0x0c010000, 0x09, 0x100fe, 0},
    {0x0c010000, 0x09, 0x300fe, 0},
    {0x0c010000, 0x09, 0x400fa, 0},
    {0x03010000, 0x09, 0x400fc, 0},
    {0x0c010000, 0x09, 0x400fe, 0},
    {0x0c010000, 0x09, 0x500fa, 0},
    {0x03010000, 0x09, 0x500fc, 0},
    {0x03010000, 0x09, 0x100fc, 0},
    {0x0c010000, 0x09, 0x200fa, 0},
    {0x03010000, 0x09, 0x200fc, 0},
    {0x0c010000, 0x09, 0x200fe, 0},
    {0x0c010000, 0x09, 0x300fa, 0},
    {0x0c010000, 0x09, 0x100fa, 0},
    {0x03010000, 0x09, 0x300fc, 0},
    {0x0c010000, 0x09, 0x100fe, 0},
    {0x0c010000, 0x09, 0x300fe, 0},
    {0x0c010000, 0x09, 0x400fa, 0},
    {0x03010000, 0x09, 0x400fc, 0},
    {0x0c010000, 0x09, 0x400fe, 0},
    {0x0c010000, 0x09, 0x500fa, 0},
    {0x03010000, 0x09, 0x500fc, 0},
    {0x0c010000, 0x09, 0x500fe, 0}
};


static int pipeline_read(int p, struct sis1100_pipelist* list, int listlen,
    u_int32_t* data, int seq)
{
    struct sis1100_pipe pipe;

    pipe.num=listlen;
    pipe.list=list;
    pipe.data=data;

    if (ioctl(p, SIS1100_PIPE, &pipe)<0) {
	printf("ioctl(SIS1100_PIPE): %s\n", strerror(errno));
        return -1;
    }
    if (pipe.error) printf("error=0x%x\n", pipe.error);
    return 0;
}

volatile int stop=0;

static void hand(int sig)
{
printf("signal %d\n", sig);
stop=1;
}

int main(int argc, char* argv[])
{
    int num, loopcount, reqcount, i, j, *data;
    int* comp, comp_valid, dot;
    struct sigaction act;

    if (argc<4)
      {
      fprintf(stderr, "usage: %s path reqcount loopcount\n", argv[0]);
      return 1;
      }
    if ((p=open(argv[1], O_RDWR, 0))<0) {
        perror("open");
        return 1;
    }

    reqcount=atoi(argv[2]);
    loopcount=atoi(argv[3]);

    act.sa_handler=hand;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    sigaction(SIGINT, &act, 0);
    sigaction(SIGQUIT, &act, 0);

    num=sizeof(list)/sizeof(struct sis1100_pipelist);
    if (reqcount<num) num=reqcount;
    printf("listlen=%d; loopcount=%d\n", num, loopcount);

    data=(u_int32_t*)malloc(num*sizeof(u_int32_t));
    comp=(u_int32_t*)malloc(num*sizeof(u_int32_t));
    comp_valid=0;

    if (!data) {
    	printf("malloc: %s\n", strerror(errno));
	return 1;
    }
    /* for (i=0; i<num; i++) data[i]=0x12345678;  just for test */
    dot=10000/num;
    for (j=0; j<loopcount; j++) {
    	if (stop || (pipeline_read(p, list, num, data, j)<0)) goto raus;
        if (comp_valid) {
            for (i=0; i<num; i++) {
                if (comp[i]!=data[i]) printf("[%d] %08x-->%08x\n",
                    i, comp[i], data[i]);
            }
        } else {
            for (i=0; i<num; i++) comp[i]=data[i];
            comp_valid=1;
        }
        if ((j%dot)==0) {printf("."); fflush(stdout);}
    }
raus:
    printf("tranferred %d words\n", j*num);
    for (i=0; i<num; i++)
    	printf("[%2d] %x: %8.8x\n", i, list[i].addr, data[i]);
    close(p);
    return 0;
}
