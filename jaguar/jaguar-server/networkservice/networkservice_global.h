#ifndef NETWORKSERVICE_GLOBAL_H
#define NETWORKSERVICE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NETWORKSERVICE_LIBRARY)
#  define NETWORKSERVICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define NETWORKSERVICESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NETWORKSERVICE_GLOBAL_H