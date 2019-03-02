#pragma once

#include <QtGlobal>

#ifndef BUILD_STATIC
# if defined(FINITESTATEMACHINE_LIB)
#  define FINITESTATEMACHINE_EXPORT Q_DECL_EXPORT
# else
#  define FINITESTATEMACHINE_EXPORT Q_DECL_IMPORT
# endif
#else
# define FINITESTATEMACHINE_EXPORT
#endif
