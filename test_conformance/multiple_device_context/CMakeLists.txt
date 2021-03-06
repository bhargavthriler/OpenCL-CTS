add_executable(test_multiples
        main.c
	test_multiple_contexts.c
	test_multiple_devices.cpp
	../../test_common/harness/errorHelpers.c
	../../test_common/harness/threadTesting.c
	../../test_common/harness/testHarness.c
	../../test_common/harness/kernelHelpers.c
	../../test_common/harness/genericThread.cpp
	../../test_common/harness/typeWrappers.cpp
    ../../test_common/harness/conversions.c
    ../../test_common/harness/mt19937.c
    ../../test_common/harness/msvc9.c
)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)" AND NOT MSVC)
set_source_files_properties(
        COMPILE_FLAGS -msse2)
endif()

set_source_files_properties(
        main.c
	test_multiple_contexts.c
    ../../test_common/harness/mt19937.c
	../../test_common/harness/errorHelpers.c
	../../test_common/harness/threadTesting.c
	../../test_common/harness/testHarness.c
	../../test_common/harness/kernelHelpers.c
    ../../test_common/harness/conversions.c
    ../../test_common/harness/msvc9.c
        PROPERTIES LANGUAGE CXX)

TARGET_LINK_LIBRARIES(test_multiples
        ${CLConform_LIBRARIES})

add_dependencies(OpenCLCTS test_multiples)
install(TARGETS test_multiples
    DESTINATION "${CLConf_OUT_DIR}"
    COMPONENT OpenCLCTS)
