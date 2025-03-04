# Development guide

Note that any command described here assumes that you are in the repository's root directory.

## Extra dependencies

Some programs, not listed in [README.md](README.md), are not required to build the project
*per se*, but are very useful for developing it:

- [clang-format](https://clang.llvm.org) (code formatting);
- [cppcheck](https://cppcheck.sourceforge.io/) (code linting);
- [TeX Live](https://www.tug.org/texlive/) (`pdflatex` in specific, for report generation);
- [Valgrind](https://valgrind.org/) (profiling);
- [Kcachegrind](https://invent.kde.org/sdk/kcachegrind) (seeing profiler results);
- [ShellCheck](https://www.shellcheck.net/) (static analysis of scripts);
- [act](https://nektosact.com) (running GitHub actions locally).

## Other build types

Builds for debugging and profiling are available, by setting the environment variables `DEBUG` and
`PROFILE` to `1`, respectively.

```console
$ DEBUG=1 make
$ PROFILE=1 make
```

Don't forget to run `make clean` after switching between different build types.

## Report compilation

Developer-written reports can be compiled with:

```console
$ make reports
```

The output PDFs will be placed in the `build` directory.

## Profiling

The code can be profiled using `callgrind` + `kcachegrind`, both automatically started by the
`callgrind.sh` script:

```console
$ ./scripts/callgrind.sh
```

A `PROFILE` build is recommended.

## Code style

### C++ formatting

The `format.sh` script will attempt to format the whole project in a temporary directory (using
`clang-format`), and then compare the results with your original source files. You can choose
whether or not you want to keep those changes (you may want to manually disable formatting for a
snippet of code, for example). Keep in mind that there's a CI action that requires formatting to be
correct, so you can't just leave your source code unformatted.

```console
$ ./scripts/format.sh
```

As any other formatter configuration, there are many tunable parameters. For your code editor,
set the column limit to 100, the tab size to 4, with tab expansion on, and let `clang-format`
handle the rest.

#### Naming

Methods and variables should be named in `camelCase`. Type names (classes and enums) should be named
in `PascalCase`. The name of files containing a class must match the class's name, and directory
names should be in `snake_case`.

### LaTeX formatting

Report formatting can be checked with the `formatlatex.sh` script. It isn't as capable as
`clang-format`, so it only checks for the use of tabs, trailing whitespace, and columns with over
100 characters. There are no checks for alignment and all errors have to be fixed manually.

## Other scripts

- `todo.sh` - looks for the `TODO` string in all C++ sources and headers.

## GitHub Actions

The CI pipeline is very simple. C++ code is formatted, linted, and compiled; LaTeX code is formatted
and compiled; and shell scripts are checked with `shellcheck`. All of these can be run locally:

 - C++ compilation: `make`
 - C++ linting: `make cppcheck`
 - C++ formatting: `./scripts/format.sh`
 - LaTeX compilation: `make reports`
 - LaTeX formatting: `./scripts/formatlatex.sh`
 - Shell linting: `cd scripts && shellcheck **`

However, you may want to run the CI actions in a environment similar to the one of a GitHub runner.
For that, you can use [`act`](https://nektosact.com). However, you are sure to expect a longer
running time, as some software is not available on `act`'s default Ubuntu image and needs to be
installed.
