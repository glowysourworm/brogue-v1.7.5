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

                           This wasn't used; but it was tried. It's easier to use
                           a base class/struct definition with concept constraints.

                           The overload for getHash has template support so that
                           the compiler can hunt around for a suitable overload. The
                           std::hash methods can be used for primitive types. (see below...)

    */

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
                        std::same_as<T, long long> ||
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
    concept isBool = std::same_as<T, bool>;

    template<typename T>
    concept isChar = std::same_as<T, char>;

    template<typename T>
    concept isEnum = std::is_enum<T>{}();

    template<typename T>
    concept isPointer = std::is_pointer<T>::value;

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

    // Forward declaration of simple string (concept isHashable wasn't compiling properly with IDE)
    class simpleString;

    template<typename T>
    concept isHashable = std::same_as<T, simpleString> ||
                         std::convertible_to<T, hashable> ||
                         std::convertible_to<T, hashableObject> ||
                         isNumber<T> ||
                         isPointer<T> ||
                         isEnum<T> ||
                         isChar<T> ||
                         isBool<T>;

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

        template<isNumber T, isHashable...Rest>
        static void hash_combine(size_t& seed, const T& next, Rest... rest)
        {
            // isNumber<T>
            size_t nextHash = std::hash<T>{}(next);

            seed = seed ^ (nextHash << 1);

            (hash_combine(seed, rest), ...);
        }

        template<isChar T, isHashable...Rest>
        static void hash_combine(size_t& seed, const T& next, Rest...rest)
        {
            // isChar<T>
            size_t nextHash = std::hash<T>{}(next);

            seed = seed ^ (nextHash << 1);

            (hash_combine(seed, rest), ...);
        }

        template<isBool T, isHashable...Rest>
        static void hash_combine(size_t& seed, const T& next, Rest...rest)
        {
            // isBool<T>
            size_t nextHash = std::hash<T>{}(next);

            seed = seed ^ (nextHash << 1);

            (hash_combine(seed, rest), ...);
        }

        template<isEnum T, isHashable...Rest>
        static void hash_combine(size_t& seed, const T& next, Rest...rest)
        {
            // isEnum<T>
            size_t nextHash = std::hash<T>{}(next);

            seed = seed ^ (nextHash << 1);

            (hash_combine(seed, rest), ...);
        }

        template<isPointer T, isHashable...Rest>
        static void hash_combine(size_t& seed, const T& next, Rest...rest)
        {
            // isPointer<T>
            std::uintptr_t ptrValue = reinterpret_cast<std::uintptr_t>(next);

            size_t nextHash = std::hash<uintptr_t>{}(ptrValue);

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


    /// <summary>
    /// Instructs the iterator to either continue or break out of its loop
    /// </summary>
    enum iterationCallback
    {
        iterate = 0,
        breakAndReturn = 1
    };
}