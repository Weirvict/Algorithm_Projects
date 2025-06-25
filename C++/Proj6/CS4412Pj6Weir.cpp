#include <iostream>
#include <vector>

using namespace std;

/**
 * Author: Victoria Weir
 * Class: CS4412
 * Date: 12-10-23
 * Details: Project 6 - B+Tree
 *
 */

// Forward declarations
class Node;
class LeafNode;
class NonLeafNode;
class Person;

// Initialize Max Keys
int MAX_KEYS;

/**
 * Attempted to implement
 * Represents information about a person.
 */
class Person {
public:
    int id;               // Unique identifier for the person
    string name;     // Name of the person
    int age;              // Age of the person
    string address;  // Address of the person

    /**
     * Constructor to initialize a Person object with provided values.
     * @param id Unique identifier for the person.
     * @param name Name of the person.
     * @param age Age of the person.
     * @param address Address of the person.
     *
     */
    Person(int id, string name, int age, string address)
            : id(id), name(std::move(name)), age(age), address(std::move(address)) {
        // Constructor to initialize a Person object with provided values
    }
};

// Forward declarations
struct NonLeafNode;
struct LeafNode;

/**
 *
 * Represents a node in the B+Tree.
 * Each node can either be a leaf or a non-leaf node.
 *
 */
struct Node {
    // Flag indicating if the node is a leaf or not
    bool isLeaf;
    union {

        // Pointer to LeafNode if this node is a leaf
        LeafNode* leafNode;
        // Pointer to NonLeafNode if this node is not a leaf
        NonLeafNode* nonLeafNode;
    };

    /**
    * Constructor for Node that explicitly sets the node type (leaf or non-leaf).
    * @param isLeafNode Flag indicating if the node is a leaf or not.
    */
    explicit Node(bool isLeafNode);
    // Pointer to the parent node
    Node* parent{};

    /**
     * Function to add a child node to the current node.
     * @param child Node* Pointer to the child node being added.
     */
    void addChild(Node* child);
};

/**
 * Represents a leaf node in the B+Tree.
 * Leaf nodes contain keys and a pointer to the next leaf node.
 */
struct LeafNode {
    // Vector to store keys in the leaf node
    vector<int> keys;
    // Pointer to the next leaf node
    LeafNode* next{};

    // Initializes a LeafNode object.
    LeafNode();

    /**
     * Function to insert a key into the leaf node.
     * @param key The key to be inserted.
     * @return bool True if the insertion was successful, false otherwise.
     */
    bool insert(int key);
};

/**
 * Represents a non-leaf node in the B+Tree.
 * Non-leaf nodes contain keys, pointers to child nodes, and a pointer to the parent node.
 */
struct NonLeafNode {
    // Pointers to child nodes
    vector<Node*> children;
    // Keys
    vector<int> keys;

    // Initializes a NonLeafNode object.
    NonLeafNode();

    // Pointer to the parent node
    Node* parent{};

    /**
     * Function to insert a key into the non-leaf node.
     * @param key The key to be inserted.
     * @return bool True if the insertion was successful, false otherwise.
     */
    bool insert(int key);

    /**
     * Function to add a child node to the non-leaf node.
     * @param child Pointer to the node to be added as a child.
     */
    void addChild(Node* child);
};

// Node constructor implementation
Node::Node(bool isLeafNode) : isLeaf(isLeafNode) {
    if (isLeaf) {
        // Initialize as a LeafNode
        leafNode = new LeafNode();
    } else {
        // Initialize as a NonLeafNode
        nonLeafNode = new NonLeafNode();
    }
}

// LeafNode constructor implementation
LeafNode::LeafNode() {
    // Initialize keys
    keys.reserve(MAX_KEYS);
}

/**
 * Inserts a key into the leaf node if it's not full.
 * If the node is full, performs splitting logic to accommodate the new key.
 * @param key The key to be inserted.
 * @return Returns true if the key is successfully inserted, false if splitting is required.
 */
