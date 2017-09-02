#include<iostream>
#include<math.h>
#include<cstring>
#define lson (x<<1)
#define rson (x<<1 |1)
#define mid ((l+r)/2)


using namespace std;

const int N = 1e4 + 2;
int t[N<<2];
int a[N];
int n;  //the size of a[] OR no of required leaf nodes is segment tree

void init() {

	memset(t, 0, sizeof(t));

}

void show();

void push_up(int x) {
	t[x] = t[lson] + t[rson];
}

void build(int x, int l, int r) {
	if (l > r) return;

	if (l == r) 
		if (l == r) { //case of leaf node
		t[x] = a[l];
	return;
	}							

	build(lson, l, mid);
	build(rson, mid + 1, r);

	push_up(x);
}

//it is a point update
//ALWAYS VALUE: X=1,l=0,r=7
//make t[p+n]=b OR equivalencly arr[p]=b and update tree accordingly
void update1(int x, int l, int r, int p, int b) {
	if (l == r && l == p) {	//NOTICE:- There is only a slight change here, w.r.t update2(..)
		t[x] = b;	//TODO:- Check if x is equal to p+n here
		return;
	}

	if (p <= mid) update1(lson, l, mid, p, b);
	else update1(rson, mid + 1, r, p, b);

	push_up(x);
}

//It is a range update
//will increment value in range [i,j] of array arr[]. And then will update the tree accrodingly	
//i,j is based on 0 index as it represents the position w.r.t arr[]
//AlWAYS VALE: x=1,l=0,r=n-1
void update2(int x, int l, int r, const int i, const int j, int val) {
	if (l > r || l > j || r < i) return;
	if (l == r) {
		t[x] += val;
		return;
	}

	update2(lson, l, mid, i, j, val);
	update2(rson, mid + 1, r, i, j, val);

	push_up(x);
}



//query(1,0,n-1,i,j) find the query for range [i,j]
//NOTE:- indexing of i,j is correspondance to array a[]. i.e, they start with indexing '0'
int query(int x, int l, int r, const int i, const int j) {
	if (l> r || l > j || r<i) return 0;
	if (l >= i && r <= j) return t[x]; //within range

	return query(lson, l, mid, i, j)
		+ query(rson, mid + 1, r, i, j);
}

int main() {

	n = 6;
	init();

	for (int i = 0; i<n; i++) a[i] = i + 1;

	build(1, 0, n - 1);
	cout << "showing the segtree after buildup\n";
	show();

	cout << endl;
	
	cout<<"\nupdating a val using point update\n";
	update1(1,0,n-1,1,12);
	show();

    cout<<"\n\nupdating a range of value using range update\n";
    update2(1,0,n-1,1,4,10);
    show();

	cout<<"Now we are doing queryREC\n";
	cout<<"query val: "<<query(1,0,n-1,2,4)<<endl;
}
void show() {
	for (int i = 1; i < 3*n; i++) cout << t[i] << "[" << i << "]" << "   ";
}
