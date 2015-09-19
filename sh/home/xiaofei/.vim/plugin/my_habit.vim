""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" My habit		 
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"ignorecase
"set ic


if $CSCOPE_DB == ""
	"shiftwidth
	"set sw=2

	"expandtab
	"set et

	"smarttab
	"set sta
endif

"set filetype indent
:filetype indent on

"autoindent
set ai	

"smart indent
set si

"cindentoption
set cino+=g0

"paste option
"set paste

"highlight search
set hls

"increase search
set is

"folder method
"set fdm=indent

"forbidden .swp files
set noswf

"The mouse can be enabled for different modes:
"	n	Normal mode
"	v	Visual mode
"	i	Insert mode
"	c	Command-line mode
"	h	all previous modes when editing a help file
"	a	all previous modes
"	r	for |hit-enter| and |more-prompt| prompt
set mouse=a

"more endode
set fencs=ucs-bom,utf-8,gb18030,gbk,gb2312,big5,euc-jp,euc-kr,latin1,cp936

"map <F2> taglist toggle
nmap <F2> :TlistToggle<CR>

"map <F3> highlight search
vmap <F3> y/<C-R>"<CR>

"map <F4> Show Full Path
nmap <F4> :call Path()<CR>

"map <F5> Update file
nmap <F5> :call UpdateFile()<CR>

"map yank to system clip board
vn y "+y
vn p "+p

function! Path()
	":echo substitute(expand("%:p:h"), ".*", "\\U\\0", "")
	"echo expand("%:p")
	:!echo "%:p"
	:let @+ = expand("%:p")
endfunction

function! UpdateFile()
	:e
endfunction
