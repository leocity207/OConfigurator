#ifndef INCLUDE_EXPECTED_HPP
#define INCLUDE_EXPECTED_HPP

#include "expected.h"

namespace O
{

	// --- Construction / destruction ---
	template<class T, class E>
	constexpr Expected<T, E>::Expected() noexcept : m_active(Active::None) {}

	template<class T, class E>
	Expected<T, E>::~Expected() noexcept { Destroy(); }

	template<class T, class E>
	Expected<T, E>::Expected(Expected&& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>) : m_active(Active::None)
	{
		Move_From(std::move(other));
	}

	template<class T, class E>
	Expected<T, E>& Expected<T, E>::operator=(Expected&& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
	{
		if (this == &other) return *this;
		Destroy();
		Move_From(std::move(other));
		return *this;
	}

	// --- Value / Error ctor templates ---
	template<class T, class E>
	template<typename U>
	requires std::constructible_from<T, U&&>
	Expected<T, E>::Expected(U&& value) noexcept(std::is_nothrow_constructible_v<T, U&&>)
	{
		new (&m_storage.value) T(std::forward<U>(value));
		m_active = Active::Value;
	}

	template<class T, class E>
	template<typename U>
	requires std::constructible_from<E, U&&> && (!std::constructible_from<T, U&&>)
	Expected<T, E>::Expected(U&& err) noexcept(std::is_nothrow_constructible_v<E, U&&>)
	{
		new (&m_storage.error) E(std::forward<U>(err));
		m_active = Active::Error;
	}

	// --- In-place factories ---
	template<class T, class E>
	template<typename... Args>
	Expected<T, E> Expected<T, E>::Make_Value(Args&&... args)
	{
		Expected out;
		out.Emplace_Value(std::forward<Args>(args)...);
		return out;
	}

	template<class T, class E>
	template<typename... Args>
	Expected<T, E> Expected<T, E>::Make_Error(Args&&... args)
	{
		Expected out;
		out.Emplace_Error(std::forward<Args>(args)...);
		return out;
	}

	// --- Queries ---
	template<class T, class E>
	constexpr bool Expected<T, E>::Has_Value() const noexcept { return m_active == Active::Value; }

	template<class T, class E>
	constexpr bool Expected<T, E>::Has_Error() const noexcept { return m_active == Active::Error; }

	template<class T, class E>
	constexpr Expected<T, E>::operator bool() const noexcept { return Has_Value(); }

	// --- Accessors ---
	template<class T, class E>
	T& Expected<T, E>::Value() &
	{
		if (!Has_Value()) throw bad_expected_access("no value");
		return Value_Ptr();
	}

	template<class T, class E>
	const T& Expected<T, E>::Value() const &
	{
		if (!Has_Value()) throw bad_expected_access("no value");
		return Value_Ptr();
	}

	template<class T, class E>
	T&& Expected<T, E>::Value() &&
	{
		if (!Has_Value()) throw bad_expected_access("no value");
		return std::move(Value_Ptr());
	}

	template<class T, class E>
	E& Expected<T, E>::Error() &
	{
		if (!Has_Error()) throw bad_expected_access("no error");
		return Error_Ptr();
	}

	template<class T, class E>
	const E& Expected<T, E>::Error() const &
	{
		if (!Has_Error()) throw bad_expected_access("no error");
		return Error_Ptr();
	}

	template<class T, class E>
	E&& Expected<T, E>::Error() &&
	{
		if (!Has_Error()) throw bad_expected_access("no error");
		return std::move(Error_Ptr());
	}

	// --- Emplace / modifiers ---
	template<class T, class E>
	template<typename... Args>
	void Expected<T, E>::Emplace_Value(Args&&... args)
	{
		Destroy();
		new (&m_storage.value) T(std::forward<Args>(args)...);
		m_active = Active::Value;
	}

	template<class T, class E>
	template<typename... Args>
	void Expected<T, E>::Emplace_Error(Args&&... args)
	{
		Destroy();
		new (&m_storage.error) E(std::forward<Args>(args)...);
		m_active = Active::Error;
	}

	template<class T, class E>
	void Expected<T, E>::Swap(Expected& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
	{
		if (this == &other) return;
		Expected tmp = std::move(other);
		other = std::move(*this);
		*this = std::move(tmp);
	}

	// --- Internal helpers ---
	template<class T, class E>
	void Expected<T, E>::Destroy() noexcept
	{
		if (m_active == Active::Value)
			m_storage.value.~T();
		else if (m_active == Active::Error)
			m_storage.error.~E();
		m_active = Active::None;
	}

	template<class T, class E>
	void Expected<T, E>::Move_From(Expected&& other) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
	{
		m_active = other.m_active;
		if (other.m_active == Active::Value)
			new (&m_storage.value) T(std::move(other.m_storage.value));
		else if (other.m_active == Active::Error)
			new (&m_storage.error) E(std::move(other.m_storage.error));
		other.Destroy();
	}

	template<class T, class E>
	T& Expected<T, E>::Value_Ptr() noexcept { return m_storage.value; }

	template<class T, class E>
	const T& Expected<T, E>::Value_Ptr() const noexcept { return m_storage.value; }

	template<class T, class E>
	E& Expected<T, E>::Error_Ptr() noexcept { return m_storage.error; }

	template<class T, class E>
	const E& Expected<T, E>::Error_Ptr() const noexcept { return m_storage.error; }

} // namespace O

#endif // INCLUDE_EXPECTED_HPP
