//
// Created by 666 on 10.07.2024.
//

#ifndef ITERABLE_H
#define ITERABLE_H

#define foreach(iterable, var) \
    for (size_t __i__ = 0; __i__ < (iterable)->size && ((var) = (iterable)->items[__i__], 1); ++__i__)

#define foreachReversed(iterable, var) \
    for (size_t __i__ = (iterable)->size - 1; __i__ >= 0 && ((var) = (iterable)->items[__i__], 1); --__i__)

#endif //ITERABLE_H
