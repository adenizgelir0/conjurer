#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int *data;
	int size;
	int cap;
} vec;

vec *new_vec()
{
	vec *v = malloc(sizeof(vec));
	v->size=0;
	v->cap=1;
	v->data = malloc(sizeof(int));
	return v;
}

void free_vec(vec *v)
{
	free(v->data);
	free(v);
}

void print_vec(vec *v)
{
	for(int i=0; i<v->size; i++)
	{
		printf("%d ", v->data[i]);
	}
	printf("\n");
}

void push_vec(vec *v, int elem)
{
	if(v->size == v->cap)
	{
		v->cap *= 2;
		v->data = realloc(v->data, (v->cap)*sizeof(int));
	}
	v->data[v->size++] = elem;
}

int pop_vec(vec *v)
{
	v->size--;
	return v->data[v->size];
}

int peek_vec(vec *v)
{
	return v->data[v->size-1];
}
