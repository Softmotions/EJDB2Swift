#pragma once
#ifndef IWUTILS_H
#define IWUTILS_H

/**************************************************************************************************
 * IOWOW library
 *
 * MIT License
 *
 * Copyright (c) 2012-2019 Softmotions Ltd <info@softmotions.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *************************************************************************************************/
/**
 * @file
 * @author Anton Adamansky (adamansky@softmotions.com)
 */

#include "basedefs.h"
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

IW_EXTERN_C_START

/* Align IW_x_ with IW_v_. IW_v_ must be simple power for 2 value. */
#define IW_ROUNDUP(IW_x_, IW_v_) (((IW_x_) + (IW_v_)-1) & ~((IW_v_)-1))

/* Round down align IW_x_ with IW_v_. IW_v_ must be simple power for 2 value. */
#define IW_ROUNDOWN(IW_x_, IW_v_) ((IW_x_) - ((IW_x_) & ((IW_v_)-1)))

#if defined(NDEBUG)
#define IW_DODEBUG(IW_expr_) \
  do {                       \
  } while (0)
#else
#define IW_DODEBUG(IW_expr_) \
  { IW_expr_; }
#endif

#if __GNUC__ >= 5
#define IW_SWAB16(IW_num_) __builtin_bswap16(IW_num_)
#else
#define IW_SWAB16(IW_num_) \
  ((((IW_num_) & 0x00ffU) << 8) | (((IW_num_) & 0xff00U) >> 8))
#endif

#if __GNUC__ >= 4
#define IW_SWAB32(IW_num_) __builtin_bswap32(IW_num_)
#else
#define IW_SWAB32(IW_num_)                                              \
  ((((IW_num_) & 0x000000ffUL) << 24) | (((IW_num_) & 0x0000ff00UL) << 8) | \
   (((IW_num_) & 0x00ff0000UL) >> 8) | (((IW_num_) & 0xff000000UL) >> 24))
#endif

#if __GNUC__ >= 4
#define IW_SWAB64(IW_num_) __builtin_bswap64(IW_num_)
#else
#define IW_SWAB64(IW_num_)                     \
  ((((IW_num_) & 0x00000000000000ffULL) << 56) | \
   (((IW_num_) & 0x000000000000ff00ULL) << 40) | \
   (((IW_num_) & 0x0000000000ff0000ULL) << 24) | \
   (((IW_num_) & 0x00000000ff000000ULL) << 8) |  \
   (((IW_num_) & 0x000000ff00000000ULL) >> 8) |  \
   (((IW_num_) & 0x0000ff0000000000ULL) >> 24) | \
   (((IW_num_) & 0x00ff000000000000ULL) >> 40) | \
   (((IW_num_) & 0xff00000000000000ULL) >> 56))
#endif

#ifdef IW_BIGENDIAN
#define IW_HTOIS(IW_num_) IW_SWAB16(IW_num_)
#define IW_HTOIL(IW_num_) IW_SWAB32(IW_num_)
#define IW_HTOILL(IW_num_) IW_SWAB64(IW_num_)
#define IW_ITOHS(IW_num_) IW_SWAB16(IW_num_)
#define IW_ITOHL(IW_num_) IW_SWAB32(IW_num_)
#define IW_ITOHLL(IW_num_) IW_SWAB64(IW_num_)
#else
#define IW_HTOIS(IW_num_) (IW_num_)
#define IW_HTOIL(IW_num_) (IW_num_)
#define IW_HTOILL(IW_num_) (IW_num_)
#define IW_ITOHS(IW_num_) (IW_num_)
#define IW_ITOHL(IW_num_) (IW_num_)
#define IW_ITOHLL(IW_num_) (IW_num_)
#endif

#define IW_WRITEBV(IW_ptr_, IW_v_, IW_m_)  \
  static_assert(sizeof(IW_v_) == 1, "Mismatch IW_v_ size"); \
  (IW_v_) = (IW_m_);                          \
  memcpy(IW_ptr_, &(IW_v_), 1); \
  (IW_ptr_) += 1

#define IW_WRITESV(IW_ptr_, IW_v_, IW_m_)  \
  static_assert(sizeof(IW_v_) == 2, "Mismatch IW_v_ size"); \
  (IW_v_) = (IW_m_);                          \
  (IW_v_) = IW_HTOIS(IW_v_);                \
  memcpy(IW_ptr_, &(IW_v_), 2); \
  (IW_ptr_) += 2

#define IW_WRITELV(IW_ptr_, IW_v_, IW_m_)  \
  static_assert(sizeof(IW_v_) == 4, "Mismatch IW_v_ size"); \
  (IW_v_) = (IW_m_);                          \
  (IW_v_) = IW_HTOIL(IW_v_);                \
  memcpy(IW_ptr_, &(IW_v_), 4); \
  (IW_ptr_) += 4

