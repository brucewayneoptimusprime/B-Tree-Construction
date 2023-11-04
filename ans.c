#include <stdio.h>
#include <stdlib.h>
#define MAX_KEYS 4 // Order of the B-Tree

// Structure for a B-Tree node
struct BTreeNode {
    int *keys;
    struct BTreeNode **childPointers;
    int isLeaf;
    int numKeys;
};

// Function to create a new node
struct BTreeNode *createNode(int isLeaf) {
    struct BTreeNode *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    newNode->isLeaf = isLeaf;
    newNode->numKeys = 0;
    newNode->keys = (int *)malloc(sizeof(int) * (MAX_KEYS - 1));
    newNode->childPointers = (struct BTreeNode **)malloc(sizeof(struct BTreeNode *) * MAX_KEYS);
    return newNode;
}

// Function to search a key in the B-Tree
int searchKey(int key, struct BTreeNode *root) {
    int index = 0;
    while (index < root->numKeys && key > root->keys[index]) {
        index++;
    }
    return index;
}

// Function to split a full child of the current node
void splitChild(struct BTreeNode *parentNode, int index, struct BTreeNode *childNode) {
    struct BTreeNode *newNode = createNode(childNode->isLeaf);
    newNode->numKeys = MAX_KEYS - 1;

    for (int i = 0; i < MAX_KEYS - 1; i++) {
        newNode->keys[i] = childNode->keys[i + MAX_KEYS];
    }

    if (childNode->isLeaf == 0) {
        for (int i = 0; i < MAX_KEYS; i++) {
            newNode->childPointers[i] = childNode->childPointers[i + MAX_KEYS];
        }
    }

    childNode->numKeys = MAX_KEYS - 1;

    for (int i = parentNode->numKeys; i >= index + 1; i--) {
        parentNode->childPointers[i + 1] = parentNode->childPointers[i];
    }
    parentNode->childPointers[index + 1] = newNode;

    for (int i = parentNode->numKeys - 1; i >= index; i--) {
        parentNode->keys[i + 1] = parentNode->keys[i];
    }
    parentNode->keys[index] = childNode->keys[MAX_KEYS - 1];

    parentNode->numKeys++;
}

// Function to insert a key into the B-Tree
void insertKeyNonFull(struct BTreeNode *node, int key) {
    int index = node->numKeys - 1;
    if (node->isLeaf) {
        while (index >= 0 && key < node->keys[index]) {
            node->keys[index + 1] = node->keys[index];
            index--;
        }
        node->keys[index + 1] = key;
        node->numKeys++;
    } else {
        while (index >= 0 && key < node->keys[index]) {
            index--;
        }
        index++;

        if (node->childPointers[index]->numKeys == MAX_KEYS - 1) {
            splitChild(node, index, node->childPointers[index]);
            if (key > node->keys[index]) {
                index++;
            }
        }
        insertKeyNonFull(node->childPointers[index], key);
    }
}

// Function to insert a key into the B-Tree
void insertKey(struct BTreeNode **root, int key) {
    if ((*root)->numKeys == MAX_KEYS - 1) {
        struct BTreeNode *newNode = createNode(0);
        newNode->childPointers[0] = *root;
        *root = newNode;
        splitChild(newNode, 0, *root);
        insertKeyNonFull(newNode, key);
    } else {
        insertKeyNonFull(*root, key);
    }
}

// Function to traverse the B-Tree
void traverseBTree(struct BTreeNode *root) {
    int i;
    for (i = 0; i < root->numKeys; i++) {
        if (root->isLeaf == 0) {
            traverseBTree(root->childPointers[i]);
        }
        printf(" %d", root->keys[i]);
    }

    if (root->isLeaf == 0) {
        traverseBTree(root->childPointers[i]);
    }
}

// Main function
int main() {
    struct BTreeNode *root = createNode(1);
    insertKey(&root, 10);
    insertKey(&root, 20);
    insertKey(&root, 5);
    insertKey(&root, 6);
    insertKey(&root, 12);
    insertKey(&root, 30);
    insertKey(&root, 7);
    insertKey(&root, 17);

    printf("B-Tree traversal: ");
    traverseBTree(root);
    printf("\n");

    return 0;
}
