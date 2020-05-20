#include <string>

using namespace std;

class TreeNode {
    private:
        string value;
        int count;
        TreeNode *left;
        TreeNode *right;
    public:
        TreeNode():value(""), count(0), left(nullptr), right(nullptr){};
        TreeNode(const TreeNode& rtn):value(rtn.value), count(rtn.count), left(rtn.left), right(rtn.right){};
        ~TreeNode();
};
