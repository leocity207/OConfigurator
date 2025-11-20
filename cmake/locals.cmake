#---------------------------------------
# get the current commit hash for version
execute_process(
	COMMAND git rev-parse HEAD
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE GIT_COMMIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

#-----------------------
#setting the config file
configure_file(${PROJECT_SOURCE_DIR}/include/locals.h.in ${PROJECT_SOURCE_DIR}/include/locals.h)
