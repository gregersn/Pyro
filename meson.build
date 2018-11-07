project('Pyro', 'cpp',
        license: 'GPLv3',
        default_options: ['cpp_std=c++17'])

add_global_link_arguments('-lfreeimage', language: 'cpp')

cairo_dep = dependency('cairo')
inc_dir = include_directories('include')

subdir('include')
subdir('src')
subdir('tests')
