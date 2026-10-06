/* Coin's portable feature configuration for the embedded Expat 2.9.0.
 * Keep this separate from Coin's config.h and from upstream Expat sources.
 */
#ifndef COIN_EXPAT_CONFIG_H
#define COIN_EXPAT_CONFIG_H 1

/* Expat's byte-order-neutral path detects host endianness at runtime. */
#define BYTEORDER 0

/* Match the previous embedded Expat feature profile. */
#define XML_GE 1

#ifdef _WIN32
#define XML_DTD 1
#define XML_NS 1
#define XML_CONTEXT_BYTES 1024
#else
#define XML_CONTEXT_BYTES 0
#define XML_DEV_URANDOM 1
#endif

#endif /* COIN_EXPAT_CONFIG_H */
