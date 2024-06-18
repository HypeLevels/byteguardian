#pragma once
#include "../include.h"

inline std::string path()
{
    char shitter[_MAX_PATH]; // defining the path
    GetModuleFileNameA(NULL, shitter, _MAX_PATH); // getting the path
    return std::string(shitter); //returning the path
}

inline std::string random_string(std::string::size_type length)
{
    static auto& chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{ std::random_device{}() };
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while (length--)
        s += chrs[pick(rg)];

    return s;
}