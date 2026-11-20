#include <iostream>
#include <unordered_set>
#include <filesystem>
#include <stack>
class TraverseTree{
    private:
    std::unordered_set <std::string> seenNodes {};
    std::string startNode{};
    void populate_seen_nodes(){
        std::stack <std::string> traversalStack{};
        traversalStack.push(startNode);
        while(!traversalStack.empty()){
            std::string thisStackIteration =  traversalStack.top();
            traversalStack.pop();
            for(const auto& file: std::filesystem::directory_iterator(thisStackIteration)){
                if (file.is_directory()){traversalStack.push(file.path().string());}
                this->seenNodes.insert(file.path().filename().string());
                std::cout << file.path().filename().string() << "\n";
        }
        }
        
    }
    public:
    TraverseTree(std::string FolderToTraverse){
        this->startNode = FolderToTraverse; //prevents you from changing the start node after creation (jsst protection for override)
        //you can run run many times but not change the actual values inside the unordered set
        //this woudl allow in theory to run multiple times if the replica changes as it is a unoredred set it is efficient to add and remove from the set
        std::cout << "initialised the traversal" << "\n";
    }
    TraverseTree(){//overload
        this->startNode = "LeaderReplication";
        std::cout << "initialised the traversal" << "\n";
    }
    std::unordered_set <std::string>& run(){
        this->populate_seen_nodes();
        return this->seenNodes;//cant just return a value ti will create a copy of the value
    }

};


int main(){
    TraverseTree travtree1{};
    std::unordered_set <std::string> recievedNodes = travtree1.run();
    return 0;
}