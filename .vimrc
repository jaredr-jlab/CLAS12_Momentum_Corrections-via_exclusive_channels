" Turn on syntax highlighting
syntax on

" Show line numbers
set number

" Disable swap files
set noswapfile

" Highlight search results, ignore case, and show results as you type
set hlsearch ignorecase incsearch

" Sets tab and shift to 4 spaces, and replaces tab with spaces
set softtabstop=4 shiftwidth=4 expandtab

" Turns vim's pre installed filetype plugins and indents accordingly
filetype plugin indent on

" Copies indetation of the previous line
" It does not interfere with other indent settings
set autoindent

" Set colorscheme
colo slate

" Set encoding that supports unicode
set encoding=utf-8

" Always display the status line
set laststatus=2

" Building the statusline
"function! GitBranch()
"  return system("git rev-parse --abbrev-ref HEAD 2>/dev/null | tr -d '\n'")
"endfunction

"function! StatuslineGit()
"  let l:branchname = GitBranch()
"  return strlen(l:branchname) > 0?'  '.l:branchname.' ':''
"endfunction

set statusline=
"set statusline+=%#DiffAdd#
"set statusline+=%{StatuslineGit()}       " Git branch status
set statusline+=%#Todo#
set statusline+=\ %<%F                   " Truncation point for status bar and File path
set statusline+=\ %#StatusLineTerm#
set statusline+=\ %y                     " Type of file in buffer
set statusline+=\ %#DiffText#
set statusline+=\ %m                     " Modified flag
set statusline+=\ %#ToolbarLine#
set statusline+=%=                       " Separation point for l/r alignment
set statusline+=\ row:%l/%L\ (%03p%%)    " Current row / total rows (percentage)
set statusline+=\ %#Pmenu#
set statusline+=\ col:%03c               " Column number
