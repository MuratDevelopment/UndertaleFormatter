#include <iostream>
#include <filesystem>
#include <vector>
#include <windows.h>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>


const char* version = "0.1a";

namespace fs = std::filesystem;

std::vector<std::string> GetFoldersInPath(const std::string& path) {
    std::vector<std::string> folders;

    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_directory(entry)) {
                folders.push_back(entry.path().string());
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return folders;
}

bool EmptyFileIfExists(const std::string& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return false;
    }

    // Open the file for writing, which clears its contents
    std::ofstream outFile(filePath);

    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }

    outFile.close();
    return true;
}

bool DeleteFileIfExists(const std::string& filePath) {
    if (std::remove(filePath.c_str()) == 0) {
        std::cout << "File deleted: " << filePath << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to delete file: " << filePath << std::endl;
        return false;
    }
}

bool DeleteFolderIfExists(const std::string& folderPath) {
    try {
        if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
            fs::remove_all(folderPath); // Delete the folder and its contents
            std::cout << "Folder deleted: " << folderPath << std::endl;
            return true;
        }
        else {
            std::cerr << "Folder does not exist: " << folderPath << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

std::string GetSteamInstallationFolder() {
    const char* subkeys[] = {
        "SOFTWARE\\Valve\\Steam",
        "SOFTWARE\\Wow6432Node\\Valve\\Steam"  // For 64-bit Windows
    };
    const char* valueName = "InstallPath";

    HKEY hKey;
    std::string steamPath;

    for (const char* subkey : subkeys) {
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD bufferSize = MAX_PATH;
            char buffer[MAX_PATH];

            if (RegQueryValueExA(hKey, valueName, nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer), &bufferSize) == ERROR_SUCCESS) {
                steamPath = buffer;
            }

            RegCloseKey(hKey);

            if (!steamPath.empty()) {
                break;  // Found the Steam installation path
            }
        }
    }

    return steamPath;
}

std::string SelectFolderFromList(const std::vector<std::string>& folders) {
    if (folders.empty()) {
        std::cout << "No folders found." << std::endl;
        return "";
    }

    std::cout << "Folders:" << std::endl;
    for (size_t i = 0; i < folders.size(); ++i) {
        std::cout << i + 1 << ". " << folders[i] << std::endl;
    }

    int selection;
    do {
        std::cout << "Select a folder (1-" << folders.size() << "): ";
        std::cin >> selection;
    } while (selection < 1 || selection > folders.size());

    return folders[selection - 1];
}


void formatUndertale(std::string userpath) {
    if (EmptyFileIfExists(userpath + "\\391540\\remote\\system_information_962")) {
        printf("Successfully deleted the remains of the genocide blank screen\n");
    };
    if (EmptyFileIfExists(userpath + "\\391540\\remote\\system_information_963")) {
        printf("Successfully deleted the remains of pacifist run after genocide\n");
    };
    if (DeleteFileIfExists(userpath + "\\391540\\remotecache.vdf")) {
        printf("Successfully deleted the cloud save cache\n");
    }
    printf("1. phase done! Now if you start Undertale from steam and get a 'Cloud Sync Conflict' everythings going great!\n");
    printf("Press enter to continue...");
    std::cin.ignore();
    if (DeleteFolderIfExists(userpath + "\\391540\\remote")) {
        printf("Successfully deleted the remote folder\n");
    }
    if (DeleteFileIfExists(userpath + "\\391540\\remotecache.vdf")) {
        printf("Successfully deleted the cloud save cache\n");
    }
    
    printf("Now please select 'Upload To The Steam Cloud', wait for the game to start, then press enter!\n");
    std::cin.ignore();
    printf("Now please right click on Undertale in the Steam Library, then seelect 'Properties', then turn off Steam Cloud Sybchronization, and then press enter!\n");
    std::cin.ignore();
    printf("Now please close Undertale and close down Steam, then press enter!\n");
    std::cin.ignore();
    printf("Finalizing format...\n");
    if (DeleteFolderIfExists(userpath + "\\391540")) {
        printf("Successfully deleted the remote folder\n");
    }

    char* localAppData;
    size_t len;
    errno_t err = _dupenv_s(&localAppData, &len, "LOCALAPPDATA");

    if (DeleteFolderIfExists((std::string)(localAppData) + "\\UNDERTALE")) {
        printf("Successfully deleted the Undertale save folder\n");
    }

}

int main() {
	printf("Welcome to UndertaleFormatter version %+s!\n ", version);
    std::string path = GetSteamInstallationFolder()+"\\userdata";
    std::vector<std::string> folders = GetFoldersInPath(path);

    std::string selectedFolder = SelectFolderFromList(folders);

    if (!selectedFolder.empty()) {
        std::cout << "Selected folder: " << selectedFolder << std::endl;
    }
    else {
        std::cout << "No folder selected." << std::endl;
    }

    printf("WARNING: This program will delete the APPDATA\\UNDERTALE folder and your local Steam Undertale save data\nAre you sure you want to continue? (y/N): ");
    std::string lastSelection;
    std::cin >> lastSelection;
    printf("\n");

    if (lastSelection == "y" || lastSelection == "Y") {
        printf("Starting format...\n");
        formatUndertale(selectedFolder);
        printf("Format done!\n");
        printf("Now you can enable back the Steam Cloud Sybchronization and play the game with no strings attached! :) \n");
        printf("Press Enter to Exit!\n");
        std::cin.ignore();
    }
    else {
        printf("Cancelling format...\n");
        return 0;
    }


	return 0;
}