map <F8> :wa<CR>:!make run<CR>
map <F9> :wa<CR>:!make debug<CR>
set wildignore+=*.d
au BufRead *.d set ft=make
