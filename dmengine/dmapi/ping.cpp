// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>
#include "ping.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

int ping(const char *servername,char **szad,unsigned long *pRoundTripTime)
{
	struct hostent *h = gethostbyname(servername);
	if (!h) {
		// Problem - name resolution failed
		*szad = (char *)0;
		return PING_NAME_RESOLUTION_FAILED;
	}
	char *ad = h->h_addr_list[0];
	*szad = (char *)malloc(16);
	sprintf(*szad,"%hd.%hd.%hd.%hd",(unsigned char)ad[0],(unsigned char)ad[1],(unsigned char)ad[2],(unsigned char)ad[3]);
	HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

	hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        return PING_INTERNAL_ERROR;	// internal error
    }
	ipaddr = inet_addr(*szad);

	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);
    if (ReplyBuffer == NULL) {
        return PING_INTERNAL_ERROR;	// internal error
    }
	dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
        NULL, ReplyBuffer, ReplySize, 5000);
    if (dwRetVal != 0) {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        struct in_addr ReplyAddr;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;
		if (pEchoReply->Status > 0) {
			// Something failed
			*pRoundTripTime = 0;
			return PING_NO_REPLY;
		}
        *pRoundTripTime = pEchoReply->RoundTripTime;
    }
    else {
       // Failed to ping
		*pRoundTripTime = 0;
        return PING_NO_REPLY;
    }
    return PING_OK;

}
#else
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
//#include <netinet/ip_var.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(__SUNOS__) || defined(__sunos__) // for bcopy
#include <strings.h>
#endif
#include <string.h>
#include <ctype.h>
#include "ping.h"
uint16_t in_cksum(uint16_t *addr, unsigned len);

#define	DEFDATALEN	(64-ICMP_MINLEN)	/* default data length */
#define	MAXIPLEN	60
#define	MAXICMPLEN	76
#define	MAXPACKET	(65536 - 60 - ICMP_MINLEN)/* max packet size */

