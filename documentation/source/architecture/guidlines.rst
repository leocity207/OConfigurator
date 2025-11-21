C++ codding Guidlines
=====================

	this are C++ codding guidlines that should be followed while adding c++ code to the project

pointer
-------

	* No raw pointer
	* pointer should always be wrapped inside unique or shared pointer

Naming
------

	* Snake_Case for function and class
	* snake_case for variable
	* SNAKE_CASE for constant and enum or exception

comments
--------

	documentation about the function shoudld be written using

	.. code-block:: cpp

		/*
		 * @brief :
		 * @param :
		 * @return:
		 */
		...

	this is mainly like the doxygene style documentation to identify parameters

spacing
-------

	Inside cpp and h add two blank line before begining the first implementation

includes
--------

	Always group and comment includes inside the cpp and header file, you may use the last folder level to group them together

	.. code-block:: cpp

		// STD
		#include <array>

		// Coordinate
		#include "includes/coordinate/planar_coordiante.h"

		// Projector
		#include "includes/coordinate/Projector/base.h"

Indentation
-----------

	Base indentation should be made with tabulation
	stylistic indentation should use space