add_library(configuration INTERFACE)

target_include_directories(configuration
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(configuration
    INTERFACE RapidJSON::rapidjson
)

add_library(${PROJECT_NAME}::configuration ALIAS configuration)
