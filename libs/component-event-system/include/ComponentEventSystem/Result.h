#ifndef COMPONENTEVENTSYSTEM_RESULT_H
#define COMPONENTEVENTSYSTEM_RESULT_H

#include <string>

// Result can be used as a return type for functions.
// It contains whether the function was successful and a message if it failed.
struct Result
{
    Result(bool success) : success(success) {}
    Result(bool success, std::string message) : success(success), message(message) {}
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