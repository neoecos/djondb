#ifndef JCLIENTGEN_GLOBAL_H
#define JCLIENTGEN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(JCLIENTGEN_LIBRARY)
#  define JCLIENTGENSHARED_EXPORT Q_DECL_EXPORT
#else
#  define JCLIENTGENSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // JCLIENTGEN_GLOBAL_H