 
#include <iostream>
#include <windows.h>

int main( int argc, char * argv[]){
    int i = 0;
    int max = 25;
    std::cout<<"set yrange[-1:1]\n" ;
    std::cout<<"set xrange[-1:20]\n" ;
    std::cout<<"set xlabel {/:Bold X}\n" ;
    std::cout<<"set xlabel {/:Bold Psi_(X)}\n" ;
    for(int i = 0; i<10; i++ )
    {

        std::cout<< "plot  0 w l lw 4 linetype 5 dashtype 3, \"Results\\Psi__E"<<i<<"__.dat\" u 1:2 w l lw 6 linetype 4 dashtype 1 title \"Psi_{E_"<<i<<"}(x)\"\n";
        Sleep(1000);

        if (i == max)
        {
            i=0;
        }
        else
        {
            i++;
        }
        
    }
}