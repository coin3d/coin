#ifndef COIN_VSNPRINTF_H
#define COIN_VSNPRINTF_H

#ifndef HAVE_VSNPRINTF
extern int vsnprintf(char * target, size_t n, const char * formatstr, ...);
#endif /* HAVE_VSNPRINTF */

#endif /* COIN_VSNPRINTF_H */
