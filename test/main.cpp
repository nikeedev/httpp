#include <fmt/core.h>
#include "httpp.h"
#include <fstream>

int main() {
    std::ofstream out("image.png", std::ofstream::out | std::ofstream::trunc);

    httpp::httpResponse resp = httpp::get("https://nikee.dev/imgr/playdatesdk_screenshot.png");

    fmt::print("Status code: {}", resp.status_code);

    out << resp.body.c_str();

    out.close();

    return 0;
}