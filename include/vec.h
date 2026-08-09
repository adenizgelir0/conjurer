#pragma once

typedef struct {
	int *data;
	int size;
	int cap;
} vec;

vec *new_vec(void);
void free_vec(vec *v);
void print_vec(vec *v);
void push_vec(vec *v, int elem);
int pop_vec(vec *v);
int peek_vec(vec *v);
