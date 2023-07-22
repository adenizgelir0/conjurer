#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "vec.h"

struct gold_int {
	int a,b;
	// aφ + b
};
typedef struct gold_int gint;

gint new_gint(int a, int b)
{
	gint g;
	g.a = a;
	g.b = b;
	return g;
}

gint add_gints(gint A, gint B)
{
	return new_gint(
		A.a + B.a,
		A.b + B.b
	);

	// (aφ + b) + (xφ + y)
	// = (a+x)φ + (b+y)
			
}
gint mult_gints(gint A, gint B)
{
	// φ^2 = φ + 1
	// (aφ + b)(xφ + y)
	// = axφ^2 + (bx + ay)φ + by
	// = ax*(φ+1) + (bx + ay)φ + by
	// = (ax + bx + ay)φ + (ax + by)
	return new_gint(
		A.a*B.a + A.b*B.a + A.a*B.b,
		A.a*B.a + A.b*B.b
	);
}
gint power(gint g, int p)
{
	//fast powering with squaring
	gint res = new_gint(0,1);
	while(p > 0)
	{
		if(p & 1) res = mult_gints(res,g);
		p >>= 1;
		g = mult_gints(g,g);
	}
	return res;
}
void print_gint(gint g)
{
	printf("%dφ + %d\n",g.a,g.b);
}

int min(int a, int b)
{
	if(a>b) return b;
	return a;
}
int max(int a, int b)
{
	if(a<b) return b;
	return a;
}

int nthfib(int n)
{
	//φ^n = Fib(n)*φ + Fib(n-1);
	gint phi_to_n = power(new_gint(1,0),n);
	return phi_to_n.a;
}

int max_fib(int upper_bound)
{
	//finds the largest Fib(n) <= upper_bound, returns n
	//need to find a reasonable Fib(n) > upper_bound, 
	//as an upper bound for binary search.
	//Fib(n) > φ^(n-2) > sqrt(2)^(n-2) > 2^((n-2)/2)
	//Fib(2*ceil(log(upperbound))+2) > upperbound
	//thus n=2*ceil(log(upperbound))+2
	int temp=upper_bound;
	int log=0;
	while(temp>0) temp/=2, log++;
	int n=2*(log+1)+2;
	//now it is sufficient to do binary search between Fib(1) and Fib(n);
	//this will circle in on the result in O(log(n)) = O(log(log(u))) iterations.
	int low=0,high=n;
	int res=0;
	while(low <= high)
	{
		int mid = low + (high - low)/2;
		int fib = nthfib(mid);
		if(upper_bound == fib) return mid;
		if(upper_bound > fib) 
			low = mid+1, res = max(res, mid);
		else high=mid-1;
	}
	return res;
	//In every iteration the fibonnacci calculation will take O(log(log(u)) 
	//steps in the worst case. Thus the complexity will be O((log(log(u)))^2) 
	//where u is the upperbound. The inital calculation of n takes O(log u) steps,
	//which overpowers the later steps in complexity, rendering the total complexity O(log u)
}

int min_fib(int lower_bound)
{
	//finds the smallest Fib(n) => lower_bound, returns n
	//need to find a reasonable Fib(n) > lower_bound, 
	//as an upper bound for binary search.
	//Fib(n) > φ^(n-2) > sqrt(2)^(n-2) > 2^((n-2)/2)
	//Fib(2*ceil(log(lower_bound))+2) > lower_bound
	//thus n=2*ceil(log(lower_bound))+2
	int temp=lower_bound;
	int log=0;
	while(temp>0) temp/=2, log++;
	int n=2*(log+1)+2;
	//now it is sufficient to do binary search between Fib(1) and Fib(n);
	//this will circle in on the result in O(log(l)) = O(log(log(l))) iterations.
	int low=0,high=n;
	int res=INT_MAX;
	while(low <= high)
	{
		int mid = low + (high - low)/2;
		int fib = nthfib(mid);
		if(lower_bound == fib) return mid;
		if(lower_bound < fib) 
			high = mid-1, res = min(res, mid);
		else low=mid+1;
	}
	return res;
	//In every iteration the fibonnacci calculation will take O(log(log(l)) 
	//steps in the worst case. Thus the complexity will be O((log(log(l)))^2) 
	//where l is the lower_bound. The inital calculation of n takes O(log l) steps,
	//which overpowers the later steps in complexity, rendering the total complexity O(log l)
}

vec *gen_fibs_upto(int n)
{
	vec *v = new_vec();
	push_vec(v,1);
	push_vec(v,1);
	while(1)
	{
		int a = v->data[v->size-1];
		int b = v->data[v->size-2];
		int next = a+b;
		if(next > n) break;
		push_vec(v, next);
	}
	return v;
}
