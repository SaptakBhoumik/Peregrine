def name_one(str name):
    print(name)
def name_two(str : name):
    print(name)


def main():
    str : a="This is valid\n"
    str b="This is also valid\n"
    colorprint("BLUE\n","BLUE")
    name_one(a)
    name_two(b)
    float arg_one=58.8
    float arg_two=10
    int round_of=arg_one//arg_two
    printf( "%g // %g = %lld\n", arg_one, arg_two, round_of)