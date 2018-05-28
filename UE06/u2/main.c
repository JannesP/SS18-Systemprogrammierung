#include <stdio.h>
#include <stdlib.h>

int main() {
    /*
     * ls -l
B) mach2s
C) machXs 10 und
mach2s
D) machXs 10& und
mach2s
E) machXs 20 > out1.txt& und
machXs 30 > out2.txt& und
mach2s
     *
     */


    //system("ls -l");
    //system("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u2/cmake-build-debug/make2s");
    //system("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u2/cmake-build-debug/makeXs 10\n/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u2/cmake-build-debug/make2s");
    //system("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u2/cmake-build-debug/makeXs 10 &\n/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u2/cmake-build-debug/make2s");

    system("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u2/cmake-build-debug/makeXs 20 > out1.txt &");
    system("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u2/cmake-build-debug/makeXs 30 > out2.txt &");
    system("/home/localadmin/Schreibtisch/SS18-Systemprogrammierung/UE06/u2/cmake-build-debug/make2s");


    printf("fertig");
    return 0;
}