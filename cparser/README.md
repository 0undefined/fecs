The purpose of this source directory is to make an alternative parser to the
FLEX+BYACC parser.

The tokenizer should create tokens,
These tokens should be a sum type with their location, as well as any additional
metadata (I think it should just be the original string, let the parser do the
rest, idk).

It should reinforce the syntax, and precedence, of the language
by how the parser should be structured.
This should be fast, and have as little back tracking as possible.
The parser should maintain a state throughout all steps.
Each "parsing function" should internally update the state.
If a parsing function would need to backtrack, it should create an ENTIRE copy
of the state before attempting to parse something.

_or maybe just the token location? I mean, I think we can just store the index
at which we were before attempting to parse. We just have the slight problem
that the state might be altered in the meantime_

Preferably we should have a growing arena allocator for both of these stages.
They are, after all only growing in memory usage as they progress.

The parser should maybe have a special "backtracking" allocator, or store the
"used memory" in its state, s.t. when it needs to backtrack, it can just _wipe_
the backtracked memory.

I want the language to be whitespace sensitive in relation to a "starting
expression":

```fecs
-- Indentation needs to be +1, if the expression is from a let expression
let x =
 y + 2

-- if a newline is encountered, we need indentation to be +1 of the "let" expression
let z = case x of
  | 0 | 1 -> ...
  | n -> ...

-- Since case is on its own line, we can match the indentation thereof.
-- After an arrow, the same rules are valid, as the let expression (+1)
let z =
 case x of
 | 0 | 1 ->
   let t = ...

-- This should be illegal, as it would create confusion with more complex
-- expressions
let z =
   case x of
 | 0 | 1 ->

-- This is also the case for function calls, at least +1 indendation
let x =
  (+)
   bar
   baz

-- This allows us to create very weird looking programs, take the equivalent of
-- (foo + bar) * (bar - baz)
let x =
  (*)
   (+)
    foo
    bar
   (-)
    bar
    baz

```

These "major" terms that require at least +1 whitespace indentation in the line
after are:
* function calls (where the arguments are on a separate line)
* let expressions
* Arrows (in pattern matching)
* opening brace

We need to track the indentations in the state, probably using some stack.
The indentation & newlines should _also_ be emitted in the tokenizer.
