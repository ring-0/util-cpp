#pragma once

#include <errno.h>
#include <string.h>
#include <stdexcept>
#include <string>

namespace util {

typedef const std::string &const_strref;

class IOException : public std::runtime_error
{
public:
    IOException(const_strref t1)										            : std::runtime_error(t1 + strerror(errno)) {}
    IOException(const_strref t1, const_strref t2) 						            : std::runtime_error(t1 + t2 + strerror(errno)) {}
    IOException(const_strref t1, const_strref t2, const_strref t3)				    : std::runtime_error(t1 + t2 + t3 + strerror(errno)) {}
    IOException(const_strref t1, const_strref t2, const_strref t3, const_strref t4) : std::runtime_error(t1 + t2 + t3 + t4 + strerror(errno)) {}
};


class RuntimeError : public std::runtime_error
{
public:
    RuntimeError(const_strref t1)										                : std::runtime_error(t1) {}
    RuntimeError(const_strref t1, const_strref t2) 							            : std::runtime_error(t1 + t2) {}
    RuntimeError(const_strref t1, const_strref t2, const_strref t3)				        : std::runtime_error(t1 + t2 + t3) {}
    RuntimeError(const_strref t1, const_strref t2, const_strref t3, const_strref t4)	: std::runtime_error(t1 + t2 + t3 + t4) {}
};


class LogicError : public std::logic_error
{
public:
    LogicError(const_strref t1)										                : std::logic_error(t1) {}
    LogicError(const_strref t1, const_strref t2) 							        : std::logic_error(t1 + t2) {}
    LogicError(const_strref t1, const_strref t2, const_strref t3)				    : std::logic_error(t1 + t2 + t3) {}
    LogicError(const_strref t1, const_strref t2, const_strref t3, const_strref t4)	: std::logic_error(t1 + t2 + t3 + t4) {}
};

} // end namespace util
