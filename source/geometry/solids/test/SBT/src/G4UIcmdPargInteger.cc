//
// G4UIcmdPargInteger.cc
//
// Implementation of an integer argument for G4UIcmdWithPargs
//

#include "G4UIcmdPargInteger.hh"

#ifdef WIN32
#  include <Strstrea.h>
#else
#  include <strstream.h>
#endif

//
// Fetch argument value from input stream
//
istream &G4UIcmdPargInteger::FetchValue( istream &istr )
{
	G4int newValue;
	istr >> newValue;
	
	if (istr) value = newValue;
	
	return istr;
}


//
// ConvertToString
//
G4String G4UIcmdPargInteger::ConvertToString()
{
	char buff[20];
	ostrstream os(buff,20);
	
	os << value << '\0';
	
	G4String answer(buff);
	return answer;
}
