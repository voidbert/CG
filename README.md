# CG

UMinho's Computer Graphics 2024/25 assignment. See [Assignment.pdf](Assignment.pdf) for more
details.

## Authors

 - Ana Oliveira (A104437)
 - Humberto Gomes (A104348)
 - Mariana Rocha (A90817)
 - Sara Lopes (A104179)

## Setup

### Building

Start by cloning this repository:

```
$ git clone https://github.com/voidbert/CG.git
```

First, you'll need to install the following dependencies:

- [GNU Make](https://www.gnu.org/software/make/) (build-time);
- [GCC](https://www.gnu.org/software/gcc/) / [Clang](https://clang.llvm.org/) (build-time);
- [GLFW](https://www.glfw.org/) (build-time);
- [GLM](https://github.com/g-truc/glm) (build-time);
- [TinyXML 2](https://github.com/leethomason/tinyxml2) (build-time).

Other libraries are already included in this repository:

 - [GLAD](https://glad.dav1d.de/).

Then, to compile the project, run:

```console
$ make
```

Build artifacts can be removed with:

```console
$ make clean
```

### Installation

After [building](#building), the program can be installed by running:

```console
$ make install
```

`$PREFIX` can be overridden, to install the program in another location:

```console
# PREFIX=/usr make install
```

The program can also be uninstalled. Just make sure you use the same `$PREFIX` you used for
installation:

```console
$ make uninstall
```

## Developers

As a university project, external contributors aren't allowed.
All contributors must read the [DEVELOPERS.md](DEVELOPERS.md) guide.
