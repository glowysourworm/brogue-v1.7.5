
#include "EnumString.h"
#include <cassert>

// Furious Five Master
enum Master
{
    Tigress = 5,
    Viper = 3,
    Monkey = 4,
    Mantis = 1,
    Crane = 2
};

// String support for Furious Five Master
Begin_Enum_String( Master )
{
    Enum_String( Tigress );
    Enum_String( Viper );
    Enum_String( Monkey );
    Enum_String( Mantis );
    Enum_String( Crane );
}
End_Enum_String;


int main(int /*argc*/, char * /*argv*/[])
{
	// Convert from a Master enumerator to a string
    const std::string &masterStr = EnumString<Master>::From( Monkey );
    assert( masterStr.compare( "Monkey" ) == 0 );

	// Convert from a string to a Master enumerator
    Master master = Tigress;
    const bool bResult = EnumString<Master>::To( master, masterStr );
    assert( bResult == true );
    assert( master == Monkey );

    return 0;
}
