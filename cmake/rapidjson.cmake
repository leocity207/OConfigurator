include(FetchContent)

FetchContent_Declare(
	rapidjson
	GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
	GIT_TAG v1.1.0
	UPDATE_DISCONNECTED ON
)

set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "Disable rapidjson tests" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "Disable rapidjson examples" FORCE)
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "Disable rapidjson docs" FORCE)
set(RAPIDJSON_BUILD_THIRDPARTY_GTEST OFF CACHE BOOL "Don't build bundled gtest" FORCE)

FetchContent_MakeAvailable(rapidjson)

add_library(RapidJSON::rapidjson INTERFACE IMPORTED)

set_target_properties(RapidJSON::rapidjson PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${rapidjson_SOURCE_DIR}/include"
)