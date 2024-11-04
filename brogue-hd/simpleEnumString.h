#pragma once

//  EnumString - A utility to provide stringizing support for C++ enums
//  Author: Francis Xavier Joseph Pulikotil
//
//  This code is free software: you can do whatever you want with it,
//	although I would appreciate if you gave credit where it's due.
//
//  This code is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef ENUMSTRING_HEADER
#define ENUMSTRING_HEADER

/* Usage example

    // WeekEnd enumeration
    enum WeekEnd
    {
        Sunday = 1,
        Saturday = 7
    };

    // String support for WeekEnd
    Begin_Enum_String( WeekEnd )
    {
        Enum_String( Sunday );
        Enum_String( Saturday );
    }
    End_Enum_String;

    // Convert from WeekEnd to string
    const std::string &str = EnumString<WeekEnd>::From( Saturday );
    // str should now be "Saturday"

    // Convert from string to WeekEnd
    WeekEnd w;
    EnumString<WeekEnd>::To( w, "Sunday" );
    // w should now be Sunday
*/

#include <map>
#include <string>
#include <memory>
// Helper macros

#define Begin_Enum_String(EnumerationName)                                      \
    template <> struct EnumString<EnumerationName> :                            \
        public EnumStringBase< EnumString<EnumerationName>, EnumerationName >   \
    {                                                                           \
        static void RegisterEnumerators()
//      {

#define Enum_String(EnumValue, EnumDescr)                                       \
            RegisterEnumerator( EnumValue, #EnumValue, EnumDescr );
//      }

#define End_Enum_String                                                         \
    }

template<class EnumType>
struct enumData
{
    EnumType enumValue;
    std::string enumValueName;
    std::string enumValueDescr;

    enumData(const EnumType& aenumValue, const std::string& aenumValueName, const std::string& aenumValueDescr)
    {
        enumValue = aenumValue;
        enumValueName = aenumValueName;
        enumValueDescr = aenumValueDescr;
    }
};

// The EnumString base class
template <class DerivedType, class EnumType>
class EnumStringBase
{
    // Types
protected:
    typedef std::map<std::string, enumData<EnumType>> AssocMap;

protected:
    // Constructor / Destructor
    explicit EnumStringBase();
    ~EnumStringBase();

private:
    // Copy Constructor / Assignment Operator
    EnumStringBase(const EnumStringBase&);
    const EnumStringBase& operator =(const EnumStringBase&);

    // Functions
private:
    static AssocMap& GetMap();

protected:
    // Use this helper function to register each enumerator
    // and its string representation.
    static void RegisterEnumerator(const EnumType e, const std::string& eStr, const std::string& eDescr);

public:
    // Converts from an enumerator to a string.
    // Returns an empty string if the enumerator was not registered.
    static const std::string& From(EnumType&& e);

    static const std::string& From(const EnumType& e);

    // Converts from a string to an enumerator.
    // Returns true if the conversion is successful; false otherwise.
    static const bool To(EnumType& e, const std::string& str);
};

// The EnumString class
// Note: Specialize this class for each enumeration, and implement
//       the RegisterEnumerators() function.
template <class EnumType>
struct EnumString : public EnumStringBase< EnumString<EnumType>, EnumType >
{
    static void RegisterEnumerators();
};

// Function definitions

template <class D, class E>
typename EnumStringBase<D, E>::AssocMap& EnumStringBase<D, E>::GetMap()
{
    // A static map of associations from strings to enumerators
    static AssocMap assocMap;
    static bool     bFirstAccess = true;

    // If this is the first time we're accessing the map, then populate it.
    if (bFirstAccess)
    {
        bFirstAccess = false;
        D::RegisterEnumerators();
    }

    return assocMap;
}

template <class D, class E>
void EnumStringBase<D, E>::RegisterEnumerator(const E e, const std::string& eStr, const std::string& eDescr)
{
    const bool bRegistered = GetMap().insert(typename AssocMap::value_type(eStr, enumData(e, eStr, eDescr))).second;
}

template <class D, class E>
const std::string& EnumStringBase<D, E>::From(E&& e)
{
    return From(e).enumValueName;
}

template <class D, class E>
const std::string& EnumStringBase<D, E>::From(const E& e)
{
    for (;;) // Code block
    {
        // Search for the enumerator in our map
        typename AssocMap::const_iterator i;
        for (i = GetMap().begin(); i != GetMap().end(); ++i)
            if ((*i).second.enumValue == e)
                break;

        // If we didn't find it, we can't do this conversion
        if (i == GetMap().end())
            break;

        // Keep searching and see if we find another one with the same value
        typename AssocMap::const_iterator j(i);
        for (++j; j != GetMap().end(); ++j)
            if ((*j).second.enumValue == e)
                break;

        // If we found another one with the same value, we can't do this conversion
        if (j != GetMap().end())
            break;

        // We found exactly one string which matches the required enumerator
        return (*i).first;
    }

    // We couldn't do this conversion; return an empty string.
    static const std::string dummy;
    return dummy;
}

template <class D, class E>
const bool EnumStringBase<D, E>::To(E& e, const std::string& str)
{
    // Search for the string in our map.
    const typename AssocMap::const_iterator itr(GetMap().find(str));

    // If we have it, then return the associated enumerator.
    if (itr != GetMap().end())
    {
        e = (*itr).second.enumValue;
        return true;
    }

    // We don't have it; the conversion failed.
    return false;
}

#endif
