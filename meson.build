project('Pyro', 'cpp',
        version: '0.0.1-alpha',
        license: 'GPLv3',
        default_options: ['cpp_std=c++17'])

project_description = 'A graphics library for creative coding.'

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

pyro_dep = declare_dependency(include_directories: inc, link_with: pyro)


pkg_mod = import('pkgconfig')
pkg_mod.generate(name: meson.project_name(),
                 filebase: meson.project_name(),
                 description: project_description,
                 subdirs: meson.project_name(),
                 libraries: pyro,
                 )

# Tests

if not meson.is_subproject()
        subdir('tests')
        subdir('screenshot-tests')
endif