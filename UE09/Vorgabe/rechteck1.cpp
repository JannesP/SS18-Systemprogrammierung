//------------------------------------------------------------------------------
//
// rechteck1.cpp
//
//
// g++ -Wall rechteck1.cpp -o rechteck1			//// nicht gcc ////
//
//------------------------------------------------------------------------------

# include <iostream>


//------------------------------------------------------------------------------
class Rechteck
{
private:
    int hoehe;
    int breite;

public:
    Rechteck()           // für Instanziierung von array-of-objects erforderlich
    {
        hoehe = 3;
        breite = 5;
    }

    Rechteck(int hoehe, int breite)
    {
        if(hoehe < 3)
            hoehe = 3;

        if(breite < 3)
            breite = 3;

        this->hoehe = hoehe;
        this->breite = breite;
    }

    void zeigDich()
    {
        bool links, rechts, oben, unten;

        for(int h = 0; h < hoehe; h++)
        {
            for(int b = 0; b < breite; b++)
            {
                links  = (b == 0);
                rechts = (b == breite - 1);
                oben   = (h == 0);
                unten  = (h == hoehe - 1);

                if((oben || unten) && (links || rechts))
                    std::cout << "+";
                else if(oben || unten)
                    std::cout << "-";
                else if(links || rechts)
                    std::cout << "!";
                else
                    std::cout << " ";

                if(rechts)
                    std::cout << std::endl;
            }
        }
    }

};

//------------------------------------------------------------------------------
int main()
{
    Rechteck r1(2, -16);	// -> 3x3 Rechteck
    Rechteck r2(3, 10);

    r1.zeigDich();
    r2.zeigDich();

    const int anzahlRechtecke = 3;
    Rechteck rechteckFeld[anzahlRechtecke];	// Standardkonstruktor ...

    for(int i = 0; i < anzahlRechtecke; i++)
        rechteckFeld[i].zeigDich();
}
