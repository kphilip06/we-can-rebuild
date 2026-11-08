#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

// Node Structure Definition
struct Node {
    int data;
    int height;
    Node* left;
    Node* right;

    Node(int val) : data(val), height(1), left(nullptr), right(nullptr) {}
};

class AVLtree {
  private:
    Node* root;

    // Helper: Safely get node height
    int getHeight(Node* node) {
        return (node == nullptr) ? 0 : node->height;
    }

    // Helper: Safely calculate balance factor
    int getBalanceFactor(Node* node) {
        return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    // Helper: Recalculate height of a node
    void updateHeight(Node* node) {
        if (node != nullptr) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    // Right Rotation (LL Case)
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Left Rotation (RR Case)
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // Find the node with minimum value in a subtree
    Node* getMinValueNode(Node* node) {
        Node* current = node;
        while (current != nullptr && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Internal Recursive Insertion
    Node* insertNode(Node* currentNode, int value) {
        if (currentNode == nullptr) {
            return new Node(value);
        }

        if (value < currentNode->data) {
            currentNode->left = insertNode(currentNode->left, value);
        } else if (value > currentNode->data) {
            currentNode->right = insertNode(currentNode->right, value);
        } else {
            return currentNode; // Duplicates are ignored
        }

        updateHeight(currentNode);
        int balance = getBalanceFactor(currentNode);

        // Left Left (LL) Case
        if (balance > 1 && value < currentNode->left->data) {
            return rotateRight(currentNode);
        }

        // Right Right (RR) Case
        if (balance < -1 && value > currentNode->right->data) {
            return rotateLeft(currentNode);
        }

        // Left Right (LR) Case
        if (balance > 1 && value > currentNode->left->data) {
            currentNode->left = rotateLeft(currentNode->left);
            return rotateRight(currentNode);
        }

        // Right Left (RL) Case
        if (balance < -1 && value < currentNode->right->data) {
            currentNode->right = rotateRight(currentNode->right);
            return rotateLeft(currentNode);
        }

        return currentNode;
    }

    // Internal Recursive Deletion
    Node* deleteNode(Node* currentNode, int value) {
        if (currentNode == nullptr) {
            return currentNode;
        }

        if (value < currentNode->data) {
            currentNode->left = deleteNode(currentNode->left, value);
        } else if (value > currentNode->data) {
            currentNode->right = deleteNode(currentNode->right, value);
        } else {
            // Node to be deleted found
            if (currentNode->left == nullptr || currentNode->right == nullptr) {
                Node* temp = currentNode->left ? currentNode->left : currentNode->right;

                if (temp == nullptr) {
                    temp = currentNode;
                    currentNode = nullptr;
                } else {
                    *currentNode = *temp;
                }
                delete temp;
            } else {
                Node* temp = getMinValueNode(currentNode->right);
                currentNode->data = temp->data;
                currentNode->right = deleteNode(currentNode->right, temp->data);
            }
        }

        if (currentNode == nullptr) {
            return currentNode;
        }

        updateHeight(currentNode);
        int balance = getBalanceFactor(currentNode);

        // Left Left (LL) Case
        if (balance > 1 && getBalanceFactor(currentNode->left) >= 0) {
            return rotateRight(currentNode);
        }

        // Left Right (LR) Case
        if (balance > 1 && getBalanceFactor(currentNode->left) < 0) {
            currentNode->left = rotateLeft(currentNode->left);
            return rotateRight(currentNode);
        }

        // Right Right (RR) Case
        if (balance < -1 && getBalanceFactor(currentNode->right) <= 0) {
            return rotateLeft(currentNode);
        }

        // Right Left (RL) Case
        if (balance < -1 && getBalanceFactor(currentNode->right) > 0) {
            currentNode->right = rotateRight(currentNode->right);
            return rotateLeft(currentNode);
        }

        return currentNode;
    }

    // Internal Pre-order Serialization Helper
    void serializeHelper(Node* currentNode, std::string& result) {
        if (currentNode == nullptr) {
            result += "N,";
            return;
        }

        result += std::to_string(currentNode->data) + ",";
        serializeHelper(currentNode->left, result);
        serializeHelper(currentNode->right, result);
    }

    // Internal Pre-order Deserialization Helper
    Node* deserializeHelper(const std::vector<std::string>& tokens, int& index) {
        if (index >= static_cast<int>(tokens.size()) || tokens[index] == "N") {
            index++;
            return nullptr;
        }

        int val = std::stoi(tokens[index++]);
        Node* newNode = new Node(val);

        newNode->left = deserializeHelper(tokens, index);
        newNode->right = deserializeHelper(tokens, index);

        updateHeight(newNode);
        return newNode;
    }

    // Internal Destructor Helper
    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

  public:
    AVLtree() : root(nullptr) {}

    ~AVLtree() {
        destroyTree(root);
    }

    void insert(int value) {
        root = insertNode(root, value);
    }

    void remove(int value) {
        root = deleteNode(root, value);
    }

    std::string serialize() {
        std::string result = "";
        serializeHelper(root, result);
        if (!result.empty()) {
            result.pop_back(); // Remove trailing comma
        }
        return result;
    }

    static AVLtree deserialize(const std::string& str) {
        AVLtree tree;
        if (str.empty() || str == "N") {
            return tree;
        }

        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        int index = 0;
        tree.root = tree.deserializeHelper(tokens, index);
        return tree;
    }
};
