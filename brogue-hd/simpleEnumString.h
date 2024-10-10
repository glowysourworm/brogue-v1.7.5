
//  EnumString - A utility to provide stringizing support for C++ enums
//  Author: Francis Xavier Joseph Pulikotil
//
//  This code is free software: you can do whatever you want with it,
//	although I would appreciate if you gave credit where it's due.
//
//  This code is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

// Added Namespace:  BrogueHd for vendor code (TODO!!)
//
//namespace brogueHd::vendor
//{ 

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

#include "simpleString.h"
#include "simpleHash.h"
#include <cassert>

using namespace brogueHd::simple;

// Helper macros

#define Begin_Enum_String(EnumType)                                             \
    template<> struct EnumString<EnumType> : simpleEnumString<EnumType>         \
    {                                                                           \
        static void Register()                                                  \
                                                                               
#define Enum_String(EnumType)                                   RegisterEnumerator( EnumType, #EnumType, "No Description");
#define Enum_String_With_Description(EnumType, EnumDescription) RegisterEnumerator( EnumType, #EnumType, EnumDescription );
                                                                               
#define End_Enum_String                                                         \
    }

// The EnumString base class
template <class EnumType>
struct simpleEnumString
{
public:

    simpleEnumString()
    {
        TypeLookup = new simpleHash<EnumType, simpleString>();
        ReverseLookup = new simpleHash<EnumType, simpleString>();
        DescriptionLookup = new simpleHash<EnumType, simpleString>();

        Register();
    }
    ~simpleEnumString()
    {
        delete TypeLookup;
        delete ReverseLookup;
        delete DescriptionLookup;
    }


    // Use this helper function to register each enumerator
    // and its string representation.
    static void RegisterEnumerator(const EnumType theEnum, const simpleString& theEnumString, const simpleString& theEnumDescription)
    {
        if (!TypeLookup->contains(theEnum))
            TypeLookup->add(theEnum, theEnumString);

        if (!ReverseLookup->contains(theEnumString))
            ReverseLookup->add(theEnumString, theEnum);

        if (!DescriptionLookup->contains(theEnum))
            DescriptionLookup->add(theEnum, theEnumDescription);
    }

    // Converts from an enumerator to a string.
    // Returns an empty string if the enumerator was not registered.
    static const simpleString& getName(const EnumType theEnum)
    {
        return TypeLookup->get(theEnum);
    }

    // Converts from a string to an enumerator.
    // Returns true if the conversion is successful; false otherwise.
    static const bool parse(EnumType& theEnum, const simpleString& str)
    {
        if (ReverseLookup->contains(str))
        {
            theEnum = ReverseLookup->get(str);
            return true;
        }

        return false;
    }


    // Returns the description for the enum type
    static const simpleString& getDescription(const EnumType theEnum)
    {
        return DescriptionLookup->get(theEnum);
    }

protected:

    simpleHash<EnumType, simpleString>* TypeLookup;
    simpleHash<simpleString, EnumType>* ReverseLookup;
    simpleHash<EnumType, simpleString>* DescriptionLookup;
};

template <typename EnumTypeA>
struct EnumString : simpleEnumString<EnumTypeA>
{
    static void Register()
    {

    }
};

#endif