bool LeafNode::insert(int key) {
    if (keys.size() < MAX_KEYS) {
        // Insert the key at the end of the leaf node
        keys.push_back(key);
        // Insertion successful
        return true;
    } else {
        // Leaf node is full, perform splitting logic
        auto* newLeaf = new LeafNode();

        // Calculate the split point
        int splitPoint = keys.size() / 2; // Split in half

        // Move keys to the new leaf node
        for (int i = splitPoint; i < keys.size(); ++i) {
            newLeaf->keys.push_back(keys[i]);
        }
        keys.erase(keys.begin() + splitPoint, keys.end());

        // Insert the key into the appropriate node
        if (key < newLeaf->keys[0]) {
            keys.push_back(key);
        } else {
            newLeaf->keys.push_back(key);
        }

        // Handle parent node adjustment
        if (key < newLeaf->keys[0]) {
            // Update the current leaf's next pointer to point to the new leaf
            newLeaf->next = next;
            next = newLeaf;
        } else {
            // Update the new leaf's next pointer to point to the current leaf's next
            newLeaf->next = next;
            next = newLeaf;
        }

        return false; // Indicate unsuccessful insertion to handle parent node adjustment
    }
}

// NonLeafNode constructor implementation
NonLeafNode::NonLeafNode() {
    children.reserve(MAX_KEYS + 1); // Initialize children and keys
    keys.reserve(MAX_KEYS);
}

/**
 * Adds a child node to this node.
 * If this node is a non-leaf node, the child is added to its list of children.
 * Sets the child's parent pointer to this node.
 * @param child The node to be added as a child.
 */
void Node::addChild(Node* child) {
    // Ensure that it is a non-leaf node before proceeding
    if (!isLeaf) {
        NonLeafNode* nonLeaf = nonLeafNode;
        nonLeaf->children.push_back(child);

        // Set the child's parent to this node
        child->parent = this;
    }
}

/**
 * Adds a child node to this non-leaf node.
 * Sets the child's parent pointer to this node.
 * @param child The node to be added as a child.
 */
void NonLeafNode::addChild(Node* child) {
    // Add child node to the list of children
    children.push_back(child);

    // Set the child's parent to this non-leaf node
    child->parent = reinterpret_cast<Node*>(this);
}

/**
 * Inserts a key into the non-leaf node and handles splitting if necessary.
 * @param key The key to be inserted.
 * @return A boolean indicating if the insertion was successful.
 */
