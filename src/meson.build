pyro_sources = ['pyro.cpp',
                'pyrocolor.cpp', 
                'pyrographics.cpp', 
                'pyrographics_cairo.cpp',
                'pyroimage.cpp',
                'pyromath.cpp',
                'pyroshape.cpp',
                'pyrovector.cpp']

pyrolib = shared_library('pyro', 
                         pyro_sources, 
                          include_directories: inc_dir,
                          dependencies: [cairo_dep],
                          install: true)
