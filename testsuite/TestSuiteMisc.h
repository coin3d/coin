#ifndef COIN_TESTSUITE_MISC
#define COIN_TESTSUITE_MISC

#include <string>
#include <ostream>
#include <boost/lexical_cast.hpp>

// Test for almostEquality
inline bool floatEquals(float Ain, float Bin, int maxUlps)
{
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
    union {
      float f32;
      uint32_t i32;
    } A,B;
    A.f32 = Ain;
    B.f32 = Bin;
    // Make A.i32 lexicographically ordered as a twos-complement int
    if (A.i32 < 0)
        A.i32 = 0x80000000 - A.i32;
    // Make B.i32 lexicographically ordered as a twos-complement int
    if (B.i32 < 0)
        B.i32 = 0x80000000 - B.i32;
    int intDiff = abs(A.i32 - B.i32);
    if (intDiff <= maxUlps)
        return true;
    return false;
}

#define COIN_TESTCASE_CHECK_FLOAT(X,Y) BOOST_CHECK_MESSAGE(floatEquals((X), (Y), 2000), std::string("unexpected value: expected ") + boost::lexical_cast<std::string>((Y)) +", got " + boost::lexical_cast<std::string>((X)) + " difference is: " + boost::lexical_cast<std::string>((X)-(Y)))

/*
 * The following ostream << operators are needed for the Boost.Test macros
 * for when they report on failures with our custom datatypes.
 * Expand as needed.
 *
 * Note that the custom classes are predeclared in the prototype to keep
 * this header file tidy as it grows...
 */

std::ostream & operator << (std::ostream & os, const class SbVec4ub & vec);
std::ostream & operator << (std::ostream & os, const class SbVec4us & vec);
std::ostream & operator << (std::ostream & os, const class SbVec4ui32 & vec);

std::ostream & operator << (std::ostream & os, const class SbColor4f & col4);

#endif // !COIN_TESTSUITE_MISC
