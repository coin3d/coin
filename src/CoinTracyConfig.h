#ifndef COIN_TRACY_CONFIG_H
#define COIN_TRACY_CONFIG_H

#ifdef COIN_USE_TRACY
#include <tracy/Tracy.hpp>
#define CoinZoneScopedN(name) ZoneScopedN(name)
#else
#define CoinZoneScopedN(x)
#endif

#endif // COIN_TRACY_CONFIG_H
