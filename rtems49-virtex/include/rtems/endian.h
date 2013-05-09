/*
 *  $Id: endian.h,v 1.1 2007/05/09 16:27:09 ralf Exp $
 */

#ifndef _RTEMS_ENDIAN_H
#define _RTEMS_ENDIAN_H

#include <rtems.h>

/*
 * BSD-style endian declaration
 */
#define BIG_ENDIAN	4321
#define LITTLE_ENDIAN	1234

#ifndef BYTE_ORDER
#if CPU_BIG_ENDIAN
# define BYTE_ORDER BIG_ENDIAN
#elif CPU_LITTLE_ENDIAN
# define BYTE_ORDER LITTLE_ENDIAN
#else
# error "Can't decide which end is which!"
#endif
#endif

#if ( CPU_BIG_ENDIAN == TRUE )

/*
 *  Very simply on big endian CPUs
 */

static inline uint32_t ntohl( uint32_t _x )
{
  return _x;
}

static inline uint16_t ntohs( uint16_t _x )
{
  return _x;
}

static inline uint32_t htonl( uint32_t _x )
{
  return _x;
}

static inline uint16_t htons( uint16_t _x )
{
  return _x;
}

#define NTOHS(x)
#define HTONS(x)
#define NTOHL(x)
#define HTONL(x)

#elif ( CPU_LITTLE_ENDIAN == TRUE )

/*
 *  A little more complicated on little endian CPUs
 */

static inline uint32_t ntohl( uint32_t _x )
{
  return CPU_swap_u32(_x);
}

static inline uint16_t ntohs( uint16_t _x )
{
  return CPU_swap_u16(_x);
}

static inline uint32_t htonl( uint32_t _x )
{
  return CPU_swap_u32(_x);
}

static inline uint16_t htons( uint16_t _x )
{
  return CPU_swap_u16(_x);
}

#define NTOHS(x) (x) = ntohs(x)
#define HTONS(x) (x) = htons(x)
#define NTOHL(x) (x) = ntohl(x)
#define HTONL(x) (x) = htonl(x)

#else
#error "Unknown endian-ness for this cpu"
#endif

#endif /* _RTEMS_ENDIAN_H */
