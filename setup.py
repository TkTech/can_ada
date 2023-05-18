from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

__version__ = "1.0.3"

setup(
    name="can_ada",
    version=__version__,
    author="Tyler Kennedy",
    author_email="tk@tkte.ch",
    url="https://github.com/tktech/can-ada",
    description="Ada is a fast spec-compliant url parser",
    ext_modules=[
        Pybind11Extension(
            "can_ada",
            sorted(["src/binding.cpp", "src/ada.cpp"]),
            define_macros=[("VERSION_INFO", __version__)],
            language='c++',
            cxx_std=17
        ),
    ],
    package_data={
        "can_ada": [
            "py.typed",
            "can_ada.pyi"
        ]
    },
    extras_require={
        "test": "pytest"
    },
    zip_safe=False,
    python_requires=">=3.7",
)
