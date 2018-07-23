#include "leetcode.h"

class Solution 
{
    void r_fillVector( TreeNode* n, int& k, int& v )
    {
        if( n->left != nullptr ) 
            r_fillVector( n->left, k, v );

        if( --k == 0 ) 
            v = n->val;

        if( n->right != nullptr ) 
            r_fillVector( n->right, k, v );
    }
public:
    int kthSmallest(TreeNode* root, int k) 
    {
        int v = 0;
        r_fillVector( root, k, v );
        return v;
    }
};

int main()
{
    TreeNode* node = stringToTreeNode("[3,1,4,null,2]");
    auto output = Solution().kthSmallest( node, 1 );
    return 0;
}
