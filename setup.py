from setuptools import setup, Extension

setup(
    name="c_pi",
    ext_modules = [
        Extension('c_pi', ['c_pi.c'],
                  extra_compile_args=['-g', '-O3']
        )
    ],
)
