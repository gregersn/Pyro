project('Pyro', 'cpp',
        license: 'GPLv3',
        default_options: ['cpp_std=c++17'])

add_global_link_arguments('-lfreeimage', language: 'cpp')

cairo_dep = dependency('cairo')
sdl_dep = dependency('sdl2')
inc_dir = include_directories('include')
catch2 = subproject('catch2')

subdir('include')
subdir('src')
subdir('tests')
