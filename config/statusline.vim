" statusline config
"let g:l_sep=""
let g:l_sep=""
"let g:r_sep=""
let g:r_sep=""

let g:msg=0

let NERDTreeStatusline='%#nerdColor# %{getcwd()} %#nerdSep#%{l_sep} '
let g:e="error"
let g:w="warning"

set statusline=%#color1#\ \ %{toupper(g:currentmode[mode()])}\ %#color1_2#%{l_sep}	" Mode
set statusline+=%#color2#%{StatusGit()}%#color2_3#%{l_sep}				" GitBranch
set statusline+=%#color3#
set statusline+=%<\ %f\ %m\ %r\ %h\ %w\ 					" File path, modified, readonly, helpfile, preview
set statusline+=%=%#color4_3#%{r_sep}%#color4#\ %{&fenc}\ 				" File encoding 
set statusline+=%#color4_2#%{r_sep}%#color2#\ %Y\ 					" Filetype
set statusline+=%#color1_2#%{r_sep}%#color1#\ %3p%%%5.5l/%.5L\ :\ %-3v\ 		" Percent, Lines, Column
set statusline+=%#w_sep#%{r_sep}%#warning#%{StatusDiagnostic(w)}			" Warning
set statusline+=%#w_error#%{r_sep}%#error#%{StatusDiagnostic(e)}%#end#%{l_sep}		" Error

hi color1	ctermfg=023 ctermbg=115
hi color1_2	ctermfg=115 ctermbg=072
hi color2	ctermfg=015 ctermbg=072
hi color2_3	ctermfg=072
hi color3	ctermfg=015
hi color3_mod	ctermfg=222
hi color4	ctermfg=015 ctermbg=239
hi color4_2	ctermfg=072 ctermbg=239
hi color4_3	ctermfg=239
hi info_sep	ctermfg=115
hi w_sep	ctermfg=222 ctermbg=115
hi warning	ctermfg=130 ctermbg=222
hi w_error	ctermfg=203 ctermbg=222
hi error	ctermfg=088 ctermbg=203
hi end		ctermfg=203

hi nerdColor	ctermfg=057 ctermbg=147
hi nerdSep	ctermfg=147

let g:currentmode={
    \ 'n'  : 'Normal',
    \ 'no' : 'Normal·Operator Pending',
    \ 'v'  : 'Visual',
    \ 'V'  : 'V·Line',
    \ '^V' : 'V·Block',
    \ 's'  : 'Select',
    \ 'S'  : 'S·Line',
    \ '^S' : 'S·Block',
    \ 'i'  : 'Insert',
    \ 'R'  : 'Replace',
    \ 'Rv' : 'V·Replace',
    \ 'c'  : 'Command',
    \ 'cv' : 'Vim Ex',
    \ 'ce' : 'Ex',
    \ 'r'  : 'Prompt',
    \ 'rm' : 'More',
    \ 'r?' : 'Confirm',
    \ '!'  : 'Shell',
    \ 't'  : 'Terminal'
    \}

" functions

function! StatusDiagnostic(type) abort
  let info = get(b:, 'coc_diagnostic_info', {})
  if empty(info) | return '' | else | g:msg=1 | endif
  let msgs = []
  
  let is_err = a:type == 'error'

  if get(info, a:type, 0)
    call add(msgs, (is_err ? 'E:' : 'W:') . info[a:type])
    call add(msgs, '(L' . (info.lnums)[is_err ? 0 : 1] . ')')
  endif
  let l:diags = join(msgs, '')
  return strlen(l:diags) > 0 ? ('   ' . l:diags . ' ') : ''
endfunction

function! GitBranch()
  return system("git rev-parse --abbrev-ref HEAD 2>/dev/null | tr -d '\n'")
endfunction

function! StatusGit() abort
  let l:branchname = GitBranch()
  return strlen(l:branchname) > 0 ? ('  '.l:branchname.' ') : ''
endfunction

function! Modified()
	if &modified | return '%#color3_mod#' | else | return '%#color3#' | endif
endfunction
