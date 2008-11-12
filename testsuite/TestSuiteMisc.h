#ifndef COIN_TESTSUITE_MISC
#define COIN_TESTSUITE_MISC

#include <string>
#include <ostream>

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
