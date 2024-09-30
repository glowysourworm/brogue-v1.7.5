#pragma once

/*
    
    Hash Explanation:  std::hash provides standard type handling; and can be
                       specialized to work with your classes and structs.

                       http://www.man6.org/docs/cppreference-doc/reference/en.cppreference.com/w/cpp/utility/hash.html

                       Using the macro, you can add the specialization after your
                       class is defined in your .h file.

                       MAKE_HASHABLE({type}, field1, field2, ...)

*/

using namespace std;


class hashGenerator
{
private:
    template<typename T, typename ...Rest>
    static void hash_combine(size_t& seed, const T& next, Rest... rest)
    {
        seed = seed ^ (next << 1);

        (hash_combine(seed, rest), ...);
    }

public:

    static size_t generateHash(const size_t& values...)
    {
        // Large Prime
        size_t seed = 5999471;
        hash_combine(seed, values);
        return seed;
    }
};

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
    virtual size_t getHash() const
    {
        return 0;
    }
};

#define MAKE_HASHABLE_CLASS(hashableClass, ...)                     \
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

#define MAKE_HASHABLE_STRUCT(type, ...)                             \
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