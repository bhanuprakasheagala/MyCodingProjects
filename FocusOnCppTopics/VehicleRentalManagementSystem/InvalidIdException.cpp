#include "InvalidIdException.hpp"

InvalidIDException::InvalidIDException(const std::string& msg) : message(msg) {}

const char* InvalidIDException::what() const noexcept {
    return message.c_str();
}