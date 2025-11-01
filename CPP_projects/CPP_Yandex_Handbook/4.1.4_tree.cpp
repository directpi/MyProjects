#include <map>
#include <string>
#include <vector>

struct Node {
    std::map<std::string, Node> children;
};

class Tree {
private:
    Node root;

public:
    bool Has(const std::vector<std::string>& node) const;
    void Insert(const std::vector<std::string>& node);
    void Delete(const std::vector<std::string>& node);
};
bool Tree::Has(const std::vector<std::string>& node) const {
    const Node* current = &root;

    for (const auto& key : node) {
        auto it = current->children.find(key);
        if (it == current->children.end()) {
            return false;
        }
        current = &it->second;
    }
    return true;
}

void Tree::Insert(const std::vector<std::string>& node) {
     Node* current = &root;
    for (const auto& key : node) {
        current = &current->children[key]; 
    }
}

void Tree::Delete(const std::vector<std::string>& node) {
    Node* current = &root;
    std::vector<Node*> path;   

    for (const auto& key : node) {
        auto it = current->children.find(key);
        if (it == current->children.end()) {
            return; 
        }
        path.push_back(current);
        current = &it->second;
    }

    if (!path.empty()) {
        path.back()->children.erase(node.back());
    }
}