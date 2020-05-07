#pragma once

#if defined(__GNUG__) || defined(__GNUC__)
#define FORCE_INLINE __attribute__((always_inline))
#else
#define FORCE_INLINE
#endif
