/*
 * Object.h
 *
 *  Created on: 12/12/2008
 *      Author: denis
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Status.h"

#define Class(_class) \
	typedef struct _class ## _ _class; \
	struct _class ## _data; \
	struct _class ## _

#define Inherits(_class) _class base
#define Fields(_class) struct _class ## _data
#define Private(_class) Fields(_class) data
#define MethodName(_class, _name) _class ## _ ## _name
#define Method(_class, _name, ...) (* _name)(_class* pThis, ##__VA_ARGS__)
#define Implement(_class, _name, ...) MethodName(_class, _name) (_class* pThis, ## __VA_ARGS__)
#define SetMethod(_class, _name) pThis -> _name = & MethodName(_class, _name)
#define OverrideMethod(_class, _name) pThis -> base. _name = & MethodName(_class, _name)
#define This(_name, ...) pThis-> _name (pThis, ## __VA_ARGS__)
#define Base(_name, ...) pThis->base. _name (&pThis->base, ## __VA_ARGS__)


#endif /* OBJECT_H_ */
