" vim config
set nocompatible
syntax on
set showmode
set showcmd
set encoding=utf-8
set t_Co=256
filetype indent on

" tab space
set softtabstop=4
set tabstop=4
set shiftwidth=4
set expandtab
set autoindent

set number
set cursorline

" plugin
call plug#begin('~/.vim/plugged')

Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'tomasiser/vim-code-dark'
Plug 'rhysd/vim-clang-format'
Plug 'jiangmiao/auto-pairs'
Plug 'Yggdroot/indentLine'

call plug#end()

colorscheme codedark

let g:clang_format#code_style='WebKit'

" Make <CR> to accept selected completion item or notify coc.nvim to format
" <C-g>u breaks current undo, please make your own choice.
inoremap <silent><expr> <CR> coc#pum#visible() ? coc#pum#confirm()
                              \: "\<C-g>u\<CR>\<c-r>=coc#on_enter()\<CR>"

