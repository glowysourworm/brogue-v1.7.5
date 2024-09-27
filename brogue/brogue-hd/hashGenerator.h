#pragma once

#include <functional>

using namespace std;

namespace brogueHd::backend::generator
{
    /// <summary>
    /// Concept that protects type convertability for hash generator
    /// </summary>
    template<typename T>
    concept isHashable = requires(T a)
    {
        { a } -> std::convertible_to<short>;
        { a } -> std::convertible_to<int>;
        { a } -> std::convertible_to<long>;
        { a } -> std::convertible_to<char>;
        { a } -> std::convertible_to<bool>;
    };

    class hashGenerator
    {
    public:

        hashGenerator(){};
        ~hashGenerator(){};

    private:

        const long LARGE_PRIME = 5999471;

        template<isHashable T>
        static long createHashImpl(T data[], long previousHash)
        {
            // Start New / Continue
            unsigned long hash = previousHash == 0 ? LARGE_PRIME : previousHash;
            bool found = false;

            for (int index = 0; index < SIZEOF(data); index++)
            {
                hash = (hash * LARGE_PRIME) ^ data[index];
                found = true;
            }

            // Make sure to indicate that there are no data to hash
            //
            if (found)
                return hash;

            else
                return 0;
        }

    public:

        template<isHashable T>
        static long createHash(T data[])
        {
            return createHashImpl(data, 0);
        }

        template<isHashable T>
        static long continueHash(T data[], const unsigned long previousHash)
        {
            return createHashImpl(data, previousHash);
        }
    };
}