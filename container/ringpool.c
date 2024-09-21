#include "ringpool.h"

// 检测一个数是否为2的指数次方
int is_power_of_two(size_t n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

// 初始化环形队列内存池
ringbuffer_t* init_ring_buffer(char *ringbuffer_name, size_t capacity) {
    if (!is_power_of_two(capacity)) {
        printf("Capacity must be a power of 2.\n");
        return NULL;  // 容量不是2的指数次方
    }
    ringbuffer_t *ringbuffer = (ringbuffer_t*)malloc(sizeof(ringbuffer_t) + capacity);
    if (ringbuffer == NULL) {
        perror("create ringbuffer_t malloc err:");
        return NULL;  // 内存分配失败
    }
    ringbuffer->buffer = (char*)ringbuffer + sizeof(ringbuffer_t);
    ringbuffer->capacity = capacity;
    ringbuffer->read = 0;
    ringbuffer->write = 0;
    ringbuffer->size = 0;
    ringbuffer->ringbuffer_name = ringbuffer_name;
    return ringbuffer;
}

// typedef void* (*write_func)(void*, size_t);

void* ring_buffer_push_struct_data(ringbuffer_t* rb, void* data, size_t write_size){
    size_t write = rb->write;
    size_t read = rb->read;
    if(rb->capacity - write > write_size){
        if(data)
            memcpy(rb->buffer + write, data, write_size);
        asm volatile ("mfence" ::: "memory");
        rb->write += write_size;
        rb->size += write_size;
        return rb->buffer + write;
    }
    if(read >= write_size){
        if(data)
            memcpy(rb->buffer, data, write_size);
        asm volatile ("mfence" ::: "memory");
        rb->write = write_size - 1;
        rb->size += rb->capacity - write + write_size;
        return rb->buffer;
    }
    return NULL;
}

void ring_buffer_pop(ringbuffer_t* rb, size_t free_size){
    if(rb->size < free_size) return;
    rb->read = (rb->read + free_size) & (rb->capacity - 1);
    rb->size -= free_size;
}

// 销毁环形队列
void destroy_ring_buffer(ringbuffer_t *rb) {
    if (rb != NULL) {
        free(rb);
    }
}

void ring_buffer_clear(ringbuffer_t* rb){
    rb->read = 0;
    rb->write = 0;
    rb->size = 0;
}

typedef struct a{
    int x,y;
}a;

int main(){
    ringbuffer_t* rb = init_ring_buffer("ring", 1024);
    a *t;
    NEW(rb, t);
    t->x = 10;
    t->y = 50;
    printf("%d %d\n", t->x, t->y);
    // int i = 1000;
    // while (i--)
    // {
    //     ring_buffer_get_write_pointer(rb, "abcde", sizeof("abcde") - 1);
    //     printf("[%d][%d %d]------------------------------\n", i, rb->read, rb->write);
    //     // printf("%.*s\n", 5,rb->buffer + rb->read);
    //     ring_buffer_pop(rb, 4);
    //     // printf("%.*s\n", 2,rb->buffer + rb->read);
    //     printf("==============================\n");
    // }
    
    destroy_ring_buffer(rb);
    return 0;
}