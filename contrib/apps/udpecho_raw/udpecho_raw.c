/*
 * Copyright (c) 2016 Stephan Linz <linz@li-pro.net>, Li-Pro.Net
 * All rights reserved.
 *
 * Based on examples provided by
 * Iwan Budi Kusnanto <ibk@labhijau.net> (https://gist.github.com/iwanbk/1399729)
 * Juri Haberland <juri@sapienti-sat.org> (https://lists.gnu.org/archive/html/lwip-users/2007-06/msg00078.html)
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Stephan Linz rewrote this file to get a basic echo example.
 */

/**
 * @file
 * UDP echo server example using raw API.
 *
 * Echos all bytes sent by connecting client,
 * and passively closes when client is done.
 *
 */

#ifndef UDP_DEBUG
#define UDP_DEBUG     LWIP_DBG_ON
#endif


#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/udp.h"
#include "udpecho_raw.h"

#define INCL_WINSOCK_API_PROTOTYPES 0
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
static const ip_addr_t* udp_target;
static u16_t udp_target_port;
static struct udp_pcb* udpecho_raw_pcb;
#if LWIP_UDP

static struct udp_pcb *udpecho_raw_pcb;

static void
udpecho_raw_recv(void *arg, struct udp_pcb *upcb, struct pbuf *p,
                 const ip_addr_t *addr, u16_t port)
{
  LWIP_UNUSED_ARG(arg);
  if (p != NULL) {
    /* send received packet back to sender */
    udp_sendto(upcb, p, addr, port);

    /*printf("udp: recv ");
    LWIP_DEBUGF(UDP_DEBUG, ("udp: recv "));


    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in destAddr;
    int result;

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return -1;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    // Set destination address
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(54321);
    inet_pton(AF_INET, "192.168.41.4", &destAddr.sin_addr);
    //const char* message = "hello from lwip2";
    result = sendto(sock, (const char*)p->payload, p->tot_len, 0, (struct sockaddr*)&destAddr, sizeof(destAddr));
    if (result == SOCKET_ERROR) {
        printf("sendto failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return -1;
    }*/


    /* free the pbuf */
    pbuf_free(p);
  }
  


  
}
static void
udpecho_raw_send(struct udp_pcb* upcb)
{
    WSADATA wsaData;
    SOCKET recvSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int recvLen;
    u_char recvBuf[2048];

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    recvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (recvSocket == INVALID_SOCKET) {
        // טיפול בשגיאה
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54321);  // פורט לדוגמה
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(recvSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(recvSocket);
        WSACleanup();
        return;
    }


    //think how to input
   
   // do
   // {
        recvLen = recvfrom(recvSocket, (char*)recvBuf, sizeof(recvBuf), 0,
            (SOCKADDR*)&clientAddr, &clientAddrLen);
        if (recvLen == SOCKET_ERROR) {
            // טיפול בשגיאה או יציאה
        //    break;
        }

        const u_char* packet = recvBuf;

        // כאן מעבירים את הפקטט ללווייפ, כמו שקורה בפונקציה המקורית

        //struct pcap_pkthdr pkt_header;
        //const u_char* packet = pcap_next(pa->adapter, &pkt_header);*/
        if (packet != NULL) {
            //think avout sending the len vs pcap header
            //pcapif_input((u_char*)pa, recvLen/*&pkt_header,*/, packet);
        }

        const char* data = packet;
        //"Hello, Echo!";
        if (upcb == NULL) {
            return; // PCB not initialized
        }

        struct pbuf* p = pbuf_alloc(PBUF_TRANSPORT, recvLen, PBUF_RAM);
        if (p != NULL) {
            memcpy(p->payload, data, recvLen);
            //
            udp_sendto(udpecho_raw_pcb, p, udp_target, udp_target_port);
            pbuf_free(p);
        }
    //} while (1);

    
}

void
udpecho_raw_init(const ip_addr_t* ping_addr, const u16_t port)
{
    udp_target = ping_addr;
    udp_target_port = port;
  udpecho_raw_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
  if (udpecho_raw_pcb != NULL) {
    err_t err;

    err = udp_bind(udpecho_raw_pcb, IP_ANY_TYPE, 10);
    if (err == ERR_OK) {
      udp_recv(udpecho_raw_pcb, udpecho_raw_recv, NULL);
    } else {
      /* abort? output diagnostic? */
    }
  } else {
    /* abort? output diagnostic? */
  }
  sys_timeout(1000, udpecho_raw_send, udpecho_raw_pcb);
}

#endif /* LWIP_UDP */
