# project construction made easy

In this text, you find tips help you start with building *C++* programs using [SCons](https://scons.org) project construction tool under *Linux*.

> *SCons* package can be installed with `sudo apt install scons ` command

### content

- [basics](#basics)
	- [compiler settings](#compiler-settings)
	- [more source files](#more-source-files)
- [third party dependencies](#third-party-dependencies)
- [static library](#static-library)


## basics

*SCons* is super easy to start with you need to know only three thinks. The first one, `scons` command to start a building process (from project root directory), `scons -c` command for project clean-up and that *SCons* build file is called `SConstruct`. The most simplest `SConstruct` file can looks like this

```python
Program('main.cpp')
```

Yes, **that is all**! I've told it is super easy. *SCons* will take `main.cpp` source file and create `main` executable from it. We are now ready to build something, go to `basic/` directory and execute `scons` by

```bash
cd basic
scons
```

commands to see *SCons* in action. The `main` output should looks like this

```bash
./main 
done!
```

> don't forget to test `scons -c` command to clean-up the sample directory

### compiler settings

Let's say we want to force building against *C++17* standard and also turn on all compiler warnings. To do that, we can adjust compiler options with `Environment` variable this way

```
cpp17 = Environment(CCFLAGS=['-std=c++17', '-Wall'])
cpp17.Program('main.cpp')
```

> go to `environment/` directory and run `scons` to build `main` executable to see *SCons* with modified environment in action 

There is not only `CCFLAGS` construction variable there but many others like

- `CCFLAGS` general options passed to the c and c++ compilers
- `CPPPATH` the list of directories that the c preprocessor will search for include directories.
- `LIBS` list of libraries that will be linked
- `LIBPATH` the list of directories that will be searched for libraries (use `#` to indicate absolute path `LIBPATH='#/libs'`)
- `CPPDEFINES` c/c++ preprocessor definitions

> full list of construction variables can be found [there](https://www.scons.org/doc/1.2.0/HTML/scons-user/a4774.html)

> **tip**: we don't need to remember all that construction variable names, there is a shortcut there we can use `parse_flags` argument this way `Environment(parse_flags='-std=c++17 -Wall -g -O0')`

> **advanced tip**: building with *clang* instead of *gcc* can be done by setting `CXX` construction variable to `CXX='clang++'`


### more source files

In some cases there are more source files there like in [3dmath](3dmath/) sample (we have `main.cpp` and `vectors.cpp` source files there) we can use *Python's* [list][sequence] instead of single [string][string] in a [`Program()`][Program] builder like this

```python
Program(['main.cpp', 'vector.cpp'])
```

We can makes our lives even easier with [`Glob(PATTERN)`][Glob] helper function and build all *cpp* files in a directory with

```python
Program('3dmath', Glob('*.cpp'))
```

> `'3dmath'` is explicitly used there as a name for program executable, feel free to change it

> **tip**: to speed up whole building process on multicore machines we can call *SCons* with `-jN` option like this `scons -j4`, where `N` represents number of parallel jobs


## third party dependencies

Let's render a triangle with *OpenGL ES2* and *GLFW* libraries, like in [triangle](triangle/triangle.cpp) sample. We need to create `SConstruct` with *OpenGL ES2* and *GLFW* library dependencies to get our sample working. 

In *Linux* there is almighty *pkg-config* there to help us with the task.

> before we can continue we need to install *GLFW*, *OpenGL ES2* libraries and *pkg-config* itself with `sudo apt install libglfw3-dev libgles2 pkg-config` command

Build environment can parse *pkg-config* output with [`ParseConfig()`][ParseConfig] function this way

```python
cpp17.ParseConfig('pkg-config --cflags --libs glfw3 glesv2')
```

> **tip**: we can use `pkg-config --list-all | grep -i LIBRARY` command to search for *GLFW* and *OpenGL ES2* libraries, under *Ubuntu 20.04 LTS* they are there as **glfw3** and **glesv2**

Now the last think needed to be done is to tell *GLFW* library we want render with *OpenGL ES2* (not *OpenGL*). It can be done with `GLFW_INCLUDE_ES2` define so we need to change `CPPDEFINES` construction variable for our environment. Final `SConstruct` file will looks like this

```python
cpp17 = Environment(
	CCFLAGS=['-std=c++17', '-Wall', '-O0', '-g'],
	CPPDEFINES=['GLFW_INCLUDE_ES2'])

cpp17.ParseConfig('pkg-config --cflags --libs glfw3 glesv2')

cpp17.Program('triangle.cpp')
```

We are done, go to [`triangle/`](triangle/) directory and run `scons` from konsole to see it is working.

> see [OGRE starter](https://github.com/sansajn/ogre-linux-starter) or [cube rain](https://github.com/sansajn/cube_rain) repositories for more *SCons* samples


## static library

It is a good practise to group domain specific code into a library like in a [physics](physics/) sample. I've moved sample physics engine from great [Game Physics Cookbook][GamePhys] into a `phys/` directory and want to use it from my project as a static library. In *SCons* it is super easy to do that with [`StaticLibrary()`][StaticLibrary] builder and [`Glob()`][Glob] function, like this

```python
cpp17 = Environment(
	CXX='clang++', CCFLAGS=['-std=c++17', '-Wall'])

cpp17.ParseConfig('pkg-config --cflags --libs glfw3 gl')

phys = cpp17.StaticLibrary('phys', Glob('phys/*.cpp'))

cpp17.Program('physics', ['main.cpp', phys])
```

> We need to use *clang* there, because physics library use non-standard *C++* language feature not supported by *GCC*. Install clang with `sudo apt install clang` command.

> **tip**: there are also builders to create shared libraries as [`SharedLibrary()`][SharedLibrary] and object files as [`Object()`][Object]



[Glob]: https://scons.org/doc/production/HTML/scons-man.html#f-Glob
[ParseConfig]: https://scons.org/doc/production/HTML/scons-man.html#f-ParseConfig
[Library]: https://scons.org/doc/production/HTML/scons-man.html#b-Library
[StaticLibrary]: https://scons.org/doc/production/HTML/scons-man.html#b-StaticLibrary
[Program]: https://scons.org/doc/production/HTML/scons-man.html#b-Program
[SharedLibrary]: https://scons.org/doc/production/HTML/scons-man.html#b-SharedLibrary
[Object]: https://scons.org/doc/production/HTML/scons-man.html#b-Object
[GamePhys]: http://gamephysicscookbook.com/
[sequence]: https://docs.python.org/3.4/library/stdtypes.html#sequence-types-list-tuple-range
[string]: https://docs.python.org/3.4/library/stdtypes.html#text-sequence-type-str

Adam Hlavatovic

