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

#include <sstream>
#include <cstdio>
#include <librexgen/parser/rexgenparsercontext.h>
#include <librexgen/regex/regex.h>
#include <librexgen/iterator/iterator.h>
#include <librexgen/iterator/topiterator.h>
#include <librexgen/osdepend.h>
#include <librexgen/parser/syntaxerror.h>

using namespace std;

int yyparse(RexgenParserContext* context);

Regex* parse_regex(RexgenParserContext* context) {
  
  try {
    if (yyparse(context) != 0) {
      return NULL;
    }
  } catch (SyntaxError& exc) {
    cerr << exc.getMessage() << endl;
    return NULL;
  }  
  if (context->hasInvalidGroupReferences()) {
    throw SyntaxError("This regular expression has an invalid back reference");
  }
  return context->result;
}

EXPORT
Regex* parse_regex(const char* regex, const RexgenOptions& options) {
  const string strRegex(regex);
  istringstream is(strRegex);
  RexgenParserContext context(&is, options);
  return parse_regex(&context);
}

EXPORT
Iterator* regex_iterator(const char* regex, const RexgenOptions& options) {
	Regex* re = parse_regex(regex, options);
  if (re == NULL) {
    return NULL;
  }
  IteratorState* state = new IteratorState();
  state->setRandomize(options.randomize);
  Iterator* iter = new TopIterator(re->getId(), re->iterator(state), state);
  // register regex alternatives
  iter->updateReferences(state);
  
  // update references
  iter->updateReferences(state);
  return iter;
}
