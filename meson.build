project('cslop', 'c', version: '0.1.2')

binary_name = 'cslop'
binary_version = '0.1.2'
binary_author = 'Keiran'

configure_file(
  input: 'version.h.in',
  output: 'version.h',
  configuration: {
    'BINARY_NAME': binary_name,
    'BINARY_VERSION': binary_version,
    'BINARY_AUTHOR': binary_author,
  }
)

inc_dir = include_directories('include')

src_files = [
  'src/main.c',
  'src/cli/cli.c',
  'src/cli/format.c',
  'src/cli/archive.c',
]

executable(
  binary_name, 
  src_files,
  include_directories: inc_dir,
  install: true
)
