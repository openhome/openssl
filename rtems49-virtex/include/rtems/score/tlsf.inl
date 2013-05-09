/** 
 *  @file  rtems/score/heap.inl
 *
 *  This file contains the static inline implementation of the inlined
 *  routines from the heap handler.
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: heap.inl,v 1.23 2008/09/04 17:38:26 ralf Exp $
 */

#ifndef _RTEMS_SCORE_TLSF_H
# error "Never use <rtems/score/tlsf.inl> directly; include <rtems/score/tlsf.h> instead."
#endif

#ifndef _RTEMS_SCORE_TLSF_INL
#define _RTEMS_SCORE_TLSF_INL

#include <rtems/system.h>

RTEMS_INLINE_ROUTINE int ls_bit(int i);
RTEMS_INLINE_ROUTINE int ms_bit(int i);
RTEMS_INLINE_ROUTINE void set_bit(int nr, u32_t * addr);
RTEMS_INLINE_ROUTINE void clear_bit(int nr, u32_t * addr);
RTEMS_INLINE_ROUTINE void MAPPING_SEARCH(size_t * _r, int *_fl, int *_sl);
RTEMS_INLINE_ROUTINE void MAPPING_INSERT(size_t _r, int *_fl, int *_sl);
RTEMS_INLINE_ROUTINE bhdr_t *FIND_SUITABLE_BLOCK(tlsf_t * _tlsf, int *_fl, int *_sl);
RTEMS_INLINE_ROUTINE bhdr_t *FIND_SUITABLE_BLOCK(tlsf_t * _tlsf, int *_fl, int *_sl);
/*
#define EXTRACT_BLOCK_HDR(_b, _tlsf, _fl, _sl)
#define EXTRACT_BLOCK(_b, _tlsf, _fl, _sl)
#define INSERT_BLOCK(_b, _tlsf, _fl, _sl)
*/
RTEMS_INLINE_ROUTINE bhdr_t *process_area(void *area, size_t size);

static const int table[] = {
    -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4,
    4, 4,
    4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5,
    5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6,
    6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6,
    6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7,
    7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7,
    7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7,
    7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7,
    7, 7, 7, 7, 7, 7, 7
};

RTEMS_INLINE_ROUTINE int ls_bit(int i)
{
    unsigned int a;
    unsigned int x = i & -i;

    a = x <= 0xffff ? (x <= 0xff ? 0 : 8) : (x <= 0xffffff ? 16 : 24);
    return table[x >> a] + a;
}

RTEMS_INLINE_ROUTINE int ms_bit(int i)
{
    unsigned int a;
    unsigned int x = (unsigned int) i;

    a = x <= 0xffff ? (x <= 0xff ? 0 : 8) : (x <= 0xffffff ? 16 : 24);
    return table[x >> a] + a;
}

RTEMS_INLINE_ROUTINE void set_bit(int nr, u32_t * addr)
{
    addr[nr >> 5] |= 1 << (nr & 0x1f);
}

RTEMS_INLINE_ROUTINE void clear_bit(int nr, u32_t * addr)
{
    addr[nr >> 5] &= ~(1 << (nr & 0x1f));
}

RTEMS_INLINE_ROUTINE void MAPPING_SEARCH(size_t * _r, int *_fl, int *_sl)
{
    int _t;

    if (*_r < SMALL_BLOCK) {
        *_fl = 0;
        *_sl = *_r / (SMALL_BLOCK / MAX_SLI);
    } else {
        _t = (1 << (ms_bit(*_r) - MAX_LOG2_SLI)) - 1;
        *_r = *_r + _t;
        *_fl = ms_bit(*_r);
        *_sl = (*_r >> (*_fl - MAX_LOG2_SLI)) - MAX_SLI;
        *_fl -= FLI_OFFSET;
        *_r &= ~_t;
    }
}

RTEMS_INLINE_ROUTINE void MAPPING_INSERT(size_t _r, int *_fl, int *_sl)
{
    if (_r < SMALL_BLOCK) {
        *_fl = 0;
        *_sl = _r / (SMALL_BLOCK / MAX_SLI);
    } else {
        *_fl = ms_bit(_r);
        *_sl = (_r >> (*_fl - MAX_LOG2_SLI)) - MAX_SLI;
        *_fl -= FLI_OFFSET;
    }
}


