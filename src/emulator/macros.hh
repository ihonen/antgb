#pragma once

#ifdef __GNUG__
#define ANTDBG_ALWAYS_INLINE __attribute__((always_inline)) inline
#else
#define ANTDBG_ALWAYS_INLINE
#endif
