#ifndef __ETL_PROFILE_H__
#define __ETL_PROFILE_H__

#define ETL_THROW_EXCEPTIONS
#define ETL_VERBOSE_ERRORS
#define ETL_CHECK_PUSH_POP

#ifdef _MSC_VER
#include "etl/profiles/msvc.h"
#else
#include "etl/profiles/gcc_generic.h"
#endif
#endif