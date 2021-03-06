//
// Copyright (c) 2017 The Khronos Group Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef UTILITY_H
#define UTILITY_H

#include "harness/compat.h"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#include <stdio.h>
#if !defined(_MSC_VER)
#include <stdint.h>
#endif

#include "harness/rounding_mode.h"
#include "harness/fpcontrol.h"

#if defined( _WIN32) && defined (_MSC_VER)
#include "harness/testHarness.h"
#endif


#include "harness/ThreadPool.h"
#define BUFFER_SIZE         (1024*1024*2)

#if defined( __GNUC__ )
    #define UNUSED  __attribute__ ((unused))
#else
    #define UNUSED
#endif

#define VECTOR_SIZE_COUNT   6
extern const char *sizeNames[VECTOR_SIZE_COUNT];
extern const int   sizeValues[VECTOR_SIZE_COUNT];

extern cl_device_type   gDeviceType;
extern cl_device_id     gDevice;
extern cl_context       gContext;
extern cl_command_queue gQueue;
extern void             *gIn;
extern void             *gIn2;
extern void             *gIn3;
extern void             *gOut_Ref;
extern void             *gOut_Ref2;
extern void             *gOut[VECTOR_SIZE_COUNT];
extern void             *gOut2[VECTOR_SIZE_COUNT];
extern cl_mem           gInBuffer;
extern cl_mem           gInBuffer2;
extern cl_mem           gInBuffer3;
extern cl_mem           gOutBuffer[VECTOR_SIZE_COUNT];
extern cl_mem           gOutBuffer2[VECTOR_SIZE_COUNT];
extern uint32_t         gComputeDevices;
extern uint32_t         gSimdSize;
extern int              gSkipCorrectnessTesting;
extern int              gMeasureTimes;
extern int              gReportAverageTimes;
extern int              gForceFTZ;
extern int              gWimpyMode;
extern int              gHasDouble;
extern int              gIsInRTZMode;
extern int                gInfNanSupport;
extern int                gIsEmbedded;
extern uint32_t         gMaxVectorSizeIndex;
extern uint32_t         gMinVectorSizeIndex;
extern uint32_t         gDeviceFrequency;
extern cl_device_fp_config gFloatCapabilities;
extern cl_device_fp_config gDoubleCapabilities;

#if !defined( _MSC_VER)
    #include <fenv.h>
#endif

#define LOWER_IS_BETTER     0
#define HIGHER_IS_BETTER    1

#if USE_ATF

    #include <ATF/ATF.h>
    #define test_start()        ATFTestStart()
    #define test_finish()       ATFTestFinish()
    #define vlog( ... )         ATFLogInfo(__VA_ARGS__)
    #define vlog_error( ... )   ATFLogError(__VA_ARGS__)
    #define vlog_perf( _number, _higherIsBetter, _units, _nameFmt, ... )    ATFLogPerformanceNumber(_number, _higherIsBetter, _units, _nameFmt, __VA_ARGS__ )

#else

    #define test_start()
    #define test_finish()
    #define vlog( ... )         printf( __VA_ARGS__ )
    #define vlog_error( ... )    printf( __VA_ARGS__ )
    #define vlog_perf( _number, _higherIsBetter, _units, _nameFmt, ... )    printf( "\t%8.2f", _number )

    void _logPerf(double number, int higherIsBetter, const char *units, const char *nameFormat, ...);
#endif

#if defined (_MSC_VER )
    //Deal with missing scalbn on windows
    #define scalbnf( _a, _i )       ldexpf( _a, _i )
    #define scalbn( _a, _i )        ldexp( _a, _i )
    #define scalbnl( _a, _i )       ldexpl( _a, _i )
#endif

#ifdef __cplusplus
extern "C" {
#endif
float Ulp_Error( float test, double reference );
//float Ulp_Error_Half( float test, double reference );
float Ulp_Error_Double( double test, long double reference );
#ifdef __cplusplus
} //extern "C"
#endif

uint64_t GetTime( void );
double SubtractTime( uint64_t endTime, uint64_t startTime );
int MakeKernel( const char **c, cl_uint count, const char *name, cl_kernel *k, cl_program *p );
int MakeKernels( const char **c, cl_uint count, const char *name, cl_uint kernel_count, cl_kernel *k, cl_program *p );

// used to convert a bucket of bits into a search pattern through double
static inline double DoubleFromUInt32( uint32_t bits );
static inline double DoubleFromUInt32( uint32_t bits )
{
    union{ uint64_t u; double d;} u;

    // split 0x89abcdef to 0x89abc00000000def
    u.u = bits & 0xfffU;
    u.u |= (uint64_t) (bits & ~0xfffU) << 32;

    // sign extend the leading bit of def segment as sign bit so that the middle region consists of either all 1s or 0s
    u.u -= (bits & 0x800U) << 1;

    // return result
    return u.d;
}

void _LogBuildError( cl_program p, int line, const char *file );
#define LogBuildError( program )        _LogBuildError( program, __LINE__, __FILE__ )

