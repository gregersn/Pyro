project('Pyro', 'cpp',
        version: '0.0.1-alpha',
        license: 'GPLv3',
        default_options: ['cpp_std=c++17'])

freetype_dep = dependency('freetype2')
libjpeg_dep = dependency('libjpeg')
libpng_dep = dependency('libpng')
cairo_dep = dependency('cairo')
sdl_dep = dependency('sdl2')
eigen_dep = dependency('eigen3')

eigen_dep = subproject('eigen').get_variable('eigen_dep')

inc_dir = include_directories('include')

subdir('include')
subdir('src')
subdir('tests')
subdir('screenshot-tests')

libpyro_dep = declare_dependency(include_directories: inc_dir, link_with: pyrolib)

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries: pyrolib,
                 version: '0.0.1-alpha',
                 name: 'libpyro',
                 filebase: 'libpyro',
                 description: 'A library to generate graphics'
                 )
