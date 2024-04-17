#ifndef __FOOWRAPPER_H__
#define __FOOWRAPPER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Foo Foo;

Foo *Foo_new();

void Foo_delete(Foo* obj);

void Foo_set(Foo *obj, int val);

int Foo_get(Foo *obj);

#ifdef __cplusplus
}
#endif

#endif
