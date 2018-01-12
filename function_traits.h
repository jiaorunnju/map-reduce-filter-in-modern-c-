#pragma once
#include <functional>
#include <type_traits>
namespace mylib {
	template <typename T>
	struct function_traits;

	template <typename F>
	struct function_traits {
	private:
		using call_type = function_traits<decltype(&F::operator())>;
	public:
		using ret_type = typename call_type::ret_type;
		using stl_func_type = typename call_type::stl_func_type;
		static constexpr std::size_t arity = call_type::arity - 1;

		template <std::size_t N>
		struct argument {
			static_assert(N < arity, "error: too large parameter index");
			using type = typename call_type::template argument<N + 1>::type;
		};
	};

	template <typename R, typename... Args>
	struct function_traits<R(Args...)> {
		using ret_type = R;
		using stl_func_type = std::function<R(Args...)>;
		static constexpr std::size_t arity = sizeof...(Args);

		template <std::size_t N>
		struct argument {
			static_assert(N < arity, "error, invalid parameter index");
			using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
		};
	};

	template <typename R, typename... Args>
	struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)> {};

	//member function pointer
	template <typename C, typename R, typename... Args>
	struct function_traits<R(C::*)(Args...)> :public function_traits<R(Args...)> {};

	//const member function pointer
	template <typename C, typename R, typename... Args>
	struct function_traits<R(C::*)(Args...) const> :public function_traits<R(Args...)> {};

	template <typename F>
	struct function_traits<F&> : public function_traits<F> {};

	template <typename F>
	struct function_traits<F&&> : public function_traits<F> {};

	template <typename T>
	auto to_stdFunc(const T& t) -> typename function_traits <std::remove_reference_t<T>>::stl_func_type {
		using funcT = function_traits <remove_reference_t<T>>;
		return static_cast<typename funcT::stl_func_type>(t);
	}
}
