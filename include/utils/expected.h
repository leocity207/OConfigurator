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

	struct bad_expected_access : std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	/// Tag type to construct an error explicitly
	struct Error_Tag_t { explicit constexpr Error_Tag_t() = default; };
	inline constexpr Error_Tag_t error_tag{};

	/**
	* @brief class to reprensent expected value on return
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

		// Default - empty (neither value nor error)
		constexpr Expected() noexcept : m_active(Active::None) {}

		// Destructor
		~Expected() noexcept { Destroy(); }

		// Move constructor
		Expected(Expected&& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>) :
			m_active(Active::None)
		{
			Move_From(std::move(other));
		}

		// Move assignment
		Expected& operator=(Expected&& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
		{
			if (this == &other) return *this;
			Destroy();
			Move_From(std::move(other));
			return *this;
		}

		template <typename U>
			requires std::constructible_from<T, U&&>
		Expected(U&& value)
			noexcept(std::is_nothrow_constructible_v<T, U&&>)
		{
			new (&m_storage.value) T(std::forward<U>(value));
			m_active = Active::Value;
		}

		// Error ctor
		template <typename U>
			requires std::constructible_from<E, U&&> &&
		(!std::constructible_from<T, U&&>) // avoid clash if both T and E accept U
			Expected(U&& err)
			noexcept(std::is_nothrow_constructible_v<E, U&&>)
		{
			new (&m_storage.error) E(std::forward<U>(err));
			m_active = Active::Error;
		}

		// In-place factories
		template<typename... Args>
		static Expected Make_Value(Args&&... args)
		{
			Expected out;
			out.Emplace_Value(std::forward<Args>(args)...);
			return out;
		}

		template<typename... Args>
		static Expected Make_Error(Args&&... args)
		{
			Expected out;
			out.Emplace_Error(std::forward<Args>(args)...);
			return out;
		}

		// Query
		constexpr bool Has_Value() const noexcept { return m_active == Active::Value; }
		constexpr bool Has_Error() const noexcept { return m_active == Active::Error; }
		explicit constexpr operator bool() const noexcept { return Has_Value(); }

		// Accessors (ref-qualified)
		T& Value() &
		{
			if (!Has_Value()) throw bad_expected_access("no value");
			return Value_Ptr();
		}
		const T& Value() const &
		{
			if (!Has_Value()) throw bad_expected_access("no value");
			return Value_Ptr();
		}
		T&& Value() &&
		{
			if (!Has_Value()) throw bad_expected_access("no value");
			return std::move(Value_Ptr());
		}

		E& Error() &
		{
			if (!Has_Error()) throw bad_expected_access("no error");
			return Error_Ptr();
		}

		const E& Error() const &
		{
			if (!Has_Error()) throw bad_expected_access("no error");
			return Error_Ptr();
		}

		E&& Error() &&
		{
			if (!Has_Error()) throw bad_expected_access("no error");
			return std::move(Error_Ptr());
		}

		// Emplace
		template<typename... Args>
		void Emplace_Value(Args&&... args) {
			Destroy();
			new (&m_storage.value) T(std::forward<Args>(args)...);
			m_active = Active::Value;
		}

		template<typename... Args>
		void Emplace_Error(Args&&... args) {
			Destroy();
			new (&m_storage.error) E(std::forward<Args>(args)...);
			m_active = Active::Error;
		}

		// Optional: swap (noexcept if underlying moves are noexcept)
		void Swap(Expected& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
		{
			if (this == &other) return;
			Expected tmp = std::move(other);
			other = std::move(*this);
			*this = std::move(tmp);
		}

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

		// helpers
		void Destroy() noexcept
		{
			if (m_active == Active::Value)
				m_storage.value.~T();
			else if (m_active == Active::Error)
				m_storage.error.~E();
			m_active = Active::None;
		}

		// move contents from other into this (leaves other empty)
		void Move_From(Expected&& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
		{
			m_active = other.m_active;
			if (other.m_active == Active::Value)
				new (&m_storage.value) T(std::move(other.m_storage.value));
			else if (other.m_active == Active::Error)
				new (&m_storage.error) E(std::move(other.m_storage.error));
			other.Destroy();
		}

		// pointer-like accessors
		T& Value_Ptr() noexcept { return m_storage.value; }
		const T& Value_Ptr() const noexcept { return m_storage.value; }
		E& Error_Ptr() noexcept { return m_storage.error; }
		const E& Error_Ptr() const noexcept { return m_storage.error; }
	};

} // namespace O

#endif // INCLUDE_EXPECTED_H
