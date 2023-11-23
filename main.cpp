#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// Node structure for the Huffman Tree
struct Node {
    char data;
    unsigned frequency;
    Node *left, *right;

    Node(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparison function for priority queue
struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->frequency > right->frequency;
    }
};

// Build the Huffman Tree
Node* buildHuffmanTree(const string& text) {
    // Count the frequency of each character in the text
    unordered_map<char, unsigned> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    // Create a priority queue of Nodes
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // Create a leaf Node for each character and add it to the priority queue
    for (auto& pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Build the Huffman Tree
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        Node* internalNode = new Node('$', left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;

        pq.push(internalNode);
    }

    return pq.top();
}

// Generate Huffman Codes
void generateHuffmanCodes(Node* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root) {
        return;
    }

    if (root->data != '$') {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Encode the text using Huffman Codes
string encodeText(const string& text, const unordered_map<char, string>& huffmanCodes) {
    string encodedText;
    for (char ch : text) {
        encodedText += huffmanCodes.at(ch);
    }
    return encodedText;
}

// Decode the text using Huffman Tree
string decodeText(Node* root, const string& encodedText) {
    string decodedText;
    Node* current = root;
    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current->data != '$') {
            decodedText += current->data;
            current = root;
        }
    }
    return decodedText;
}

int main() {
    string text = "hello world";

    // Build Huffman Tree
    Node* huffmanTree = buildHuffmanTree(text);

    // Generate Huffman Codes
    unordered_map<char, string> huffmanCodes;
    generateHuffmanCodes(huffmanTree, "", huffmanCodes);

    // Encode the text
    string encodedText = encodeText(text, huffmanCodes);
    cout << "Encoded Text: " << encodedText << endl;

    // Decode the text
    string decodedText = decodeText(huffmanTree, encodedText);
    cout << "Decoded Text: " << decodedText << endl;

    return 0;
}
