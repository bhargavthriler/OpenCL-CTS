add_executable(test_events
	main.c
	test_events.cpp
	test_event_dependencies.cpp
	test_userevents.cpp
	test_waitlists.cpp
	test_callbacks.cpp
	action_classes.cpp
	test_userevents_multithreaded.cpp
	../../test_common/harness/errorHelpers.c
	../../test_common/harness/threadTesting.c
	../../test_common/harness/testHarness.c
	../../test_common/harness/genericThread.cpp
	../../test_common/harness/kernelHelpers.c
	../../test_common/harness/typeWrappers.cpp
	../../test_common/harness/mt19937.c
	../../test_common/harness/msvc9.c
	../../test_common/harness/conversions.c
	../../test_common/harness/ThreadPool.c
)

set_source_files_properties(
	main.c
	test_events.cpp
	test_event_dependencies.cpp
	test_userevents.cpp
	test_waitlists.cpp
	test_callbacks.cpp
	action_classes.cpp
	test_userevents_multithreaded.cpp
	../../test_common/harness/errorHelpers.c
	../../test_common/harness/threadTesting.c
	../../test_common/harness/testHarness.c
	../../test_common/harness/genericThread.cpp
	../../test_common/harness/kernelHelpers.c
	../../test_common/harness/typeWrappers.cpp
	../../test_common/harness/mt19937.c
	../../test_common/harness/msvc9.c
	../../test_common/harness/conversions.c
	../../test_common/harness/ThreadPool.c
        PROPERTIES LANGUAGE CXX)

TARGET_LINK_LIBRARIES(test_events
        ${CLConform_LIBRARIES})

add_dependencies(OpenCLCTS test_events)
install(TARGETS test_events
    DESTINATION "${CLConf_OUT_DIR}"
    COMPONENT OpenCLCTS)
