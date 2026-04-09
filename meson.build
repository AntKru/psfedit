project('psfedit', 'cpp',
    default_options : ['cpp_std=c++23'],
    license : 'GPL-3.0-only',
    license_files : 'LICENSE',
    version: run_command('scripts/version.sh', 'get_vcs', check : true).stdout().strip(),
)

meson.add_dist_script('scripts/version.sh', 'set_dist', meson.project_version())

src_inc = include_directories('src')
deps = [
  dependency('readline'),
  dependency('ncurses'),
]

subdir('src')

config = configuration_data()
config.set_quoted('version', meson.project_version())
configure_file(
    input : 'config.h.in',
    output : 'config.h',
    configuration : config,
)

executable(
    'psfedit',
    sources : src,
    dependencies: deps,
    include_directories : src_inc,
    install : true,
)
