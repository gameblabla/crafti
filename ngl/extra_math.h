#ifndef EXTRA_MATH
#define EXTRA_MATH

#ifdef DREAMCASTs
#include "sh4_math.h"
#define sqrt_real MATH_Fast_Sqrt
#define min_real MATH_Fast_Fminf
#define max_real MATH_Fast_Fmaxf
#define divide_real(a,b) MATH_Fast_Divide(a, b)
#else
#define sqrt_real sqrt
#define min_real std::min
#define max_real std::max
#define divide_real(a,b) (a / b)
#endif

#endif
