#include <vector>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_set>
#include <iostream>
/*
intentions - take in a file then distribute it across the nodes (in this implimentation nodes are represented as files)
quorum - 
this will store all the location for the replica
checks for data?

inherited class:
write_quorum 
read_quorum

*/
static std::unordered_set <std::filesystem::path> HiddenFolders{};
class Quorum{
    private:
    std::vector <std::filesystem::path> locationOfQuorums{};
    Quorum() = default;
    int minAckRead{2};
    int minAckWrite{3};
    public:
    Quorum (const Quorum&) = delete;
    Quorum& operator=(const Quorum&) = delete; 
    static Quorum& getInstance(){
        static Quorum instance;
        return instance;
    }
    void findReplicaNodes(std::string startingLoc) {
        for (const auto& file : std::filesystem::directory_iterator(startingLoc)){
            if(file.is_directory() && !HiddenFolders.contains(file.path().filename())){
                locationOfQuorums.push_back(file.path());
            }
        }
    }
    void prettyPrintLocationOfQuorums(){
        if (locationOfQuorums.empty()){
            std::cout << "please create files and run findReplicaNodes";
            return;
        }
        for (const auto& loc: this->locationOfQuorums){
            std::cout << loc.string() << "\n";
        }
    }
    std::vector <std::filesystem::path> getReplicaQuery(std::filesystem::path& path){
        /*
        this method will be adapted but it will return all the nodes to write to
        currently we write to all to get the basic system working
        */
        return this->locationOfQuorums;
    }
    /*
    write takes in path and then we put in nodes
    */
    bool writeToReplica(std::filesystem::path& path){
        //i think fs::path is a big value so by ref
        int writes {};
        try{
            for(const auto& locOfReplica: this->getReplicaQuery(path)){
            std::filesystem::copy_file(path, locOfReplica);
            ++writes;
        }
        }catch(const std::exception& e){
            std::cout << "Error: " << e.what() << "\n";
            return false;
        }
        return this->minAckRead > writes;
    }
    /*read we only need to traverse and find within the given nodes*/
    void readToReplica(std::string path){
        //i think fs::path is a big value so by ref
        try{
            for(const auto& locOfReplica: this->getReplicaQuery(path)){
            std::filesystem::copy_file(path, locOfReplica);
        }
        }
        catch(const std::exception& e){
            std::cout << "Error: " << e.what() << "\n";
        }
    }
};
int main(){
    HiddenFolders.reserve(10);//maximum 10 folders we want to hide
    HiddenFolders.insert(".vscode");
    HiddenFolders.insert(".git");
    Quorum& quorum = quorum.getInstance();
    quorum.findReplicaNodes(".");
    quorum.prettyPrintLocationOfQuorums();
    return 0;
}