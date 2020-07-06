# Sequence
Cross-platform (currently linux and windows) library to identify and handle list of names
having a numerical token as a collection.

![Linux C/C++ Build & Test](https://github.com/samson-jerome/sequence/workflows/Linux%20C/C++%20Build%20&%20Test/badge.svg)
![Windows C/C++ Build & Test](https://github.com/samson-jerome/sequence/workflows/Windows%20C/C++%20Build%20&%20Test/badge.svg)

# Inspiration
* Clique python module
* rvls utility from TweakSoftware
* fileseq - http://justinfx.com/gofileseq/cpp/index.html

# Dependencies
* C++11
* cmake-3.5
* fmt-6.0.0

# Roadmap
## v0.2.1
* refactor build (see spdlog build system) to use fmt:fmt from external
* remove fmt submodule
* add fmt as a bundled, driven by cmake options
* add Install
* add FindSequence.cmake

## v0.2.2
* refactor build add support for fmt::fmt from bundled source

## v0.3.0
* support padding
* more constructors
* Manipulating indexes, allow frame insertion/removal/update
* support multiple formatting
* full doc
* better coverage

## v0.4.0
* python binding

# backlog
* add flag to consider or ignore single frame collection
* support variable padding

# References
* https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#submod.root
* https://opensource.org/licenses/MIT
* https://exhale.readthedocs.io/en/latest/mastering_doxygen.html
* https://cristianadam.eu/20191222/using-github-actions-with-c-plus-plus-and-cmake/
* https://github.com/actions/starter-workflows/issues/53