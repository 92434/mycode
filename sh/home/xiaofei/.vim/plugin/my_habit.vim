""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" My habit		 
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

"ignorecase
"set ic

au FileType c setlocal sw=8 ts=8 sts=8
au FileType cpp setlocal sw=8 ts=8 sts=8
au FileType java setlocal sw=4 ts=4 sts=4
au FileType python setlocal sw=4 ts=4 sts=4 et
au FileType html setlocal sw=4 ts=4 sts=4 et
au FileType xhtml setlocal sw=4 ts=4 sts=4 et
au FileType css setlocal sw=4 ts=4 sts=4 et
au FileType htmldjango setlocal sw=4 ts=4 sts=4 et

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
"nmap <S-F> <Esc>:call CodeFormat()<CR>
nmap <silent> <leader>ff <Esc>:call CodeFormat()<CR>

function! Path()
	":echo substitute(expand("%:p:h"), ".*", "\\U\\0", "")
	"echo expand("%:p")
	:!echo "%:p"
	:let @+ = expand("%:p")
endfunction

function! UpdateFile()
	:e
endfunction


"调用AStyle程序，进行代码美化
func CodeFormat()
	"取得当前光标所在行号
	let lineNum = line(".")
	"C源程序
	if &filetype == 'c'
		"执行调用外部程序的命令
		exec "%! astyle -A8Lfpjk3NST"
	"H头文件(文件类型识别为cpp)，CPP源程序
	elseif &filetype == 'cpp'
		"执行调用外部程序的命令
		exec "%! astyle -A8Lfpjk3NST"
	"JAVA源程序
	elseif &filetype == 'java'
		"执行调用外部程序的命令
		exec "%! astyle -A2Lfpjk3NS"
	"JS源程序
	elseif &filetype == 'javascript'
		"执行调用外部程序的命令
		call JsBeautify()
	elseif &filetype == 'html'
		"执行调用外部程序的命令
		call HtmlBeautify()
	elseif &filetype == 'xhtml'
		"执行调用外部程序的命令
		call HtmlBeautify()
	elseif &filetype == 'css'
		"执行调用外部程序的命令
		call CSSBeautify()
	else
		"提示信息
		echo "不支持".&filetype."文件类型。按c类型处理-_-"
		exec "%! astyle -A8Lfpjk3NST"
	endif
	"返回先前光标所在行
	exec lineNum
endfunc
"映射代码美化函数到Shift+f快捷键
