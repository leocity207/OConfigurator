file(GLOB CONFIGURATION_SRC "./*.cpp")

add_library(configuration STATIC ${CONFIGURATION_SRC})

target_include_directories(configuration
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(configuration
	PRIVATE RapidJSON::rapidjson
)

add_library(${PROJECT_NAME}::configuration ALIAS configuration)