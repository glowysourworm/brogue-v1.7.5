#pragma once

#include <array>
#include <concepts>
#include <type_traits>

namespace brogueHd::simple
{
    /*

        Hash Explanation:  std::hash provides standard type handling; and can be
                           specialized to work with your classes and structs.

                           http://www.man6.org/docs/cppreference-doc/reference/en.cppreference.com/w/cpp/utility/hash.html

                           Using the macro, you can add the specialization after your
                           class is defined in your .h file.

                           MAKE_HASHABLE({type}, field1, field2, ...)

    */

    // More hashable issues:  Check for std::hash support:
    //
    // These were copied in. Essentially, these are playing with the
    // way to know if std::hash was successful. If there are cleaner ways
    // then they'll be tested later when more familiar with things...
    //

    template <typename T, typename = std::void_t<>>
    struct is_std_hashable : std::false_type { };

    template <typename T>
    struct is_std_hashable<T, std::void_t<decltype(std::declval<std::hash<T>>()(std::declval<T>()))>> : std::true_type { };

    template <typename T>
    constexpr bool is_std_hashable_v = is_std_hashable<T>::value;

    // ------

    /// <summary>
    /// Class to extend std::hash functionality. Inherit and extend getHash().
    /// </summary>
    class hashableObject
    {
    public:
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
        virtual size_t getHash() const
        {
            return 0;
        }
    };

    /*
        Concepts
    */

    class simpleString;

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
    concept isIntLike = std::same_as<T, unsigned long> ||
                        std::same_as<T, long> ||
                        std::same_as<T, int> ||
                        std::same_as<T, unsigned int> ||
                        std::same_as<T, short> ||
                        std::same_as<T, unsigned short> ||
                        std::same_as<T, size_t>;

    template<typename T>
    concept isFloatLike = std::same_as<T, double> ||
                          std::same_as<T, float>;

    template<typename T>
    concept isNumber = (isIntLike<T> || isFloatLike<T>);

    template<typename T>
    concept isChar = std::same_as<T, char>;

    //template<typename T>
    //concept isEnum = std::is_enum<T>{}();
    //
    //template<typename T>
    //concept isBool = std::same_as<T, bool>;

    template<typename T>
    concept isPointer = std::is_pointer<T>::value;

    template<typename T>
    concept isStruct = !std::is_pointer<T>::value;

    // Single char, bounded array char[], array pointer char*
    template<typename T>
    concept isStringLike = isChar<T> ||
                            std::same_as<T, char[]> ||
                            std::same_as<T, const char[]> ||
                            std::same_as<T, char*> ||
                            std::same_as<T, const char*> ||
                            std::convertible_to<T, char*> ||
                            std::convertible_to<T, const char*>;

    template<typename T>
    concept isStringConvertible = isNumber<T> || isStringLike<T>;

    template<typename T>
    concept isStdHashable = is_std_hashable<T>::value;

    template<typename T>
    concept isHashable = std::convertible_to<T, hashable> ||
                         std::convertible_to<T, hashableObject> ||
                         std::same_as<T, simpleString> ||
                         isStdHashable<T>;

    class hashGenerator
    {
    private:

        template<isHashable...Rest>
        static void hash_combine(size_t& seed, const hashable& next, Rest... rest)
        {
            // isHashable<T>
            size_t nextHash = next.getHash();

            seed = seed ^ (nextHash << 1);

            (hash_combine(seed, rest), ...);
        }

        template<isHashable...Rest>
        static void hash_combine(size_t& seed, const hashableObject& next, Rest... rest)
        {
            // isHashable<T>
            size_t nextHash = next.getHash();

            seed = seed ^ (nextHash << 1);

            (hash_combine(seed, rest), ...);
        }

        template<isStdHashable T, isHashable...Rest>
        static void hash_combine(size_t& seed, const T& next, Rest...rest)
        {
            // isChar<T>
            size_t nextHash = std::hash<T>{}(next);

            seed = seed ^ (nextHash << 1);

            (hash_combine(seed, rest), ...);
        }

    public: // These methods will look for the above overloads, variadically... ... ...

        template<isHashable ...T>
        static size_t generateHash(const T&...values)
        {
            // Large Prime
            size_t seed = 5999471;
            (hash_combine(seed, values), ...);
            return seed;
        }

        template<isHashable ...T>
        static size_t combineHash(const size_t& hashValue, const T&...values)
        {
            size_t result = hashValue;
            (hash_combine(result, values), ...);
            return result;
        }
    };

    struct default_value
    {
        template<isPointer T>
        inline static const T value()
        {
            return nullptr;
        }

        template<isStruct T>
        inline static const T value()
        {
            return T();
        }
    };


    /// <summary>
    /// Instructs the iterator to either continue or break out of its loop
    /// </summary>
    enum iterationCallback
    {
        iterate = 0,
        breakAndReturn = 1
    };
}