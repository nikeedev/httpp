#include <fmt/core.h>
#include "httpp.h"
#include <fstream>

int main() {
    std::ofstream out;
    out.open("image.png", std::ios::out | std::ios::binary);

    httpp::httpResponse resp = httpp::get("https://picsum.photos/5000");

    fmt::print("Status code: {}", resp.status_code);

    out.write(resp.body.data(), resp.body.size());

    out.close();

    return 0;
}