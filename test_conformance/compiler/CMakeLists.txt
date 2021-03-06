add_executable(test_compiler    
	main.c
	test_build_helpers.c
	test_compile.c
	test_compiler_defines_for_extensions.cpp
	test_async_build.c
	test_build_options.cpp
	test_preprocessor.c
	test_image_macro.c
	../../test_common/harness/errorHelpers.c
	../../test_common/harness/threadTesting.c
	../../test_common/harness/testHarness.c
	../../test_common/harness/kernelHelpers.c
	../../test_common/harness/typeWrappers.cpp
	../../test_common/harness/mt19937.c
	../../test_common/harness/msvc9.c
	../../test_common/harness/conversions.c
)

set_source_files_properties(
	main.c
	test_build_helpers.c
	test_compile.c
	test_compiler_defines_for_extensions.cpp
	test_async_build.c
	test_build_options.cpp
	test_preprocessor.c
	test_image_macro.c
	../../test_common/harness/errorHelpers.c
	../../test_common/harness/threadTesting.c
	../../test_common/harness/testHarness.c
	../../test_common/harness/kernelHelpers.c
	../../test_common/harness/typeWrappers.cpp
	../../test_common/harness/mt19937.c
	../../test_common/harness/msvc9.c
	../../test_common/harness/conversions.c
    PROPERTIES LANGUAGE CXX)

TARGET_LINK_LIBRARIES(test_compiler
        ${CLConform_LIBRARIES})

# Need to copy the test include folders to sit beside the executable
add_custom_command(TARGET test_compiler POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/includeTestDirectory"
		"$<TARGET_FILE_DIR:test_compiler>/includeTestDirectory"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/secondIncludeTestDirectory"
		"$<TARGET_FILE_DIR:test_compiler>/secondIncludeTestDirectory")

add_dependencies(OpenCLCTS test_compiler)
install(TARGETS test_compiler
    DESTINATION "${CLConf_OUT_DIR}"
    COMPONENT OpenCLCTS)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/includeTestDirectory
		DESTINATION "${CLConf_OUT_DIR}"
		COMPONENT OpenCLCTS)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/secondIncludeTestDirectory
		DESTINATION "${CLConf_OUT_DIR}"
		COMPONENT OpenCLCTS)
