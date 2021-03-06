#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "dev/pci/sis1100_var.h"

/*****************************************************************************/
static const char *hwnames[]={
    "invalid",
    "PCI",
    "VME",
    "CAMAC",
    "FZJ-LVDS-System"
};

static void
sis1100_print_ident(int p)
{
    struct sis1100_ident ident;

    if (ioctl(p, SIS1100_IDENT, &ident)<0) {
        printf("sis1100_print_ident: fcntl(IDENT): %s\n", strerror(errno));
        return;
    }
    printf("sis1100 local ident:\n");
    printf("  hw_type    = %d\n", ident.local.hw_type);
    printf("  hw_version = %d\n", ident.local.hw_version);
    printf("  fw_type    = %d\n", ident.local.fw_type);
    printf("  fw_version = %d\n", ident.local.fw_version);
    if (ident.remote_ok) {
        printf("sis1100 remote ident:\n");
        printf("  hw_type    = %d\n", ident.remote.hw_type);
        printf("  hw_version = %d\n", ident.remote.hw_version);
        printf("  fw_type    = %d\n", ident.remote.fw_type);
        printf("  fw_version = %d\n", ident.remote.fw_version);
        if (ident.remote.hw_type<sizeof(hwnames)/sizeof(const char*)) {
            printf("remote side is %s\n", hwnames[ident.remote.hw_type]);
        } else {
            printf("remote side is of unknown type %d\n", ident.remote.hw_type);
        }
        printf("remote side is O%sLINE\n", ident.remote_online?"N":"FF");
    } else {
        printf("remote side is OFFLINE\n");
    }
}
/****************************************************************************/
int main(int argc, char* argv[])
{
    int p, count=0;
    struct sis1100_irq_ctl irqctl;
    struct sis1100_irq_get irqget;

    if (argc!=2) {
        printf("usage: %s path\n", argv[0]);
        return 1;
    }

    if ((p=open(argv[1], O_RDWR, 0))<0) {
        printf("open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    irqctl.irq_mask=0;
    irqctl.signal=-1;
    if (ioctl(p, SIS1100_IRQ_CTL, &irqctl)<0) {
        printf("ioctl(SIS1100_IRQ_CTL): %s\n", strerror(errno));
        return 1;    
    }

    while (count++<10) {
        int n=0;
        irqget.irq_mask=0;
        do {
            n++;
            sleep(1);
            if (ioctl(p, SIS1100_IRQ_GET, &irqget)<0) {
                printf("ioctl(SIS1100_IRQ_GET): %s\n", strerror(errno));
                return 1;
            }
        } while (irqget.remote_status==0);

        printf("irqs=%08x, remote_status=%d, n=%d\n",
                irqget.irqs, irqget.remote_status, n);
        if (irqget.remote_status!=0) {
            sis1100_print_ident(p);
        }
        /* no acknowledge necessary */
    }

    irqctl.irq_mask=0;
    irqctl.signal=0;
    if (ioctl(p, SIS1100_IRQ_CTL, &irqctl)<0) {
        printf("ioctl(SIS1100_IRQ_CTL): %s\n", strerror(errno));
        return 1;    
    }

    close(p);
    return 0;    
}
/****************************************************************************/
/****************************************************************************/
