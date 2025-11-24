include(FetchContent)

# Name this appropriately; the variable names will be <name>_SOURCE_DIR
FetchContent_Declare(
  cmake_sphinx
  GIT_REPOSITORY https://github.com/k0ekk0ek/cmake-sphinx.git
)

# Populate (download) if not already present
FetchContent_GetProperties(cmake_sphinx)
if(NOT cmake_sphinx_POPULATED)
  message(STATUS "Downloading cmake-sphinx from ${cmake_sphinx_GIT_REPOSITORY} (tag=${cmake_sphinx_GIT_TAG})")
  FetchContent_Populate(cmake_sphinx)
endif()

# Add the modules folder to CMAKE_MODULE_PATH:
# The variable cmake_sphinx_SOURCE_DIR is set by FetchContent when populated.
if(DEFINED cmake_sphinx_SOURCE_DIR AND EXISTS "${cmake_sphinx_SOURCE_DIR}/cmake/Modules")
  list(APPEND CMAKE_MODULE_PATH "${cmake_sphinx_SOURCE_DIR}/cmake/Modules")
  message(STATUS "Appended cmake-sphinx modules: ${cmake_sphinx_SOURCE_DIR}/cmake/Modules")
else()
  message(WARNING "cmake-sphinx was fetched but '${cmake_sphinx_SOURCE_DIR}/cmake/Modules' not found. Check the repository layout or pin a different tag.")
endif()
