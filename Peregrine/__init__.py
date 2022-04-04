import os

lexer_src = [
    'lexer/lexer.cpp',
    'lexer/tokens.cpp'
]
error_src = ['errors/errors.cpp']
parser_src = [
    'parser/parser.cpp',
    'parser/utils.cpp',
    'parser/literals.cpp'
]

ast_src = [
    'ast/ast.cpp',
    'ast/types.cpp',
    'ast/visitor.cpp'
]

doc_src = [
    'docgen/html/docgen.cpp'
]

analyzer_src = [
    'analyzer/typeChecker.cpp',
    'analyzer/ast_validate.cpp'
]

codegen_src = [
    'codegen/cpp/codegen.cpp',
    'codegen/cpp/utils.cpp',
    'codegen/js/codegen.cpp',
    'codegen/js/utils.cpp'
]
cli_src = [
    'cli/cli.cpp'
]
main_src = ["main.cpp"]
all_src = (lexer_src + parser_src + error_src + ast_src + 
    doc_src + analyzer_src + codegen_src + cli_src+main_src)
