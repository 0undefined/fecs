The purpose of this source directory is to make an alternative parser to the
FLEX+BYACC parser.

The tokenizer should create tokens,
These tokens should be a sum type with their location, as well as any additional
metadata (I think it should just be the original string, idk).

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
