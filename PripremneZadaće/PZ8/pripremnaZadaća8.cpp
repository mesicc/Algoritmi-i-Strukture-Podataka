#include <iostream>
#include <algorithm>
using namespace std;

// Struktura čvora
struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
};

// Visina čvora
int height(Node* n) {
    return (n == nullptr) ? 0 : n->height;
}

// Novi čvor
Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    return node;
}

// Balans faktor
int getBalance(Node* n) {
    return (n == nullptr) ? 0 : height(n->left) - height(n->right);
}

// Desna rotacija (LL)
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Lijeva rotacija (RR)
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Umetanje u AVL stablo
Node* insert(Node* node, int key) {

    if (node == nullptr)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // LL
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // RR
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // LR
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Inorder ispis
void inorder(Node* root) {
    if (root != nullptr) {
        inorder(root->left);
        cout << root->key << " ";
        inorder(root->right);
    }
}

int main() {
    Node* root = nullptr;

    int day, month;
    cout << "Unesi dan i mjesec rodjenja: ";
    cin >> day >> month;

    int values[] = {10, 20, 40, 5, 3, 1, 50, 60, 55};
    int n = sizeof(values) / sizeof(values[0]);

    cout << "\nDatum rodjenja: " << day << "." << month << ".\n";
    cout << "AVL niz: ";

    for (int i = 0; i < n; i++) {
        cout << values[i] << " ";
        root = insert(root, values[i]);
    }

    cout << "\n\nInorder ispis AVL stabla:\n";
    inorder(root);

    return 0;
}
