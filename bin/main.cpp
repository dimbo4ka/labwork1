#include "lib/Application/Application.hpp"

int main(int argc, char** argv) {
    Application app;
    if (!app.Run(argc, argv))
        return 1;
    return 0;
}
