import os

fn main() {
    filename := "parser/parser.v"
    content := os.read_file(filename) or {
        panic("failed to read $filename")
    }
    println(content)
    println(os.args)//gives the location do i can add the built i file location
}