bool NonLeafNode::insert(int key) {
    Node* root; // Declaration of root node
    Node* currentNode = root;
    Node* parent = nullptr;  // Initialize parent here
    NonLeafNode* newParent = nullptr;
    NonLeafNode* nonLeaf = nullptr;  // Define nonLeaf here

    // Find the child node to insert into
    int i = 0;
    while (i < keys.size() && key >= keys[i]) {
        ++i;
    }

    while (!currentNode->isLeaf) {
        NonLeafNode* nonLeaf = currentNode->nonLeafNode;
        int j = 0; // Use a different variable name, or reuse 'i' from the outer scope
        while (j < nonLeaf->keys.size() && key >= nonLeaf->keys[j]) {
            ++j;
        }
        // Set the parent before moving to the next node
        parent = currentNode;
        // Move to the child node
        currentNode = nonLeaf->children[j];
    }

    // Now, 'currentNode' is the leaf node to insert the key into
    LeafNode* leaf = currentNode->leafNode;
    if (children[i]->isLeaf) {
        // If the child is a leaf node, insert the key into it
        LeafNode *childLeafNode = children[i]->leafNode;
        // Move to the child node
        parent = currentNode; // Set the parent before moving to the next node
        currentNode = nonLeaf->children[i];

        if (childLeafNode->keys.size() < MAX_KEYS - 1) {
            // Leaf node is not full, insert the key
            childLeafNode->insert(key);
            return true; // Insertion successful
        } else {
            // Perform splitting of the leaf node
            LeafNode *newLeaf = new LeafNode();

            // Calculate the split point
            int splitPoint = (MAX_KEYS + 1) / 2;

            // Move half of the keys to the new leaf node
            for (int j = splitPoint; j < childLeafNode->keys.size(); ++j) {
                newLeaf->keys.push_back(childLeafNode->keys[j]);
            }
            childLeafNode->keys.erase(childLeafNode->keys.begin() + splitPoint, childLeafNode->keys.end());

            // Insert the key into the appropriate node
            if (key < newLeaf->keys[0]) {
                childLeafNode->insert(key);
            } else {
                newLeaf->insert(key);
            }

            // Adjust the pointers and keys in the NonLeafNode after splitting
            children.insert(children.begin() + i + 1, reinterpret_cast<Node *const>(newLeaf));

            // Add the smallest key of the new leaf node to the parent
            keys.insert(keys.begin() + i, newLeaf->keys[0]);

            // Check if the parent node is full and handle splitting recursively if needed
            if (keys.size() >= MAX_KEYS) {
                // Parent node is full, perform splitting
                NonLeafNode *newParent = new NonLeafNode();
                int parentSplitPoint = (MAX_KEYS + 1) / 2;

                // Move half of the keys to the new parent node
                for (int j = parentSplitPoint; j < keys.size(); ++j) {
                    newParent->keys.push_back(keys[j]);
                }
                keys.erase(keys.begin() + parentSplitPoint, keys.end());

                // Move corresponding children to the new parent node
                for (int j = parentSplitPoint; j < children.size(); ++j) {
                    newParent->children.push_back(children[j]);
                }
                children.erase(children.begin() + parentSplitPoint, children.end());

                // Adjust the pointers and keys in the parent after splitting
                if (this->parent != nullptr) {
                    // The current node has a parent
                    // Add the newParent as a child to the current node's parent
                    this->parent->addChild(reinterpret_cast<Node *>(newParent)); // AddChild function adds the node to parent's children list
                } else {
                    // The current node is the root
                    // Create a new root node and set it as the parent
                    NonLeafNode *newRoot = new NonLeafNode();
                    newRoot->addChild(reinterpret_cast<Node *>(this)); // Add the current node as a child
                    newRoot->addChild(reinterpret_cast<Node *>(newParent)); // Add the newParent as a child
                    this->parent = reinterpret_cast<Node *>(newRoot); // Set the newRoot as the parent of the current node
                }
                return false; // Indicate unsuccessful insertion to handle parent node adjustment
            }
        }
    } else {
        // If the child is a non-leaf node, recursively insert into it
        NonLeafNode *childNonLeafNode = children[i]->nonLeafNode;
        if (!childNonLeafNode->insert(key)) {
            // Handle the splitting of child non-leaf node if needed
            if (childNonLeafNode->keys.size() >= MAX_KEYS) {
                // Child node is full, perform splitting
                NonLeafNode *newChildNode = new NonLeafNode();
                int childSplitPoint = (MAX_KEYS + 1) / 2;

                // Move half of the keys to the new child node
                for (int j = childSplitPoint; j < childNonLeafNode->keys.size(); ++j) {
                    newChildNode->keys.push_back(childNonLeafNode->keys[j]);
                }
                childNonLeafNode->keys.erase(childNonLeafNode->keys.begin() + childSplitPoint,childNonLeafNode->keys.end());

                // Move corresponding children to the new child node
                for (int j = childSplitPoint; j < childNonLeafNode->children.size(); ++j) {
                    newChildNode->children.push_back(childNonLeafNode->children[j]);
                }
                childNonLeafNode->children.erase(childNonLeafNode->children.begin() + childSplitPoint,childNonLeafNode->children.end());

                // Adjust the pointers and keys in the parent after splitting
                if (this->parent != nullptr) {
                    // The current node has a parent
                    // Add the newParent as a child to the current node's parent
                    this->parent->addChild(reinterpret_cast<Node *>(newParent)); // AddChild function adds the node to parent's children list
                } else {
                    // The current node is the root
                    // Create a new root node and set it as the parent
                    NonLeafNode *newRoot = new NonLeafNode();
                    newRoot->addChild(reinterpret_cast<Node *>(this)); // Add the current node as a child
                    newRoot->addChild(reinterpret_cast<Node *>(newParent)); // Add the newParent as a child
                    this->parent = reinterpret_cast<Node *>(newRoot); // Set the newRoot as the parent of the current node
                }
                // Indicate unsuccessful insertion to handle parent node adjustment
                return false;
            }
        }
        // Insertion successful
        return true;
    }
    return false;
}

