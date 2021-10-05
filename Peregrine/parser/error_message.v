module parser

// Original Author(chalk): etienne-napoleone
const (
	prefix            = '\e['
	suffix            = 'm'
	foreground_colors = {
		'black':         30
		'red':           31
		'green':         32
		'yellow':        33
		'blue':          34
		'magenta':       35
		'cyan':          36
		'default':       39
		'light_gray':    37
		'dark_gray':     90
		'light_red':     91
		'light_green':   92
		'light_yellow':  93
		'light_blue':    94
		'light_magenta': 95
		'light_cyan':    96
		'white':         97
	}
	background_colors = {
		'black':         40
		'red':           41
		'green':         42
		'yellow':        44
		'blue':          44
		'magenta':       45
		'cyan':          46
		'default':       49
		'light_gray':    47
		'dark_gray':     100
		'light_red':     101
		'light_green':   102
		'light_yellow':  103
		'light_blue':    104
		'light_magenta': 105
		'light_cyan':    106
		'white':         107
	}
	style = {
		'bold':      1
		'dim':       2
		'underline': 4
		'blink':     5
		'reverse':   7
		'hidden':    8
	}
	reset = '${prefix}0$suffix'
)

pub fn fg(text string, color string) string {
	return '$parser.prefix${parser.foreground_colors[color]}$parser.suffix$text$parser.reset'
}

pub fn bg(text string, color string) string {
	return '$parser.prefix${parser.background_colors[color]}$parser.suffix$text$parser.reset'
}

pub fn style(text string, color string) string {
	return '$parser.prefix${parser.style[color]}$parser.suffix$text$parser.reset'
}

// Original Author(errors): Ethan Olchik
pub struct SWError {
pub mut:
	loc    Location
	msg    string
	submsg string
	hint   string
	ecode  Code
}

pub struct Code {
	code string
}

pub struct Location {
	line int
	col  int
	file string
	code string
}

pub fn (e SWError) display() {
	println('  ╭- ' +
		fg(style('Error ${'-'.repeat(40)} $e.loc.file:$e.loc.line:$e.loc.col', 'bold'), 'light_red'))
	println('  | ' + fg(style(e.msg, 'bold'), 'light_red'))
	print('${'  |\n'.repeat(3)}')

	println('$e.loc.line | ' + e.loc.code[..e.loc.code.len])
	print('  |${' '.repeat(e.loc.col)}${fg('~', 'red')}')
	println(fg(style(' <----- $e.submsg', 'bold'), 'light_red'))
	print('${'  |\n'.repeat(2)}')
	if e.hint != '' {
		println('  ├- ' + fg(style('Try: ', 'bold'), 'light_blue') + e.hint)
		println('  |')
	}

	println('  ╰- ' + fg(style('Hint: ', 'bold'), 'cyan') + 'Use Peregrine --explain=$e.ecode.code')

	exit(-1)
}
