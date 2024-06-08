#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

// TrieNode structure for a trie node
struct TrieNode {
	unordered_map<char, TrieNode*> children;
	string definition;
	bool isEndOfWord;

	TrieNode() : isEndOfWord(false) {}
};

// Class for the dictionary using a trie
class TrieDictionary {
private:
	TrieNode* root;

public:
	TrieDictionary() : root(new TrieNode()) {}

	// Function to add a word and its definition to the trie
	void addWord(const string& word, const string& definition) {
		TrieNode* current = root;
		for (char ch : word) {
			if (current->children.find(ch) == current->children.end()) {
				current->children[ch] = new TrieNode();
			}
			current = current->children[ch];
		}
		current->isEndOfWord = true;
		current->definition = definition;
	}

	// Function to search for a word in the trie
	void searchWord(const string& word) const {
		TrieNode* current = root;
		for (char ch : word) {
			if (current->children.find(ch) == current->children.end()) {
				cout << "Word not found in the dictionary." << endl;
				return;
			}
			current = current->children[ch];
		}

		if (current->isEndOfWord) {
			cout << "Definition of '" << word << "': " << current->definition << endl;
		}
		else {
			cout << "Word not found in the dictionary." << endl;
		}
	}

	// Function to save the trie to a file
	void saveToFile(const string& filename) const {
		ofstream file(filename);
		if (file.is_open()) {
			saveToFileHelper(root, "", file);
			file.close();
			cout << "Dictionary saved to " << filename << endl;
		}
		else {
			cout << "Unable to open file for saving." << endl;
		}
	}

	// Recursive helper function to save trie to a file
	void saveToFileHelper(TrieNode* node, const string& currentWord, ofstream& file) const {
		if (node->isEndOfWord) {
			file << currentWord << "," << node->definition << "\n";
		}

		for (const auto& child : node->children) {
			saveToFileHelper(child.second, currentWord + child.first, file);
		}
	}

	// Function to clear the trie
	void clear() {
		clearHelper(root);
		root = new TrieNode();
	}

	// Recursive helper function to clear the trie
	void clearHelper(TrieNode* node) {
		for (auto& child : node->children) {
			clearHelper(child.second);
		}
		delete node;
	}

	~TrieDictionary() {
		clear(); // Destructor to free memory
	}
};

int main() {
	TrieDictionary trieDictionary;

	// Load dictionary from file
	ifstream file("trie_dictionary.txt");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			size_t commaPos = line.find(",");
			if (commaPos != string::npos) {
				string word = line.substr(0, commaPos);
				string definition = line.substr(commaPos + 1);
				trieDictionary.addWord(word, definition);
			}
		}
		file.close();
		cout << "Dictionary loaded from trie_dictionary.txt" << endl;
	}
	else {
		cout << "Unable to open file for loading." << endl;
	}

	int choice;
	string word, definition;

	do {
		cout << "\n1. Add Word\n2. Search Word\n3. Exit\nEnter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			cout << "Enter the word: ";
			cin >> word;
			cout << "Enter the definition: ";
			cin.ignore(); // Ignore newline character in buffer
			getline(cin, definition);
			trieDictionary.addWord(word, definition);
			cout << "Word added to the dictionary." << endl;
			break;
		case 2:
			cout << "Enter the word to search: ";
			cin >> word;
			trieDictionary.searchWord(word);
			break;
		case 3:
			cout << "Exiting the program. Goodbye!" << endl;
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}
	} while (choice != 3);

	// Save dictionary to file during program termination
	trieDictionary.saveToFile("trie_dictionary.txt");

	return 0;
}
