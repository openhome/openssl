#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__

/* Define platform endianness */
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif /* BYTE_ORDER */

/* Define generic types used in lwIP */
typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   int     u32_t;
typedef signed     int     s32_t;

typedef u32_t              mem_ptr_t;

/* Define (sn)printf formatters for these lwIP types */
#define X8_F  "02x"
#define U16_F "hu"
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"

/* If only we could use C99 and get %zu */
#define SZT_F "u"

/* Compiler hints for packing structures */
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>

#ifndef ERRNO
#define ERRNO
#endif

#define LWIP_RAND() 0

#ifdef __cplusplus
extern "C" {
#endif

/* Plaform specific diagnostic output */
void lwipPlatformDiag(const char* aFormat, ...);

/* Platform specific assert handler */
void lwipAssertHandler(const char *aFile, int aLine, const char *aMessage);

/* ARM optimized checksum routine */
u16_t inet_cksum_arm(void *dataptr, u16_t len);

#ifdef __cplusplus
}
#endif

#define LWIP_PLATFORM_DIAG(x)           lwipPlatformDiag x

#define LWIP_PLATFORM_ASSERT(x)         do { lwipAssertHandler(__FILE__, __LINE__, x); } while (0)

#define LWIP_CHKSUM                     inet_cksum_arm


extern u8_t __attribute__ ((aligned (32))) memp_memory_PBUF_POOL_base[];

#endif

