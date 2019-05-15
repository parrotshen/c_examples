#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
    char *pIfName = "eth0";
    struct ifreq ifreq;
    int fd;

    if (argc > 1)
    {
        pIfName = argv[1];
    }

    fd = socket(AF_PACKET, SOCK_PACKET, 0);
    if (fd < 0)
    {
        perror ("socket");
        return -1;
    }

    strcpy(ifreq.ifr_name, pIfName);

    /* Interface index */
    {
        if (ioctl(fd, SIOCGIFINDEX, &ifreq) < 0)
        {
            perror ("SIOCGIFINDEX");
            close( fd );
            return -1;
        }

        printf("ifr_index   %d\n", ifreq.ifr_ifindex);
    }

    /* MAC address */
    {
        unsigned char *pMac;

        if (ioctl(fd, SIOCGIFHWADDR, &ifreq) < 0)
        {
            perror ("SIOCGIFHWADDR");
            close( fd );
            return -1;
        }

        pMac = (unsigned char *)(ifreq.ifr_hwaddr.sa_data);
        printf(
            "ifr_hwaddr  %02X:%02X:%02X:%02X:%02X:%02X\n",
            pMac[0],
            pMac[1],
            pMac[2],
            pMac[3],
            pMac[4],
            pMac[5]
        );
    }

    /* IP address */
    {
        struct sockaddr_in  *pSaAddr;
        struct sockaddr_in6 *pSaAddr6;

        if (ioctl(fd, SIOCGIFADDR, &ifreq) < 0)
        {
            perror ("SIOCGIFADDR");
            close( fd );
            return -1;
        }

        pSaAddr = (struct sockaddr_in *)&(ifreq.ifr_addr);
        if (pSaAddr->sin_family == AF_INET)
        {
            printf("ifr_addr    %s\n", inet_ntoa(pSaAddr->sin_addr));
        }
        else if (pSaAddr->sin_family == AF_INET6)
        {
            pSaAddr6 = (struct sockaddr_in6 *)&(ifreq.ifr_addr);
            printf("ifr_addr    %02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X\n",
                pSaAddr6->sin6_addr.s6_addr[0],
                pSaAddr6->sin6_addr.s6_addr[1],
                pSaAddr6->sin6_addr.s6_addr[2],
                pSaAddr6->sin6_addr.s6_addr[3],
                pSaAddr6->sin6_addr.s6_addr[4],
                pSaAddr6->sin6_addr.s6_addr[5],
                pSaAddr6->sin6_addr.s6_addr[6],
                pSaAddr6->sin6_addr.s6_addr[7],
                pSaAddr6->sin6_addr.s6_addr[8],
                pSaAddr6->sin6_addr.s6_addr[9],
                pSaAddr6->sin6_addr.s6_addr[10],
                pSaAddr6->sin6_addr.s6_addr[11],
                pSaAddr6->sin6_addr.s6_addr[12],
                pSaAddr6->sin6_addr.s6_addr[13],
                pSaAddr6->sin6_addr.s6_addr[14],
                pSaAddr6->sin6_addr.s6_addr[15]
            );
        }
    }

    /* MTU */
    {
        if (ioctl(fd, SIOCGIFMTU, &ifreq) < 0)
        {
            perror ("SIOCGIFMTU");
            close( fd );
            return -1;
        }

        printf("ifr_mtu     %d\n", ifreq.ifr_mtu);
    }

    close( fd );

    return 0;
}

