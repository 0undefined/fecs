#ifndef ALGO_H
#define ALGO_H

#define LinkedList(tt) \
typedef struct LinkedList_##tt {\
	tt value;\
	struct LinkedList_##tt *next;\
	struct LinkedList_##tt *prev;\
} LinkedList_##tt;\
\
LinkedList_##tt* LinkedList_##tt##_new(tt value);\
LinkedList_##tt* LinkedList_##tt##_append(LinkedList_##tt *ll, tt value);\
LinkedList_##tt* LinkedList_##tt##_prepend(LinkedList_##tt *ll, tt value);\
void LinkedList_##tt##_free(LinkedList_##tt *ll, tt value);\
LinkedList_##tt* LinkedList_##tt##_reverse(LinkedList_##tt *ll, tt value);

#define LinkedList_Implementation(tt) \
LinkedList_##tt* LinkedList_##tt##_new(tt value) {\
	LinkedList_##tt* ll = (LinkedList_##tt*)calloc(1, sizeof(LinkedList_##tt));\
	ll->prev = NULL;\
	ll->next = NULL;\
	ll->value = value;\
	return ll;\
}\
LinkedList_##tt* LinkedList_##tt##_append(LinkedList_##tt *ll, tt value) {\
	if (ll == NULL) {\
		ll = (LinkedList_##tt*)calloc(1, sizeof(LinkedList_##tt));\
		ll->prev = NULL;\
		ll->next = NULL;\
	}\
\
	ll->next = (LinkedList_##tt*)calloc(1, sizeof(LinkedList_##tt));\
	ll->next->value = value;\
	ll->next->prev = ll;\
	return ll;\
}\
LinkedList_##tt* LinkedList_##tt##_prepend(LinkedList_##tt *ll, tt value) {\
	if (ll == NULL) return NULL;\
	ll->prev = (LinkedList_##tt*)calloc(1, sizeof(LinkedList_##tt));\
	ll->prev->value = value;\
	ll->prev->next = ll;\
	return ll->prev;\
}\
void LinkedList_##tt##_free(LinkedList_##tt *ll, tt value) {\
	LinkedList_##tt *n = ll;\
	while (n != NULL) {\
		LinkedList_##tt *m = n;\
		n = n->next;\
		free(m);\
	}\
}\
LinkedList_##tt* LinkedList_##tt##_reverse(LinkedList_##tt *ll, tt value) {\
	LinkedList_##tt *head = ll;\
	if (head == NULL) return NULL;\
	while (head->next != NULL) {\
		LinkedList_##tt *t = head;\
		head = head->next;\
		t->prev = head;\
		head->next = t;\
	}\
	return head;\
}

#endif
