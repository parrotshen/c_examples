#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>


int main(void)
{
    struct ifaddrs *pIfAddr = NULL;
    struct ifaddrs *pIfNext = NULL;
    struct sockaddr *pSaAddr;
    int  retval;

    retval = getifaddrs( &pIfAddr );
    if (retval != 0)
    {
        perror( "getifaddrs" );
        return -1;
    }

    pIfNext = pIfAddr;
    while ( pIfNext )
    {
        pSaAddr = pIfNext->ifa_addr;

        if ( pSaAddr )
        {
            if (pSaAddr->sa_family == AF_PACKET)
            {
                struct sockaddr_ll *pSaAddrLl; /* link-layer */

                pSaAddrLl = (struct sockaddr_ll *)pIfNext->ifa_addr;
                printf("[AF_PACKET]\n");
                printf("ifa_name     %s\n", pIfNext->ifa_name);
                printf("ifa_addr     %02x:%02x:%02x:%02x:%02x:%02x\n",
                    pSaAddrLl->sll_addr[0],
                    pSaAddrLl->sll_addr[1],
                    pSaAddrLl->sll_addr[2],
                    pSaAddrLl->sll_addr[3],
                    pSaAddrLl->sll_addr[4],
                    pSaAddrLl->sll_addr[5]
                );
                printf("ifa_flags    0x%08X\n", pIfNext->ifa_flags);
                printf("\n");
            }
            else if (pSaAddr->sa_family == AF_INET)
            {
                struct sockaddr_in *pSaAddr4; /* IPv4 */
                struct sockaddr_in *pSaMask4;

                pSaAddr4 = (struct sockaddr_in *)pIfNext->ifa_addr;
                pSaMask4 = (struct sockaddr_in *)pIfNext->ifa_netmask;
                printf("[AF_INET]\n");
                printf("ifa_name     %s\n", pIfNext->ifa_name);
                printf("ifa_addr     %s\n", inet_ntoa(pSaAddr4->sin_addr));
                printf("ifa_netmask  %s\n", inet_ntoa(pSaMask4->sin_addr));
                printf("ifa_flags    0x%08X\n", pIfNext->ifa_flags);
                printf("\n");
            }
            else if (pSaAddr->sa_family == AF_INET6)
            {
                struct sockaddr_in6 *pSaAddr6; /* IPv6 */
                struct sockaddr_in6 *pSaMask6;

                pSaAddr6 = (struct sockaddr_in6 *)pIfNext->ifa_addr;
                pSaMask6 = (struct sockaddr_in6 *)pIfNext->ifa_netmask;
                printf("[AF_INET6]\n");
                printf("ifa_name     %s\n", pIfNext->ifa_name);
                printf("ifa_addr     %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
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
                printf("ifa_netmask  %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
                    pSaMask6->sin6_addr.s6_addr[0],
                    pSaMask6->sin6_addr.s6_addr[1],
                    pSaMask6->sin6_addr.s6_addr[2],
                    pSaMask6->sin6_addr.s6_addr[3],
                    pSaMask6->sin6_addr.s6_addr[4],
                    pSaMask6->sin6_addr.s6_addr[5],
                    pSaMask6->sin6_addr.s6_addr[6],
                    pSaMask6->sin6_addr.s6_addr[7],
                    pSaMask6->sin6_addr.s6_addr[8],
                    pSaMask6->sin6_addr.s6_addr[9],
                    pSaMask6->sin6_addr.s6_addr[10],
                    pSaMask6->sin6_addr.s6_addr[11],
                    pSaMask6->sin6_addr.s6_addr[12],
                    pSaMask6->sin6_addr.s6_addr[13],
                    pSaMask6->sin6_addr.s6_addr[14],
                    pSaMask6->sin6_addr.s6_addr[15]
                );
                printf("ifa_flags    0x%08X\n", pIfNext->ifa_flags);
                printf("\n");
            }
        }

        pIfNext = pIfNext->ifa_next;
    }

    freeifaddrs( pIfAddr );

    return 0;
}

