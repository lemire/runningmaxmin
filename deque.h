
#ifndef DEQUE_H
#define DEQUE_H

#include "common.h"

// nextPowerOfTwo returns a power of two that is larger or equal than x.
inline uint nextPowerOfTwo(uint x) {
    uint result = 1;
    while (result < x) {
        result <<= 1;
    }
    return result;
}

struct intfloatnode {
    uint index;
    floattype value;
};

struct intfloatqueue {
    intfloatnode * nodes;
    uint head;
    uint tail;
    uint mask;
};

inline uint count(intfloatqueue * q) {
    return (q->tail - q->head) & q->mask;
}

inline void init(intfloatqueue * q, uint size) {
    size = nextPowerOfTwo(size + 1);
    q->nodes =
        reinterpret_cast<intfloatnode *>(malloc(sizeof(intfloatnode) * size));
    q->head = 0;
    q->tail = 0;
    q->mask = size - 1;
}

inline void free(intfloatqueue * q) {
    free(q->nodes);
}

inline uint headindex(intfloatqueue * q) {
    return q->nodes[q->head].index;
}

inline void push(intfloatqueue * q, uint index, floattype value) {
    q->nodes[q->tail].index = index;
    q->nodes[q->tail].value = value;
    q->tail = (q->tail + 1) & q->mask;
}

inline floattype tailvalue(intfloatqueue * q) {
    return q->nodes[(q->tail - 1) & q->mask].value;
}

inline floattype headvalue(intfloatqueue * q) {
    return q->nodes[q->head].value;
}
inline void prunehead(intfloatqueue * q) {
    q->head = (q->head + 1) & q->mask;
}

inline void prunetail(intfloatqueue * q) {
    q->tail = (q->tail - 1) & q->mask;
}

inline int nonempty(intfloatqueue * q) {
    return static_cast<int>(q->tail != q->head);
}

#endif
