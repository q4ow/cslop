project('cslop', 'c', version: '0.1.0')

binary_name = 'cslop'
binary_version = '0.1.0'
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

executable(binary_name, 'src/main.c', install: true)
