project('gnome-animated-background-xml', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

utest = dependency('criterion')
glibdep = dependency('glib-2.0')
src = ['src/input_parser.c', 'src/xml_creator.c']


executable('gnome-animated-background-xml',
           'main.c',
           install : false,
           dependencies : glibdep,
           sources : src)

tests = executable('tests',
	'tests.c',
	install : false,
	dependencies : utest
	sources : src)

test('works?', tests)