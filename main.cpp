#include "simulation.h"
#include "reconstruction.h"
#include "ztrue.h"
#include "moltepl.h"
# include <iostream>

int main(){

    simulation("~/Desktop/C++/TANS-ProgettoEsame/VertexingMC/","sim",1e4,1,20);
    reconstruction("sim");

    std::cout << "reconstruction completed - ChadFace" << std::endl;

    moltepl("rec_sim");
    ztrue("rec_sim");

    std::cout << std::endl << "All graphs produced - PogChamp" << std::endl;
    return 0;
}