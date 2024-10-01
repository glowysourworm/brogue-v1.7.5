#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

namespace brogueHd
{
	// STD Library / substitutions
	#define SIZEOF(x)						sizeof(x)			// Size of an array
	#define STRCMP(x, y)					strcmp(x,y)			// String compare
	#define STRCPY(dest, src, count)		strncpy_s(dest, src, count)

    // Create default values for types
    template<typename T> struct default_value
    {
        inline static const T value = T();
        inline static T ctor()
        {
            return T();
        }

        /// <summary>
        /// Calls default constructor for new value of type T
        /// </summary>
        //inline static T ctor()
        //{
        //    return T();
        //}
    };

    //#define MAKE_DEFAULT_VALUE(type)							\
    //                                                            \
    //template<> struct default_value<type>						\
    //{                                                           \
    //    constexpr type value = type();					        \
    //};                                                          
}