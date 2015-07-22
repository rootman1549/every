#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>

static char * get_mac(/*IN*/char* nic_name)
{
    int sock = 0;
    struct ifreq ifr;
    int i = 0;
    static char buf[64] = {0};

    memset(buf, 'F', 12);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    strcpy(ifr.ifr_name, nic_name);
    ioctl(sock, SIOCGIFADDR, &ifr);

    strcpy(ifr.ifr_name, nic_name);
    ioctl(sock, SIOCGIFHWADDR, &ifr);

    close(sock);

    for(i = 0; i != 6; ++i)
    {
        sprintf(buf + 2*i, "%02x", (unsigned char)ifr.ifr_hwaddr.sa_data[i]);
    }

    return buf;
}


int main()
{
    char *mac = NULL;
    mac = get_mac("eth0");
    printf("%s\n", mac);
    return 0;
}
