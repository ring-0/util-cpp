#pragma once

#include <stdint.h>
#include <stdio.h>


typedef uint64_t			cycles_t;
typedef volatile uint64_t	vcycles_t;

inline cycles_t rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return (static_cast<uint64_t>(lo) | static_cast<uint64_t>(hi)<<32 );
}

inline cycles_t rdtscd(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("cpuid; rdtsc" : "=a"(lo), "=d"(hi) : : "ebx", "ecx");
    return (static_cast<uint64_t>(lo) | static_cast<uint64_t>(hi)<<32 );
}


namespace util {

struct ScopedRDTSC
{
    const char*	name;
    cycles_t	start;

    ScopedRDTSC() 				: name("Elapsed"), start(rdtsc())   { }
    ScopedRDTSC(const char* n)	: name(n), start(rdtsc())			{ }

    ~ScopedRDTSC()
    {
        cycles_t diff = rdtsc() - start;
        printf("%s: %lu\n", name, diff);
    }

private:
    ScopedRDTSC& operator=(const ScopedRDTSC &) = delete;
    ScopedRDTSC(const ScopedRDTSC &) = delete;
};

struct ScopedRDTSCD
{
    const char*	name;
    cycles_t	start;

    ScopedRDTSCD() 				: name("Elapsed"), start(rdtscd())	{ }
    ScopedRDTSCD(const char* n) : name(n), start(rdtscd()) 			{ }

    ~ScopedRDTSCD()
    {
        cycles_t diff = rdtscd() - start;
        printf("%s: %lu\n", name, diff);
    }

private:
    ScopedRDTSCD& operator=(const ScopedRDTSCD &) = delete;
    ScopedRDTSCD(const ScopedRDTSCD &) = delete;
};


} // end namespace util

