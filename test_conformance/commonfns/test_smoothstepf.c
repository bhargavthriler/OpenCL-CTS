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

static const char *smoothstep_kernel_code =
"__kernel void test_smoothstep(__global float *edge0, __global float *edge1, __global float *x, __global float *dst)\n"
"{\n"
"    int  tid = get_global_id(0);\n"
"\n"
"    dst[tid] = smoothstep(edge0[tid], edge1[tid], x[tid]);\n"
"}\n";

static const char *smoothstep2_kernel_code =
"__kernel void test_smoothstep2f(__global float *edge0, __global float *edge1, __global float2 *x, __global float2 *dst)\n"
"{\n"
"    int  tid = get_global_id(0);\n"
"\n"
"    dst[tid] = smoothstep(edge0[tid], edge1[tid], x[tid]);\n"
"}\n";

static const char *smoothstep4_kernel_code =
"__kernel void test_smoothstep4f(__global float *edge0, __global float *edge1, __global float4 *x, __global float4 *dst)\n"
"{\n"
"    int  tid = get_global_id(0);\n"
"\n"
"    dst[tid] = smoothstep(edge0[tid], edge1[tid], x[tid]);\n"
"}\n";

#define MAX_ERR (1e-5f)

extern "C" float
verify_smoothstep(float *edge0, float *edge1, float *x, float *outptr, int n, int veclen)
{
  float       r, t, delta, max_err = 0.0f;
  int         i, j;

  for (i = 0; i < n; ++i) {
    int vi = i * veclen;
    for (j = 0; j < veclen; ++j, ++vi) {
      t = (x[vi] - edge0[i]) / (edge1[i] - edge0[i]);
      if (t < 0.0f)
        t = 0.0f;
      else if (t > 1.0f)
        t = 1.0f;
      r = t * t * (3.0f - 2.0f * t);
      delta = (float)fabs(r - outptr[vi]);
      if (delta > max_err)
        max_err = delta;
    }
  }
  return max_err;
}

const static char *fn_names[] = { "SMOOTHSTEP float", "SMOOTHSTEP float2", "SMOOTHSTEP float4"};

