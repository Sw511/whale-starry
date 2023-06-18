#include <iostream>
enum RBColor { RED, BLACK };
template <class T>
struct RBTreeNode {
  T _data;
  RBColor _color;
  RBTreeNode<T>* _left;
  RBTreeNode<T>* _right;
  RBTreeNode<T>* _parent;

  RBTreeNode(const T& x = T(), RBColor c = RED)
      : _left(nullptr), _right(nullptr), _parent(nullptr), _data(x), _color(c) {}
};

template <class T>
class RBTree {
  typedef RBTreeNode<T> Node;

 public:
  RBTree() {
    _head = new Node;
    _head->_left = _head;
    _head->_right = _head;
    _head->_parent = nullptr;
  }

  ~RBTree() {
    Destroy(_head->_parent);
    delete _head;
    _head = nullptr;
  }

  bool insert(const T& data) {
    // 先找到红黑树的根节点
    Node*& root = _GetRoot();
    if (root == nullptr)  // 说明红黑树为空
    {
      root = new Node(data, BLACK);
      root->_parent = _head;
      _head->_left = root;
      _head->_right = root;
      _head->_parent = root;
      return true;
    }

    Node* cur = root;
    Node* parent = _head;
    while (cur) {
      parent = cur;
      if (data > cur->_data)
        cur = cur->_right;
      else if (data < cur->_data)
        cur = cur->_left;
      else  // 不插入重复的元素
        return false;
    }

    cur = new Node(data);
    cur->_parent = parent;
    if (data > parent->_data)
      parent->_right = cur;
    else
      parent->_left = cur;

    // 检测节点颜色

    while (parent != _head && parent->_color == RED) {
      Node* grandfather = parent->_parent;  // 祖父

      if (parent == grandfather->_left)  // 在左侧
      {
        Node* uncle = grandfather->_right;  // 叔叔节点
        if (uncle && uncle->_color == RED)  // 第一种情况
        {
          parent->_color = BLACK;
          uncle->_color = BLACK;
          grandfather->_color = RED;
          cur = grandfather;
          parent = cur->_parent;
        } else  // 第2 3种情况
        {
          if (cur == parent->_right)  // 第三种情况,先左单旋，交换cur 和parent节点
          {
            RotateLeft(parent);
            swap(parent, cur);
          }

          RotateRight(grandfather);  // 再对grandfather节点进行右单旋
          parent->_color = BLACK;
          grandfather->_color = RED;  // 重新染色
        }
      } else {                              // 在右侧
        Node* uncle = grandfather->_left;   // 叔叔节点
        if (uncle && uncle->_color == RED)  // 第4种情况
        {
          parent->_color = BLACK;
          uncle->_color = BLACK;
          grandfather->_color = RED;
          cur = grandfather;
          parent = cur->_parent;
        } else  // 第5 6种情况
        {
          if (cur == parent->_left)  // 第5种情况,先右单旋，交换cur 和parent节点
          {
            RotateRight(parent);
            swap(parent, cur);
          }

          RotateLeft(grandfather);  // 再对grandfather节点进行左单旋
          parent->_color = BLACK;
          grandfather->_color = RED;  // 重新染色
        }
      }
    }

    root->_color = BLACK;
    _head->_left = LeftMost();
    _head->_right = RightMost();
    return true;
  }

  void inOrder() {
    _inOrder(GetRoot());
    cout << endl;
  }

  bool isRBTree() {
    Node* root = _head->_parent;
    Node* cur = root;
    int BLACKCOUNT = 0;
    while (cur != nullptr) {
      if (cur->_color == BLACK) ++BLACKCOUNT;
      cur = cur->_left;
    }

    return _isRBTree(root, 0, BLACKCOUNT);
  }

 private:
  bool _isRBTree(Node* root, int PATHBLACKCOUNT, const int& BLACKCOUNT) {
    if (root == nullptr) return true;
    if (root->_color == BLACK) PATHBLACKCOUNT++;

    // 性质3
    if (root->_left == nullptr && root->_right == nullptr &&
        (PATHBLACKCOUNT != BLACKCOUNT)) {
      cout << "违反了性质3" << endl;
      return false;
    }

    // 性质2
    if (root->_parent != _head && root->_color == RED && root->_parent->_color == RED) {
      cout << "违反了性质2" << endl;
      return false;
    }

    return _isRBTree(root->_left, PATHBLACKCOUNT, BLACKCOUNT) &&
           _isRBTree(root->_right, PATHBLACKCOUNT, BLACKCOUNT);
  }

  Node*& _GetRoot() const { return _head->_parent; }

  void Destroy(Node*& root) {
    if (root == nullptr) return;
    Destroy(root->_left);
    Destroy(root->_right);
    delete root;
    root = nullptr;
  }

  Node* LeftMost() const {
    Node* cur = _head->_parent;
    while (cur->_left) cur = cur->_left;
    return cur;
  }

  Node* RightMost() const {
    Node* cur = _head->_parent;
    while (cur->_right) cur = cur->_right;
    return cur;
  }

  void RotateRight(Node* parent)  // 右单旋
  {
    Node* subL = parent->_left;

    parent->_left = subL->_right;
    if (subL->_right != nullptr) subL->_right->_parent = parent;
    subL->_right = parent;
    subL->_parent = parent->_parent;
    if (parent->_parent != _head) {
      if (parent == parent->_parent->_left)
        parent->_parent->_left = subL;
      else
        parent->_parent->_right = subL;
    } else {
      parent->_parent->_parent = subL;
    }
    parent->_parent = subL;
  }

  void RotateLeft(Node* parent)  // 左单旋
  {
    Node* subR = parent->_right;
    parent->_right = subR->_left;
    subR->_parent = parent->_parent;
    if (subR->_left != nullptr) subR->_left->_parent = parent;
    subR->_left = parent;
    if (parent->_parent != _head) {
      if (parent == parent->_parent->_left)
        parent->_parent->_left = subR;
      else
        parent->_parent->_right = subR;
    } else {
      parent->_parent->_parent = subR;
    }
    parent->_parent = subR;
  }

  void _inOrder(Node* root) const {
    if (root == nullptr) return;
    _inOrder(root->_left);

    std::cout << root->_data << "  ";

    _inOrder(root->_right);
  }

 private:
  Node* _head;
};
