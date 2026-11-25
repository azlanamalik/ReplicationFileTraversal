#include "quorum.h"

int main() {
    HiddenFolders.reserve(10);
    HiddenFolders.insert(".vscode");
    HiddenFolders.insert(".git");

    Quorum& quorum = Quorum::getInstance();
    quorum.findReplicaNodes(".");

    std::cout << "--- replica nodes ---\n";
    quorum.prettyPrintLocationOfQuorums();

    // --- WRITE ---
    // copies README.md into every replica folder
    std::cout << "\n--- write ---\n";
    bool writeOk = quorum.writeToReplica("README.md", "README.md");
    std::cout << (writeOk ? "write quorum met\n" : "write quorum NOT met\n");

    // --- READ ---
    // searches every replica folder for README.md and prints where it found it
    std::cout << "\n--- read ---\n";
    quorum.readFromReplica("README.md");

    // --- DETECT DEVIATED NODES ---
    // compare the two replica folders to see if any files differ
    std::cout << "\n--- deviation check ---\n";
    detectDeviated("LeaderReplication", "NonLeaderReplication");

    return 0;
}