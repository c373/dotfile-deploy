call plug#begin('~/.vim/plugged')
"Plug 'vim-airline/vim-airline'
"Plug 'vim-airline/vim-airline-themes'
Plug 'preservim/nerdtree'
"Plug 'tpope/vim-fugitive'
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'ryanoasis/vim-devicons'
call plug#end()

source ~/.config/nvim/statusline.vim

" let g:airline_powerline_fonts=1
" let g:airline_theme='deus'
" let g:airline_section_b='%{StatuslineGit()}'
" let g:airline_section_z='%4p%%%5.5l/%.10L : %-5v'
" let g:airline_section_warning='%{StatusDiagnostic("warning")}'
" let g:airline_section_error='%{StatusDiagnostic("error")}'
"end statusline config


autocmd VimEnter * if &filetype !=# 'gitcommit' | NERDTree | wincmd p

" Exit Vim if NERDTree is the only window remaining in the only tab.
autocmd BufEnter * if tabpagenr('$') == 1 && winnr('$') == 1 && exists('b:NERDTree') && b:NERDTree.isTabTree() | quit | endif

set number
