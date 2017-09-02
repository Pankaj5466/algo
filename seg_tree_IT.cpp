#include<iostream>
#include<math.h>
#include<cstring>
#define lson (x<<1)
#define rson (x<<1 |1)
#define mid ((l+r)/2)
#define par (x>>1)
//NOTE:- see codeforces "easy and efficent segment tree" blog if u cant understand here

using namespace std;

const int N = 1e4 + 2;
int t[N<<2];
int n;

void init() {
	memset(t, 0, sizeof(t));
}

void show();

void push_up(int x) {
	t[x] = t[lson] + t[rson];
}

void updateNodeIT(int x, int val){

	for (t[x = x + n] = val; x>1; x >>= 1) {
		push_up(par);  //update the parent
	}
}

//queryIT paramertes will be [l,r). i.e, the right boundary will not be included here
int queryIT(int l, int r) {
	int res = 0;
	for (l += n, r += n; l<r; l >>= 1, r >>= 1) {
		if (l & 1) res += t[l++];
		if (r & 1) res += t[--r];

	}
	return res;
}

void buildIT() {
	for (int i = n - 1; i > 0; i--) push_up(i);
}

int main() {
	//freopen("input1.txt", "r+", stdin);
//	freopen("output1.txt", "rw+", stdout);

	//cin >> n;
	n = 8;
	init();

	for (int i = 0; i<n; i++) t[i + n] = i + 1;


	buildIT();
	cout << "showing the segtree after buildup\n";
	show();

	cout << endl;
	
	cout<<"updating a val using IT update\n";
	updateNodeIT(1,12);
	show();

	cout<<"Now calling queryIT\n";
	cout<<" queryIT val: "<<queryIT(3,8)<<endl;
	
}
void show() {
	for (int i = 1; i < 2*n; i++) cout << t[i] << "[" << i << "]" << "   ";
}