#ifndef INCLUDE_EXPECTED_H
#define INCLUDE_EXPECTED_H

#include <type_traits>
#include <utility>
#include <new>
#include <stdexcept>
#include <cassert>

namespace O
{

	struct bad_expected_access : std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	template<class T, class E>
	class Expected {
		static_assert(!std::is_reference<T>::value, "T must not be a reference");
		static_assert(!std::is_reference<E>::value, "E must not be a reference");
	public:
		// disable copy
		Expected(const Expected&) = delete;
		Expected& operator=(const Expected&) = delete;

		// default destructor
		~Expected() { destroy(); }

		// move ctor
		Expected(Expected&& other) noexcept(
			std::is_nothrow_move_constructible<T>::value&&
			std::is_nothrow_move_constructible<E>::value)
			: active_(other.active_)
		{
			if (other.active_ == Active::Value) {
				new (&storage_) T(std::move(*other.value_ptr()));
			}
			else if (other.active_ == Active::Error) {
				new (&storage_) E(std::move(*other.error_ptr()));
			}
			other.destroy();
		}

		// move assign
		Expected& operator=(Expected&& other) noexcept(
			std::is_nothrow_move_constructible<T>::value&&
			std::is_nothrow_move_constructible<E>::value)
		{
			if (this == &other) return *this;
			destroy();
			active_ = other.active_;
			if (other.active_ == Active::Value) {
				new (&storage_) T(std::move(*other.value_ptr()));
			}
			else if (other.active_ == Active::Error) {
				new (&storage_) E(std::move(*other.error_ptr()));
			}
			other.destroy();
			return *this;
		}

		// construct from T (rvalue)
		explicit Expected(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
			: active_(Active::Value)
		{
			new (&storage_) T(std::move(value));
		}

		// construct from E (rvalue)
		explicit Expected(E&& err, bool is_error) noexcept(std::is_nothrow_move_constructible<E>::value)
			: active_(Active::Error)
		{
			(void)is_error;
			new (&storage_) E(std::move(err));
		}

		// in-place construction for T
		template<typename... Args>
		static Expected Make_Value(Args&&... args) {
			Expected out;
			out.emplace_value(std::forward<Args>(args)...);
			return out;
		}

		// in-place construction for E
		template<typename... Args>
		static Expected Make_Error(Args&&... args) {
			Expected out;
			out.emplace_error(std::forward<Args>(args)...);
			return out;
		}

		// query
		bool Has_Value() const noexcept { return active_ == Active::Value; }
		explicit operator bool() const noexcept { return Has_Value(); }

		// accessors
		T& Value()& {
			if (!Has_Value()) throw bad_expected_access("no value");
			return *value_ptr();
		}
		const T& Value() const& {
			if (!Has_Value()) throw bad_expected_access("no value");
			return *value_ptr();
		}
		// move the value out if called on rvalue Expected
		T&& Value()&& {
			if (!Has_Value()) throw bad_expected_access("no value");
			return std::move(*value_ptr());
		}

		E& Error()& {
			if (Has_Value()) throw bad_expected_access("no error");
			return *error_ptr();
		}
		const E& Error() const& {
			if (Has_Value()) throw bad_expected_access("no error");
			return *error_ptr();
		}
		E&& Error()&& {
			if (Has_Value()) throw bad_expected_access("no error");
			return std::move(*error_ptr());
		}

		// emplace
		template<typename... Args>
		void emplace_value(Args&&... args) {
			destroy();
			new (&storage_) T(std::forward<Args>(args)...);
			active_ = Active::Value;
		}

		template<typename... Args>
		void emplace_error(Args&&... args) {
			destroy();
			new (&storage_) E(std::forward<Args>(args)...);
			active_ = Active::Error;
		}

	private:
		Expected() : active_(Active::None) {}

		enum class Active : unsigned char { None = 0, Value = 1, Error = 2 };

		static constexpr std::size_t max_size = sizeof(T) > sizeof(E) ? sizeof(T) : sizeof(E);
		static constexpr std::size_t max_align = alignof(T) > alignof(E) ? alignof(T) : alignof(E);
		using storage_t = typename std::aligned_storage<max_size, max_align>::type;

		storage_t storage_;
		Active active_;

		void destroy() noexcept {
			if (active_ == Active::Value) {
				value_ptr()->~T();
			}
			else if (active_ == Active::Error) {
				error_ptr()->~E();
			}
			active_ = Active::None;
		}

		T* value_ptr() noexcept { return reinterpret_cast<T*>(&storage_); }
		const T* value_ptr() const noexcept { return reinterpret_cast<const T*>(&storage_); }
		E* error_ptr() noexcept { return reinterpret_cast<E*>(&storage_); }
		const E* error_ptr() const noexcept { return reinterpret_cast<const E*>(&storage_); }
	};

} // namespace O

#endif // INCLUDE_EXPECTED_H