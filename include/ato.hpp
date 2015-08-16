#pragma once

// use branch advises
#define  __likely(cond)    __builtin_expect(!!(cond), 1)
#define  __unlikely(cond)  __builtin_expect(!!(cond), 0)

namespace util {

// a faster atoi/atoll function, this function is called so frequently
// its worth optimizing and advising the compiler with branch predictions
template<typename T>
inline T ato(const char *in) {
    T value = 0;

    if(__unlikely(*in == '-')) {
        in++;
        while(*in >= '0' && *in <= '9') {
            value = value*10 + *in++ - '0';
        }

        return(-value);
    }

    while(*in >= '0' && *in <= '9') {
        value = value*10 + *in++ - '0';

    }

    return(value);
}


// we need to specialize the template for double '.'
// unroll the loop for less dereferencing
template<>
inline double ato(const char *in) {
    double value = 0;
    int div = 10;

    if(__unlikely(*in == '-')) {
        in++;
        while((*in >= '0' && *in <= '9') || *in == '.') {
            in++;
            if(__unlikely(*in == '.')) {
                while(*in >= '0' && *in <= '9') {
                    value += static_cast<double>(*in++ - '0') / div;
                    div *= 10;
                }
                return(-value);
            }

            value = value*10 + *in++ - '0';
        }

        return(-value);
    }

    while((*in >= '0' && *in <= '9') || *in == '.') {
        if(__unlikely(*in == '.')) {
            in++;
            while(*in >= '0' && *in <= '9') {
                value += static_cast<double>(*in++ - '0') / div;
                div *= 10;
            }
            return(value);
        }

        value = value*10 + *in++ - '0';
    }

    return(value);
}



} // end namespace util

#undef  __likely
#undef  __unlikely
