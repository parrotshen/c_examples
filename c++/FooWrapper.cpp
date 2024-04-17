#include "Foo.h"
#include "FooWrapper.h"

extern "C" {
    Foo* Foo_new()
    {
        return new Foo();
    }

    void Foo_delete(Foo *obj)
    {
        delete obj;
    }

    void Foo_set(Foo *obj, int val)
    {
        obj->set(val);
    }

    int Foo_get(Foo *obj)
    {
        return obj->get();
    }
}

