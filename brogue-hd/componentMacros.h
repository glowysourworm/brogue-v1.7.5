#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

namespace brogueHd
{
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