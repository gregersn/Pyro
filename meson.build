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

inc = include_directories('include')

subdir('include')
subdir('src')
subdir('tests')
subdir('screenshot-tests')

pyro_dep = declare_dependency(include_directories: inc, link_with: pyro)

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries: pyro,
                 version: '0.0.1-alpha',
                 name: 'pyro',
                 filebase: 'pyro',
                 description: 'A library to generate graphics'
                 )
