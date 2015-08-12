/*
    rexgen - a tool to create words based on regular expressions
    Copyright (C) 2012-2013  Jan Starke <jan.starke@outofbed.org>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 2 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin St, Fifth Floor, Boston, MA 02110, USA
*/


#include <ctype.h>
#include <librexgen/string/simplestring.h>

SimpleString::SimpleString()
  :length(0) {
	characters = new uchar_t[SIMPLESTRING_MAXLEN];
}

bool SimpleString::isalpha(unsigned int n) const {
  return (::isalpha(characters[n].codepoint));
}
bool SimpleString::islower(unsigned int n) const {
  return (::islower(characters[n].codepoint));
}

bool SimpleString::isupper(unsigned int n) const {
  return ( ::isupper(characters[n].codepoint));
}

void SimpleString::tolower(unsigned int n) {
  characters[n].codepoint = ::tolower(characters[n].codepoint);
}
void SimpleString::toupper(unsigned int n) {
  characters[n].codepoint = ::toupper(characters[n].codepoint);
}

void SimpleString::append(const char* ch, size_t ch_len) {
  fprintf(stderr, "APPEND\n");
	/* ensure that buffer is not full */
	if (length > SIMPLESTRING_MAXLEN) {
		return;
	}

	/* make sure we do not write more bytes than available */
	if ((length + ch_len) > SIMPLESTRING_MAXLEN) {
		ch_len = SIMPLESTRING_MAXLEN - length;
	}

	/* convert and copy characters */
	for (size_t idx=0; idx<ch_len; ++idx) {
		characters[length++] = ch[idx];

    fprintf(stderr, "%04x\n", characters[length-1].codepoint);
	}
}

size_t SimpleString::get_buffer_size() const {
  return SIMPLESTRING_MAXLEN;
}

size_t SimpleString::to_ansi_string(char* dst, const size_t buffer_size) const {
  size_t idx;
	const size_t len = (length>=buffer_size) ? (buffer_size-1) : (length);

	for (idx=0; idx<len; ++idx) {
		if (characters[idx].codepoint > 255 || characters[idx].plane!= BMP) {
			dst[idx] = '?';
		} else {
    	dst[idx] = (char)characters[idx].codepoint;
		}
  }

	dst[idx] = 0;
	return idx;
}

void SimpleString::print(FILE* out) const {
  for (size_t idx=0; idx<length; ++idx) {
    fputc(characters[idx].codepoint, out);
  }
}

#define UNI_SUR_HIGH_START      0xD800
#define UNI_SUR_HIGH_END        0xDBFF
#define UNI_SUR_LOW_START       0xDC00
#define UNI_SUR_LOW_END         0xDFFF
#define UNI_MAX_LEGAL_UTF32     0x0010FFFF

const uint32_t byte_mask = 0xbf;
const uint32_t byte_mark = 0x80;

size_t SimpleString::to_utf8_string(char* dst, const size_t buffer_size) const {
  size_t idx;     /* index in source string
	                 * must be <length
									 */
	size_t count;

	for (idx=0, count=0; idx<length && count<buffer_size-4; ++idx) {
		const uint32_t &value = characters[idx].full_codepoint();
    if (value < 0x80) {
      dst[count++] = static_cast<byte>(characters[idx].codepoint);
		} else if (value < 0x800) {
			dst[count++] = ((char)value | byte_mark) & byte_mask;
			dst[count++] = ((char)(value>>6) | 0xc0);
		} else if (value < 0x10000) {
			dst[count++] = ((char)value | byte_mark) & byte_mask;
			dst[count++] = ((char)(value>>6) | byte_mark) & byte_mask;
			dst[count++] = ((char)(value>>12) | 0xE0);
		} else if (value < UNI_MAX_LEGAL_UTF32) {
			dst[count++] = ((char)value | byte_mark) & byte_mask;
			dst[count++] = ((char)(value>>6) | byte_mark) & byte_mask;
			dst[count++] = ((char)(value>>12) | byte_mark) & byte_mask;
			dst[count++] = ((char)(value>>18) | 0xf0);
		} else {
			dst[count++] = '?';
		}
  }

	/* do not increment cur, because we do not count the terminating \0 */
	dst[count] = 0;
	return count;
}


