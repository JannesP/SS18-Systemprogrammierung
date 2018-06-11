#include <iostream>
#include "rechteck.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Rechteck r1(2, -16, 'o');	// -> 3x3 Rechteck
    Rechteck r2(3, 10, '#');

    r1.zeigDich();
    r2.zeigDich();

    const int anzahlRechtecke = 3;
    Rechteck rechteckFeld[anzahlRechtecke];	// Standardkonstruktor ...

    for(int i = 0; i < anzahlRechtecke; i++)
    {
        rechteckFeld[i].bewegDichUm(i * 5, 0);
        rechteckFeld[i].zeigDich();
        rechteckFeld[i].drehDich90Grad();
        rechteckFeld[i].zeigDich();
    }


    return 0;
}