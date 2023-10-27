# Parser

This module provides parsing functionality for the Functional Entity Component
System language.

## Overview

* [`parser_api.h`](./parser_api.h) provides a simple API to parse a file into an
  AST (Abstract Syntax Tree). Other options for passing might become available
  here in the future.
* [`fecs.l`](./fecs.l) contains token matching rules for
  [flex](https://github.com/westes/flex) scanner.
* [`fecs.y`](./fecs.y) provides parsing rules and all of the parsing logic
  itself for [bison](https://www.gnu.org/software/bison/), a parser generator.
* [`ast.h`](./ast.h) defines node types and constructors to create the AST. It
  is used in the parser, [`fecs.l`](./fecs.l).

Flex + Bison is set up to be reentrant which means that it does not rely on
global variables, instead state is passed around as a function parameter. This
should make it thread safe, and allow for multiple compilation objects to be
compiled simultaneously.
