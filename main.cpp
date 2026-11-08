#include <iostream>

#include "src/factorial.hpp"

// Example Usage Demonstration
int main() {
    AVLtree tree;

    // Insert nodes
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);

    // Serialize tree
    std::string serializedData = tree.serialize();
    std::cout << "Serialized Tree: " << serializedData << std::endl;

    // Reconstruct tree using deserialize
    AVLtree restoredTree = AVLtree::deserialize(serializedData);
    std::cout << "Deserialized and Re-serialized: " << restoredTree.serialize() << std::endl;

    // Remove a node
    restoredTree.remove(30);
    std::cout << "After deleting 30: " << restoredTree.serialize() << std::endl;

    return 0;
}
