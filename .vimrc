syntax on
set number
set incsearch
set nowrap
"TAB CONFIG
set showmatch "show matching bracket
set tabstop=8 "actual tab size
set softtabstop=8 "tab size seen while editing
"set expandtab
set smartindent
set shiftwidth=8 "indent size (same as tab if indents are done by tabing)
"FAT CURSOR
set guicursor=
"HIGHLIGHT UNWANTED SPACES
highlight ExtraWhitespace ctermbg=red guibg=red
match ExtraWhitespace /\s\+$/
"GET RID OF SWAPFILES
set noundofile
set noswapfile
set backupdir=~/.config/nvim/backupdir/
let mapleader = " "
nnoremap <leader>r :set relativenumber!<CR>
"Arrow Keys remapped to No Operation in Normal and Insert mode
noremap <Up> <Nop>
noremap <Down> <Nop>
noremap <Left> <Nop>
noremap <Right> <Nop>
noremap <Up> <Nop>
noremap <Down> <Nop>
noremap <Left> <Nop>
noremap <Right> <Nop>

call plug#begin("~/.config/nvim/plugged")
        Plug 'neoclide/coc.nvim', {'branch': 'release'}
	" support for JS, TS and JSX
	Plug 'pangloss/vim-javascript'
	Plug 'maxmellon/vim-jsx-pretty'
        " multiple cursors ctrl+n to select a word
        " n to jump to next occurrence, N to jump to previous occurrence
        Plug 'mg979/vim-visual-multi', {'branch': 'master'}
call plug#end()
