# FECS

_Functional ECS_

FECS is a WIP Functional DSL, that
[transpiles][transpiler] to a ECS
library written in ANSI C. The purpose of FECS is to allow programmers to write
safe, simple and data-parallel entity component systems for use in other
projects.


## Flags

Implement a `hint` command, s.t. it will warn the user if they have:
* Not defined a default value for a struct.
* enable struct nation to signify that the struct "owns" an attribute, ie. if we
  have an attribute of some pointer-type, `*a`, we want to be able to distinguish
  whether it is a reference to an `a`, or that the struct should call free on
  the reference when the struct itself gets deleted.

[transpiler]: https://en.wikipedia.org/wiki/Source-to-source_compiler
