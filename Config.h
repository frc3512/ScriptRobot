#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <vector>

typedef struct SectionData
{
    std::string section;
    unsigned int line;

    std::string identifier;
    std::string key;
    std::string value_str;

    std::vector<std::string> params;

    void* value;

} SectionData;

class Config
{
public:
    typedef enum ParseError
    {
        NoError = 0, //succesfully built
        NoFile = -1, //if the file is empty
        NoType = -2,    //if the type name is missing
        NoVarName = -3, //if the var name is missing
        BadValue = -4, //if there is no ; at the end of a line/section or the = is missing
        VarNameCollision = -5 //if two variables have the same name

    } ParseError;

    static std::string printParseError(ParseError error);

    Config();
    ~Config();

    /*loads a file and parses it
     *
     */
    ParseError load(std::string path);

    /*file is the full text file of a config
     * in one string. including \n and line breaks
     * ect.
     * returns < 0 if and error occurred while parsing
     * erases the currently parsed data
     */
    ParseError parse(std::string file);

    /*called internally
     * builds default value types.
     */
    void build();

    /* returns a string representing the
     * contents of the new config file.
     */
    std::string write();

    /*clears all the sections
     * and deletes pointers to data
     *
     */
    void clearSections();

    /*if the value is a primitive
     * or supported type, void*
     * will point to said type
     */
    void* lookup(std::string name);

    //GETTERS
    int getInt(std::string name);
    unsigned int getUInt(std::string name);
    float getFloat(std::string name);
    std::string getString(std::string name);
    bool getBool(std::string name);

    std::vector<int> getIntVec(std::string name);
    std::vector<unsigned int> getUIntVec(std::string name);
    std::vector<float> getFloatVec(std::string name);
    std::vector<std::string> getStringVec(std::string name);
    std::vector<bool> getBoolVec(std::string name);

    //SETTERS
    void set(std::string name, std::string type, std::string value_str, void* value);
    void setInt(std::string name, int v);
    void setUInt(std::string name, unsigned int v);
    void setFloat(std::string name, float v);
    void setString(std::string name, std::string v);
    void setBool(std::string name, bool v);

    /*returns the parameters of the config variable
     * which are separated by , in the config.
     */
    std::vector<std::string> getParams(std::string name);

    /*returns the section data with the same name
     *
     */
    SectionData* getSectionData(std::string name);

    /*returns the name of all variables
     * with the type specified.
     */
    std::vector<std::string> getNamesOfType(std::string type);

    std::string configStr;
    std::vector<SectionData> sections;

};

#endif /* CONFIG_H_ */
