//Lazy Practice Problem: https://codeforces.com/problemset/problem/52/C

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
#define cc if (0)
// #define DEBUG

#define lson (2 * idx + 1 )
//(idx<<1)
#define rson (2 * idx + 2)
//(idx<<1|1)
#define mid ((left + right) / 2)

inline bool currentNodeOutsideUpdateRange(int, int, int, int);
inline bool currentNodeInsideUpdateRange(int, int, int, int);

struct treeNode
{
    int min{INT_MAX}; // range min
    int max{INT_MIN}; // range max
    int sum{};        // range sum

    int lazy{};

    treeNode operator+(const treeNode& node) const { //Not being used
        treeNode res;
        res.min = std::min(this->min,node.min);
        res.max = std::max(this->max,node.max);
        res.sum = this->sum  + node.sum;
        
        res.lazy = 0;

        return res;
    }
};

class SegmentTree
{
public:
    vector<treeNode> tree; // sidenote: or treeNode *tree; //tree = new treeNode[treeSize];
    const int arraySize;
    const int treeSize;

public:
    SegmentTree(const vector<int> &arr) : arraySize(arr.size()), treeSize(4 * arraySize)
    {
        tree = vector<treeNode>(treeSize);
        build(0, 0, arraySize - 1, arr);
    }

    treeNode build(int idx, int left, int right, const vector<int> &arr);
    void update(int idx, int left, int right, const int START, const int END, const int HEIGHT);
    treeNode query(int idx, int left, int right, const int START, const int END);

    void print(int node, int left, int right, int depth);

public:
    void push_down(int idx, int left, int right);
    treeNode combine(const treeNode &lhs, const treeNode &rhs);
};

// COMMENT1
void SegmentTree::update(int idx, int left, int right, const int START, const int END, const int HEIGHT)
{
    if (currentNodeOutsideUpdateRange(left, right, START, END)) // not a valid range
        return;

    if (currentNodeInsideUpdateRange(left, right, START, END)) // this node can represent whole [START,END] range. (Which Node => 1st parameter of update)
    {
        tree[idx].lazy += HEIGHT; // lazy update this node
        return;
    }

    // If reached here => currentNodeOverlape with update range. (update range = [START,END], currentRange=[left,right])

    push_down(idx,left,right); // push lazy to child nodes

    update(lson, left, mid, START, END, HEIGHT);      // lazy update the left child
    update(rson, mid + 1, right, START, END, HEIGHT); // lazy update the right child

    tree[idx] = combine(tree[lson], tree[rson]); // write variation in which left,right node is saved above
}

treeNode SegmentTree::query(int idx, int left, int right, const int START, const int END)
{
    if (currentNodeOutsideUpdateRange(left, right, START, END))
    {
        return {}; // Return neutral element for combine operation
    }

    push_down(idx,left,right); //remove lazyiness before processing

    if (currentNodeInsideUpdateRange(left, right, START, END))
    {
        return tree[idx];
    }

    treeNode lhs = query(lson, left, mid, START, END);
    treeNode rhs = query(rson, mid + 1, right, START, END);

    return combine(lhs, rhs);
}

treeNode SegmentTree::combine(const treeNode &lhs, const treeNode &rhs)
{
    treeNode ret;
    ret.min = min(lhs.min + lhs.lazy, rhs.min + rhs.lazy);
    ret.max = max(lhs.max + lhs.lazy, rhs.max + rhs.lazy);

    ret.sum = lhs.sum + rhs.sum + lhs.lazy + rhs.lazy; //NOTE: if you make sure lhs & rhs are not lazy, then you can safely remove +.lazy.
    //In our case we can safely remove.

    return ret;
}

//node 'idx' represent range [left,right]
void SegmentTree::push_down(int idx, int left, int right)
{
    // (a) clear lazy from current node by transferring it to child nodes
    tree[idx].min += tree[idx].lazy; 
    tree[idx].max += tree[idx].lazy;
    tree[idx].sum +=tree[idx].lazy * (right-left+1); 

    
    if (left != right) // (b) update lazy[] for children nodes
    {
        tree[lson].lazy += tree[idx].lazy;
        tree[rson].lazy += tree[idx].lazy;
    }

    tree[idx].lazy = 0; // (c) current node processed. No longer lazy
}

