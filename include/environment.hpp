#pragma once

#include <string>
#include <stdio.h>
#include "exception.hpp"

namespace util {

namespace env {

bool		isSet(const char* key);
void		set(const char* key, const char* value);
const char*	get(const char* key);
int64_t		getInt(const char* key);
std::string	getString(const char* key);
bool		getBoolean(const char* key);


class ParameterError : public util::RuntimeError
{
public:
    ParameterError(const char* c1, const char* c2, const char* c3)
        : util::RuntimeError(c1, c2, c3) {}
};


struct RequiredParameters {
    struct {
        const char* parameter;
        const char* defaultValue;
    } required[64];

    void validate() const
    {
        for(int i=0; required[i].parameter != NULL; i++) {
            if(env::isSet(required[i].parameter) == false) {
                if(required[i].defaultValue != NULL) {
                    env::set(required[i].parameter, required[i].defaultValue);
                } else {
                    throw ParameterError("Required parameter ", required[i].parameter, " is missing");
                }
            }
        }
    }

    // returns false if string is ""
    bool isSet(const char* key) const
    {
        if(env::isSet(key) == false)
            return(false);

        return(env::getString(key).size() > 0);
    }

    // forwards
    const char*	get(const char* key) const
    {
        return(env::get(key));
    }

    int64_t getInt(const char* key) const
    {
        return(env::getInt(key));
    }

    std::string	getString(const char* key) const
    {
        return(env::getString(key));
    }

    bool getBoolean(const char* key) const
    {
        return(env::getBoolean(key));
    }

    // usage helper
    void usage(const char* name) const
    {
        fprintf(stderr,"Usage: %s\n\nEnvironment parameters:\n", name);
        for(int i=0; required[i].parameter != NULL; i++) {
            fprintf(stderr, "%-16s  Default=%-24s Set=%-s\n",
                required[i].parameter,
                required[i].defaultValue != NULL ? required[i].defaultValue : "",
                this->isSet(required[i].parameter) ? this->get(required[i].parameter) : "");
        }
    }
};

// implementation

bool util::env::isSet(const char* key)
{
    return(getenv(key) != NULL);
}

void util::env::set(const char* key, const char* value)
{
    setenv(key, value, 1);
}

const char* util::env::get(const char* key)
{
    return(getenv(key));
}


int64_t util::env::getInt(const char* key)
{
    if(getenv(key) == NULL)
        return 0;

    return(strtoll(getenv(key), NULL, 0));
}


std::string	util::env::getString(const char* key)
{
    return(getenv(key));
}


bool util::env::getBoolean(const char* key)
{
    std::string value = util::env::getString(key);
    if(value == "true" || value == "TRUE")
        return(true);
    else
        return(false);
}


} // end namespace environment
} // end namespace util
