#ifndef OPTION_H_
#define OPTION_H_

#include<type_traits>
#include<iostream>
#include<memory>

namespace mylib {
	using std::cout;
	using std::endl;
	using std::unique_ptr;

	template <typename T, typename Small = void>
	class Option {
		using data_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;
		bool m_hasInit = false;
		data_t m_data;

		template<typename... Args>
		void create(Args&&... args) {
			new (&m_data) T(std::forward<Args>(args)...);
			m_hasInit = true;
		}

		void swapContent(Option&& other) {
			if (m_hasInit) {
				destroy();
			}
			std::swap(this->m_data, other.m_data);
			std::swap(this->m_hasInit, other.m_hasInit);
			//cout << __FUNCTION__ << endl;
		}

		void destroy() {
			if (m_hasInit) {
				m_hasInit = false;
				reinterpret_cast<T*>(&m_data)->~T();
			}
			//cout << __FUNCTION__ << endl;
		}

	public:
		bool first = true;
		Option() {}
		Option(const T& t) {
			create(t);
		}

		Option(T&& t) {
			create(std::move(t));
		}

		Option(Option&& other) {
			swapContent(std::move(other));
		}

		Option(const Option& other) = delete;

		Option& operator= (const Option&) = delete;

		Option& operator= (Option&& other) {
			swapContent(std::move(other));
			return *this;
		}

		void clear() {
			this->operator=(std::move(Option<T>()));
		}

		~Option() {
			destroy();
		}

		template <typename... Args>
		void emplace(Args&&... args) {
			cout << __FUNCTION__ << endl;
			destroy();
			create(std::forward<Args>(args)...);
		}

		bool isInit() const { return m_hasInit; }

		T& get() {
			//cout << __FUNCSIG__ << endl;
			if (isInit()) {
				return *(reinterpret_cast<T*>(&m_data));
			}
			throw std::logic_error("option is not init");
		}

		const T& get() const {
			//cout << __FUNCTION__ << endl;
			if (isInit()) {
				return *(reinterpret_cast<const T*>(&m_data));
			}
			throw std::logic_error("option is not init");
		}
	};


	template <typename T>
	class Option<T, typename std::enable_if_t<(sizeof(T) > 16)>> {
		bool m_hasInit = false;
		unique_ptr<T> ptr;

		template<typename... Args>
		void create(Args&&... args) {
			ptr = make_unique<T>(std::forward<Args>(args)...);
			m_hasInit = true;
			//cout << __FUNCTION__ << endl;
		}

		void swapContent(Option&& other) {
			if (m_hasInit) {
				destroy();
			}
			std::swap(this->ptr, other.ptr);
			std::swap(this->m_hasInit, other.m_hasInit);
			//cout << __FUNCTION__ << endl;
		}

		void destroy() {
			if (m_hasInit) {
				m_hasInit = false;
				ptr = std::move(unique_ptr<T>());
			}
			//cout << __FUNCTION__ << endl;
		}

	public:
		bool first = false;
		Option() {}
		Option(const T& t) {
			create(t);
		}

		Option(T&& t) {
			create(std::move(t));
		}

		Option(Option&& other) {
			swapContent(std::move(other));
		}

		Option(const Option& other) = delete;

		Option& operator= (const Option&) = delete;

		Option& operator= (Option&& other) {
			swapContent(std::move(other));
			return *this;
		}

		~Option() {
			destroy();
		}

		void clear() {
			this->operator=(Option<T>());
		}

		template <typename... Args>
		void emplace(Args&&... args) {
			//cout << __FUNCTION__ << endl;
			destroy();
			create(std::forward<Args>(args)...);
		}

		bool isInit() const { return m_hasInit; }

		T& get() {
			//cout << __FUNCSIG__ << endl;
			if (isInit()) {
				return *ptr;
			}
			throw std::logic_error("option is not init");
		}

		const T& get() const {
			//cout << __FUNCTION__ << endl;
			if (isInit()) {
				return *ptr;
			}
			throw std::logic_error("option is not init");
		}
	};
}
#endif