#ifndef MAX
    #define MAX(_a, _b)     ((_a) > (_b) ? (_a) : (_b))
#endif
#ifndef MIN
    #define MIN(_a, _b)     ((_a) < (_b) ? (_a) : (_b))
#endif

#define PERF_LOOP_COUNT 100

// Note: though this takes a double, this is for use with single precision tests
static inline int IsFloatSubnormal( double x )
{
#if 2 == FLT_RADIX
    // Do this in integer to avoid problems with FTZ behavior
    union{ float d; uint32_t u;}u;
    u.d = fabsf((float)x);
    return (u.u-1) < 0x007fffffU;
#else
    // rely on floating point hardware for non-radix2 non-IEEE-754 hardware -- will fail if you flush subnormals to zero
    return fabs(x) < (double) FLT_MIN && x != 0.0;
#endif
}


static inline int IsDoubleSubnormal( long double x )
{
#if 2 == FLT_RADIX
    // Do this in integer to avoid problems with FTZ behavior
    union{ double d; uint64_t u;}u;
    u.d = fabs((double) x);
    return (u.u-1) < 0x000fffffffffffffULL;
#else
    // rely on floating point hardware for non-radix2 non-IEEE-754 hardware -- will fail if you flush subnormals to zero
    return fabs(x) < (double) DBL_MIN && x != 0.0;
#endif
}

//The spec is fairly clear that we may enforce a hard cutoff to prevent premature flushing to zero.
// However, to avoid conflict for 1.0, we are letting results at TYPE_MIN + ulp_limit to be flushed to zero.
static inline int IsFloatResultSubnormal( double x, float ulps )
{
    x = fabs(x) - MAKE_HEX_DOUBLE( 0x1.0p-149, 0x1, -149) * (double) ulps;
    return x < MAKE_HEX_DOUBLE( 0x1.0p-126, 0x1, -126 );
}

static inline int IsDoubleResultSubnormal( long double x, float ulps )
{
    x = fabsl(x) - MAKE_HEX_LONG( 0x1.0p-1074, 0x1, -1074) * (long double) ulps;
    return x < MAKE_HEX_LONG( 0x1.0p-1022, 0x1, -1022 );
}

static inline int IsFloatInfinity(double x)
{
  union { cl_float d; cl_uint u; } u;
  u.d = (cl_float) x;
  return ((u.u & 0x7fffffffU) == 0x7F800000U);
}

static inline int IsFloatMaxFloat(double x)
{
  union { cl_float d; cl_uint u; } u;
  u.d = (cl_float) x;
  return ((u.u & 0x7fffffffU) == 0x7F7FFFFFU);
}

static inline int IsFloatNaN(double x)
{
  union { cl_float d; cl_uint u; } u;
  u.d = (cl_float) x;
  return ((u.u & 0x7fffffffU) > 0x7F800000U);
}

extern cl_uint RoundUpToNextPowerOfTwo( cl_uint x );

// Windows (since long double got deprecated) sets the x87 to 53-bit precision
// (that's x87 default state).  This causes problems with the tests that
// convert long and ulong to float and double or otherwise deal with values
// that need more precision than 53-bit. So, set the x87 to 64-bit precision.
static inline void Force64BitFPUPrecision(void)
{
#if __MINGW32__
    // The usual method is to use _controlfp as follows:
    //     #include <float.h>
    //     _controlfp(_PC_64, _MCW_PC);
    //
    // _controlfp is available on MinGW32 but not on MinGW64. Instead of having
    // divergent code just use inline assembly which works for both.
    unsigned short int orig_cw = 0;
    unsigned short int new_cw = 0;
    __asm__ __volatile__ ("fstcw %0":"=m" (orig_cw));
    new_cw = orig_cw | 0x0300;   // set precision to 64-bit
    __asm__ __volatile__ ("fldcw  %0"::"m" (new_cw));
#elif defined( _WIN32 ) && defined( __INTEL_COMPILER )
    int cw;
    __asm { fnstcw cw };    // Get current value of FPU control word.
    cw = cw & 0xfffffcff | ( 3 << 8 ); // Set Precision Control to Double Extended Precision.
    __asm { fldcw cw };     // Set new value of FPU control word.
#else
    /* Implement for other platforms if needed */
#endif
}

#ifdef __cplusplus
extern "C"
#else
extern
#endif
void memset_pattern4(void *dest, const void *src_pattern, size_t bytes );

typedef union
{
    int32_t i;
    float   f;
}int32f_t;

typedef union
{
    int64_t l;
    double  d;
}int64d_t;

void MulD(double *rhi, double *rlo, double u, double v);
void AddD(double *rhi, double *rlo, double a, double b);
void MulDD(double *rhi, double *rlo, double xh, double xl, double yh, double yl);
void AddDD(double *rhi, double *rlo, double xh, double xl, double yh, double yl);
void DivideDD(double *chi, double *clo, double a, double b);
int compareFloats(float x, float y);
int compareDoubles(double x, double y);

#endif /* UTILITY_H */


