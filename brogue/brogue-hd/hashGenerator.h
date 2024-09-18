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

    static class hashGenerator
    {

    private:

        const unsigned long LARGE_PRIME = 5999471;

        template<isHashable T>
        static unsigned long createHashImpl(T data[], unsigned long previousHash)
        {
            // Start New / Continue
            unsigned long hash = previousHash ?? LARGE_PRIME;
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
        static unsigned long createHash(T data[])
        {
            return createHashImpl(data, 0);
        }

        template<isHashable T>
        static unsigned long continueHash(T data[], const unsigned long previousHash)
        {
            return createHashImpl(data, previousHash);
        }
    };
}