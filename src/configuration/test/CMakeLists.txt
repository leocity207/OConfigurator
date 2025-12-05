include(../../../cmake/add_simple_test.cmake)
Add_Simple_Googletest_Target(configuration_test
	RapidJSON::rapidjson
	${PROJECT_NAME}::configuration
	OUtils::utils
)