#ifndef CONFIGURATION_APPLICATION_HOLDER_H
#define CONFIGURATION_APPLICATION_HOLDER_H

#include <tuple>

namespace O::Configuration::Application
{
	template<class... DataModules>
	struct Container
	{
		std::tuple<DataModules...> modules;

		template<class T>
		T& Get()
		{
			return std::get<T>(modules);
		}
	};

} // namespace O::Configuration::Application

#endif //CONFIGURATION_APPLICATION_HOLDER_H
