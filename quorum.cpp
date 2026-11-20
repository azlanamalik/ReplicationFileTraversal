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
static std::unordered_set <std::filesystem::path> HiddenFolders{};//just files we dont want to include like .git
//interesting as static isnt stored in heap or main stack it has its own area where it stored in memory similar to the gloabal variable

class Quorum{
    private:
    std::vector <std::filesystem::path> locationOfQuorums{};
    public:
    Quorum(){
        std::cout << "Created the Qourum please run findReplicaNodes" << "\n";
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
    protected:
};

/*
class WriteQuorum: public Quorum{
};
*/


int main(){
    HiddenFolders.reserve(10);//maximum 10 folders we want to hide
    HiddenFolders.insert(".vscode");
    HiddenFolders.insert(".git");
    Quorum quorum{};
    quorum.findReplicaNodes(".");
    quorum.prettyPrintLocationOfQuorums();
    return 0;
}