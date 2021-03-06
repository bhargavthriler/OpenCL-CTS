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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#if !defined(_WIN32)
#include <stdbool.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include "procs.h"

static int min_verify_float( float *x, float *y, float *out, int numElements, int vecSize )
{
    for( int i = 0; i < numElements * vecSize; i++ )
    {
        float v = ( y[ i ] < x[ i ] ) ? y[ i ] : x[ i ];
        if( v != out[ i ] ) {
      log_error("x[%d]=%g y[%d]=%g out[%d]=%g, expected %g. (index %d is vector %d, element %d, for vector size %d)\n", i, x[i], i, y[i], i, out[i], v, i, i/vecSize, i%vecSize, vecSize);
            return -1;
    }
    }
    return 0;
}

static int min_verify_double( double *x, double *y, double *out, int numElements, int vecSize )
{
    for( int i = 0; i < numElements * vecSize; i++ )
    {
        double v = ( y[ i ] < x[ i ] ) ? y[ i ] : x[ i ];
        if( v != out[ i ] ) {
      log_error("x[%d]=%g y[%d]=%g out[%d]=%g, expected %g. (index %d is vector %d, element %d, for vector size %d)\n", i, x[i], i, y[i], i, out[i], v, i, i/vecSize, i%vecSize, vecSize);
            return -1;
    }
    }
    return 0;
}

int test_min(cl_device_id device, cl_context context, cl_command_queue queue, int n_elems)
{
    return test_binary_fn( device, context, queue, n_elems, "min", true, min_verify_float, min_verify_double );
}


