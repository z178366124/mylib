#ifndef RINGPOOL__H
#define RINGPOOL__H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct ringbuffer_s{
    char *buffer;       // 指向数据缓冲区的指针
    char *ringbuffer_name;
    size_t capacity;    // 缓冲区的总容量
    size_t read;        // 队列的头部索引
    size_t write;        // 队列的尾部索引
    size_t size;        // 当前队列中数据的大小
} ringbuffer_t;
void* ring_buffer_push_struct_data(ringbuffer_t* rb, void* data, size_t write_size);


#define ALLOC(rb, nbytes) \
    ring_buffer_push_struct_data(rb, NULL, nbytes)
#define NEW(rb, p) ((p) = ALLOC(rb, (long)sizeof *(p)))


#endif