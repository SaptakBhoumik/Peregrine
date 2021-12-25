test_src = [
    '../Peregrine/errors/errors.cpp',
    'compiler/lexer_test.cpp',
    'compiler/main.cpp'
]

include = include_directories('../Peregrine/')

run_command('wget',  '-nc',  'https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h', check: true)

exe = executable(
    'compiler_test.elf', 
    sources: test_src, 
    include_directories: include,
    link_with: lexer
)

test('Test the compiler', exe)