/**
 * Represents a B+Tree structure.
 */
class BPlusTree {
public:

    // Pointer to the root node of the B+Tree
    Node* root;

    /**
     * Constructor to create a B+Tree with the given maximum keys.
     * @param maxKeys The maximum keys allowed in a node.
     */
    explicit BPlusTree(size_t maxKeys) : root(nullptr), MAX_KEYS(maxKeys) {}

    /**
     * Inserts a key into the B+Tree.
     * @param key The key to be inserted.
     */
    void insert(int key);

    /**
     * Searches for a key in the B+Tree.
     * @param key The key to search for.
     * @return A boolean indicating if the key was found.
     */
    bool search(int key);

private:
    // Maximum keys allowed in a node
    size_t MAX_KEYS;
};


/**
 * Searches for a key in the B+Tree.
 * @param key The key to search for.
 * @return A boolean indicating if the key was found.
 */
bool BPlusTree::search(int key) {
    Node* currentNode = root;

    while (!currentNode->isLeaf) {
        // Traverse down to the appropriate leaf node
        NonLeafNode* nonLeaf = currentNode->nonLeafNode;
        // Find the child node to move to based on the key
        int i = 0;
        while (i < nonLeaf->keys.size() && key >= nonLeaf->keys[i]) {
            ++i;
        }
        // Move to the child node
        currentNode = nonLeaf->children[i];
    }

    // 'currentNode' is now a leaf node
    LeafNode* leaf = currentNode->leafNode;

    // Linear search in the leaf node
    for (int leaf_key : leaf->keys) {
        if (key == leaf_key) {
            return true; // Key found
        }
    }
    return false; // Key isn't found
}


/**
 * Inserts a key into the B+Tree.
 * @param key The key to be inserted.
 */
