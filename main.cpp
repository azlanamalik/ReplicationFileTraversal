int main(){
    HiddenFolders.reserve(10);//maximum 10 folders we want to hide
    HiddenFolders.insert(".vscode");
    HiddenFolders.insert(".git");
    Quorum& quorum = quorum.getInstance();
    quorum.findReplicaNodes(".");
    quorum.prettyPrintLocationOfQuorums();
    return 0;
}