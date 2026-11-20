#include <iostream>
#include <unordered_set>
#include <filesystem>
#include <stack>
#include <vector>
#include <string>
#include <thread>
#include <functional>
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
void checkNodes(std::vector <std::string>& differentNodes, const std::unordered_set <std::string>& firstSet, const std::unordered_set <std::string>& secondSet){
    for (const auto& firstSetVal : firstSet){
        if(!secondSet.contains(firstSetVal)){
            differentNodes.push_back(firstSetVal);
        }
    }
}

int main(){
    TraverseTree leaderTrav{};
    const auto& leaderNodes = leaderTrav.run();//const auto here prevents copying even ythough it returns a reference
    TraverseTree childTrav{"NonLeaderReplication"};
    const auto& childNodes1 = childTrav.run();
    std::vector <std::string> missingNodes{};
    std::vector <std::string> extraNodes{};
    std::thread worker1(checkNodes,std::ref(missingNodes), std::ref(leaderNodes), std::ref(childNodes1));
    std::thread worker2(checkNodes,std::ref(extraNodes), std::ref(childNodes1) , std::ref(leaderNodes));//new implimenation creates 2 vectors and passes reference
    //this is important as thread takes in value as copy rather than reference
    //this prevents race condition to add
    worker1.join(); worker2.join();
    std::cout << "missing nodes:" << "\n";
    for (const auto& toPrint: missingNodes){
        std::cout << toPrint << "\n";
    }
    std::cout << "extra nodes:" << "\n";
    for (const auto& toPrint: extraNodes){
        std::cout << toPrint << "\n";
    }
    return 0;
}