RTEMS_INLINE_ROUTINE bhdr_t *FIND_SUITABLE_BLOCK(tlsf_t * _tlsf, int *_fl, int *_sl)
{
    u32_t _tmp = _tlsf->sl_bitmap[*_fl] & (~0 << *_sl);
    bhdr_t *_b = NULL;

    if (_tmp) {
        *_sl = ls_bit(_tmp);
        _b = _tlsf->matrix[*_fl][*_sl];
    } else {
        *_fl = ls_bit(_tlsf->fl_bitmap & (~0 << (*_fl + 1)));
        if (*_fl > 0) {         /* likely */
            *_sl = ls_bit(_tlsf->sl_bitmap[*_fl]);
            _b = _tlsf->matrix[*_fl][*_sl];
        }
    }
    return _b;
}


#define EXTRACT_BLOCK_HDR(_b, _tlsf, _fl, _sl) do {                 \
        _tlsf -> matrix [_fl] [_sl] = _b -> ptr.free_ptr.next;      \
        if (_tlsf -> matrix[_fl][_sl])                              \
            _tlsf -> matrix[_fl][_sl] -> ptr.free_ptr.prev = NULL;  \
        else {                                                      \
            clear_bit (_sl, &_tlsf -> sl_bitmap [_fl]);             \
            if (!_tlsf -> sl_bitmap [_fl])                          \
                clear_bit (_fl, &_tlsf -> fl_bitmap);               \
        }                                                           \
        _b -> ptr.free_ptr.prev =  NULL;                            \
        _b -> ptr.free_ptr.next =  NULL;                            \
    }while(0)


#define EXTRACT_BLOCK(_b, _tlsf, _fl, _sl) do {                         \
        if (_b -> ptr.free_ptr.next)                                    \
            _b -> ptr.free_ptr.next -> ptr.free_ptr.prev = _b -> ptr.free_ptr.prev; \
        if (_b -> ptr.free_ptr.prev)                                    \
            _b -> ptr.free_ptr.prev -> ptr.free_ptr.next = _b -> ptr.free_ptr.next; \
        if (_tlsf -> matrix [_fl][_sl] == _b) {                         \
            _tlsf -> matrix [_fl][_sl] = _b -> ptr.free_ptr.next;       \
            if (!_tlsf -> matrix [_fl][_sl]) {                          \
                clear_bit (_sl, &_tlsf -> sl_bitmap[_fl]);              \
                if (!_tlsf -> sl_bitmap [_fl])                          \
                    clear_bit (_fl, &_tlsf -> fl_bitmap);               \
            }                                                           \
        }                                                               \
        _b -> ptr.free_ptr.prev = NULL;                                 \
        _b -> ptr.free_ptr.next = NULL;                                 \
    } while(0)

#define INSERT_BLOCK(_b, _tlsf, _fl, _sl) do {                          \
        _b -> ptr.free_ptr.prev = NULL;                                 \
        _b -> ptr.free_ptr.next = _tlsf -> matrix [_fl][_sl];           \
        if (_tlsf -> matrix [_fl][_sl])                                 \
            _tlsf -> matrix [_fl][_sl] -> ptr.free_ptr.prev = _b;       \
        _tlsf -> matrix [_fl][_sl] = _b;                                \
        set_bit (_sl, &_tlsf -> sl_bitmap [_fl]);                       \
        set_bit (_fl, &_tlsf -> fl_bitmap);                             \
    } while(0)

RTEMS_INLINE_ROUTINE bhdr_t *process_area(void *area, size_t size)
{
    bhdr_t *b, *lb, *ib;
    area_info_t *ai;

    ib = (bhdr_t *) area;
    ib->size =
        (sizeof(area_info_t) <
         MIN_BLOCK_SIZE) ? MIN_BLOCK_SIZE : ROUNDUP_SIZE(sizeof(area_info_t)) | USED_BLOCK | PREV_USED;
    b = (bhdr_t *) GET_NEXT_BLOCK(ib->ptr.buffer, ib->size & BLOCK_SIZE);
    b->size = ROUNDDOWN_SIZE(size - 3 * BHDR_OVERHEAD - (ib->size & BLOCK_SIZE)) | USED_BLOCK | PREV_USED;
    b->ptr.free_ptr.prev = b->ptr.free_ptr.next = 0;
    lb = GET_NEXT_BLOCK(b->ptr.buffer, b->size & BLOCK_SIZE);
    lb->prev_hdr = b;
    lb->size = 0 | USED_BLOCK | PREV_FREE;
    ai = (area_info_t *) ib->ptr.buffer;
    ai->next = 0;
    ai->end = lb;
    return ib;
}

#endif
