#ifndef _RTEMS_SCORE_TLSF_H
#define _RTEMS_SCORE_TLSF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <string.h>                 /* memcpy / memset */

/*************************************************************************/
/* Definition of the structures used by TLSF */

/* Some IMPORTANT TLSF parameters */
/* Unlike the preview TLSF versions, now they are statics */
#define BLOCK_ALIGN                 (sizeof(void *) * 2)

#define MAX_FLI                     (30)
#define MAX_LOG2_SLI                (5)
#define MAX_SLI                     (1 << MAX_LOG2_SLI)     /* MAX_SLI = 2^MAX_LOG2_SLI */

#define FLI_OFFSET                  (6)     /* tlsf structure just will manage blocks bigger */
                                            /* than 128 bytes */
#define SMALL_BLOCK                 (128)
#define REAL_FLI                    (MAX_FLI - FLI_OFFSET)
#define MIN_BLOCK_SIZE              (sizeof (free_ptr_t))
#define BHDR_OVERHEAD               (sizeof (bhdr_t) - MIN_BLOCK_SIZE)
#define TLSF_SIGNATURE              (0x2A59FA59)

#define PTR_MASK                    (sizeof(void *) - 1)
#define BLOCK_SIZE                  (0xFFFFFFFF - PTR_MASK)

#define GET_NEXT_BLOCK(_addr, _r)   ((bhdr_t *) ((char *) (_addr) + (_r)))
#define MEM_ALIGN                   ((BLOCK_ALIGN) - 1)
#define ROUNDUP_SIZE(_r)            (((_r) + MEM_ALIGN) & ~MEM_ALIGN)
#define ROUNDDOWN_SIZE(_r)          ((_r) & ~MEM_ALIGN)

#define BLOCK_STATE                 (0x1)
#define PREV_STATE                  (0x2)

/* bit 0 of the block size */
#define FREE_BLOCK                  (0x1)
#define USED_BLOCK                  (0x0)

/* bit 1 of the block size */
#define PREV_FREE                   (0x2)
#define PREV_USED                   (0x0)

#define TLSF_ADD_SIZE(tlsf, b)       do{}while(0)
#define TLSF_REMOVE_SIZE(tlsf, b)    do{}while(0)


typedef uint32_t u32_t;     /* NOTE: Make sure that this type is 4 bytes long on your computer */
typedef uint8_t u8_t;      /* NOTE: Make sure that this type is 1 byte on your computer */

typedef struct free_ptr_struct {
    struct bhdr_struct *prev;
    struct bhdr_struct *next;
} free_ptr_t;

typedef struct bhdr_struct {
    /* This pointer is just valid if the first bit of size is set */
    struct bhdr_struct *prev_hdr;
    /* The size is stored in bytes */
    size_t size;                /* bit 0 indicates whether the block is used and */
    /* bit 1 allows to know whether the previous block is free */
    union {
        struct free_ptr_struct free_ptr;
        u8_t buffer[1];         /*sizeof(struct free_ptr_struct)]; */
    } ptr;
} bhdr_t;

/* This structure is embedded at the beginning of each area, giving us
 * enough information to cope with a set of areas */

typedef struct area_info_struct {
    bhdr_t *end;
    struct area_info_struct *next;
} area_info_t;

typedef struct TLSF_struct {
    /* the TLSF's structure signature */
    u32_t tlsf_signature;

    /* A linked list holding all the existing areas */
    area_info_t *area_head;

    /* the first-level bitmap */
    /* This array should have a size of REAL_FLI bits */
    u32_t fl_bitmap;

    /* the second-level bitmap */
    u32_t sl_bitmap[REAL_FLI];

    bhdr_t *matrix[REAL_FLI][MAX_SLI];
} tlsf_t;

void *malloc_ex(size_t size, tlsf_t *);
void *malloc_aligned_ex(size_t size, tlsf_t *tlsf, size_t page_size);
void free_ex(void *ptr, tlsf_t *);
void *realloc_ex(void *ptr, size_t new_size, tlsf_t *);
void *calloc_ex(size_t nelem, size_t elem_size, tlsf_t *);

#include <rtems/score/tlsf.inl>

#ifdef __cplusplus
}
#endif

#endif
