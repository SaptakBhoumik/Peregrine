import sys

prefix = '\033[1;'
suffix = 'm'
foreground_colors = {
    'black': 30,
    'red': 31,
    'green': 32,
    'yellow': 33,
    'blue': 34,
    'magenta': 35,
    'cyan': 36,
    'default': 39,
    'light_gray': 37,
    'dark_gray': 90,
    'light_red': 91,
    'light_green': 92,
    'light_yellow': 93,
    'light_blue': 94,
    'light_magenta': 95,
    'light_cyan': 96,
    'white': 97
}
background_colors = {
    'black': 40,
    'red': 41,
    'green': 42,
    'yellow': 44,
    'blue': 44,
    'magenta': 45,
    'cyan': 46,
    'default': 49,
    'light_gray': 47,
    'dark_gray': 100,
    'light_red': 101,
    'light_green': 102,
    'light_yellow': 103,
    'light_blue': 104,
    'light_magenta': 105,
    'light_cyan': 106,
    'white': 107,
}
styles = {
    'bold': 1,
    'dim': 2,
    'underline': 4,
    'blink': 5,
    'reverse': 7,
    'hidden': 8,
}
reset = f'{prefix}0{suffix}'

def fg(text: str, colour: str):
    return f'{prefix}{foreground_colors[colour]}{suffix}{text}{reset}'

def bg(text: str, colour: str):
    return f'{prefix}{background_colors[colour]}{suffix}{text}{reset}'

def style(text: str, s: str):
    return f'{prefix}{styles[s]}{suffix}{text}{reset}'

class Location:
    def __init__(self, line: int, col: int, filename: str, code: str):
        self.line = line
        self.col = col
        self.filename = filename
        self.code = code


class PEError:
    def __init__(self, loc: Location, msg: str, submsg: str, hint: str, code: str):
        self.loc = loc
        self.msg = msg
        self.submsg = submsg
        self.hint = hint
        self.code = code
    
    def display(self):
        print("  ╭- " + fg(style(f'Error {"-" * 40} {self.loc.filename}:{self.loc.line}:{self.loc.col}', 'bold'), 'light_red'))
        print("  | " + fg(style(self.msg, 'bold'), 'light_red'))
        print("  |\n"*3, end="")
        print(f"{self.loc.line} | {self.loc.code[:len(self.loc.code)]}")
        print(f"  |{' '*self.loc.col}{fg('~', 'red')}", end="")
        print(fg(style(f' <----- {self.submsg}', 'bold'), 'light_red'))
        print("  |\n"*2, end="")
        
        if self.hint != "":
            print("  ├- " + fg(style("Try: ", "bold"), "light_blue") + self.hint)
            print("  |")
        
        print("  ╰- " + fg(style("Hint: ", "bold"), "cyan") + f"Use peregrine --explain={self.code}")

        code = int(self.code.strip("E"))
        sys.exit(code)


class PEWarning:
    def __init__(self, loc: Location, msg: str, submsg: str, hint: str, code: str):
        self.loc = loc
        self.msg = msg
        self.submsg = submsg
        self.hint = hint
        self.code = code
    
    def display(self):
        print("  ╭- " + fg(style(f'Warning {"-" * 40} {self.loc.filename}:{self.loc.line}:{self.loc.col}', 'bold'), 'yellow'))
        print("  | " + fg(style(self.msg, 'bold'), 'magenta'))
        print("  |\n"*3, end="")
        print(f"{self.loc.line} | {self.loc.code[:len(self.loc.code)]}")
        print(f"  |{' '*self.loc.col}{fg('~', 'light_yellow')}", end="")
        print(fg(style(f' <----- {self.submsg}', 'bold'), 'light_yellow'))
        print("  |\n"*2, end="")
        
        if self.hint != "":
            print("  ├- " + fg(style("Try: ", "bold"), "light_magenta") + self.hint)
            print("  |")
        
        print("  ╰- " + fg(style("Hint: ", "bold"), "cyan") + f"Use peregrine --explain={self.code}")
