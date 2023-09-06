Dynamic ECS:

Define Entity (trivial)
Define Component;
  Defined by some component_def {
    mem size;
    subcomponent-indices;
    padding;
    8-byte aligns the upcoming array; ceil(sizeof(component)) = ( sizeof(component) + 8 - 1 ) / 8; == size of bytes in multiples of 8
  }
  create some data structure that can contain a singular component
  maybe create helper "labels" to ease unpacking of individual subcomponents?
    --> for systems, of course
    --> even necessary?
  _ONLY_ allow raw data, no ptrs!

Define System;
  * arbitrary LUA functions. defined in lua, but how?
  * maybe just contain everything in lua, and simply call it with the apropriate
    data?
  * needs to be able to make systems that work on compile-time components, as well
    as dynamic components.
  * systems must be of type `a -> [... ->] a`, where `a` is a component, and
    `[... ->]` substitutes any other components required to execute this system
    on an entity.


```
// Structs cannot be directly used in systems
struct vector {
  i64 x
  i64 y
}

// create a component containing vector and a boolean attribute `static`
component position {  // "contains" a vector
  vector  // "expands" to the attributes of `vector`
  bool static [= default value]
}

// aliases velocity to `vector`
component velocity vector;

-- a type binds components together, allowing for complex entities
type [typename] {component list};

fn (+) (a: vector) (b: vector) : position {
  vector {
    .x = a.x + b.x,
    .y = a.y + b.y
  }
}

// Will execute `move` on all entities on world-tick.
system move (p: position) (v: velocity) : position {
  p + v
}
```

Will export the following:
* `ecs_new_world(systems) : ecs_world`
* `entity_new(world) : entity_t`
* `entity_new_with_component(world, {type or component})`
* `entity_add_component(world, entity_id, component-name)`
* `ecs_progress(world)`

Discussion: should we allow `entity_add_component`? It'd be far easier to know
what components an entity has, if we only allow

Ecs DSL spec:

```
EcsDef ::= Spec EcsDef | ε

Spec ::= "struct"    VName SDef
       | "component" VName CDef
       | "type"      VName TDef
       | "fn"        VName      ParamZ ':' Type '{' FunBody '}'
       | "fn"        '(' Op ')' ParamZ ':' Type '{' FunBody '}'
       | "system" VName Param ParamZ : Type '{' FunBody '}'

VName ::= [a-zA-Z][a-zA-Z0-9_']*
Op ::= '+' | '-' | '*' | '/' | "++"
Stmt ::= "let" VName Type? '=' Expr Expr | Expr
```

If this system should be truly dynamic, we should allow for some `ecs_component_new`, `ecs_type_new`, and `ecs_system_register`.

Create a catalogue of monads,
maybe : just a | nothing
either a b : left a | right b
