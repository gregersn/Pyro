pyro_sources = ['pyro.cpp',
                'pyro_runner.cpp',
                'pyro_sdl.cpp',
                'pyrocolor.cpp', 
                'pyrofont.cpp',
                'pyrofont_impl.cpp',
                'pyrographics.cpp', 
                'pyrographics_cairo.cpp',
                'pyroimage.cpp',
                'pyromath.cpp',
                'pyroshape.cpp',
                'pyrovector.cpp',
                'pyronoise.cpp',
                'pyrostring.cpp',
                'pyrotransformer.cpp',
                'opensimplexnoise.cpp']

pyrolib = shared_library('pyro', 
                         pyro_sources, 
                          include_directories: inc_dir,
                          dependencies: [libpng_dep, cairo_dep, libjpeg_dep, freetype_dep, sdl_dep, catch2.get_variable('catch2_dep')],
                          install: true)
