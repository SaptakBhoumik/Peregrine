// Original author: MD Gaziur Rahman Noor
/* Tests for _format() builtin function */
#include<assert.h>
#include "../../Peregrine/builtin.h"

int main() {
    assert(strcmp(_format("Hello {}!", "world"), "Hello world!") == 0);
    assert(strcmp(_format(""), "") == 0);
    assert(strcmp(_format("{", "someRandomString"), "{") == 0);
    assert(strcmp(_format("Hello"), "Hello") == 0);
    assert(strcmp(_format("{}, welcome!", "someRandomPerson"), "someRandomPerson, welcome!") == 0);
    assert(strcmp(_format("{}", "format1"), "format1") == 0);
}
