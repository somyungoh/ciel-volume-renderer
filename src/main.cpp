#include "cielApp.h"

#include <iostream>
#include <omp.h>
#include <print>

// import std;  # clang doesn't support modules yet

int main(int, char **)
{
// Beginning of parallel region
#pragma omp parallel for
    for (size_t i = 0; i < 16; i++) {
        std::cout << std::format("Hello World... from thread = {}\n",
                                 omp_get_thread_num());
    }
    // Ending of parallel region

    ciel::CielApp app;
    app.init(ciel::RenderSetting{800, 600});

    try {
        app.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    std::println("Hello, from Ciel!");
}
