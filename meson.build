project('Pyro', 'cpp',
        version: '0.0.1-alpha',
        license: 'GPLv3',
        default_options: ['cpp_std=c++17'])

freetype_dep = dependency('freetype2')
libjpeg_dep = dependency('libjpeg')
libpng_dep = dependency('libpng')
cairo_dep = dependency('cairo')
sdl_dep = dependency('sdl2')

catch2 = subproject('catch2')
eigen = subproject('eigen')

inc_dir = include_directories('include')

subdir('include')
subdir('src')
subdir('tests')
subdir('screenshot-tests')

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries: pyrolib,
                 version: '0.0.1-alpha',
                 name: 'libpyro',
                 filebase: 'libpyro',
                 description: 'A library to generate graphics'
                 )
