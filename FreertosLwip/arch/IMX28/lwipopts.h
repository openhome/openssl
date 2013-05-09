#define NO_SYS                          0
#define SYS_LIGHTWEIGHT_PROT            1

/* Pool-based memory allocation */
#define MEM_USE_POOLS                   1
#define MEMP_USE_CUSTOM_POOLS           1
#define MEM_USE_POOLS_TRY_BIGGER_POOL   1
#define MEMP_SEPARATE_POOLS             1

#define MEMP_OVERFLOW_CHECK             0

#define MEM_ALIGNMENT                   4

/* ARM cache line alignment */
#define LWIP_MEM_ALIGN_SIZE(size)       (((size) + 31) & ~31)

#define MEMP_NUM_PBUF                   64
#define MEMP_NUM_UDP_PCB                128
#define MEMP_NUM_TCP_PCB                1024
#define MEMP_NUM_TCP_PCB_LISTEN         32
#define MEMP_NUM_TCP_SEG                384
#define MEMP_NUM_REASSDATA              64
#define MEMP_NUM_ARP_QUEUE              32
#define MEMP_NUM_IGMP_GROUP             16
#define MEMP_NUM_NETBUF                 768
#define MEMP_NUM_NETCONN                256
#define MEMP_NUM_TCPIP_MSG_API          32
#define MEMP_NUM_TCPIP_MSG_INPKT        256

#define PBUF_POOL_SIZE                  512

#define LWIP_ARP                        1
#define ARP_QUEUEING                    1
#define ETH_PAD_SIZE                    2

#define IP_REASS_MAX_PBUFS              512

#define LWIP_ICMP                       1

#define LWIP_RAW                        0

#define LWIP_DHCP                       1

#define LWIP_IGMP                       1

#define LWIP_DNS                        1
#define DNS_TABLE_SIZE                  16

#define LWIP_UDP                        1

#define LWIP_TCP                        1
#define TCP_MSS                         1460
#define TCP_WND                         (32 * 1024)
#define TCP_SND_BUF                     TCP_WND
#define TCP_LISTEN_BACKLOG              1
#define LWIP_TCP_TIMESTAMPS             1

#define PBUF_POOL_BUFSIZE               LWIP_MEM_ALIGN_SIZE(1600)

#define LWIP_NETIF_API                  1
#define LWIP_NETIF_STATUS_CALLBACK      1
#define LWIP_NETIF_LINK_CALLBACK        1
#define LWIP_NETIF_LOOPBACK             1

#define LWIP_HAVE_LOOPIF                1

#define TCPIP_THREAD_STACKSIZE          (60 * 1024)
#define TCPIP_THREAD_PRIO               143
#define TCPIP_MBOX_SIZE                 384

#define DEFAULT_THREAD_PRIO             142
#define DEFAULT_UDP_RECVMBOX_SIZE       256
#define DEFAULT_TCP_RECVMBOX_SIZE       256
#define DEFAULT_ACCEPTMBOX_SIZE         256

#define LWIP_NETCONN                    1

#define LWIP_SOCKET                     1
#define LWIP_COMPAT_SOCKETS             1
#define LWIP_POSIX_SOCKETS_IO_NAMES     1
#define LWIP_TCP_KEEPALIVE              1
#define LWIP_SO_RCVTIMEO                1
#define LWIP_SO_RCVBUF                  1
#define LWIP_SO_SNDBUF                  1
#define RECV_BUFSIZE_DEFAULT            (1024 * 1024)
#define SO_REUSE                        1
#define SO_REUSE_RXTOALL                1
#define DFLT_IP_MULTICAST_LOOP          1

#define LWIP_INLINE_IP_CHKSUM           0

#define LWIP_CHECKSUM_ON_COPY           1
// Disable checksum-on-copy for TCP because of
// lwIP defect #36153
#define TCP_CHECKSUM_ON_COPY            0

#define LWIP_TIMEVAL_PRIVATE            0

#define LWIP_STATS                      1

#if LWIP_STATS
#define LWIP_STATS_DISPLAY              0
#define LINK_STATS                      0
#define ETHARP_STATS                    0
#define IP_STATS                        0
#define IPFRAG_STATS                    0
#define ICMP_STATS                      0
#define IGMP_STATS                      0
#define UDP_STATS                       0
#define TCP_STATS                       0
#define MEMP_STATS                      1
#define SYS_STATS                       0
#endif

/*
These are errnos are not provided by our toolchain,
so we define them here. They're taken from

${LWIP_SOURCE}/include/lwip/arch.h
*/

#define ENSRNOTFOUND            163     /* Domain name not found */

#ifdef LWIP_DEBUG
#undef LWIP_DEBUG
#endif


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
