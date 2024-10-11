#pragma once

/* Usage example:  Use helper macros to declare the static struct EnumString<T>

                   The template specialization will declare two static functions:
                   GetName, and GetDescr. Use the macro functions to add data about
                   the enums to the global namespace.

                   To retrieve:     EnumString<TEnum>::GetName(...)
                                    EnumString<TEnum>::GetDescr(...)

Register_Enum(gameColors)

    Declare_Enum_Descr(gameColors)
        Enum_Descr(gameColors, rouge, "Rouge is red");
    End_Enum_Descr;

    Declare_Enum_Name(gameColors)
        Enum_Name(gameColors, thing)
        Enum_Name(gameColors, white)
        Enum_Name(gameColors, rouge)
    End_Enum_Name;

End_Enum

*/

#include "simple.h"

using namespace brogueHd::simple;

template<typename EnumType>
struct EnumString
{
    static const char* GetDescr(const EnumType& enumValue)
    {
        return nullptr;
    }
    static const char* GetName(const EnumType& enumValue)
    {
        return nullptr;
    }
};

// Helper macros
#define Register_Enum(EnumType)                                                         \
    template<>                                                                          \
    struct EnumString<EnumType>                                                         \
    {                                                                                   
#define Declare_Enum_Descr(EnumType)                                                    \
        static const char* GetDescr(const EnumType& enumValue)                          \
        {                                                                               \
            switch(enumValue)                                                           \
            {                                                                           \
                default: return nullptr;                                                
#define Enum_Descr(EnumType, EnumValue, EnumDescr)                                      \
                case EnumType::EnumValue:                                               \
                    return ##EnumDescr;
#define End_Enum_Descr } }

#define Declare_Enum_Name(EnumType)                                                     \
        static const char* GetName(const EnumType& enumValue)                           \
        {                                                                               \
            switch(enumValue)                                                           \
            {                                                                           \
                default: return nullptr;                                                
#define Enum_Name(EnumType, EnumValue)                                                  \
                case EnumType::EnumValue:                                               \
                    return #EnumValue;  
#define End_Enum_Name } }
#define End_Enum };