#include <fmt/core.h>
#include "httpp.h"
#include <fstream>

using namespace httpp;

int main() {

    std::ofstream out;
    out.open("output.json", std::ios::out | std::ios::binary);
    
    auto request = get("https://api.scratch.mit.edu/users/stio_studio");

    fmt::println("Status code: {}", request.status_code);

    // fmt::println("Output Body: {}", request.body);

    out.write(request.body.data(), request.body.size());

    out.close();

    return 0;
}