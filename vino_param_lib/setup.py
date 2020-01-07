from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

# fetch values from package.xml
setup_args = generate_distutils_setup(
    packages=['vino_param_lib'],
    package_dir={'': 'src/python_wrapper'})

setup(**setup_args)