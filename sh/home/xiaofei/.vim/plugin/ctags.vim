""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" CTAGS settings for vim           
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
if $CTAGS_DB != ""
let dbs = split($CTAGS_DB, ',')
	for i in dbs
		"echo i
		let cmd = "set tags+=" . i
		exec cmd
	endfor
endif
