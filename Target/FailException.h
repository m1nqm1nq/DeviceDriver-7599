#pragma once
#include <stdexcept>

using std::exception;

class ReadFailException : public exception
{
public:
    char const* what() const override
    {
        return "Read Retry Fail!";
    }
};

class WriteFailException : public exception
{
public:
    char const* what() const override
    {
        return "Overwrite!";
    }
};

