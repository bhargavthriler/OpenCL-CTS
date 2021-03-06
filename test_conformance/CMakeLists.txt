cmake_minimum_required(VERSION 3.0)

option(D3D10_IS_SUPPORTED "Run DirectX 10 interop tests" OFF)
option(D3D11_IS_SUPPORTED "Run DirectX 11 interop tests" OFF)
option(GL_IS_SUPPORTED "Run OpenGL interop tests" OFF)

# CLConform_GL_LIBRARIES_DIR - path to OpenGL libraries
if(GL_IS_SUPPORTED AND CLConform_GL_LIBRARIES_DIR)
    link_directories(${CLConform_GL_LIBRARIES_DIR})
endif (GL_IS_SUPPORTED AND CLConform_GL_LIBRARIES_DIR)

if(MSVC)
  # The CTS will not link on MSVC 32-bit builds. To workaround this, add the
  # /LARGEADDRESSAWARE option as a linker flag. This is automatically set for
  # MSVC 64-bit so it does no harm to enable it on all MSVC builds.
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
else()
  # Allowing the compiler to contract floating-point math causes wrong results
  # to be output from the reference math of bruteforce tests, and for the
  # host rounding code in conversions and image_streams tests.
  set(CTS_EXTRA_FLAGS "-ffp-contract=off")

  if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    # We set '-mfpmath=sse' because it is the default on the x86-64 compiler,
    # and we want consistent results for x86 code too.
    # We set '-msse2' as most (but not all) of the sub CMakeLists.txt we include
    # set it, so setting it here once means we have consistent compiliation
    # flags across the entire conformance_test_* executables.
    set(CTS_EXTRA_FLAGS "${CTS_EXTRA_FLAGS} -mfpmath=sse -msse2")
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(ARM)|(ARM64)|(aarch64)")
    # The OpenCL CTS assumes that the char type is signed, which is not the
    # default on ARM compilers, so we need to set it.
    set(CTS_EXTRA_FLAGS "${CTS_EXTRA_FLAGS} -fsigned-char")
  endif()

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CTS_EXTRA_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CTS_EXTRA_FLAGS}")
endif()

# Suppress narrowing warnings given by Clang and GCC
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Clang)|(GNU)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-narrowing -Wno-narrowing")
endif()

if( WIN32 AND "${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qlong-double -Qpc80 /DWIN32 /D_WINDOWS /W3 /GR /EHsc -nologo -Od -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -EHsc -Wall -Qdiag-disable:68,111,177,186,161,869,1028,2259,2553,181,239,265,1188 -fp:strict -fp:source")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Qlong-double -Qpc80 /DWIN32 /D_WINDOWS /W3 /GR /EHsc -nologo -Od -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -EHsc -Wall -Qdiag-disable:68,111,177,186,161,869,1028,2259,2553,181,239,265,1188 -fp:strict -fp:source")
endif()

include_directories(BEFORE SYSTEM ${CLConform_HEADERS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../test_common)
link_directories(${CLConform_LIB_DIR})

add_definitions(-DCL_TARGET_OPENCL_VERSION=120)

add_custom_target(OpenCLCTS)

add_subdirectory(allocations)
add_subdirectory(api)
add_subdirectory(atomics)
add_subdirectory(basic)
add_subdirectory(buffers)
add_subdirectory(commonfns)
add_subdirectory(compiler)
add_subdirectory(computeinfo)
add_subdirectory(contractions)
add_subdirectory(conversions)
add_subdirectory(device_partition)
if(D3D10_IS_SUPPORTED)
	add_subdirectory(d3d10)
endif(D3D10_IS_SUPPORTED)
if(D3D11_IS_SUPPORTED)
	add_subdirectory(d3d11)
endif(D3D11_IS_SUPPORTED)
add_subdirectory(events)
add_subdirectory(geometrics)
if(GL_IS_SUPPORTED)
	add_subdirectory(gl)
endif(GL_IS_SUPPORTED)
add_subdirectory(half)
add_subdirectory(headers)
add_subdirectory(images)
add_subdirectory(integer_ops)
add_subdirectory(math_brute_force)
add_subdirectory(mem_host_flags)
add_subdirectory(multiple_device_context)
add_subdirectory(printf)
add_subdirectory(profiling)
add_subdirectory(relationals)
add_subdirectory(select)
add_subdirectory(thread_dimensions)
add_subdirectory(vec_align)
add_subdirectory(vec_step)

# Add any extension folders
add_subdirectory(spir)

# Copy all csv files
FILE(GLOB CSV_FILES
 	 "${CMAKE_CURRENT_SOURCE_DIR}/*.csv"
)
FILE(COPY ${CSV_FILES} DESTINATION "${CMAKE_BINARY_DIR}")

# Copy test runner
FILE(COPY "${CMAKE_CURRENT_SOURCE_DIR}/run_conformance.py" DESTINATION "${CMAKE_BINARY_DIR}")

