/************************************************************

GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
Copyright (c) 2017, KondeU, All rights reserved.

Project:     AOP
File:        AOP.hpp
Description: Implement C++ Aspect-Oriented Programming
Date:        2017-12-28
Version:     1.0
Authors:     Deyou Kong <370242479@qq.com>
History:     01, 17-12-28, Deyou Kong, Create file and implement it.

************************************************************/

#pragma once

namespace aop
{
	class NonCopyable
	{
	public:
		NonCopyable(const NonCopyable &) = delete;                // Deleted
		NonCopyable & operator = (const NonCopyable &) = delete;  // Deleted
		NonCopyable() = default;                                  // Available
	};

    #define HAS_MEMBER(member)                                                             \
                                                                                           \
    template<typename T, typename... Args> struct has_member_##member {	                   \
                                                                                           \
    private:                                                                               \
                                                                                           \
    	template<typename U> static auto Check(int)                                        \
    	-> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());  \
                                                                                           \
    	template<typename U> static std::false_type Check(...);                            \
                                                                                           \
    public:                                                                                \
                                                                                           \
        enum { value = std::is_same<decltype(Check<T>(0)), std::true_type>::value };       }

	HAS_MEMBER(Before); // Add Before aspect
	HAS_MEMBER(After);  // Add After aspect

	template<typename Ret, typename Func, typename... Args>
	struct Aspect : NonCopyable
	{
		Aspect(Func && f) : m_func(std::forward<Func>(f)) { }

		template<typename T>
		typename std::enable_if<
			has_member_Before<T, Args...>::value&&
			has_member_After<T, Args...>::value>::type
			Invoke(Args &&... args, T && aspect)
		{
			aspect.Before(std::forward<Args>(args)...);  // Run codes before core codes
			m_ret = m_func(std::forward<Args>(args)...); // Run core codes
			aspect.After(std::forward<Args>(args)...);   // Run codes after core codes
		}

		template<typename T>
		typename std::enable_if<
			has_member_Before<T, Args...>::value&&
			!has_member_After<T, Args...>::value>::type
			Invoke(Args &&... args, T && aspect)
		{
			aspect.Before(std::forward<Args>(args)...);  // Run codes before core codes
			m_ret = m_func(std::forward<Args>(args)...); // Run core codes
		}

		template<typename T>
		typename std::enable_if<
			!has_member_Before<T, Args...>::value&&
			has_member_After<T, Args...>::value>::type
			Invoke(Args &&... args, T && aspect)
		{
			m_ret = m_func(std::forward<Args>(args)...); // Run core codes
			aspect.After(std::forward<Args>(args)...);   // Run codes after core codes
		}

		template<typename Head, typename... Tail>
		void Invoke(Args &&... args, Head && headAspect, Tail &&... tailAspect)
		{
			headAspect.Before(std::forward<Args>(args)...);
			Invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
			headAspect.After(std::forward<Args>(args)...);
		}

		Ret GetReturn() { return m_ret; }

	private:
		Func m_func;  // Function that be invoked
		Ret  m_ret;   // Core codes return value
	};
	template<typename T> using identity_t = T;

	// AOP function, for export
	template<typename Ret, typename... AP, typename... Args, typename Func>
	Ret AOP(Func && f, Args &&... args)
	{
		Aspect<Ret, Func, Args...> asp(std::forward<Func>(f));
		asp.Invoke(std::forward<Args>(args)..., identity_t<AP>()...);
		return asp.GetReturn();
	}
}

using aop::AOP;

/* --- AOP user manual ---

  If you need to add aspects to a function and then call this function,
  you can call the function as follows:

    [func_return_value] AOP<[func_return_type], [aspect_struct...]>([func], [func_params...]);

  [func_return_value] : the original function return value
  [func_return_type]  : the original function return type, must not be "void" type
  [aspect_struct...]  : aspects list that will be added to original function
  [func]              : the original function pointer
  [func_params...]    : the original function params list

  Aspect struct needs to be coded as follows:

  struct [aspect_struct_name]
  {
      void Before(...)
      {
          // TODO
      }
      
      void After(...)
      {
          // TODO
      }
      
      [vars]
      
      private:
      
      [private_vars]
  };

------------------------*/