//
// Created by localadmin on 11.06.18.
//

#ifndef RECHTECK2_RECHTECK_H
#define RECHTECK2_RECHTECK_H


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
    int x = 0;
    int y = 0;
    char* corner;

public:
    Rechteck() : Rechteck(3, 5) { }

    Rechteck(int hoehe, int breite) : Rechteck(hoehe, breite, '+') { }

    Rechteck(int hoehe, int breite, char eckChar)
            : corner(new char(eckChar))
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
        for (int i = 0; i < y; i++) {
            std::cout << std::endl;
        }
        for (int i = 0; i < x; i++) {
            std::cout << ' ';
        }
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
                    std::cout << *corner;
                else if(oben || unten)
                    std::cout << "-";
                else if(links || rechts)
                    std::cout << "!";
                else
                    std::cout << " ";

                if(rechts) {
                    std::cout << std::endl;
                    if (h != hoehe - 1) {
                        for (int i = 0; i < x; i++) {
                            std::cout << ' ';
                        }
                    }
                }
            }
        }
    }

    void bewegDichUm(int deltaX, int deltaY){
        x += deltaX;
        y += deltaY;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
    }

    void drehDich90Grad()
    {
        int buf = hoehe;
        hoehe = breite;
        breite = buf;
    }

};

#endif //RECHTECK2_RECHTECK_H
