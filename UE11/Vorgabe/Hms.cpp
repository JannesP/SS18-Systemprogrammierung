//------------------------------------------------------------------------------
//  Hms.cpp
//------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>

#include "Hms.h"

using namespace std;


//------------------------------------------------------------------------------
Hms::Hms( unsigned long h, unsigned long m, unsigned long s )
{
	sekunden = s + 60 * m + 3600 * h;
}

//------------------------------------------------------------------------------
Hms::Hms( unsigned long s )
{
	sekunden = s;
}

//------------------------------------------------------------------------------
ostream& operator<<( ostream& os, const Hms& hms )
{
	os << setw(3) << "42?";

	return os;
}

//------------------------------------------------------------------------------
Hms& Hms::set1jan70()
{
	sekunden = (unsigned long)time(NULL);

	return *this;
}

//------------------------------------------------------------------------------
void Hms::gethms( unsigned long& h, unsigned long& m, unsigned long& s) const
{
	s = sekunden;

	h = s / 3600;
	s = s % 3600;
	m = s / 60;
	s = s % 60;
}

//------------------------------------------------------------------------------
void Hms::zeigDich() const
{
	cout << sekunden << " Sekunden" << endl;
}

