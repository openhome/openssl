#define LWIP_TCP                        1
#define LWIP_DHCP                       1
#define LWIP_IGMP                       1
#define LWIP_DNS                        1

#define SO_REUSE                        1

#define MEM_LIBC_MALLOC                 1
#define MEMP_MEM_MALLOC                 1

#define MEMP_NUM_SYS_TIMEOUT            20
#define MEMP_NUM_NETBUF                 100
#define MEMP_NUM_RAW_PCB                4
#define MEMP_NUM_UDP_PCB                100
#define MEMP_NUM_TCP_PCB                100
#define MEMP_NUM_TCP_PCB_LISTEN         100
#define MEMP_NUM_NETCONN                ( MEMP_NUM_RAW_PCB + MEMP_NUM_UDP_PCB + MEMP_NUM_TCP_PCB + MEMP_NUM_TCP_PCB_LISTEN )

#define LWIP_DEBUG                      1

#define LWIP_COMPAT_SOCKETS             1
#define LWIP_POSIX_SOCKETS_IO_NAMES     1

#define ERRNO

/*
These are errnos are not provided by our toolchain,
so we define them here. They're taken from

${LWIP_SOURCE}/include/lwip/arch.h
*/

#define ENSROK                    0 /* DNS server returned answer with no data */
#define ENSRNODATA              160 /* DNS server returned answer with no data */
#define ENSRFORMERR             161 /* DNS server claims query was misformatted */
#define ENSRSERVFAIL            162 /* DNS server returned general failure */
#define ENSRNOTFOUND            163 /* Domain name not found */
#define ENSRNOTIMP              164 /* DNS server does not implement requested operation */
#define ENSRREFUSED             165 /* DNS server refused query */
#define ENSRBADQUERY            166 /* Misformatted DNS query */
#define ENSRBADNAME             167 /* Misformatted domain name */
#define ENSRBADFAMILY           168 /* Unsupported address family */
#define ENSRBADRESP             169 /* Misformatted DNS reply */
#define ENSRCONNREFUSED         170 /* Could not contact DNS servers */
#define ENSRTIMEOUT             171 /* Timeout while contacting DNS servers */
#define ENSROF                  172 /* End of file */
#define ENSRFILE                173 /* Error reading file */
#define ENSRNOMEM               174 /* Out of memory */
#define ENSRDESTRUCTION         175 /* Application terminated lookup */
#define ENSRQUERYDOMAINTOOLONG  176 /* Domain name is too long */
#define ENSRCNAMELOOP           177 /* Domain name is too long */


#define DEFAULT_UDP_RECVMBOX_SIZE       40

#define DEFAULT_TCP_RECVMBOX_SIZE       40
#define TCPIP_THREAD_STACKSIZE          (80 * 1024)
#define TCPIP_THREAD_PRIO               140
#define TCP_SND_BUF                     (2*TCP_MSS)

#define LWIP_NETIF_API                  1
#define LWIP_HAVE_LOOPIF                1
#define LWIP_NETIF_LINK_CALLBACK        1
#define LWIP_NETIF_STATUS_CALLBACK      1
#define LWIP_NETIF_LOOPBACK             1

#define SO_REUSE_RXTOALL                1
#define LWIP_SO_RCVBUF                  1
#define LWIP_TCP_KEEPALIVE              1

#define LWIP_CHKSUM_ALGORITHM           2
#define LWIP_CHECKSUM_ON_COPY           1

#define LWIP_RAW                        0

#define PBUF_POOL_SIZE                  512


#if 1

