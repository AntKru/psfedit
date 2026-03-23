project('psfedit', 'cpp', default_options : ['cpp_std=c++23'])

src_inc = include_directories('src')
deps = [
  dependency('readline'),
]

subdir('src')

executable(
    'psfedit',
    sources : src,
    dependencies: deps,
    include_directories : src_inc,
    install : true,
)
