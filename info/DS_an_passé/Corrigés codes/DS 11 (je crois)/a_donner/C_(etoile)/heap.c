#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "heap.h"

struct heap_t {
    int capacity;
    int size;
    edge_t *data;
};

heap_t *heap_create(int capacity){
    heap_t *heap = malloc(sizeof(heap_t));
    heap->capacity = capacity;
    heap->size = 0;
    heap->data = malloc(capacity * sizeof(edge_t));
    return heap;
}

void heap_free(heap_t *heap){
    free(heap->data);
    free(heap);
}

bool heap_is_empty(heap_t *heap){
    return heap->size == 0;
}

int heap_get_capacity(heap_t *heap){
    return heap->capacity;
}

int heap_get_size(heap_t *heap){
    return heap->size;
}

edge_t heap_get_min(heap_t *heap){
    assert(heap->size > 0);
    return heap->data[0];
}

void swap(heap_t *heap, int i, int j){
    edge_t tmp = heap->data[i];
    heap->data[i] = heap->data[j];
    heap->data[j] = tmp;
}

void sift_up(heap_t *heap, int i){
    int parent = (i - 1) / 2;
    if (i == 0 || heap->data[parent].weight <= heap->data[i].weight) return;
    swap(heap, i, parent);
    sift_up(heap, parent);
}

void sift_down(heap_t *heap, int i){
    int i_min = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    edge_t *tab = heap->data;
    if (left < heap->size && tab[left].weight < tab[i_min].weight){
        i_min = left;
    }
    if (right < heap->size && tab[right].weight < tab[i_min].weight){
        i_min = right;
    }
    if (i_min != i){
        swap(heap, i, i_min);
        sift_down(heap, i_min);
    }
}

void heap_push(heap_t *heap, edge_t pair){
    assert(heap->size < heap->capacity);
    heap->size++;
    heap->data[heap->size - 1] = pair;
    sift_up(heap, heap->size - 1);
}

edge_t heap_extract_min(heap_t *heap){
    assert(heap->size > 0);
    edge_t result = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    sift_down(heap, 0);
    return result;
}

void heap_print(heap_t *heap){
    for (int i = 0; i < heap->size; i++){
        edge_t edge = heap->data[i];
        printf("i = %2d: (%.1f, %d-%d)\n", i, edge.weight, edge.x, edge.y);
    }
}
