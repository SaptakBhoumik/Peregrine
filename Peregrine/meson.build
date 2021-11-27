lexer_src = [
    'lexer/lexer.cpp',
    'lexer/tokens.cpp'
]

parser_src = [
    'parser/parser.cpp',
    'parser/literals.cpp'
]

ast_src = [
    'ast/ast.cpp',
    'ast/types.cpp',
    'ast/visitor.cpp'
]

analyzer_src = [
    'analyzer/typeChecker.cpp'
]

codegen_src = [
    'codegen/cpp/codegen.cpp',
    'codegen/cpp/utils.cpp'
]

lexer = static_library('lexer', sources: lexer_src)
parser = static_library('parser', sources: parser_src)
ast = static_library('ast', sources: ast_src)
analyzer = static_library('analyzer', sources: analyzer_src)
codegen = static_library('codegen', sources: codegen_src)