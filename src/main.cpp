#include "cielApp.h"

#include <iostream>
#include <omp.h>
#include <print>

// import std;  # clang doesn't support modules yet

int main(int argn, char** argv)
{
    // TODO: take window size as an argument
    ciel::CielApp app;
    app.init(800, 600);

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
