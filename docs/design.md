# Language design (Syntax)

## Comments

`fecs` only support line comments. Line comments start with `--`, and must not
be directly next to any leading, or trailing, non-whitespace characters.

## Types & Declarations

A declaration has the following syntax, in backus naur form:

```EBNF
Declaration ::=
    VName ";"
  | VName ":" PType ";"
  | VName "=" Expr ";"
  | VName ":" PType "=" Expr ";" ;

PType ::= "*" Type |  Type ;

Type ::=
    "bool"
  | "i8" | "i16" | "i32" | "i64"
  | "u8" | "u16" | "u32" | "u64"
  | "f32" | "f64"
  | "usize" | "isize"
  | TypeArray
  | TypeTuple
  | TypeRecord
  | TypeSum
  | TypeAlias
  | "(" Type ")"
  ;

TypeArray  ::= "[" Expr "]" Type ;
TypeTuple  ::= "(" Type "," Type ")" ;
TypeRecord ::= "{" EDeclarationO "}" ;
TypeSum    ::= Type "|" Type ;
TypeAlias  ::= VName ;
```

## Structures

A structure is a sequence of variable declarations.
Structure definition example:
```fecs
struct MyStruct {
  UntypedMember;
  TypedMember : i32;
  UntyedMemberWithDefaultValue = 42;
  TypedMemberWithDefaultValue : i32 = 43;
}
```

A struct is in reality an aliased type, to the struct-type itself.

The variable declarations in type declarations, have a unique feature compared
to regular variable declarations when it comes to declaring pointers: There are
two options when declaring a member of a pointer type:
1. Pointer (regular reference to an object located somewhere else), declared as
   `v : *i32;`, for a member called `v` of type pointer to 32-bit signed
   integer.
2. Owner reference, declared as `v : &i32;`, for a member called `v` of type
   pointer to 32-bit signed integer. This type of reference has the special
   meaning that the referenced value will be freed when an instance of the type
   holding the owner reference is freed.

In backus naur it would read


```EBNF
StructDeclaration ::= "struct" VName "{" EDeclarationO "}" ;

EDeclarationO ::= EDeclaration EDeclarationO | EDeclaration ;

EDeclaration ::=
    VName ";"
  | VName ":" Type ";"
  | VName "=" Expr ";"
  | VName ":" Type "=" Expr ";"
  ;

EType ::= "&" Type | PType ;
```

## Entity Component Features
