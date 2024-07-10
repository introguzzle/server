//
// Created by 666 on 10.07.2024.
//

#ifndef METHOD_H
#define METHOD_H

typedef enum Method {
    METHOD_GET     = (unsigned int) 1 << 0,
    METHOD_POST    = (unsigned int) 1 << 1,
    METHOD_PUT     = (unsigned int) 1 << 2,
    METHOD_PATCH   = (unsigned int) 1 << 3,
    METHOD_DELETE  = (unsigned int) 1 << 4,
    METHOD_UNKNOWN = (unsigned int) 1 << 5,
} Method;

const char* MethodToString(Method method);
Method ParseMethod(const char* methodData);

#endif //METHOD_H
