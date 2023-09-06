#ifndef TYPES_H
#define TYPES_H

typedef   int8_t       i8;
typedef  int16_t      i16;
typedef  int32_t      i32;
typedef  int64_t      i64;
typedef  uint8_t       u8;
typedef uint16_t      u16;
typedef uint32_t      u32;
typedef uint64_t      u64;
typedef float_t       f32;
typedef double_t      f64;

#if __x86_64__ || __ppc64__ || _WIN64
typedef  int64_t      isize;
typedef uint64_t      usize;
#else
typedef  int32_t      isize;
typedef uint32_t      usize;
#endif

typedef i8            c8;
typedef const char*   string;

typedef usize         alias_t;

#endif
