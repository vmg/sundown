cxx_library(
  name = 'sundown',
  header_namespace = '',
  exported_headers = subdir_glob([
    ('src', '**/*.h'),
    ('html', '**/*.h'),
  ]),
  srcs = glob([
    'src/**/*.c',
    'html/**/*.c',
  ]),
  visibility = [
    'PUBLIC',
  ],
)
