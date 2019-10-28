//
// Created by john on 27/10/2019.
//

#include "load_buffer.h"

#include <fmt/printf.h>
#include <gsl/string_span>

#include <cstdio>
#include <memory>
#include <vector>

auto load_buffer(gsl::cstring_span<> filename)
-> std::optional<std::vector<char>>
{
    auto f{std::unique_ptr<std::FILE, decltype(&fclose)>{
            std::fopen(filename.data(), "r"), &fclose}};

    if (!f) {
        fmt::print(stderr, "error: Could not open '{}'.\n",
                gsl::to_string(filename));
        return std::nullopt;
    }

    std::fseek(f.get(), 0, SEEK_END);
    auto const file_size{std::ftell(f.get())};
    std::fseek(f.get(), 0, SEEK_SET);

    std::vector<char> buffer(file_size);
    auto const read{std::ptrdiff_t(
            std::fread(buffer.data(), 1, file_size, f.get()))};
    if (read!=file_size) {
        fmt::print(stderr, "error: could not read, '{}'.\n",
                gsl::to_string(filename));
        return std::nullopt;
    }

    return buffer;
}
