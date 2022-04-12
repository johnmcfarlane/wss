# Project Maintainability

## Fail Loud

Errors and bugs should be prominent.
If an error can be dealt with by the client code, it's not an error.

## `grep`ability

`grep`ability is the quality that:

* given an object (file, programming construct, resource, etc.) referred to in a software project, and
* given a name used to define the object (filename, identifier, key, etc.),
* when the name is used to refer to the object elsewhere in the project,
* then a simple text search (e.g. using `grep`) can be employed to navigate between definition and reference.

`grep`ability is a specific type of traceability. Traceability is helpful to understanding a system.
When creating a reference between two things, ask yourself: is this identifier `grep`pable?
More generally, how easy will it be to search for something in order to find out more about it.

## Glob Sparingly

Absence of a file is not the filing of absence.

If you want to add a CMake library consisting of all of the .cpp files in a directory, list those files explicitly - ideally in an [`add_library`](https://cmake.org/cmake/help/latest/command/add_library.html) call.

If you need a file but you accidentally deleted it, you want to Fail Loud.
But globbing silences this failure.

Named elements and their points of reference are edges in a graph.
It should be reasonably easy to traverse this graph.
Globbing obscures the link.
Globbing reduces `grep`ability.

## Isolation and Multiplexity

We acknowledge that when you take a simple program with global variables and encapsulate those variables in an object, it allows the program to process multiple items of work without those items interfering with one another. We further acknowledge that where we can treat these items as immutable, it brings many further advantages. These principles extend to software in general. In the case of a software project, they inform the following rules:

1. You should be able to clone your project multiple times into multiple separate working directories and *view* them in isolation from one another. Working directories can be located anywhere on the readable filesystem for *viewing*.
2. You should be able to configure, build, test and install your project in multiple separate build directories in isolation from one another and *use* them in isolation. Build directories can be located anywhere on the read/writeable filesystem for *using*. Working directories can be located anywhere on the readable filesystem for *using*.
3. You should be able to configure, build, test and install your project in multiple separate build directories in isolation from one another and *test* them in isolation. Build directories can be located anywhere on the read/writeable filesystem for *using*. Working directories can be located anywhere on the readable filesystem for *using* (with the exception below).
4. You should be able to clone your project multiple times into multiple separate working directories and *develop* them in isolation from one another. Working directories can be located anywhere on the read/writeable filesystem for *developing*.

### Exception: Using the Working Directory as the Expected Result of a Test

Some tests are far easier to implement when the working directory is writeable.
They test an operation that is performed on the contents of the working directory.
They use the state of the working directory before the operation as the *expected result*, and the state after as the *actual result*.
Testing that expected and actual results are the same is a matter of testing whether there are unstaged changes.
This can be performed in a Git repository with `git diff --exit-code`.

These tests represent an exception to rule (3) and do not include unit tests; unit tests should *never* require a writeable working directory.

Like all the best tests, these tests must be deterministic on success.

Examples:

* Some linters work by applying fixes to linted files. If the files are well linted, this operation should not change the working directory, but it *will* write to it.
* Some tests of data transformations are performed at the filesystem level. Acceptance tests in which the code under test performs a large amount of work, may measure success in terms of whether the output is as expected - even if it's not clear *why* the output is exactly so. A functional change to the program may result in widespread change to the expected data. Updating acceptance tests accordingly may be a major effort, unless performing the test is the same task as changing the results.

## Isolation + Don't Glob

If you had two repos and none of the file paths were the same, it ought to be possible to store them in the same directory without any possibility of colision. 

## Be Kind to Your Future Self

## Don't Be Kind to Computers

Does my computer work too hard? Is the work I give my computer as easy as it can be? Can I save a byte here, an instruction there? 
Stop worrying about your computer.
Computers have no feelings (yet) and don't care *what* data goes through them. They are happy to execute poorly-organised programs full of spaghetti code and badly-named variables. Clearly they have poor taste and should not be consulted when making software design choices.

Yes, if you an algorithm with the wrong complexity guarantees, you computer will grind to a halt as datasets grow.
But even your choice of algorithm is a message to your future self: "this operation is random access", "the order of this sequence's must remain stable", etc..
If your function is _O(n^2)_, it's helpful to everyone (both your computer and and your fellow androids) if you care about that _(n^2)_.
But don't dwell on the _O_.

Write code which is easy to maintain and easy for your collaborators (and future self) to read.
It's easy to write code that is optimised for matinainability and then make it fast tomorrow.
It's hard to write code that is optimised for performance and then make it maintainable later.

It's even hard to write code that is optimised for performance today, and that still runs fast on computers later.
Imagine you are asked to optimise some critical infrastructure that is 10 years old and conclude that it needs to run on a modern GPU.
Do you think that will be easier to achieve if it is optimised for a human, or if it is optimised for a Pentium III?

## Entities Have Many Attributes But Only One Name

Naming is hard, really hard. Naming is also critical.
In order to Be Kind to Your Future Self, not only do you need to give an entity a good name, but if the entity changes, you must revise the name.
Two major pitfalls beset the budding namer:

Just because something is true of an entity doesn't mean it belongs in the name. Is your middle name Reader? Why not?! You're reading, aren't you?

A good rule of thumb: is the thing still well named if an incidental detail about it changes. E.g.

* `new_resource_manager` - is this still a good name after five years? If not, `new` is *circumstantial*.
* `resource_manager_2018` - is this still in use in 2019? Is not, `2018` has become *dated*.
* `book_resource_manager<T>` - sure, you're using this entity to manage books currently. But if `T` was `pebble`, `jingle` or `solar_system`, would it still do a good job managing those resources? If so, `book` is *overspecific*?

### It's Possible to Name an Entity Without Naming It.

Now I've convinced you not to use `new`, `2018` or `book`, let's keep going.

Some words have little or no meaning in the context of software. Remember, software is just data and operations on data. Many badly-chosen names fit into one or both of those categories.

Useless proxies for 'data' include:

* `object`,
* `item`,
* `element`, and...
* `resource`!

Useless proxies for `operations on data` include:

* `process`,
* `run`,
* `complete`,
* `execute`, and...
* `manage`!

In highly generic code, these are good names for entities... occasionally. Most everywhere else, they are the absence of a name. You didn't spend the time to understand what the entity *was* so you gave up.

The result is that you're computer is happy. But you have made your future self sad.

## Avoid Circular Dependency

Dependency may arise in many settings. Header files that include one another are a sure sign then something is wrong.
Generally, where there are dependencies, the system can be visualised as a directed graph - a graph with arrows for edges.
If you can follow the arrows around in a circle, the graph is circular, or cyclic.
A cyclic graph is far harder to deal with than an acyclic graph for reasons that are beyond this document.

One example of where a cyclic graph is bad news, is when it describes the build and package systems.

Conan's [_cmake_ generator](https://docs.conan.io/en/latest/integrations/build_system/cmake/cmake_generator.html) is one way to provide a CMake project with its essential packages.
Thus, the Conan system refers to the CMake system within a codebase project.

But the _cmake_ generator requires that the CMake system includes Conan commands.
Thus, the CMake system refers to the Conan system within a codebase project.

This is not end of the world; most Conan+CMake projects use this generator quite happily.
But consider a project which wishes to remain package system-agnostic?
What if I wish to support vcpkg _as well as_ Conan?
The Conan-specific code in the CMake scripts are now unwanted at best, and breaking at worst.
Further, they're entirely unnecessary.

By using a decoupled generator, such as the [_cmake_find_package_ generator](https://docs.conan.io/en/latest/integrations/build_system/cmake/cmake_find_package_generator.html),
both package managers are supported in the same project and the CMake is cleaner to boot.

The moral of the story: don't go 'round in circles!
