# VS2005 optimization WAR for geom_length)
if(MSVC)
set(CMAKE_C_FLAGS_RELEASE "/Od /Ob0")
set(CMAKE_CXX_FLAGS_RELEASE "/Od /Ob0")
endif(MSVC)

add_executable(test_geometrics
        main.c
	test_geometrics_double.cpp
	test_geometrics.cpp
	../../test_common/harness/errorHelpers.c
	../../test_common/harness/threadTesting.c
	../../test_common/harness/testHarness.c
	../../test_common/harness/kernelHelpers.c
    ../../test_common/harness/mt19937.c
    ../../test_common/harness/conversions.c
    ../../test_common/harness/msvc9.c
)

set_source_files_properties(
        main.c
    ../../test_common/harness/mt19937.c
	../../test_common/harness/errorHelpers.c
	../../test_common/harness/threadTesting.c
	../../test_common/harness/testHarness.c
	../../test_common/harness/kernelHelpers.c
    ../../test_common/harness/conversions.c
    ../../test_common/harness/msvc9.c    
    PROPERTIES LANGUAGE CXX)

TARGET_LINK_LIBRARIES(test_geometrics
        ${CLConform_LIBRARIES})

add_dependencies(OpenCLCTS test_geometrics)
install(TARGETS test_geometrics
    DESTINATION "${CLConf_OUT_DIR}"
    COMPONENT OpenCLCTS)