//#define LWIP_DBG_TYPES_ON               LWIP_DBG_ON
//#define ETHARP_DEBUG                    LWIP_DBG_ON
//#define NETIF_DEBUG                     LWIP_DBG_ON
//#define PBUF_DEBUG                      LWIP_DBG_ON
//#define API_LIB_DEBUG                   LWIP_DBG_ON
//#define API_MSG_DEBUG                   LWIP_DBG_ON
//#define SOCKETS_DEBUG                   LWIP_DBG_ON
//#define ICMP_DEBUG                      LWIP_DBG_ON
//#define IGMP_DEBUG                      LWIP_DBG_ON
//#define INET_DEBUG                      LWIP_DBG_ON
//#define IP_DEBUG                        LWIP_DBG_ON
//#define IP_REASS_DEBUG                  LWIP_DBG_ON
//#define RAW_DEBUG                       LWIP_DBG_ON
//#define MEM_DEBUG                       LWIP_DBG_ON
//#define MEMP_DEBUG                      LWIP_DBG_ON
//#define SYS_DEBUG                       LWIP_DBG_ON
//#define TIMERS_DEBUG                    LWIP_DBG_ON
//#define TCP_DEBUG                       LWIP_DBG_ON
//#define TCP_INPUT_DEBUG                 LWIP_DBG_ON
//#define TCP_FR_DEBUG                    LWIP_DBG_ON
//#define TCP_RTO_DEBUG                   LWIP_DBG_ON
//#define TCP_CWND_DEBUG                  LWIP_DBG_ON
//#define TCP_WND_DEBUG                   LWIP_DBG_ON
//#define TCP_OUTPUT_DEBUG                LWIP_DBG_ON
//#define TCP_RST_DEBUG                   LWIP_DBG_ON
//#define TCP_QLEN_DEBUG                  LWIP_DBG_ON
//#define UDP_DEBUG                       LWIP_DBG_ON
//#define TCPIP_DEBUG                     LWIP_DBG_ON
//#define PPP_DEBUG                       LWIP_DBG_ON
//#define SLIP_DEBUG                      LWIP_DBG_ON
//#define DHCP_DEBUG                      LWIP_DBG_ON
//#define AUTOIP_DEBUG                    LWIP_DBG_ON
//#define SNMP_MSG_DEBUG                  LWIP_DBG_ON
//#define SNMP_MIB_DEBUG                  LWIP_DBG_ON
//#define DNS_DEBUG                       LWIP_DBG_ON

#else

#define LWIP_DBG_TYPES_ON               LWIP_DBG_ON
#define ETHARP_DEBUG                    LWIP_DBG_ON
#define NETIF_DEBUG                     LWIP_DBG_ON
#define PBUF_DEBUG                      LWIP_DBG_ON
#define API_LIB_DEBUG                   LWIP_DBG_ON
#define API_MSG_DEBUG                   LWIP_DBG_ON
#define SOCKETS_DEBUG                   LWIP_DBG_ON
#define ICMP_DEBUG                      LWIP_DBG_ON
#define IGMP_DEBUG                      LWIP_DBG_ON
#define INET_DEBUG                      LWIP_DBG_ON
#define IP_DEBUG                        LWIP_DBG_ON
#define IP_REASS_DEBUG                  LWIP_DBG_ON
#define RAW_DEBUG                       LWIP_DBG_ON
#define MEM_DEBUG                       LWIP_DBG_ON
#define MEMP_DEBUG                      LWIP_DBG_ON
#define SYS_DEBUG                       LWIP_DBG_ON
#define TIMERS_DEBUG                    LWIP_DBG_ON
#define TCP_DEBUG                       LWIP_DBG_ON
#define TCP_INPUT_DEBUG                 LWIP_DBG_ON
#define TCP_FR_DEBUG                    LWIP_DBG_ON
#define TCP_RTO_DEBUG                   LWIP_DBG_ON
#define TCP_CWND_DEBUG                  LWIP_DBG_ON
#define TCP_WND_DEBUG                   LWIP_DBG_ON
#define TCP_OUTPUT_DEBUG                LWIP_DBG_ON
#define TCP_RST_DEBUG                   LWIP_DBG_ON
#define TCP_QLEN_DEBUG                  LWIP_DBG_ON
#define UDP_DEBUG                       LWIP_DBG_ON
#define TCPIP_DEBUG                     LWIP_DBG_ON
#define PPP_DEBUG                       LWIP_DBG_ON
#define SLIP_DEBUG                      LWIP_DBG_ON
#define DHCP_DEBUG                      LWIP_DBG_ON
#define AUTOIP_DEBUG                    LWIP_DBG_ON
#define SNMP_MSG_DEBUG                  LWIP_DBG_ON
#define SNMP_MIB_DEBUG                  LWIP_DBG_ON
#define DNS_DEBUG                       LWIP_DBG_ON

#endif
