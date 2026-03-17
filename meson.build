project('psfedit', 'cpp', default_options : ['cpp_std=c++23'])

src_inc = include_directories('src')

subdir('src')

executable(
    'psfedit',
    sources : src,
    include_directories : src_inc,
    install : true,
)
