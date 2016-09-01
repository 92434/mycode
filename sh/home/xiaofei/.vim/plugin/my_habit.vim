""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" My habit		 
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"ignorecase
"set ic


"shiftwidth
"set sw=2

"set width of table
set ts=8

"table to space||||retab!
"set et

"smarttab
"set sta

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

"format code
nmap <S-F> <Esc>:call CodeFormat()<CR>

function! Path()
	":echo substitute(expand("%:p:h"), ".*", "\\U\\0", "")
	"echo expand("%:p")
	:!echo "%:p"
	:let @+ = expand("%:p")
endfunction

function! UpdateFile()
	:e
endfunction


"����AStyle���򣬽��д�������
func CodeFormat()
	"ȡ�õ�ǰ��������к�
	let lineNum = line(".")
	"CԴ����
	if &filetype == 'c'
		"ִ�е����ⲿ���������
		exec "%! astyle -A3Lfpjk3NS"
	"Hͷ�ļ�(�ļ�����ʶ��Ϊcpp)��CPPԴ����
	elseif &filetype == 'cpp'
		"ִ�е����ⲿ���������
		exec "%! astyle -A3Lfpjk3NS"
	"JAVAԴ����
	elseif &filetype == 'java'
		"ִ�е����ⲿ���������
		exec "%! astyle -A2Lfpjk3NS"
	"JSԴ����
	elseif &filetype == 'js'
		"ִ�е����ⲿ���������
		call g:Jsbeautify()
	else
		"��ʾ��Ϣ
		echo "��֧��".&filetype."�ļ����͡�"
	endif
	"������ǰ���������
	exec lineNum
endfunc
"ӳ���������������Shift+f��ݼ�
