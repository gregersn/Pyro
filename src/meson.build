cpp_args = ['-Wall', '-Wextra', '-Wpedantic',]

pyro_sources = ['pyro.cpp',
                'runner.cpp',
                'sdl.cpp',
                'color.cpp', 
                'font.cpp',
                'font_impl.cpp',
                'graphics.cpp', 
                'graphics_cairo.cpp',
                'image.cpp',
                'math.cpp',
                'shape.cpp',
                'vector.cpp',
                'noise.cpp',
                'string.cpp',
                'transformer.cpp',
                'opensimplexnoise.cpp']

pyro = library(meson.project_name(), 
                  pyro_sources,
                  install: true, 
                  cpp_args: cpp_args,
                  include_directories: inc,
                  dependencies: [libpng_dep, cairo_dep, libjpeg_dep, freetype_dep, sdl_dep, eigen_dep],
                  )
