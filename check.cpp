#include <iostream>
#include <unordered_set>
#include <filesystem>
#include <stack>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <mutex>
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
        }}//dont need to worry about deleting stack its stored on the poulate_seen_nodes stack and reference to heapthis means that we dont have to worry about deletion
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
std::mutex vectorMutex{};
std::vector <std::string>& checkNodes(std::vector <std::string>& missingNodes, std::unordered_set <std::string>& firstSet, std::unordered_set <std::string>& secondSet){
    for (const auto& firstSetVal : firstSet){
        if(!secondSet.contains(firstSetVal)){
            std::lock_guard<std::mutex> lock(vectorMutex);
            missingNodes.push_back(firstSetVal);
        }
    }
    return missingNodes;
}

int main(){
    TraverseTree leaderTrav{};
    std::unordered_set <std::string> leaderNodes = leaderTrav.run();
    TraverseTree childTrav{"NonLeaderReplication"};
    std::unordered_set <std::string> childNodes1 = childTrav.run();
    std::vector <std::string> missingNodes{};
    std::thread worker1(checkNodes,std::ref(missingNodes), std::ref(leaderNodes), std::ref(childNodes1));
    std::thread worker2(checkNodes,std::ref(missingNodes), std::ref(childNodes1) , std::ref(leaderNodes));
    worker1.join(); worker2.join();
    std::cout << "missing nodes:" << "\n";
    for (const auto& toPrint: missingNodes){
        std::cout << toPrint << "\n";
    }
    return 0;
}