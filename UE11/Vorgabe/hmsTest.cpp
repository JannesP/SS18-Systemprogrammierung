//------------------------------------------------------------------------------
//  hmsTest.cpp
//------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include "Hms.h"

using namespace std;


//------------------------------------------------------------------------------
int main()
{
	Hms t1( 1,  61, 61);
	Hms t2(38, 240,  1);

	t1.zeigDich();

	unsigned long h, m, s;
	t2.gethms(h, m, s);
	cout << setw(4) << h << " Stunden\n";
	cout << setw(4) << m << " Minuten\n";
	cout << setw(4) << s << " Sekunden\n";

	t2 = t1;
	t2.zeigDich();

	cout << "Zeit seit 1.1.1970 0 Uhr: " << t1.set1jan70() << endl;
}

