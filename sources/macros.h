#ifndef QPDFVIEW_MACROS_H
#define QPDFVIEW_MACROS_H

#if __cplusplus >= 201703L
#ifndef DECL_NODISCARD
#define DECL_NODISCARD [[nodiscard]]
#endif

#ifndef DECL_UNUSED
#define DECL_UNUSED [[maybe_unused]]
#endif

#ifndef FALLTHROUGH
#define FALLTHROUGH [[fallthrough]];
#endif

#else

#define DECL_UNUSED __attribute__((__unused__))
#define DECL_NODISCARD

#endif

#endif   // QPDFVIEW_MACROS_H
