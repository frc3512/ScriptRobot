#ifndef _VXW_ASSERT_H
#define _VXW_ASSERT_H

/*assert is not actually defined on vxworks so we had to define it ourselves.
 */

#include <stdio.h>

#ifdef assert
#undef assert
#endif

#define assert(n) if(!(n)) printf("%s:%d: Assertion failed\n", __FILE__, __LINE__)

#endif /* VXW_ASSERT_H */