void BPlusTree::insert(int key) {
    if (root == nullptr) {
        root = new Node(true); // Create a leaf node as root
    }

    Node* currentNode = root;
    Node* parent = nullptr;

    while (!currentNode->isLeaf) {
        // Traverse down to the appropriate leaf node
        NonLeafNode* nonLeaf = currentNode->nonLeafNode;
        // Find the child node to move to
        int i = 0;
        while (i < nonLeaf->keys.size() && key >= nonLeaf->keys[i]) {
            ++i;
        }
        // Move to the child node
        parent = currentNode;
        currentNode = nonLeaf->children[i];
    }

    // Now, 'currentNode' is the leaf node to insert the key into
    LeafNode* leaf = currentNode->leafNode;

    if (!leaf->insert(key)) {
        // Leaf node is full, perform splitting logic
        auto *newLeaf = new LeafNode();
        // Split the keys between 'leaf' and 'newLeaf'
        if (parent == nullptr) {
            // If the root splits, create a new root
            Node *newRoot = new Node(false); // Create a non-leaf node
            newRoot->nonLeafNode->children.push_back(currentNode); // Add existing leaf as child
            newRoot->nonLeafNode->children.push_back(reinterpret_cast<Node *const>(newLeaf)); // Add new leaf as child
            newRoot->nonLeafNode->keys.push_back(newLeaf->keys[0]); // Add the smallest key of the new leaf node to the parent
            root = newRoot; // Update the root
        } else {
            // Add the newLeaf to the parent
            NonLeafNode *nonLeaf = parent->nonLeafNode;
            int i = 0;
            while (i < nonLeaf->children.size() && nonLeaf->children[i] != currentNode) {
                ++i;
            }
            // Insert the newLeaf and its corresponding key into the parent
            nonLeaf->children.insert(nonLeaf->children.begin() + i + 1, reinterpret_cast<Node *const>(newLeaf));
            // Add the smallest key of the new leaf node to the parent
            nonLeaf->keys.insert(nonLeaf->keys.begin() + i,newLeaf->keys[0]);

            // Check if the parent node is full and handle splitting recursively if needed
            while (nonLeaf->keys.size() >= MAX_KEYS) {
                // Parent node is full, perform splitting
                NonLeafNode* newParent = new NonLeafNode();

                // Calculate the split point
                int splitPoint = (MAX_KEYS + 1) / 2;

                // Move half of the keys to the new parent node
                for (int j = splitPoint; j < nonLeaf->keys.size(); ++j) {
                    newParent->keys.push_back(nonLeaf->keys[j]);
                }
                nonLeaf->keys.erase(nonLeaf->keys.begin() + splitPoint, nonLeaf->keys.end());

                // Move corresponding children to the new parent node
                for (int j = splitPoint; j < nonLeaf->children.size(); ++j) {
                    newParent->children.push_back(nonLeaf->children[j]);
                }
                nonLeaf->children.erase(nonLeaf->children.begin() + splitPoint, nonLeaf->children.end());

                // Adjust pointers and keys in the grandparent (if it exists)
                if (parent != nullptr) {
                    NonLeafNode* grandparent = parent->nonLeafNode;
                    // Find the position to insert the newParent into the grandparent's children list
                    int i = 0;
                    while (i < grandparent->children.size() && grandparent->children[i] != reinterpret_cast<Node *>(nonLeaf)) {
                        ++i;
                    }
                    // Insert newParent and its corresponding key into the grandparent
                    grandparent->children.insert(grandparent->children.begin() + i + 1, reinterpret_cast<Node *>(newParent));
                    grandparent->keys.insert(grandparent->keys.begin() + i, newParent->keys[0]);

                    // Check if the grandparent is also full and adjust recursively if needed
                    parent = parent->parent;
                    nonLeaf = grandparent;
                } else {
                    // The current node is the root, create a new root
                    Node* newRoot = new Node(false);
                    newRoot->nonLeafNode->children.push_back(reinterpret_cast<Node *>(nonLeaf));
                    newRoot->nonLeafNode->children.push_back(reinterpret_cast<Node *>(newParent));
                    newRoot->nonLeafNode->keys.push_back(newParent->keys[0]); // Add the smallest key of the new parent node to the root
                    root = newRoot; // Update the root
                    break; // Exit the loop as the root has been updated
                }
            }
        }
    }
}

/**
 * Recursively prints the structure of the B+Tree starting from a given node.
 * @param node The starting node to print from.
 * @param depth The depth of the current node in the B+Tree (default: 0).
 */
void printBPlusTree(Node* node, int depth = 0) {
    if (node == nullptr) {
        return;
    }

    if (node->isLeaf) {
        LeafNode* leaf = node->leafNode;
        cout << "Leaf Node - Keys: ";
        for (int key : leaf->keys) {
            cout << key << " ";
        }
        cout << endl;
    } else {
        NonLeafNode* nonLeaf = node->nonLeafNode;
        cout << "Non-Leaf Node - Keys: ";
        for (int key : nonLeaf->keys) {
            cout << key << " ";
        }
        cout << endl;

        for (Node* child : nonLeaf->children) {
            printBPlusTree(child, depth + 1);
        }
    }
}

int main(){
    size_t maxKeys;
    cout << "Enter the maximum number of keys in a leaf node: ";
    cin >> maxKeys;

    BPlusTree bPlusTree(maxKeys);

    int value;
    cout << "Enter values to insert into the B+Tree (enter -1 to stop):\n";
    while (true) {
        cin >> value;
        if (value == -1) {
            break;
        }
        bPlusTree.insert(value);
    }

    // Call this after you finish inserting values
    printBPlusTree(bPlusTree.root);

    return 0;
}

