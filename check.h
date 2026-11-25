#pragma once
#include <iostream>
#include <unordered_set>
#include <filesystem>
#include <stack>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <optional>

/*
traverseTree
1 -- first thing it does it search for nodes that are not the same
2 -- to read from a file we need to traverse nodes so this will also aim to complete that
^^ issue with second is we add another branch which can decrease performance as decrease in branch prediction
also issue in readability of code and also creates nodes that are bigger insize
i tried to help performance by using && clause to prevent comparison when there is no value as lhs evaluates to False
it goes next without doing the harder comparison which converts path --> string and compares both which is letter by letter in c++
 */

class TraverseTree{
    private:
    std::unordered_set <std::string> seenNodes {};
    std::string startNode{};
    std::optional<std::string> toFind;//set this to null init as we dont need this constantly
    std::vector<std::filesystem::path> whereFind{};//do we really need multiple files where it is on a single node??
    void populate_seen_nodes(){
        std::stack<std::filesystem::path> traversalStack{};
        traversalStack.push(startNode);
        while(!traversalStack.empty()){
            std::filesystem::path thisStackIteration =  traversalStack.top();
            traversalStack.pop();
            for(const auto& file: std::filesystem::directory_iterator(thisStackIteration)){
                if (file.is_directory()){traversalStack.push(file.path());};
                if (toFind && toFind == file.path().filename().string()){this->whereFind.push_back(file.path());}
                this->seenNodes.insert(file.path().filename().string());
        }}
    }
    public:
    TraverseTree(std::string FolderToTraverse){
        this->seenNodes.reserve(10000);
        this->startNode = FolderToTraverse;
        std::cout << "initialised the traversal" << "\n";
    }
    TraverseTree(){
        this->startNode = "LeaderReplication";
        std::cout << "initialised the traversal" << "\n";
    }
    TraverseTree(std::string FolderToTraverse, std::string targetFile){
        /*in quorum we need to traverse might aswell just adapt this class */
        this->startNode = FolderToTraverse;
        this->toFind = targetFile;
    }
    std::vector <std::filesystem::path>& getWhereStored(){
        return this->whereFind;
    } 
    std::unordered_set <std::string>& run(){
        this->populate_seen_nodes();
        return this->seenNodes;//cant just return a value ti will create a copy of the value
    }
    const std::unordered_set <std::string>& nodes() const { return seenNodes; }
};
void checkNodes(std::vector <std::string>& differentNodes, const std::unordered_set <std::string>& firstSet, const std::unordered_set <std::string>& secondSet){
    for (const auto& firstSetVal : firstSet){
        if(!secondSet.contains(firstSetVal)){
            differentNodes.push_back(firstSetVal);
        }
    }
}

// Compare two replica folders and print any files that differ between them
void detectDeviated(const std::string& nodeA, const std::string& nodeB){
    TraverseTree travA{nodeA};
    TraverseTree travB{nodeB};
    travA.run();
    travB.run();

    std::vector<std::string> missingFromB{};  // in A but not B
    std::vector<std::string> extraInB{};      // in B but not A
    checkNodes(missingFromB, travA.nodes(), travB.nodes());
    checkNodes(extraInB,     travB.nodes(), travA.nodes());

    if (missingFromB.empty() && extraInB.empty()){
        std::cout << "[deviated] nodes match: " << nodeA << " == " << nodeB << "\n";
        return;
    }
    for (const auto& f : missingFromB)
        std::cout << "[deviated] missing from " << nodeB << ": " << f << "\n";
    for (const auto& f : extraInB)
        std::cout << "[deviated] extra in "    << nodeB << ": " << f << "\n";
}

/*
int main(){
    TraverseTree leaderTrav{};
    TraverseTree childTrav{"NonLeaderReplication"};
    std::thread worker1(&TraverseTree::run, &leaderTrav);
    std::thread worker2(&TraverseTree::run, &childTrav);
    worker1.join(); worker2.join();
    const auto& leaderNodes = leaderTrav.nodes();
    const auto& childNodes1 = childTrav.nodes();
    std::vector <std::string> missingNodes{};
    std::vector <std::string> extraNodes{};
    std::thread worker3(checkNodes,std::ref(missingNodes), std::ref(leaderNodes), std::ref(childNodes1));
    std::thread worker4(checkNodes,std::ref(extraNodes), std::ref(childNodes1) , std::ref(leaderNodes));//new implimenation creates 2 vectors and passes reference
    //this is important as thread takes in value as copy rather than reference
    //this prevents race condition to add
    worker3.join(); worker4.join();
    std::cout << "missing nodes:" << "\n";
    for (const auto& toPrint: missingNodes){
        std::cout << toPrint << "\n";
    }
    std::cout << "extra nodes:" << "\n";
    for (const auto& toPrint: extraNodes){
        std::cout << toPrint << "\n";
    }
    return 0;
}*/