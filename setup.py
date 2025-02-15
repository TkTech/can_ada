from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

__version__ = "2.0.0"

setup(
    name="can_ada",
    version=__version__,
    author="Tyler Kennedy",
    author_email="tk@tkte.ch",
    long_description_content_type="text/markdown",
    long_description=open("README.md").read(),
    url="https://github.com/tktech/can_ada",
    description="Ada is a fast spec-compliant url parser",
    license="MIT",
    packages=["can_ada-stubs"],
    package_data={"can_ada-stubs": ["__init__.pyi"]},
    ext_modules=[
        Pybind11Extension(
            "can_ada",
            sorted(["src/binding.cpp", "src/ada.cpp"]),
            define_macros=[("VERSION_INFO", __version__)],
            language='c++',
            cxx_std=17
        ),
    ],
    extras_require={
        "test": [
            "pytest",
            "pytest-benchmark",
            "ada_url",
        ]
    },
    zip_safe=False,
    python_requires=">=3.9",
)
