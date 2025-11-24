#ifndef CONFIGURATION_APPLICATION_CONTAINER_H
#define CONFIGURATION_APPLICATION_CONTAINER_H

#include <tuple>

namespace O::Configuration::Application
{
	/**
	 * @brief Heterogeneous container holding module data.
	 *
	 *  @tparam DataModules... : the concrete data types for each module.
	 *
	 * The container exposes the raw tuple as the member `modules` so callers may iterate, access the contained modules. 
	 * Use `Get<T>()` to obtain a reference to the module by its type.
	 */
	template<class... DataModules>
	struct Container
	{
		/// The modules tuple (public for structured access and iteration).
		std::tuple<DataModules...> modules;

		/**
		 * @brief Return a reference to the module of type T.
		 *
		 * @tparam T The module data type stored in the tuple.
		 * @return T& Reference to the module within the tuple.
		 *
		 * @note This uses std::get<T> and therefore will fail to compile if T is not part of DataModules...
		 */
		template<class T>
		T& Get()
		{
			return std::get<T>(modules);
		}
	};

} // namespace O::Configuration::Application

#endif //CONFIGURATION_APPLICATION_CONTAINER_H
