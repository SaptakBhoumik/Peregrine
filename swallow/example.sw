def name_one(str name):
    print(name)
def name_two(str : name):
    print(name)
def main():
    str : a="This is valid\n"
    str b="This is also valid\n"
    name_one(a)
    name_two(b)