#pragma once

#ifndef RESUMEF_ENABLE_CONCEPT
#define RESUMEF_ENABLE_CONCEPT	0
#endif

#if RESUMEF_ENABLE_CONCEPT
#include <concepts>
#endif

RESUMEF_NS
{

#if RESUMEF_ENABLE_CONCEPT

	template<typename T>
	concept _AwaitorT = requires(T && v)
	{
		{ v.await_ready() } -> bool;
		{ v.await_suspend(std::declval<std::experimental::coroutine_handle<promise_t<>>>()) };
		{ v.await_resume() };
	};

	template<typename T> 
	concept _HasStateT = requires(T && v)
	{
		{ v._state };
		{ traits::is_state_pointer_v<decltype(v._state)> != false };
	};

	template<typename T>
	concept _FutureT = _AwaitorT<T> && _HasStateT<T> && requires
	{
		{ T::value_type };
		{ T::state_type };
		{ T::promise_type };
	};

	template<typename T>
	concept _CallableT = std::invocable<T>;

	template<typename T>
	concept _GeneratorT = std::is_same_v<T, generator_t<_Ty>>;

	template<typename T>
	concept _WhenTaskT = _AwaitorT<T> || _CallableT<T>;

	template<typename T>
	concept _IteratorT = requires(T && u, T && v)
	{
		{ ++u }->T;
		{ u != v } -> bool;
		{ *u };
	};

	template<typename T>
	concept _WhenIterT = _IteratorT<T> && requires(T&& u, T&& v)
	{
		requires _WhenTaskT<decltype(*u)>;
	};

	template<typename T>
	concept _ContainerT = requires(T && v)
	{
		{ std::begin(v) };
		{ std::end(v) };
		requires std::same_as<decltype(std::begin(v)), decltype(std::end(v))>;
		requires _IteratorT<decltype(std::begin(v))>;
		requires _IteratorT<decltype(std::end(v))>;
	};

#define COMMA_RESUMEF_ENABLE_IF(...) 
#define RESUMEF_ENABLE_IF(...) 
#define RESUMEF_REQUIRES(...) requires __VA_ARGS__

#else

#define _AwaitorT typename
#define _HasStateT typename
#define _FutureT typename
#define _CallableT typename
#define _GeneratorT typename
#define _WhenTaskT typename
#define _IteratorT typename
#define _WhenIterT typename
#define _ContainerT typename

#define COMMA_RESUMEF_ENABLE_IF(...) ,typename=std::enable_if_t<__VA_ARGS__>
#define RESUMEF_ENABLE_IF(...) typename=std::enable_if_t<__VA_ARGS__>
#define RESUMEF_REQUIRES(...) 

#endif

}
