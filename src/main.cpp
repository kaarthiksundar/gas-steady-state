#include <iostream>
#include <gravity/solver.h>

#include "files.h"

using namespace gravity;


int main (int argc, char * argv[]) {

    auto input_filenames = get_filenames();
    std::cout << input_filenames.nodes << std::endl;

}