#define IW_WRITELLV(IW_ptr_, IW_v_, IW_m_) \
  static_assert(sizeof(IW_v_) == 8, "Mismatch IW_v_ size"); \
  (IW_v_) = (IW_m_);                          \
  (IW_v_) = IW_HTOILL(IW_v_);               \
  memcpy((IW_ptr_), &(IW_v_), 8); \
  (IW_ptr_) += 8

#define IW_READBV(IW_ptr_, IW_t_, IW_m_)   \
  static_assert(sizeof(IW_t_) == 1, "Mismatch IW_t_ size"); \
  (IW_t_) = 0; \
  memcpy(&(IW_t_), IW_ptr_, 1);  \
  (IW_m_) = (IW_t_);   \
  (IW_ptr_) += 1

#define IW_READSV(IW_ptr_, IW_t_, IW_m_)   \
  static_assert(sizeof(IW_t_) == 2, "Mismatch IW_t_ size"); \
  (IW_t_) = 0; \
  memcpy(&(IW_t_), IW_ptr_, 2);  \
  (IW_m_) = IW_ITOHS(IW_t_);  \
  (IW_ptr_) += 2

#define IW_READLV(IW_ptr_, IW_t_, IW_m_)   \
  static_assert(sizeof(IW_t_) == 4, "Mismatch IW_t_ size"); \
  (IW_t_) = 0; \
  memcpy(&(IW_t_), IW_ptr_, 4);  \
  (IW_m_) = IW_ITOHL(IW_t_);  \
  (IW_ptr_) += 4

#define IW_READLLV(IW_ptr_, IW_t_, IW_m_)  \
  static_assert(sizeof(IW_t_) == 8, "Mismatch IW_t_ size"); \
  (IW_t_) = 0; \
  memcpy(&(IW_t_), IW_ptr_, 8);  \
  (IW_m_) = IW_ITOHLL(IW_t_); \
  (IW_ptr_) += 8

#ifndef SIZE_T_MAX
#define SIZE_T_MAX ((size_t)-1)
#endif

#ifndef OFF_T_MIN
#define OFF_T_MIN ((off_t)(((uint64_t)1) << (8 * sizeof(off_t) - 1)))
#endif
#ifndef OFF_T_MAX
#define OFF_T_MAX ((off_t) ~(((uint64_t)1) << (8 * sizeof(off_t) - 1)))
#endif

#ifdef __GNUC__
#define IW_LIKELY(x_) __builtin_expect(!!(x_), 1)
#define IW_UNLIKELY(x_) __builtin_expect(!!(x_), 0)
#else
#define IW_LIKELY(x_)
#define IW_UNLIKELY(x_)
#endif

#define IW_RANGES_OVERLAP(IW_s1_, IW_e1_, IW_s2_, IW_e2_) \
  (((IW_e1_) > (IW_s2_) && (IW_e1_) <= (IW_e2_)) ||       \
   ((IW_s1_) >= (IW_s2_) && (IW_s1_) < (IW_e2_)) ||       \
   ((IW_s1_) <= (IW_s2_) && (IW_e1_) >= (IW_e2_)))


///////////////////////////////////////////////////////////////////////////
//                    Variable length number encoding                    //
///////////////////////////////////////////////////////////////////////////

/* set a buffer for a variable length 32 bit number */
#define IW_SETVNUMBUF(len_, buf_, num_) \
  do { \
    int32_t _num_ = (num_); \
    if (_num_ == 0){ \
      ((signed char *)(buf_))[0] = 0; \
      (len_) = 1; \
    } else { \
      (len_) = 0; \
      while(_num_ > 0) { \
        int _rem_ = _num_ & 0x7f; \
        _num_ >>= 7; \
        if(_num_ > 0){ \
          ((signed char *)(buf_))[(len_)] = ~(_rem_); \
        } else { \
          ((signed char *)(buf_))[(len_)] = _rem_; \
        } \
        (len_)++; \
      } \
    } \
  } while(0)

/* set a buffer for a variable length 64 number */
#define IW_SETVNUMBUF64(len_, buf_, num_) \
  do { \
    int64_t _num_ = (num_); \
    if (_num_ == 0){ \
      ((signed char *)(buf_))[0] = 0; \
      (len_) = 1; \
    } else { \
      (len_) = 0; \
      while(_num_ > 0) { \
        int _rem_ = _num_ & 0x7f; \
        _num_ >>= 7; \
        if(_num_ > 0){ \
          ((signed char *)(buf_))[(len_)] = ~(_rem_); \
        } else { \
          ((signed char *)(buf_))[(len_)] = _rem_; \
        } \
        (len_)++; \
      } \
    } \
  } while(0)


/* read a 32 bit variable length buffer */
#define IW_READVNUMBUF(buf_, num_, step_) \
  do { \
    (num_) = 0; \
    int32_t _base_ = 1; \
    int _i_ = 0; \
    while(1){ \
      if (((const signed char *)(buf_))[_i_] >= 0){ \
        (num_) += _base_ * ((const signed char *)(buf_))[_i_]; \
        break; \
      } \
      (num_) += _base_ * ~(((const signed char *)(buf_))[_i_]); \
      _base_ <<= 7; \
      _i_++; \
    } \
    (step_) = _i_ + 1; \
  } while(0)

