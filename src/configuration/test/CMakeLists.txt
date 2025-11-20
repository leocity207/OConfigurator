include(GoogleTest)

file(GLOB CONFIGURATION_TEST_SRC "./*.cpp")

add_executable( configuration_test 
	${CONFIGURATION_TEST_SRC}
)

target_include_directories(configuration_test
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(configuration_test
	PRIVATE GTest::gtest_main
	PRIVATE ${PROJECT_NAME}::configuration
	PRIVATE RapidJSON::rapidjson
)

gtest_discover_tests(configuration_test)