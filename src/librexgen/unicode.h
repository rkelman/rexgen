/*
    Copyright (C) 2012-2013  Jan Starke <rexgen@outofbed.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/
*/

#ifndef __unicode_h__
#define __unicode_h__
#include <cstdarg>

/* for readability issues:
 *   we use 'char' for characters; one character per 'char'-value
 *   we use 'byte' for binary values, which may contain
 *     a character or part of a character
 */
typedef char byte; 


#ifdef _WIN32
#include <tchar.h>

#ifndef UNICODE
#error UNICODE is not defined
#endif

#ifndef _UNICODE
#error _UNICODE is not defined
#endif

typedef wchar_t char_type;
#define utf_snprintf(buffer,count,format,...) _snwprintf((buffer),(count),format,__VA_ARGS__)
#define utf_strlen wcslen
#define utf_strncat wcsncat
#define utf_strncpy wcsncpy
#define utf_strconv_from_locale u8_strconv_from_locale

//#define _T(STR) ( const_cast<char_type*>( reinterpret_cast<const char_type*>( _T ## STR )))
#define _C(STR) ( (const char_type*) _T(STR) )

#define PRINTF_FORMAT "%s"


#else /* ! _WIN32 */
#include <unistdio.h>
#include <unistr.h>
#include <uniconv.h>

#ifndef UTF_VARIANT
#define UTF_VARIANT 32
#endif

#define UTF32

typedef uint32_t char_type;
#define utf_snprintf u32_snprintf
#define utf_strlen u32_strlen
#define utf_strncat u32_strncat
#define utf_strncpy u32_strncpy
#define utf_strconv_from_locale u32_strconv_from_locale

#define _T(STR) ( const_cast<char_type*>( reinterpret_cast<const char_type*>( u ## STR )))
#define _C(STR) ( (const char_type*) u ## STR )

#define PRINTF_FORMAT "%llU"

#endif /* _WIN32 */

#endif /* __unicode_h__ */
