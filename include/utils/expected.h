#ifndef INCLUDE_EXPECTED_H
#define INCLUDE_EXPECTED_H

#include <type_traits>
#include <utility>
#include <new>
#include <stdexcept>
#include <cassert>
#include <cstddef>
#include <memory>

namespace O
{

	/**
	 * @brief Exception thrown when accessing the value/error of an empty Expected.
	 *
	 * Thin wrapper over std::runtime_error used by Value()/Error() accessors.
	 */
	struct bad_expected_access : std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	/// Tag type to construct an error explicitly
	struct Error_Tag_t { explicit constexpr Error_Tag_t() = default; };
	inline constexpr Error_Tag_t error_tag{};

	/**
	 * @brief A simple "expected" type representing either a value (T) or an error (E).
	 *
	 * The type is move-only and stores either a value or an error in-place using a trivial union. Construction, destruction and moves are performed manually.
	 *
	 * @tparam T Value type (must not be a reference)
	 * @tparam E Error type (must not be a reference)
	 *
	 * @note All non-trivial member functions are defined out-of-class in the accompanying header implementation file `expected.hpp`.
	 */
	template<class T, class E>
	class Expected
	{
		static_assert(!std::is_reference_v<T>, "T must not be a reference");
		static_assert(!std::is_reference_v<E>, "E must not be a reference");

	public:
		// Move-only
		Expected(const Expected&) = delete;
		Expected& operator=(const Expected&) = delete;

		/** @name Construction / destruction */
		/**@{*/
		constexpr Expected() noexcept;
		~Expected() noexcept;
		Expected(Expected&& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);
		Expected& operator=(Expected&& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);
		/**@}*/

		/** @name Value / Error construction helpers */
		/**@{*/
		template <typename U>
		requires std::constructible_from<T, U&&>
		Expected(U&& value) noexcept(std::is_nothrow_constructible_v<T, U&&>);
		template <typename U>
		requires std::constructible_from<E, U&&> && (!std::constructible_from<T, U&&>)
		Expected(U&& err) noexcept(std::is_nothrow_constructible_v<E, U&&>);

		// In-place factories
		template<typename... Args>
		static Expected Make_Value(Args&&... args);

		template<typename... Args>
		static Expected Make_Error(Args&&... args);
		/**@}*/

		/** @name State queries */
		/**@{*/
		constexpr bool Has_Value() const noexcept;
		constexpr bool Has_Error() const noexcept;
		explicit constexpr operator bool() const noexcept;
		/**@}*/

		/** @name Accessors (ref-qualified) */
		/**@{*/
		T& Value() &;
		const T& Value() const &;
		T&& Value() &&;

		E& Error() &;
		const E& Error() const &;
		E&& Error() &&;
		/**@}*/

		/** @name Emplace / modifiers */
		/**@{*/
		template<typename... Args>
		void Emplace_Value(Args&&... args);

		template<typename... Args>
		void Emplace_Error(Args&&... args);

		void Swap(Expected& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);
		/**@}*/

	private:
		enum class Active : unsigned char { None = 0, Value = 1, Error = 2 };

		union Storage
		{
			T value;
			E error;
			constexpr Storage() noexcept {}
			~Storage() noexcept {}
		} m_storage;

		Active m_active;

		/** @name Internal helpers */
		/**@{*/
		void Destroy() noexcept;
		void Move_From(Expected&& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);

		// pointer-like accessors (non-throwing)
		T& Value_Ptr() noexcept;
		const T& Value_Ptr() const noexcept;
		E& Error_Ptr() noexcept;
		const E& Error_Ptr() const noexcept;
		/**@}*/
	};

} // namespace O

#include "expected.hpp"

#endif // INCLUDE_EXPECTED_H