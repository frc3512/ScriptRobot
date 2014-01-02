#ifndef PACKAGESECTION_H_
#define PACKAGESECTION_H_

#include <string>

/**PackageSection class.
 * Represents a file stored in memory as a string
 * with its file name.
 */
class PackageSection
{
public:
    /**PackageSection constructor.
     * Creates an empty instance of a PackageSection class
     * with nameSize and fileSize set to 0.
     */
    PackageSection();

    /**Section constructor.
     * Creates an empty instance of a PackageSection class.
     * Then calls setup(std::string, std::string) on it with the parameters passed in.
     * @see setup(std::string, std::string)
     * @param name the name of the PackageSection.
     * @param file the Section file as a string.
     */
    PackageSection(std::string name, std::string file);

    /**Section destructor.
     */
    ~PackageSection();

    /**Sets the name and file of a PackageSection.
     * This will only work if the name or file were not previously set.
     * @param name the name of the PackageSection.
     * @param file the Section file as a string.
     */
    void setup(std::string name, std::string file);

    /**Sets the name of the PackageSection.
     * This can only be done if the name was not previously set.
     * @param name the name of the PackageSection.
     */
    void setName(std::string name);

    /**Returns the name of the PackageSection.
     * @return the name of the PackageSection.
     */
    std::string getName();

    /**Sets the PackageSection file.
     * This can only be done if the Section file was not previously set.
     * @param file the Section file as a string.
     */
    void setFile(std::string file);

    /**Returns the PackageSection file as a string.
     * @return the PackageSection file as a string.
     */
    std::string getFile();

    /**Returns the number of bytes in the name.
     * @return the number of bytes in the name.
     */
    int* getNameSize();

    /**Returns the number of bytes in the file.
     * @return the number of bytes in the file.
     */
    int* getFileSize();

private:
    int m_nameSize; ///< The number of bytes in the name
    std::string m_name; ///< The name of the PackageSection.
    int m_fileSize; ///< The number of bytes in the name
    std::string m_file; ///< The PackageSection file as a string.

};

#endif /* PACKAGESECTION_H_ */
