#ifndef INVALID_ID_EXCEPTION
#define INVALID_ID_EXCEPTION

#include <exception>
#include <string>

class InvalidIDException : public std::exception {
    private:
        std::string message;
    public:
        explicit InvalidIDException(const std::string& msg);
        const char* what() const noexcept override;
};

#endif