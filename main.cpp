
#include "GUI.h"
#include <iostream>
#include <exception>

int main(){
    try{
        GUI gui;
        gui.run;
    } catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;

}

