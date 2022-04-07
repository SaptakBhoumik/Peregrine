project('peregrine', 'cpp', version: '0.1')

cpp_src = [
    'Peregrine/main.cpp',
    'Peregrine/errors/errors.cpp'
]

include = include_directories('Peregrine/')

add_project_arguments('-std=c++17', language: 'cpp')

build_tests = get_option('build_tests')

subdir('Peregrine/')

executable(
    'peregrine.elf',
    sources: cpp_src, 
    include_directories: include,
    link_with: [lexer, parser, ast, analyzer, codegen,docgen,cli,utils]
)

if build_tests
    subdir('tests/')
endif
