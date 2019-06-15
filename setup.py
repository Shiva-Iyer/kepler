# setup.py - PyPI package creation.
# Copyright (C) 2019 Shiva Iyer <shiva.iyer AT g m a i l DOT c o m>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import setuptools

with open("README.md", "r") as fh:
    readme = fh.read()

setuptools.setup(
    name = "pykepler",
    version = "1.0.1",
    author = "Shiva Iyer",
    author_email = "shiva.iyer@gmail.com",
    description = "Algorithms for positional astronomy",
    long_description = readme,
    long_description_content_type = "text/markdown",
    url = "https://github.com/Shiva-Iyer/kepler",
    packages = ["pykepler"],
    include_package_data = True,
    classifiers = [
        "Intended Audience :: Developers",
        "Intended Audience :: Education",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)",
        "Operating System :: POSIX",
        "Operating System :: POSIX :: Linux",
        "Programming Language :: C",
        "Programming Language :: Python :: 3",
        "Topic :: Scientific/Engineering :: Astronomy",
        "Topic :: Scientific/Engineering :: Mathematics",
        "Topic :: Scientific/Engineering :: Physics"
    ],
)
