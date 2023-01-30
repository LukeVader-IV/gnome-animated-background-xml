project('gnome-animated-background-xml', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

glibdep = dependency('glib-2.0')
src = ['src/input_parser.c', 'src/xml_creator.c']

executable('gnome-animated-background-xml',
           'main.c',
           install : false,
           dependencies : glibdep,
           sources : src)