int main()
{
    #ifndef ONLINE_JUDGE 
        freopen("input.txt", "r", stdin);
    #endif
    // Practice Problem
    int size;
    cin >> size;

    vector<int> arr;
    for (int i = 0; i < size; i++)
    {
        int t;
        cin >> t;
        arr.push_back(t);
    }

    SegmentTree st = SegmentTree(arr);
#ifdef DEBUG
    for (auto k : st.tree)
        cout << k.min << " ";
    cout << endl;
#endif

#ifdef DEBUG
    cout << "Printing segtree\n";
    st.print(0, 0, size - 1, 0);
#endif

    int NOperation;
    cin >> NOperation;
    cc cout << "NOperations: " << NOperation << endl;
    string temp;
    getline(cin, temp);

    for (int i = 0; i < NOperation; i++)
    {
        string line;
        getline(cin, line);

        stringstream ss(line);

        int a, b, c{};
        ss >> a;
        ss >> b;

        bool update = false;
        if (ss >> c)
        {
            update = true;
        }

        if (update == true)
        {
            cc printf("update (%d,%d,%d)\n", a, b, c);
            if (a > b)
            {
                st.update(0, 0, size - 1, 0, b, c);
                st.update(0, 0, size - 1, a, size - 1, c);
            }
            else
            {
                st.update(0, 0, size - 1, a, b, c);
            }
#ifdef DEBUG
            cout << "\n\t\t::st after update\n";
            st.print(0, 0, size - 1, 0);
#endif
        }
        else
        {
            cc printf("\n\t\t:: query (%d,%d)\n", a, b);
            treeNode result;

            if (a > b)
            {
                auto lhs = st.query(0, 0, size - 1, 0, b);
                auto rhs = st.query(0, 0, size - 1, a, size - 1);

                cc printf("lhs((%d,%d):%d), rhs((%d,%d):%d)\n", 0, b, lhs.min, a, size - 1, rhs.min);
                result = st.combine(lhs, rhs);
            }
            else
            {
                result = st.query(0, 0, size - 1, a, b);
            }
            cout << result.min << endl;
        }
    }
}

treeNode SegmentTree::build(int idx, int left, int right, const vector<int> &arr)
{
    if (left == right) // leaf node. store value in node.
    {
        
        int val = arr[left];

        tree[idx] = {val, val, val, 0}; //(min,max,sum,lazy)
        return tree[idx];
    }

    auto lhs = build(lson, left, mid, arr);
    auto rhs = build(rson, mid + 1, right, arr);

    // merge build results
    tree[idx] = combine(lhs, rhs);

    /*
        due to array implemention you can also use
        tree[idx] = combine(tree[lson],tree[rson])
    */

    return tree[idx];
}



void SegmentTree::print(int idx, int left, int right, int depth)
{
    if (left > right)
    {
        return;
    }

    // Print the current node
    std::cout << std::string(4 * depth, ' ') << "[" << left << ", " << right << "] ";
    std::cout << "Min: " << tree[idx].min << " ";
    // std::cout << "Max: " << tree[idx].max << " ";
    // std::cout << "Sum: " << tree[idx].sum << " ";
    std::cout << "Lazy: " << tree[idx].lazy;
    cout << std::endl;

    if (left == right)
    {
        return; // Leaf node
    }

    // Print the left subtree
    print(lson, left, mid, depth + 1);

    // Print the right subtree
    print(rson, mid + 1, right, depth + 1);
}
inline bool currentNodeOutsideUpdateRange(int left, int right, int START, int END)
{
    if (left > END)
        return true;
    if (right < START)
        return true;
    return false;
}

inline bool currentNodeInsideUpdateRange(int left, int right, int START, int END)
{
    if (START <= left && right <= END) // START -- left --right -- END
        return true;
    else
        return false;
}

// === Important Comment Section ===

/* COMMENT1:
        node: current node of tree
        left,right : left and right border of the segment. That this node stores (stores min or max if they exist)

        START,END : starting & ending index of update query. (NOTE: this is always greator than left ,right)
        height: we need to add height in the range [START,END]

*/

/*COMMENT2:

                We are coming from top-down manner. +
                We found that the current node which represent range [left,right] is withing the update range. =>
                we can lazly update here, instead of going down to childs of this node. (as they in turn will get updated by same)
*/

// =================================

// https://codeforces.com/problemset/problem/52/C

/*

SOLUTION1: https://codeforces.com/contest/52/submission/62163732

https://codeforces.com/contest/52/submission/30862123

SOLUTION3: leaf detection: https://codeforces.com/contest/52/submission/75825174

Solution4: better print: https://codeforces.com/contest/52/submission/195791377

Solution5: more similary: https://codeforces.com/contest/52/submission/195791377

Solution6: using common merge: https://codeforces.com/contest/52/submission/205204248

good sol: https://codeforces.com/contest/52/submission/207423983
*/
