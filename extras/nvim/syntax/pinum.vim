"******************************************************************"
"   _____ _ _   _                       _                          "
"  |  __ (_) \ | |                     | |                         "
"  | |__) ||  \| |_   _ _ __ ___ ______| |     __ _ _ __   __ _    "
"  |  ___/ | . ` | | | | '_ ` _ \______| |    / _` | '_ \ / _` |   "
"  | |   | | |\  | |_| | | | | | |     | |___| (_| | | | | (_| |   "
"  |_|   | |_| \_|\__,_|_| |_| |_|     |______\__,_|_| |_|\__, |   "
"                                                          __/ |   "
"                                                         |___/    "
"                                                                  "
"  Copyright (c) 2026 tanvir-techbro.                              "
"  You may opt to use, copy, modify, merge, publish, distribute    "
"  and/or sell copies of the Software, and permit persons to whom  "
"  the Software is furnished to do so, under the conditions of the "
"  LICENSE.                                                        "
"  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, "
"  EXPRESS OR IMPLIED.                                             "
"                                                                  "
"  If you find any bug you would be highly encouraged to create a  "
"  github issue at <https://github.com/tanvir-techbro/PiNum-Lang>  "
"  or contact <surjointelligence.team@gmail.com>                   "
"******************************************************************"

" Vim syntax file
" Language: PiNum
" Maintainer: tanvir-techbro

if exists("b:current_syntax")
  finish
endif

" 1. Directives & Imports (The @ stuff)
syntax match pinumDirective "@\w\+"
syntax keyword pinumLib stdlib math engine
highlight link pinumDirective PreProc
highlight link pinumLib Special

" 2. Keywords (The 'Actions')
syntax keyword pinumKeyword if else print read return import
highlight link pinumKeyword Statement

" 3. Data Types & Prefixes
syntax keyword pinumType int float double char string bool long short signed unsigned
highlight link pinumType Type

" 4. Numbers (Integers and Floats)
" Handles 42, 3.14, and .5
syntax match pinumNumber "\b\d\+\(\.\d\+\)\?\b"
syntax match pinumNumber "\.\d\+\b"
highlight link pinumNumber Constant

" 5. Strings and Chars
syntax region pinumString start='"' end='"' skip='\\"'
syntax region pinumChar start="'" end="'" skip="\\'"
highlight link pinumString String
highlight link pinumChar Character

" 6. Operators and Symbols
" We group these so they don't look like plain text
syntax match pinumOperator "[+=\-\*/%&^|!<>?~]"
syntax match pinumDelimiter "[()\[\]{};,.]"
highlight link pinumOperator Operator
highlight link pinumDelimiter Delimiter

" 7. Comments
syntax match pinumComment "#.*$"
highlight link pinumComment Comment

" 8. Special Escapes (the \n, \t stuff)
syntax match pinumSpecial "\\n\|\\t\|\\0"
highlight link pinumSpecial SpecialChar

let b:current_syntax = "pinum"
