include(../cmake/cmake_sphinx.cmake)

find_package(Doxygen)
find_package(Sphinx)

IF(Doxygen_FOUND AND Sphinx_FOUND)
	find_package(Sphinx REQUIRED breathe)
	set(DOXYGEN_GENERATE_HTML NO)
	set(DOXYGEN_GENERATE_XML YES)
	doxygen_add_docs(doxygen
		${PROJECT_SOURCE_DIR}/include
		COMMENT "Generate man pages")


	sphinx_add_docs(
		Oconfigurator_Sphinx_documentation
		BREATHE_PROJECTS doxygen
		BUILDER html
		SOURCE_DIRECTORY source
		CONFIG_DIRECTORY source
	)
else()
	message("Cannot build documentation since Doxygen Sphinx and breathe cannot be found")
endif()