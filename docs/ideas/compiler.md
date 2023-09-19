# Compiler ideas

This document serves as a collection of ideas for the compiler.


## Flags

Implement a `hint` command, s.t. it will warn the user if they have:
* Not defined a default value for a struct.

## Features
* enable struct notation to signify that the struct "owns" an attribute, ie. if
  we have an attribute of some pointer-type, `*a`, we want to be able to
  distinguish whether it is a reference to an `a`, or that the struct should
  call free on the reference when the struct itself gets deleted.
