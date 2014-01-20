#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <string>
#include <vector>

bool getFilesFromPath(std::string path, std::vector<std::string>& files);

bool isDirectory(std::string path);

std::string getExstension(std::string file);

std::string getFile(std::string path);

#endif /* FILESYSTEM_H_ */
