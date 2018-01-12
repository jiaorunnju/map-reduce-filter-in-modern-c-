#ifndef COMPOSE_H_
#define COMPOSE_H_

namespace mylib {
	template<typename OuterFn, typename InnerFn>
	class Composed {
		OuterFn m_outerFn;
		InnerFn m_innerFn;
	public:
		explicit Composed(OuterFn out, InnerFn in)
			:m_outerFn(out), m_innerFn(in) {}

		template<typename... Args>
		auto operator()(Args&&... args) -> decltype(m_outerFn(m_innerFn(std::forward<Args>(args)...))) {
			return m_outerFn(m_innerFn(std::forward<Args>(args)...));
		}
	};

	template<typename Fun1, typename Fun2>
	Composed<Fun1, Fun2> compose(Fun1 f1, Fun2 f2) {
		return Composed<Fun1, Fun2>(f1, f2);
	}
}
#endif