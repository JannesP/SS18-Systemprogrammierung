#include <iostream>
#include <iomanip>
#include "FixStack.h"
#include "Stack.h"

using namespace std;

int main()
{
    Stack<int, 5> int5Stapel;
    Stack<std::string, 6> string6Stapel;
    int i = 1;
    while(int5Stapel.push(i))
        i++;
    while(int5Stapel.pop(i))
        std::cout << i << std::endl;
    string6Stapel.push("!\n");
    string6Stapel.push("Flensburg");
    string6Stapel.push("n ");
    string6Stapel.push("i ");
    string6Stapel.push("o ");
    string6Stapel.push("\nM ");
    std::string s;
    while(string6Stapel.pop(s))
        std::cout << s;
}