/* cc_dict and its prime-number helper are private and are consequently not
   exported by the Windows DLL. CoinTests compiles a private copy of cc_dict;
   use the same helper implementation as the library for that copy. */
#include "../src/coin_geq_prime_number.ic"
