if(WIN32)

set(D3D11_INCLUDE_DIR $ENV{NV_TOOLS}/sdk/DirectX_Aug2009/Include)

if(${ARCH} STREQUAL "i686")
set(D3D11_LIB_DIR $ENV{NV_TOOLS}/sdk/DirectX_Aug2009/Lib/x86)
endif(${ARCH} STREQUAL "i686")

if(${ARCH} STREQUAL "x86_64")
set(D3D11_LIB_DIR $ENV{NV_TOOLS}/sdk/DirectX_Aug2009/Lib/x64)
endif(${ARCH} STREQUAL "x86_64")

list(APPEND CLConform_INCLUDE_DIR ${D3D11_INCLUDE_DIR})
include_directories (${CLConform_INCLUDE_DIR})
link_directories(${CL_LIB_DIR}, ${D3D11_LIB_DIR})

list(APPEND CLConform_LIBRARIES d3d11 dxgi)


set(D3D11_SOURCES
    buffer.cpp
    texture2d.cpp 
    texture3d.cpp
    misc.cpp 
    main.cpp 
    harness.cpp
    ../../test_common/harness/errorHelpers.c
    ../../test_common/harness/threadTesting.c
    ../../test_common/harness/testHarness.c
    ../../test_common/harness/kernelHelpers.c
    ../../test_common/harness/mt19937.c
    ../../test_common/harness/conversions.c
    ../../test_common/harness/msvc9.c)

add_executable(test_d3d11
    ${D3D11_SOURCES})

set_source_files_properties(
    ${D3D11_SOURCES}
    PROPERTIES LANGUAGE CXX)
        
TARGET_LINK_LIBRARIES(test_d3d11
    ${CLConform_LIBRARIES})

add_dependencies(OpenCLCTS test_d3d11)
install(TARGETS test_d3d11
    DESTINATION "${CLConf_OUT_DIR}"
    COMPONENT OpenCLCTS)

endif(WIN32)
