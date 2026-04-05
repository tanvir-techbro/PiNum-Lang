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

" 7. Special Escapes (the \n, \t stuff)
syntax match pinumSpecial "\\n\|\\t\|\\0"
highlight link pinumSpecial SpecialChar

let b:current_syntax = "pinum"
