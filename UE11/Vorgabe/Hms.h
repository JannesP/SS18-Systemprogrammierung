//------------------------------------------------------------------------------
//  Hms.h
//------------------------------------------------------------------------------

#include <iostream>
#include <string>

using namespace std;


//------------------------------------------------------------------------------
class Hms
{
	unsigned long sekunden;

public:
	Hms( unsigned long h, unsigned long m, unsigned long s );
	Hms( unsigned long s );

	friend ostream& operator<<( ostream& os, const Hms& hms );

	Hms& set1jan70();

	void gethms( unsigned long& h, unsigned long& m, unsigned long& s) const;

	void zeigDich() const;
};
