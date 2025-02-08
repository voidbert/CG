# Development guide

Note that any command described here assumes that you are in the repository's root directory.

## Extra dependencies

Some programs, not listed in [README.md](README.md), are not required to build the project
*per se*, but are very useful for developing it:

- [clang-format](https://clang.llvm.org) (code formatting);
- [TeX Live](https://www.tug.org/texlive/) (`pdflatex` in specific, for report generation);
- [Valgrind](https://valgrind.org/) (profiling);
- [Kcachegrind](https://invent.kde.org/sdk/kcachegrind) (seeing profiling results);
- [ShellCheck](https://www.shellcheck.net/) (static analysis of scripts);
- [act](https://nektosact.com) (running GitHub actions locally).

## Other build types

Builds for debugging and profiling are available, by setting the environment variables `DEBUG` and
`PROFILE` to `1`, respectively.

```console
$ DEBUG=1 make
$ PROFILE=1 make
```

Don't forget to run `make clean` between different build types.

## Report compilation

Developer-written reports can be compiled with:

```console
$ make report
```

## Profiling

The code can be profiled using `callgrind` + `kcachegrind`, both automatically started by the
`callgrind.sh` script:

```console
$ ./scripts/callgrind.sh
```

A `PROFILE` build is recommended.

## Code style

### Formatting

The `format.sh` script will attempt to format the whole project to a temporary directory (using
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

### Naming

Methods and variables should be named in `camelCase`. Type names (classes and enums) should be named
in `PascalCase`.

## Other scripts

- `todo.sh` - looks for the `TODO` string in all C++ sources and headers.

## GitHub Actions

The CI pipeline is very simple: it checks if the code is correctly formatted, and it builds and
lints the project. `shellcheck` verification of scripts is also present. All of these can be run
locally, without any containerization.

However, you may want to run the CI actions in a environment similar to the one in a GitHub runner,
for example, to use the same version of `clang-format`. Our actions are compatible with
[`act`](https://nektosact.com). However, you are sure to expect a longer running time, as
`clang-format`, not available on `act`'s default Ubuntu image, needs to be installed.