int
test_smoothstepf(cl_device_id device, cl_context context, cl_command_queue queue, int n_elems)
{
  cl_mem      streams[4];
  cl_float    *input_ptr[3], *output_ptr, *p, *p_edge0;
  cl_program  program[3];
  cl_kernel   kernel[3];
  size_t  threads[1];
  float max_err = 0.0f;
  int num_elements;
  int err;
  int i;
  MTdata d;

  num_elements = n_elems * 4;

  input_ptr[0] = (cl_float*)malloc(sizeof(cl_float) * num_elements);
  input_ptr[1] = (cl_float*)malloc(sizeof(cl_float) * num_elements);
  input_ptr[2] = (cl_float*)malloc(sizeof(cl_float) * num_elements);
  output_ptr = (cl_float*)malloc(sizeof(cl_float) * num_elements);
  streams[0] = clCreateBuffer( context, (cl_mem_flags)(CL_MEM_READ_WRITE),  sizeof(cl_float) * num_elements, NULL, NULL );
  if (!streams[0])
  {
    log_error("clCreateBuffer failed\n");
    return -1;
  }
  streams[1] = clCreateBuffer( context, (cl_mem_flags)(CL_MEM_READ_WRITE),  sizeof(cl_float) * num_elements, NULL, NULL );
  if (!streams[1])
  {
    log_error("clCreateBuffer failed\n");
    return -1;
  }
  streams[2] = clCreateBuffer( context, (cl_mem_flags)(CL_MEM_READ_WRITE),  sizeof(cl_float) * num_elements, NULL, NULL );
  if (!streams[2])
  {
    log_error("clCreateBuffer failed\n");
    return -1;
  }

  streams[3] = clCreateBuffer( context, (cl_mem_flags)(CL_MEM_READ_WRITE),  sizeof(cl_float) * num_elements, NULL, NULL );
  if (!streams[3])
  {
    log_error("clCreateBuffer failed\n");
    return -1;
  }

  d = init_genrand( gRandomSeed );
  p = input_ptr[0];
  for (i=0; i<num_elements; i++)
  {
    p[i] = get_random_float(-0x00200000, 0x00200000, d);
  }

  p = input_ptr[1];
  p_edge0 = input_ptr[0];
  for (i=0; i<num_elements; i++)
  {
    float edge0 = p_edge0[i];
    float edge1;
    do {
      edge1 = get_random_float( -0x00200000, 0x00200000, d);
      if (edge0 < edge1)
        break;
    } while (1);
    p[i] = edge1;
  }

  p = input_ptr[2];
  for (i=0; i<num_elements; i++)
  {
    p[i] = get_random_float(-0x00200000, 0x00200000, d);
  }
  free_mtdata(d);
  d = NULL;

  err = clEnqueueWriteBuffer( queue, streams[0], true, 0, sizeof(cl_float)*num_elements, (void *)input_ptr[0], 0, NULL, NULL );
  if (err != CL_SUCCESS)
  {
    log_error("clWriteArray failed\n");
    return -1;
  }
  err = clEnqueueWriteBuffer( queue, streams[1], true, 0, sizeof(cl_float)*num_elements, (void *)input_ptr[1], 0, NULL, NULL );
  if (err != CL_SUCCESS)
  {
    log_error("clWriteArray failed\n");
    return -1;
  }
  err = clEnqueueWriteBuffer( queue, streams[2], true, 0, sizeof(cl_float)*num_elements, (void *)input_ptr[2], 0, NULL, NULL );
  if (err != CL_SUCCESS)
  {
    log_error("clWriteArray failed\n");
    return -1;
  }

  err = create_single_kernel_helper( context, &program[0], &kernel[0], 1, &smoothstep_kernel_code, "test_smoothstep" );
  if (err)
    return -1;
  err = create_single_kernel_helper( context, &program[1], &kernel[1], 1, &smoothstep2_kernel_code, "test_smoothstep2f" );
  if (err)
    return -1;
  err = create_single_kernel_helper( context, &program[2], &kernel[2], 1, &smoothstep4_kernel_code, "test_smoothstep4f" );
  if (err)
    return -1;

  for (i=0; i<3; i++)
  {
      err = clSetKernelArg(kernel[i], 0, sizeof streams[0], &streams[0] );
      err |= clSetKernelArg(kernel[i], 1, sizeof streams[1], &streams[1] );
      err |= clSetKernelArg(kernel[i], 2, sizeof streams[2], &streams[2] );
      err |= clSetKernelArg(kernel[i], 3, sizeof streams[3], &streams[3] );
      if (err != CL_SUCCESS)
    {
      log_error("clSetKernelArgs failed\n");
      return -1;
    }
  }

  threads[0] = (size_t)n_elems;
  for (i=0; i<3; i++)
  {
    err = clEnqueueNDRangeKernel( queue, kernel[i], 1, NULL, threads, NULL, 0, NULL, NULL );
    if (err != CL_SUCCESS)
    {
      log_error("clEnqueueNDRangeKernel failed\n");
      return -1;
    }

    err = clEnqueueReadBuffer( queue, streams[3], true, 0, sizeof(cl_float)*num_elements, (void *)output_ptr, 0, NULL, NULL );
    if (err != CL_SUCCESS)
    {
      log_error("clEnqueueReadBuffer failed\n");
      return -1;
    }

    switch (i)
    {
      case 0:
        max_err = verify_smoothstep(input_ptr[0], input_ptr[1], input_ptr[2], output_ptr, n_elems, 1);
        break;
      case 1:
        max_err = verify_smoothstep(input_ptr[0], input_ptr[1], input_ptr[2], output_ptr, n_elems, 2);
        break;
      case 2:
        max_err = verify_smoothstep(input_ptr[0], input_ptr[1], input_ptr[2], output_ptr, n_elems, 4);
        break;
    }

    if (max_err > MAX_ERR)
    {
      log_error("%s test failed %g max err\n", fn_names[i], max_err);
      err = -1;
    }
    else
    {
      log_info("%s test passed %g max err\n", fn_names[i], max_err);
      err = 0;
    }

    if (err)
      break;
  }

  clReleaseMemObject(streams[0]);
  clReleaseMemObject(streams[1]);
  clReleaseMemObject(streams[2]);
  clReleaseMemObject(streams[3]);
  for (i=0; i<3; i++)
  {
    clReleaseKernel(kernel[i]);
    clReleaseProgram(program[i]);
  }
  free(input_ptr[0]);
  free(input_ptr[1]);
  free(input_ptr[2]);
  free(output_ptr);

  return err;
}


