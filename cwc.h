/*
	Copyright (c) 2015 Aleksander B. Birkeland

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1.  The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgement in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _CWC_H
#define _CWC_H

#include <stdlib.h>
#define new(o) malloc(sizeof(o))
#define delete(o) free(o)

#ifdef __cplusplus
#define defclass(cls) struct cls {
#define endclass };
#else
#define defclass(cls) typedef struct cls cls; struct cls {
#define endclass };
#endif

#define method(cls, ret, name, ...) ret (*name)(cls *self, ## __VA_ARGS__);
#define decl_method(cls, ret, name, ...) ret cls ## _ ## name(cls *self, ## __VA_ARGS__);
#define defmethod(cls, ret, name, ...) ret cls ## _ ## name(cls *self, ## __VA_ARGS__) {
#define endmethod }

#define decl_constructor(cls, ...) cls* new ## cls (__VA_ARGS__);
#define defconstructor(cls, ...) cls* new ## cls (__VA_ARGS__) { cls* self = (cls*) new(cls);
#define add_method(cls, name) self->name = cls ## _ ## name;
#define endconstructor return self; }

#endif
