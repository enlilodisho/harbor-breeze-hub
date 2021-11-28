#ifndef COMPONENTEVENTSYSTEM_RESULT_H
#define COMPONENTEVENTSYSTEM_RESULT_H

#include <cstdarg>
#include <string>

constexpr size_t MAX_MESSAGE_LENGTH = 1000;

// Result can be used as a return type for functions.
// It contains whether the function was successful and a message if it failed.
struct Result
{
    explicit Result(bool success) : success(success) {}
    //Result(bool success, const char* messagef, const std::string& message) : success(success), message(message) {}
    Result(bool success, const char* messagef, ...) : success(success)
    {
        va_list args;
        char buffer[MAX_MESSAGE_LENGTH];
        va_start(args, messagef);
        int length = vsprintf(buffer, messagef, args);
        va_end(args);
        if (length > 0)
        {
            message = std::string(buffer, length);
        }
        else
        {
            message = "";
        }
    }
    Result& operator+=(const Result& other)
    {
        success = success && other.success;
        if (other.message.length() > 0)
        {
            if (message.length() > 0)
            {
                message += "\n";
            }
            message += other.message;
        }
        return *this;
    }
    
    bool success;
    std::string message;
};

inline Result operator+(Result lhs, const Result& rhs)
{
    return lhs += rhs;
}

#endif // COMPONENTEVENTSYSTEM_RESULT_H