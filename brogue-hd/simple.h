#pragma once

#include <concepts>

namespace brogueHd::simple
{
    template<typename T>
    struct default_value
    {
        inline static const T value = T();
        inline static T ctor()
        {
            return T();
        }
    };

    /*

        Hash Explanation:  std::hash provides standard type handling; and can be
                           specialized to work with your classes and structs.

                           http://www.man6.org/docs/cppreference-doc/reference/en.cppreference.com/w/cpp/utility/hash.html

                           Using the macro, you can add the specialization after your
                           class is defined in your .h file.

                           MAKE_HASHABLE({type}, field1, field2, ...)

    */

    class hashGenerator
    {
    private:
        template<typename T, typename ...Rest>
        static void hash_combine(size_t& seed, const T& next, Rest... rest)
        {
            size_t nextHash = std::hash<T>{}(next);

            seed = seed ^ (nextHash << 1);

            (hash_combine(seed, rest), ...);
        }

    public:

        template<typename ...T>
        static size_t generateHash(const T&...values)
        {
            // Large Prime
            size_t seed = 5999471;
            (hash_combine(seed, values), ...);
            return seed;
        }

        template<typename ...T>
        static size_t combineHash(const size_t& hashValue, const T&...values)
        {
            size_t result = hashValue;
            (hash_combine(result, values), ...);
            return result;
        }
    };

    /// <summary>
    /// Class to extend std::hash functionality. Inherit and extend getHash().
    /// </summary>
    class hashableObject
    {
    public:
        virtual bool operator==(const hashableObject& other) const
        {
            return false;
        }
        virtual bool operator!=(const hashableObject& other) const
        {
            return false;
        }
        virtual size_t getHash() const
        {
            return 0;
        }
    };

    /// <summary>
    /// Struct to extend std::hash functionality. Inherit and extend getHash().
    /// </summary>
    struct hashable
    {
    public:
        virtual bool operator==(const hashable& other) const
        {
            return false;
        }
        virtual bool operator!=(const hashable& other) const
        {
            return false;
        }
        virtual size_t getHash() const
        {
            return 0;
        }
    };

    // Hashable Declarations:  These will extend the std namespace to
    //                         create hash code support for std::hash.
    //                         You can either use unordered_map, or 
    //                         your own std::hash container class.
    // 
    //                         Add these to the GLOBAL NAMESPACE outside
    //                         your class or struct definition (just below it).
    //
    // MAKE_HASHABLE_CLASS(brogueHd::backend::model::brogueObject)
    // MAKE_HASHABLE_STRUCT(brogueHd::backend::model::game::color)


    /// <summary>
    /// Instructs the iterator to either continue or break out of its loop
    /// </summary>
    enum iterationCallback
    {
        iterate = 0,
        breakAndReturn = 1
    };

    // Constraint that the type T must inherit from gridOperator<T>
    template<typename T>
    concept isComparable = requires(T a, T b)
    {
        { a <= b } -> std::convertible_to<bool>;
        { a < b }  -> std::convertible_to<bool>;
        { a > b }  -> std::convertible_to<bool>;
        { a >= b } -> std::convertible_to<bool>;
        { a == b } -> std::convertible_to<bool>;
    };

    template<typename T>
    concept isNumber =
        std::same_as<T, double> ||
        std::same_as<T, float> ||
        std::same_as<T, unsigned long> ||
        std::same_as<T, long> ||
        std::same_as<T, int> ||
        std::same_as<T, short> ||
        std::same_as<T, size_t>;

    class simpleString;

    template<typename T>
    concept isStringLike =
        std::same_as<T, simpleString> ||
        std::same_as<T, char> ||
        std::same_as<T, const char*> ||
        std::same_as<T, char[]> ||
        std::same_as<T, const char[]>;

    template<typename T>
    concept isStringConvertible = isNumber<T> || isStringLike<T>;


}

/*

    Global Macros:  std::hash extension, enum string (simpleEnumString.h)

*/

#define COMMA   ,

#define MAKE_HASHABLE_CLASS(hashableClass)                          \
    namespace std                                                   \
    {                                                               \
        template<> struct hash<hashableClass>                       \
        {                                                           \
            size_t operator()(const hashableClass &theObject) const \
            {                                                       \
                return theObject.getHash();                         \
            }                                                       \
        };                                                          \
    }                                                               \

#define MAKE_HASHABLE_STRUCT(type)                                  \
    namespace std                                                   \
    {                                                               \
        template<> struct hash<type>                                \
        {                                                           \
            size_t operator()(const type &theObject) const          \
            {                                                       \
                return theObject.getHash();                         \
            }                                                       \
        };                                                          \
    }                                                               \