/* read a 64 bit variable length buffer */
#define IW_READVNUMBUF64(buf_, num_, step_) \
  do { \
    (num_) = 0; \
    int64_t _base_ = 1; \
    int _i_ = 0; \
    while(1){ \
      if (((const signed char *)(buf_))[_i_] >= 0){ \
        (num_) += _base_ * ((const signed char *)(buf_))[_i_]; \
        break; \
      } \
      (num_) += _base_ * ~(((const signed char *)(buf_))[_i_]); \
      _base_ <<= 7; \
      _i_++; \
    } \
    (step_) = _i_ + 1; \
  } while(0)


/* read a 64 bit variable length buffer */
#define IW_READVNUMBUF64_2(buf_, num_) \
  do { \
    (num_) = 0; \
    int64_t _base_ = 1; \
    int _i_ = 0; \
    while(1){ \
      if (((const signed char *)(buf_))[_i_] >= 0){ \
        (num_) += _base_ * ((const signed char *)(buf_))[_i_]; \
        break; \
      } \
      (num_) += _base_ * ~(((const signed char *)(buf_))[_i_]); \
      _base_ <<= 7; \
      _i_++; \
    } \
  } while(0)


#define IW_VNUMBUFSZ 10

#define IW_VNUMSIZE32(num_) \
  ((num_) < 0x80ULL ? 1 : \
   (num_) < 0x4000ULL ? 2 : \
   (num_) < 0x200000ULL ? 3 : \
   (num_) < 0x10000000ULL ? 4 : 5)

/* Size of variable number in bytes */
#ifdef IW_32
#define IW_VNUMSIZE IW_VNUMSIZE32
#else
#define IW_VNUMSIZE(num_) \
  ((num_) < 0x80ULL ? 1 : \
   (num_) < 0x4000ULL ? 2 : \
   (num_) < 0x200000ULL ? 3 : \
   (num_) < 0x10000000ULL ? 4 : \
   (num_) < 0x800000000ULL ? 5 : \
   (num_) < 0x40000000000ULL ? 6 : \
   (num_) < 0x2000000000000ULL ? 7 : \
   (num_) < 0x100000000000000ULL ? 8 : \
   (num_) < 0x8000000000000000ULL ? 9 : 10)
#endif

/* Lexicographic comparison of values */
#define IW_CMP(rv_, vp1_, vp1sz_, vp2_, vp2sz_) \
  do { \
    (rv_) = 0; \
    int min_ = (vp1sz_) < (vp2sz_) ? (vp1sz_) : (vp2sz_); \
    for (int i = 0; i < min_; i++) { \
      (rv_) = (int) (((const uint8_t *)(vp1_))[i] - ((const uint8_t *)(vp2_))[i]); \
      if (rv_) { \
        break; \
      } \
    } \
    if ((rv_) == 0) (rv_) = (vp1sz_) - (vp2sz_); \
  } while(0)


/* Lexicographic comparison common prefix of values */
#define IW_CMP2(rv_, vp1_, vp1sz_, vp2_, vp2sz_) \
  do { \
    (rv_) = 0; \
    int min_ = (vp1sz_) < (vp2sz_) ? (vp1sz_) : (vp2sz_); \
    for (int i = 0; i < min_; i++) { \
      (rv_) = (int) (((const uint8_t *)(vp1_))[i] - ((const uint8_t *)(vp2_))[i]); \
      if (rv_) { \
        break; \
      } \
    } \
  } while(0)

/**
 * @brief Set seed to random generator
 */
IW_EXPORT void iwu_rand_seed(uint32_t seed);

/**
 * @brief Generate random in [0, 0xffffffff]
 */
IW_EXPORT uint32_t iwu_rand_u32(void);

/**
 * @brief Create normal distributed random number.
 * @param avg Distribution pivot
 * @param sd Avg square deviation
 */
IW_EXPORT double_t iwu_rand_dnorm(double_t avg, double_t sd);

/**
 * @brief Create uniform distributed integer random number in: `[0, range)`
 */
IW_EXPORT uint32_t iwu_rand_range(uint32_t range);

/**
 * @brief Create normal distributed integer random number.
 */
IW_EXPORT uint32_t iwu_rand_inorm(int range);

IW_EXPORT int iwlog2_32(uint32_t val);

IW_EXPORT int iwlog2_64(uint64_t val);

IW_EXPORT uint32_t iwu_crc32(const uint8_t *buf, int len, uint32_t init);

/**
 * @brief Replaces a char @a sch with @a rch in a null terminated @a data char buffer.
 */
IW_EXPORT char *iwu_replace_char(char *data, char sch, char rch);

IW_EXPORT int iwu_cmp_files(FILE *f1, FILE *f2, bool verbose);

IW_EXPORT char *iwu_file_read_as_buf(const char *path);

IW_EXTERN_C_END

#endif
