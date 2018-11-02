#pragma once
/*
utest - The micro unit test framework for C++11

Author - Oli Wilkinson (https://github.com/evolutional/upptest)

This file provides both the interface and the implementation.

To provide the implementation for this library, you MUST define
UTEST_CPP_IMPLEMENTATION in exactly on source file.

	#define UTEST_CPP_IMPLEMENTATION
	#include "upptest.h"

USAGE
-----

See README.md for details


LICENSE
-------

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
*/

#include <chrono>
#include <exception>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace utest
{
#define TEST_FIXTURE(name)		class name : public utest::test	
#define SETUP()			virtual void pre_test() override
#define TEARDOWN()		virtual void post_test() override
#define TEST_AUTO_NAME(name)	g_utestautoreg_test_##name
#define TEST_F(name, fixture, category)	\
	class name : public fixture	\
	{	\
		public:	\
			name() : fixture() {}	\
			void execute_test() override;	\
			static utest::info s_info;	\
	};	\
	utest::info name::s_info([]() {return std::make_unique< name >();}, #name, category, __FILE__, __LINE__);	\
	namespace { utest::auto_registered_test TEST_AUTO_NAME(name)(&name::s_info); } \
	void name::execute_test()

#define TEST(name, category)	TEST_F(name, utest::test, category)

#define UASSERT(e)	::utest::assert::expr(e, __FILE__ ,__LINE__)
#define UASSERT_EQ(expected, actual)	::utest::assert::eq(expected, actual, __FILE__ ,__LINE__)
#define UASSERT_NEQ(not_expected, actual)	::utest::assert::neq(not_expected, actual, __FILE__ ,__LINE__)
#define UASSERT_TRUE(v)	::utest::assert::is_true(v, __FILE__ ,__LINE__)
#define UASSERT_FALSE(v)	::utest::assert::is_false(v, __FILE__ ,__LINE__)
#define UASSERT_NULL(ptr)	::utest::assert::is_null(ptr, __FILE__ ,__LINE__)
#define UASSERT_NOT_NULL(ptr)	::utest::assert::is_not_null(ptr, __FILE__ ,__LINE__)
#define UASSERT_FAIL(msg)	::utest::assert::fail(msg, __FILE__ ,__LINE__)

	class assert_fail_exception
		: public std::exception
	{
	public:
		explicit assert_fail_exception(const std::string& msg, const char* file_name, const int line_num)
			: _msg(msg)
			, _file_name(file_name)
			, _line_num(line_num)
		{
		}
		const char* what() const noexcept override { return _msg.c_str(); }
		std::string what_str() const { return _msg; }
		const char* file_name() const { return _file_name; }
		int line_num() const { return _line_num; }
	private:
		std::string _msg;
		const char* _file_name;
		const int _line_num;
	};

	enum class status
	{
		not_run,
		pass,
		fail
	};
	
	class info;

	struct result final
	{
		result()
			: info(nullptr)
			, status(status::not_run)
			, duration()
			, err_message()
			, err_file()
			, err_line(0)
		{}

		void exception(const std::exception& ex)
		{
			status = status::fail;
			err_message = "unhandled exception";
			if (ex.what())
			{
				err_message.append(": ");
				err_message.append(ex.what());
			}
		}

		void fail(const std::string& msg, const std::string& file_name, const int line_num)
		{
			status = status::fail;
			err_message = msg;
			err_file = file_name;
			err_line = line_num;
		}

		const info* info;
		status status;
		std::chrono::milliseconds duration;
		std::string err_message;
		std::string err_file;
		int err_line;
	};

	class test;
	
	class info
	{
	public:
		typedef std::function<std::unique_ptr<test>()> factory_func;
		info(const factory_func& f_, const char* name_, const char* category_, const char* file_, const int line_)
			: f(f_)
			, name(name_)
			, category(category_)
			, file(file_)
			, line(line_)
		{}

		factory_func f;
		const char* name;
		const char* category;
		const char* file;
		const int line;
	};

	class default_fail_handler
	{
	public:
		static void handle(const std::string& message, const char* file_name = "", const int line_num = 0)
		{
			throw assert_fail_exception(message, file_name, line_num);
		}
	};

	template<class fail_handler>
	class basic_assert
	{
	public:
		template<typename T1, typename T2>
		static void eq(const T1& expected, const T2& actual, const char* file_name = "", const int line_num = 0)
		{
			if (expected == actual)
			{
				return;
			}
			std::ostringstream err;
			err << "Expected [" << expected << "] saw [" << actual << "]";
			fail(err.str(), file_name, line_num);
		}

		template<typename T1, typename T2>
		static void neq(const T1& expected, const T2& actual, const char* file_name = "", const int line_num = 0)
		{
			if (expected != actual)
			{
				return;
			}
			std::ostringstream err;
			err << "Expected not [" << expected << "] saw [" << actual << "]";
			fail(err.str(), file_name, line_num);
		}

		static void expr(const bool ex, const char* file_name = "", const int line_num = 0)
		{
			if (ex)
			{
				return;
			}
			fail("Assert expression failed", file_name, line_num);
		}

		static void is_true(const bool condition, const char* file_name = "", const int line_num = 0)
		{
			if (condition)
			{
				return;
			}
			fail("Expected [true] saw [false]", file_name, line_num);
		}

		static void is_false(const bool condition, const char* file_name = "", const int line_num = 0)
		{
			if (!condition)
			{
				return;
			}
			fail("Expected [false] saw [true]", file_name, line_num);
		}

		template<typename T>
		static void is_null(const T* ptr, const char* file_name = "", const int line_num = 0)
		{
			if (ptr == nullptr)
			{
				return;
			}
			fail("Expected [nullptr]", file_name, line_num);
		}

		template<typename T>
		static void is_not_null(const T* ptr, const char* file_name = "", const int line_num = 0)
		{
			if (ptr != nullptr)
			{
				return;
			}
			fail("Expected not [nullptr]", file_name, line_num);
		}

		static void fail(const std::string& message, const char* file_name = "", const int line_num = 0)
		{
			fail_handler::handle(message, file_name, line_num);
		}
	};

	typedef basic_assert<default_fail_handler> assert;

	class test
	{
	public:
		virtual ~test() {}

		status execute(result& res)
		{
			auto test_start_time = std::chrono::steady_clock::now();
			try
			{
				pre_test();
				execute_test();
				res.status = status::pass;
			}
			catch (const assert_fail_exception& ex)
			{
				res.fail(ex.what_str(), ex.file_name(), ex.line_num());
			}
			catch (const std::exception& ex)
			{
				res.exception(ex);
			}
			post_test();			
			auto test_end_time = std::chrono::steady_clock::now();
			auto test_duration = std::chrono::duration_cast<std::chrono::milliseconds>(test_end_time - test_start_time);
			res.duration = test_duration;
			return res.status;
		}

	protected:
		test(){}
		virtual void pre_test() {}
		virtual void post_test() {}
	private:
		virtual void execute_test() = 0;
	};	

	class registry
	{
	public:
		typedef std::vector<const info*> container_type;

		virtual ~registry();

		static registry& get();
		void add(const info* test)
		{
			_tests.push_back(test);
		}
		container_type& tests() { return _tests; }

	protected:
		registry();		
	private:
		container_type _tests;
		static std::unique_ptr<registry> _instance;
	};

	class auto_registered_test
	{
	public:
		explicit auto_registered_test(const info* info) : _info(info)
		{
			registry::get().add(info);
		}
		const info* _info;
	};

	class runner
	{
	public:
		typedef const info* const info_type;

		static status run(const info* const ti, result& out_res)
		{
			auto tst = ti->f();
			out_res.info = ti;
			return tst->execute(out_res);
		}

		template<typename iterator_type, class execution_observer>
		static status run(iterator_type itr_begin, iterator_type itr_end,
			const execution_observer& observer)
		{
			return run(itr_begin, itr_end, [](info_type) { return true; }, observer);
		}

		template<typename iterator_type, class binary_predicate, class execution_observer>
		static status run(iterator_type itr_begin, iterator_type itr_end, const binary_predicate& filter,
			const execution_observer& observer)
		{
			size_t pass(0), test_count(0);
			for (auto itr = itr_begin; itr != itr_end; ++itr)
			{
				const auto* const ti = *itr;
				if (filter(ti))
				{
					result res;
					auto r = run(ti, res);
					if (r == status::pass)
					{
						++pass;
					}
					++test_count;
					observer(res);
				}
			}
			return pass == test_count ? status::pass : status::fail;
		}

		template<typename container_type, class execution_observer>
		static status run(const container_type& tests, const execution_observer& observer)
		{
			return run(tests.begin(), tests.end(), observer);
		}

		template<typename container_type, class binary_predicate, class execution_observer>
		static status run(const container_type& tests, const binary_predicate& filter, const execution_observer& observer)
		{
			return run(tests.begin(), tests.end(), filter, observer);
		}

		template<class execution_observer>
		static status run_registered(const execution_observer& observer)
		{
			return run(registry::get().tests(), [](info_type) { return true; }, observer);
		}

		template<class binary_predicate, class execution_observer>
		static status run_registered(const binary_predicate& filter, const execution_observer& observer)
		{
			return run(registry::get().tests(), filter, observer);
		}
	};

#ifdef UTEST_CPP_IMPLEMENTATION

	std::unique_ptr<registry> registry::_instance;

	namespace detail
	{
		class _concrete_registry : public registry {};
	}

	registry& registry::get()
	{
		if(!_instance)
		{
			_instance = std::make_unique<detail::_concrete_registry>();
		}
		return *_instance;
	}

	registry::~registry()
	{		
	}

	registry::registry()
		: _tests()
	{
	}

#endif	// UTEST_CPP_IMPLEMENTATION

}

