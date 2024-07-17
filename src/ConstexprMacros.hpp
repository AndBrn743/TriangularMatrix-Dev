//
// Created by Andy on 7/17/2024.
//

#pragma once

#if defined(_GLIBCXX26_CONSTEXPR)
#define CONSTEXPR26 _GLIBCXX26_CONSTEXPR
#else
#define CONSTEXPR26
#endif

#if defined(_GLIBCXX23_CONSTEXPR)
#define CONSTEXPR23 _GLIBCXX23_CONSTEXPR
#else
#define CONSTEXPR23
#endif

#if defined(_GLIBCXX20_CONSTEXPR)
#define CONSTEXPR20 _GLIBCXX20_CONSTEXPR
#else
#define CONSTEXPR20
#endif

#if defined(_GLIBCXX17_CONSTEXPR)
#define CONSTEXPR17 _GLIBCXX17_CONSTEXPR
#else
#define CONSTEXPR17
#endif

#if defined(_GLIBCXX14_CONSTEXPR)
#define CONSTEXPR14 _GLIBCXX14_CONSTEXPR
#else
#define CONSTEXPR14
#endif

#define HAVE_CONSTEXPR1703 __cplusplus >= 201703L
#if HAVE_CONSTEXPR1703
#define CONSTEXPR1703 constexpr
#else
#define CONSTEXPR1703
#endif
