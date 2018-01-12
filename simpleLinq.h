#ifndef SIMPLELINQ_H_
#define SIMPLELINQ_H_

#include "option.h"
#include "composeFunc.h"
#include <iostream>
#include <type_traits>
#include <functional>

namespace mylib {
	using mylib::Option;
	using mylib::compose;

	template <typename T, typename F, typename Iter>
	class linqTemp {
		Iter m_begin;
		Iter m_end;
		using opt_T_F = std::function<Option<F>(Option<T>&&)>;
		opt_T_F func;

		template <typename R>
		linqTemp<T, R, Iter> getNew(std::function<Option<R>(Option<F>&&)> fun) {
			using opt_F_R = std::function<Option<R>(Option<F>&&)>;
			using opt_T_R = typename std::function<Option<R>(Option<T>&&)>;
			auto temp = static_cast<opt_T_R>(compose(fun, this->func));
			linqTemp<T, R, Iter> l(m_begin, m_end, temp);
			return l;
		}

		template <typename R>
		linqTemp<T, R, Iter> mapf(const std::function<R(F)>& f) {
			using opt_F_R = typename std::function<Option<R>(Option<F>&&)>;
			using opt_T_R = typename std::function<Option<R>(Option<T>&&)>;
			opt_F_R optionF = static_cast<opt_F_R>([=](Option<F>&& op) {
				if (!op.isInit())
					return Option<R>();
				else
				{
					return Option<R>(f(op.get()));
				}
			});
			return getNew(optionF);
		}

		template <typename R>
		linqTemp<T, R, Iter> mapf(const std::function<R(F&)>& f) {
			using opt_F_R = typename std::function<Option<R>(Option<F>&&)>;
			using opt_T_R = typename std::function<Option<R>(Option<T>&&)>;
			opt_F_R optionF = static_cast<opt_F_R>([=](Option<F>&& op) {
				if (!op.isInit())
					return Option<R>();
				else
				{
					return Option<R>(f(op.get()));
				}
			});
			return getNew(optionF);
		}

		linqTemp<T, F, Iter> wheref(const std::function<bool(F)>& f) {
			using opt_F_F = typename std::function<Option<F>(Option<F>&&)>;
			opt_F_F optionF = static_cast<opt_F_F>([=](Option<F>&& op) {
				if (!op.isInit())
					return Option<F>();
				else {
					if (f(op.get())) {
						//attention here, rvalue ref parameter should be returned as rvalue
						return std::move(op);
					}
					else {
						return Option<F>();
					}
				}
			});
			return getNew(optionF);
		}

		linqTemp<T, F, Iter> wheref(const std::function<bool(F&)>& f) {
			using opt_F_F = typename std::function<Option<F>(Option<F>&&)>;
			opt_F_F optionF = static_cast<opt_F_F>([=](Option<F>&& op) {
				if (!op.isInit())
					return Option<F>();
				else {
					if (f(op.get())) {
						//attention here, rvalue ref parameter should be returned as rvalue
						return std::move(op);
					}
					else {
						return Option<F>();
					}
				}
			});
			return getNew(optionF);
		}

	public:

		linqTemp(Iter it1, Iter it2, const opt_T_F& f) :m_begin(it1), m_end(it2), func(f) {}

		template <typename U>
		auto map(U u) {
			auto f = to_stdFunc(u);
			return mapf(f);
		}

		template <typename U>
		linqTemp<T, F, Iter> where(U u) {
			auto f = to_stdFunc(u);
			return wheref(f);
		}

		void print() {	
			for (auto it = m_begin; it != m_end; ++it) {
				auto z = func(Option<T>(*it));
				if (z.isInit()) {
					cout << z.get() << endl;
				}
			}
		}
	};

	template <typename T, typename Iter>
	class linqTemp<T, T, Iter> {
		Iter m_begin;
		Iter m_end;
		using Identical = typename std::function<Option<T>(Option<T>&&)>;		
		Identical func;

		template <typename R>
		linqTemp<T, R, Iter> getNew(std::function<Option<R>(Option<T>&&)> fun) {
			using opt_T_R = typename std::function<Option<R>(Option<T>&&)>;
			auto temp = static_cast<opt_T_R>(compose(fun, this->func));
			linqTemp<T, R, Iter> l(m_begin, m_end, temp);
			return l;
		}

		template <typename R>
		linqTemp<T, R, Iter> mapf(const std::function<R(T)>& f) {
			using opt_T_R = typename std::function<Option<R>(Option<T>&&)>;
			opt_T_R optionF = static_cast<opt_T_R>([=](Option<T>&& op) {
				if (!op.isInit())
					return Option<R>();
				else
				{
					return Option<R>(f(op.get()));
				}
			});
			return getNew(optionF);
		}

		template <typename R>
		linqTemp<T, R, Iter> mapf(const std::function<R(T&)>& f) {
			using opt_T_R = typename std::function<Option<R>(Option<T>&&)>;
			opt_T_R optionF = static_cast<opt_T_R>([=](Option<T>&& op) {
				if (!op.isInit())
					return Option<R>();
				else
				{
					return Option<R>(f(op.get()));
				}
			});
			return getNew(optionF);
		}

		linqTemp<T, T, Iter> wheref(const std::function<bool(T)>& f) {
			using opt_T_T = typename std::function<Option<T>(Option<T>&&)>;
			opt_T_T optionF = static_cast<opt_T_T>([=](Option<T>&& op) {
				if (!op.isInit())
					return Option<T>();
				else {
					if (f(op.get())) {
						//attention here, rvalue ref parameter should be returned as rvalue
						return std::move(op);
					}
					else {
						return Option<T>();
					}
				}
			});
			return getNew(optionF);
		}

		linqTemp<T, T, Iter> wheref(const std::function<bool(T&)>& f) {
			using opt_T_T = typename std::function<Option<T>(Option<T>&&)>;
			opt_T_T optionF = static_cast<opt_T_T>([=](Option<T>&& op) {
				if (!op.isInit())
					return Option<T>();
				else {
					if (f(op.get())) {
						//attention here, rvalue ref parameter should be returned as rvalue
						return std::move(op);
					}
					else {
						return Option<T>();
					}
				}
			});
			return getNew(optionF);
		}

	public:

		linqTemp(Iter it1, Iter it2) :m_begin(it1), m_end(it2) {
			func = static_cast<Identical>([](Option<T>&& op) {return std::move(op); });
		}
		linqTemp(Iter it1, Iter it2, Identical fun) :m_begin(it1), m_end(it2), func(fun) {}

		template <typename U>
		auto map(const U& u) {
			auto f = to_stdFunc(u);
			return mapf(f);
		}

		template <typename U>
		linqTemp<T, T, Iter> where(const U& u) {
			auto f = to_stdFunc(u);
			return wheref(f);
		}

		void print() {
			for (auto it = m_begin; it != m_end; ++it) {
				auto z = func(std::move(Option<T>(*it)));
				if (z.isInit()) {
					cout << z.get() << endl;
				}
			}
		}
	};

	template <typename Iter>
	auto from(Iter it_begin, Iter it_end) {
		using iter_type = Iter;
		using value_type = std::remove_reference_t<decltype(*it_begin)>;
		return linqTemp<value_type, value_type, Iter>(it_begin, it_end);
	}
}
#endif