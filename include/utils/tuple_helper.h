#ifndef INCLUDE_TUPLE_HELPER_H
#define INCLUDE_TUPLE_HELPER_H

#include <tuple>

namespace O
{
	template<class Tuple, class F>
	void For_Each_In_Tuple(Tuple& t, F&& f)
	{
		std::apply([&](auto&... e) {(f(e), ...);}, t);
	}
} // namespace O

#endif // INCLUDE_TUPLE_HELPER_H