#include "ast.h"
#include "ast_print.h"

void print_ast(Spec* s) {
  if (s == NULL) {printf("<empty ast>\n"); return;}
  //printf("Num constructs: %lu\n", s->num_definitions);
  LinkedList_DExpr *head = s->definitions;

  while (head != NULL) {
    printf("def %s [%lu]{\n", head->value.name, head->value.exp.struct_t->num_declarations);
    LinkedList_Declaration* head1 = head->value.exp.struct_t->declarations;

    while (head1 != NULL) {
      printf("  ");
      print_attrib(&head1->value);
      head1 = head1->next;
    }

    printf("}\n");

    head = head->next;
    if (head != NULL) printf("\n");
  }
}

void print_word(const char* w) {
  puts(w);
}

void print_vname(VName n) {
  printf("%s_%zd", n.name, n.tag);
}

void print_type(Types_t t) {
  printf("%s", Types_str[t]);
}

void print_value(Value v) {
  switch (v.type) {
    case Type_i8:  printf("%d",  v.value.i8_t.v); break;
    case Type_i16: printf("%d",  v.value.i16_t.v); break;
    case Type_i32: printf("%d",  v.value.i32_t.v); break;
    case Type_i64: printf("%ld", v.value.i64_t.v); break;
    case Type_u8:  printf("%d",  v.value.u8_t.v); break;
    case Type_u16: printf("%d",  v.value.u16_t.v); break;
    case Type_u32: printf("%d",  v.value.u32_t.v); break;
    case Type_u64: printf("%lu", v.value.u64_t.v); break;
    default:
      printf("?");
  }
}

void print_attrib(Declaration* a) {
  print_vname(a->vname);
  printf(" : ");
  print_type(a->value.type);
  printf(" = ");
  print_value(a->value);
  printf("\n");
}
