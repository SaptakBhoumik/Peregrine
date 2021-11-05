#include "doctest.h"

#include <vector>
#include <lexer/lexer.hpp>
#include <lexer/tokens.hpp>

TEST_CASE("Tokenize basic primitives") {
    std::vector<Token> res;

    res = lexer("69420", "");
    CHECK(res[0].tk_type == tk_integer);
    CHECK(res[0].keyword == "69420");

    res = lexer("5.32006", "");
    CHECK(res[0].tk_type == tk_decimal);
    CHECK(res[0].keyword == "5.32006");

    res = lexer("\"A blazing fast language\"", "");
    CHECK(res[0].tk_type == tk_string);
    CHECK(res[0].keyword == std::string("A blazing fast language"));

    res = lexer("True", "");
    CHECK(res[0].tk_type == tk_true);
}

TEST_CASE("Tokenize variable declarations") {
    std::vector<Token> res = lexer("int test = 23", "");
    
    REQUIRE(res.size() == 5);

    CHECK(res[0].tk_type == tk_identifier); 
    CHECK(res[1].tk_type == tk_identifier);
    CHECK(res[2].tk_type == tk_assign);
    CHECK(res[3].tk_type == tk_integer);
    CHECK(res[4].tk_type == tk_eof);
}

TEST_CASE("Tokenize if-else statements") {
    std::vector<Token> res = lexer("if test:\n    print(\"true!\")\nelse:\n    print(\"false!\")", "");

    REQUIRE(res.size() == 18);

    SUBCASE("Emit ident and dedent tokens") {
        CHECK(res[3].tk_type == tk_ident);
        CHECK(res[8].tk_type == tk_dedent);
        CHECK(res[11].tk_type == tk_ident);
        CHECK(res[16].tk_type == tk_dedent);
    }

    SUBCASE("Emit if and else tokens") {
        CHECK(res[0].tk_type == tk_if);
        CHECK(res[2].tk_type == tk_colon);

        CHECK(res[9].tk_type == tk_else);
        CHECK(res[10].tk_type == tk_colon);
    }
}
