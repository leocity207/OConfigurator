include(FetchContent)

FetchContent_Declare(
    rapidjson
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    GIT_TAG v1.1.0
    UPDATE_DISCONNECTED ON
)

FetchContent_GetProperties(rapidjson)
if(NOT rapidjson_POPULATED)
    FetchContent_Populate(rapidjson)
endif()

add_library(RapidJSON::rapidjson INTERFACE IMPORTED)

set_target_properties(RapidJSON::rapidjson PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${rapidjson_SOURCE_DIR}/include"
    INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${rapidjson_SOURCE_DIR}/include"
)