int ping(const char *servername,char **szad,unsigned long *pRoundTripTime)
{
	struct hostent *h = gethostbyname(servername);
	if (!h) {
		// Problem - name resolution failed
		*szad = (char *)0;
		return PING_NAME_RESOLUTION_FAILED;
	}
      int s, i, cc, packlen, datalen = DEFDATALEN;
	struct hostent *hp;
	struct sockaddr_in to, from;
	//struct protoent	*proto;
	struct ip *ip;
	u_char *packet, outpack[MAXPACKET];
	char hnamebuf[1024];
	char hostname[1024];
	struct icmp *icp;
	int ret, fromlen, hlen;
	fd_set rfds;
	struct timeval tv;
	int retval;
	struct timeval start, end;
	int /*start_t, */end_t;
	bool cont = true;

	to.sin_family = AF_INET;

	// try to convert as dotted decimal address, else if that fails assume it's a hostname
	to.sin_addr.s_addr = inet_addr(servername);
	if (to.sin_addr.s_addr != (u_int)-1)
	{
		strcpy(hostname,servername);
	        unsigned char *ad = (unsigned char *)&(to.sin_addr.s_addr);
	        *szad = (char *)malloc(16);
	        sprintf(*szad,"%hd.%hd.%hd.%hd",(unsigned char)ad[0],(unsigned char)ad[1],(unsigned char)ad[2],(unsigned char)ad[3]);
	}
	else
	{
		hp = gethostbyname(servername);
		if (!hp)
		{
			 *pRoundTripTime = 0;
                         return PING_NO_REPLY;
		}
		to.sin_family = hp->h_addrtype;
		bcopy(hp->h_addr, (caddr_t)&to.sin_addr, hp->h_length);
                memset(hnamebuf,'\0',sizeof(hnamebuf));
		strncpy(hnamebuf, hp->h_name, sizeof(hnamebuf) - 1);
		strcpy(hostname,hnamebuf);
	        unsigned char *ad = (unsigned char *)h->h_addr_list[0];
	        *szad = (char *)malloc(16);
	        sprintf(*szad,"%hd.%hd.%hd.%hd",(unsigned char)ad[0],(unsigned char)ad[1],(unsigned char)ad[2],(unsigned char)ad[3]);
	}
	// Call external ping (IPPROTO_ICMP requires root privileges)
	char pingcmd[128];
	sprintf(pingcmd,"ping -q -c 1 %s",*szad);  // SBT remove -t 10 since its too short for AWS
	FILE *ppipe = popen(pingcmd,"r");
	char pingbuf[1024];
	while (fgets(pingbuf,sizeof(pingbuf),ppipe));
	int res = pclose(ppipe);
	if (res==0) {
		// last line will be something like:
		// round-trip min/avg/max/stddev = 0.042/0.042/0.042/0.000 ms
		char *p = strchr(pingbuf,'=');
		if (p) {
			// Value quoted is in milliseconds. So if we take the . out
			// we should have microseconds
			char *ep = strchr(p,'/');
			if (ep) *ep='\0';
			char *x=p+1;
			char *rtt = (char *)malloc(strlen(p)+1);
			char *y = rtt;
			while (*x) {
				if (*x>='0' && *x<='9') *y++=*x;
				x++;
			}
			*y='\0';
			*pRoundTripTime = atol(rtt);
			free(rtt);
		}
	}
	return (res==0)?PING_OK:PING_NO_REPLY;
#ifdef THIS_IS_OLD_CODE
	packlen = datalen + MAXIPLEN + MAXICMPLEN;
	if ( (packet = (u_char *)malloc((u_int)packlen)) == NULL)
	{
			 *pRoundTripTime = 0;
                         return PING_NO_REPLY;
	}

	if ( (s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		perror("socket");	/* probably not running as superuser */
			 *pRoundTripTime = 0;
                         return PING_NO_REPLY;
	}


	int pid = getpid() & 0x7fff;
	icp = (struct icmp *)outpack;
	icp->icmp_type = ICMP_ECHO;
	icp->icmp_code = 0;
	icp->icmp_cksum = 0;
	icp->icmp_seq = 12345;	/* seq and id must be reflected */
	icp->icmp_id = pid;

	cc = datalen + ICMP_MINLEN;
	icp->icmp_cksum = in_cksum((unsigned short *)icp,cc);

	gettimeofday(&start, NULL);

i = sendto(s, (char *)outpack, cc, 0, (struct sockaddr*)&to, (socklen_t)sizeof(struct sockaddr_in));
	if (i < 0 || i != cc)
	{
			 *pRoundTripTime = 0;
                         return PING_NO_REPLY;
	}

	// Watch stdin (fd 0) to see when it has input.
	FD_ZERO(&rfds);
	FD_SET(s, &rfds);
	// Wait up to one seconds.
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while(cont)
	{
		retval = select(s+1, &rfds, NULL, NULL, &tv);
		if (retval == -1)
		{
			 *pRoundTripTime = 0;
                         return PING_NO_REPLY;
		}
		else if (retval)
		{
			fromlen = sizeof(sockaddr_in);
			if ( (ret = recvfrom(s, (char *)packet, packlen, 0,(struct sockaddr *)&from, (socklen_t*)&fromlen)) < 0)
			{
			 *pRoundTripTime = 0;
                         return PING_NO_REPLY;
			}

			// Check the IP header
			ip = (struct ip *)((char*)packet);
			hlen = sizeof( struct ip );
			if (ret < (hlen + ICMP_MINLEN))
			{
			 *pRoundTripTime = 0;
                         return PING_NO_REPLY;
			}

			// Now the ICMP part
			icp = (struct icmp *)(packet + hlen);
			if (icp->icmp_type == ICMP_ECHOREPLY)
			{
				if (icp->icmp_seq != 12345)
				{
					continue;
				}
				if (icp->icmp_id != pid)
				{
					continue;
				}
				cont = false;
			}
			else
			{
				continue;
			}

			gettimeofday(&end, NULL);
			end_t = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

			if(end_t < 1)
				end_t = 1;

                        *pRoundTripTime = end_t;
		}
		else
		{
        *pRoundTripTime = 0;
        return PING_NO_REPLY;
		}
	}
    return PING_OK;
#endif
}

uint16_t in_cksum(uint16_t *addr, unsigned len)
{
  uint16_t answer = 0;
  /*
   * Our algorithm is simple, using a 32 bit accumulator (sum), we add
   * sequential 16 bit words to it, and at the end, fold back all the
   * carry bits from the top 16 bits into the lower 16 bits.
   */
  uint32_t sum = 0;
  while (len > 1)  {
    sum += *addr++;
    len -= 2;
  }

  // mop up an odd byte, if necessary
  if (len == 1) {
    *(unsigned char *)&answer = *(unsigned char *)addr ;
    sum += answer;
  }

  // add back carry outs from top 16 bits to low 16 bits
  sum = (sum >> 16) + (sum & 0xffff); // add high 16 to low 16
  sum += (sum >> 16); // add carry
  answer = ~sum; // truncate to 16 bits
  return answer;
}
#endif
