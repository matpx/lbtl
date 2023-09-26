project('lbtl', ['c', 'cpp'])
executable(
    'lbtl',
    [
        'src/main.cpp',
        'src/impl.cpp',
        'src/renderer.cpp',
        'src/renderer.hpp',
        'src/flecs/flecs.h',
        'src/flecs/flecs.c'
        ],
    dependencies: [
        dependency('GL'), dependency('X11'), dependency('xi'), dependency('xcursor')
    ],
    cpp_args : ['-DHANDMADE_MATH_NO_SSE', '-DHANDMADE_MATH_USE_TURNS'],
)
