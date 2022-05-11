lexer_src = [
    'lexer/lexer.cpp',
    'lexer/tokens.cpp'
]

parser_src = [
    'parser/parser.cpp',
    'parser/utils.cpp',
    'parser/literals.cpp',
    'parser/types.cpp',
    'parser/expression.cpp',
    'parser/statement.cpp',
    'parser/define.cpp',
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
utils_src = [
    'utils/symbolTable.cpp'
]
#TODO: Also link the linker
lexer = static_library('lexer', sources: lexer_src)
parser = static_library('parser', sources: parser_src)
ast = static_library('ast', sources: ast_src)
analyzer = static_library('analyzer', sources: analyzer_src)
codegen = static_library('codegen', sources: codegen_src)
cli = static_library('cli', sources: cli_src)
docgen = static_library('docgen',sources:doc_src)
utils = static_library('utils',sources:utils_src)
