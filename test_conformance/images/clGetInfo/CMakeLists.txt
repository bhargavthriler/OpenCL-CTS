add_executable(test_cl_get_info
	main.cpp
	test_1D.cpp
	test_2D.cpp
	test_1D_2D_array.cpp
	../image_helpers.cpp 
	test_loops.cpp
	test_3D.cpp
	../../../test_common/harness/errorHelpers.c
	../../../test_common/harness/threadTesting.c
	../../../test_common/harness/kernelHelpers.c
	../../../test_common/harness/imageHelpers.cpp
	../../../test_common/harness/conversions.c
	../../../test_common/harness/testHarness.c
	../../../test_common/harness/mt19937.c
	../../../test_common/harness/msvc9.c
	../../../test_common/harness/typeWrappers.cpp
)

set_source_files_properties(
	main.cpp
	test_1D.cpp
	test_2D.cpp
	test_1D_2D_array.cpp
	../image_helpers.cpp 
	test_loops.cpp
	test_3D.cpp
	../../../test_common/harness/errorHelpers.c
	../../../test_common/harness/threadTesting.c
	../../../test_common/harness/kernelHelpers.c
	../../../test_common/harness/imageHelpers.cpp
	../../../test_common/harness/conversions.c
	../../../test_common/harness/testHarness.c
	../../../test_common/harness/mt19937.c
	../../../test_common/harness/msvc9.c
	../../../test_common/harness/typeWrappers.cpp
        PROPERTIES LANGUAGE CXX)

TARGET_LINK_LIBRARIES(test_cl_get_info
        ${CLConform_LIBRARIES})

add_dependencies(OpenCLCTS test_cl_get_info)
install(TARGETS test_cl_get_info
    DESTINATION "${CLConf_OUT_DIR}"
    COMPONENT OpenCLCTS)
