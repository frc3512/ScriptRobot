#ifndef SCRIPTPACKAGE_H_
#define SCRIPTPACKAGE_H_

#include <list>
#include <string>
#include <string.h>
#include <angelscript.h>

#include "PackageArchive.h"
#include "ScriptRoutine.h"
#include "PropertyFactory.h"
#include "GlobalProperty.h"

#include "ScriptPlugin.h"

/**ScriptPackage class.
 * The ScriptPackage class loads and writes ScriptPackage files (.scpkg)
 * A ScriptPackage file is a binary file that archives scripts and configs.
 * Into PackageSections containing the file and original file path.
 * The ScriptPackage once successfully loaded is configured and built using
 * a project.cfg and a robot.cfg. robot.cfg contains info about the robots devices
 * and their ports and the project.cfg contains info about ScriptRoutines for the
 * robot to execute. TODO: make this more consice
 */
class ScriptPackage
{
public:
    /**ScriptPackage constructor.
     * Creates an empty instance of a ScriptPackage class
     * with the angelscript engine set to NULL.
     */
    ScriptPackage();

    /**ScriptPackage destructor
     * calls unload() on the ScriptPackage releasing all
     * angelscript objects created by the ScriptPackage.
     * @see unload()
     */
    ~ScriptPackage();

    /**Error enum.
     * Used to characterize the an error caused while calling
     * load, unload, build, release or write.
     */
    typedef enum Error
    {
        NoError = 0, ///< The ScriptPackage is ready for use.
        NotRead = -1,
        NotLoaded = -2, ///< The ScriptPackage hasn't been loaded yet.
        NotBuilt = -3, ///< The ScriptPackage hasn't been built yet.
        EngineIsNull = -4, ///< The Engine used to build was null.
        ConfigError = -5, ///< There was an error while parsing a config.
        CouldNotRegisterProperty = -6, ///< There was an error while registering a property.
        CouldNotBuildScript = -7, ///< There was an error while building a script.
        ArchiveError = -8

    } Error;

    /**Returns the last error set.
     * @return the last error set.
     */
    ScriptPackage::Error getLastError();

    /**Returns the last error message set.
     * @return the last error set.
     */
    std::string getLastErrorMessage();

    /**Returns true if a ScriptPackage is ready for use.
     * The ScriptPackage will only be valid if it has been loaded
     * and built successfully.
     * @return true if the ScriptPackage is ready for use.
     */
    bool isValid();

    ScriptPackage::Error read(std::string path);

    void clear();

    /**Loads a ScriptPackage from the path.
     * Loads a .scpkg file into memory and prepares the ScriptPackage
     * to be built. If the ScriptPackage is unsuccessfully loaded unload()
     * is called on it.
     * @see unload()
     * @param path the full path to a .scpkg file.
     * @return ScriptPackage::Error::NotBuilt on success.
     */
    ScriptPackage::Error load();

    /**Unloads a ScriptPackage from memory.
     * Calls release() on the ScriptPackage releasing all
     * angelscript objects created by the ScriptPackage.
     * The ScriptPackage will no longer be valid if it previously was.
     * @see release()
     */
    void unload();

    /**Builds a ScriptPackage.
     * Registers all GlobalProperties. And Builds all scripts.
     * Then fetches all hooks for ScriptRoutines. If building was not successful
     * release() is called on the ScriptPackage.
     * @param engine a fully registered angelscript engine.
     * @return < 0 if an error occurred while building.
     */
    ScriptPackage::Error build(asIScriptEngine* engine);

    /**Releases all angelscript objects.
     * All angelscript objects owned by the ScriptPackage will be released.
     * The ScriptPackage will no longer be valid if it previously was.
     */
    void release();

    /**Writes the ScriptPackage to a file.
     * Only writes objects created using add functions.
     * @param path the full path to a .scpkg file.
     * @return ScriptPackage::Error::CouldNotOpenFile on error.
     */
    ScriptPackage::Error write(std::string path);

    /**Returns the last set path.
     * The path is set when write or load is called.
     * @return the path to the current package.
     */
    std::string getPath();

    /**Adds .scpkg to the end of the file path if isn't already there.
     * @param path a path.
     */
    static std::string addExstension(std::string path);

    /**Returns true if the file name ends in .as
     * @param param the name of a file to test.
     * @return true if the file name ends in .as
     */
    static bool isScript(std::string name);

    /**Returns true if the file name ends in .cfg
     * @param name the name of a file to test.
     * @return true if the file name ends in .cfg
     */
    static bool isConfig(std::string name);

    /**Adds a section to a ScriptPackage.
     * If the section already existed it is overwritten.
     * @param name the file name of section.
     * @param file the file as a string.
     */
    void addSection(std::string name, std::string file);

    /**Removes a section from a ScriptPackage.
     * The ScriptPackage will no longer be valid if it previously was.
     * @param name the name of the section to remove.
     */
    void remSection(std::string name);

    /**Returns a section.
     * @param name the name of the section to fetch.
     * @return NULL if the section didn't exist.
     */
    PackageSection* getSection(std::string name);

    void addRoutine(std::string name, std::vector<std::string> params);

    /**Adds a routine to a ScriptPakcage.
     * If the routine already existed it is overwritten.
     * @param name the name of the routine.
     * @param type the routine type.
     * @param scripts the scripts utilized by the routine.
     */
    void addRoutine(std::string name, ScriptRoutine::Type type, std::vector<std::string> scripts);

    /**Removes a routine from a ScriptPackage.
     * The ScriptPackage will no longer be valid if it previously was.
     * @param name the name of the routine to remove.
     */
    void remRoutine(std::string name);

    /**Returns a routine.
     * @param name the name of the routine to fetch.
     * @return NULL if the routine didn't exist.
     */
    ScriptRoutine* getRoutine(std::string name);

    /**Returns the init routine.
     * @return the init routine.
     */
    ScriptRoutine* getInitRoutine();

    /**Returns the default disabled routine.
     * @return the default disabled routine.
     */
    std::string getDefaultDisabledRoutine();

    /**Returns the default autonomous routine.
     * @return the default autonomous routine.
     */
    std::string getDefaultAutonomousRoutine();

    /**Returns the default operator control routine.
     * @return the default operator control routine.
     */
    std::string getDefaultOperatorControlRoutine();

    /**Returns the default test routine.
     * @return the default test routine.
     */
    std::string getDefaultTestRoutine();

    /**Adds a property to a ScriptPakcage.
     * If the property already existed it is overwritten.
     * @param definition the line used to generate the property from a config.
     * @param type the property type as used in angelscript.
     * @param name the name of the property as to be used in angelscript code.
     * @param ptr the pointer to the property.
     */
    void addProperty(std::string definition, std::string type, std::string name, void* ptr);

    /**Removes a property from a ScriptPackage.
     * The ScriptPackage will no longer be valid if it previously was.
     * @param name the name of the property to remove.
     */
    void remProperty(std::string name);

    /**Returns a property.
     * @param name the name of the property to fetch.
     * @return NULL if the property didn't exist.
     */
    GlobalProperty* getProperty(std::string name);

    void addPlugin(ScriptPlugin* plugin);

    void remPlugin(std::string name);

    ScriptPlugin* getPlugin(std::string name);

private:
    /**Sets a ScriptPackages' error.
     * @param error the ScriptPackage::Error.
     * @param errorMessage a message about the error.
     */
    void setError(ScriptPackage::Error error, std::string errorMessage);

    std::string m_lastErrorMessage; ///< The last error message set.
    ScriptPackage::Error m_lastError; ///< The last error set.

    PackageArchive m_archive;

    asIScriptEngine* m_engine; ///< The engine used to build the ScriptPackage.

    ScriptRoutine* m_initRoutine; ///< The robot init routine.

    std::list<ScriptRoutine*> m_routines; ///< All routines loaded from project.cfg.

    std::list<ScriptPlugin*> m_plugins;

    std::list<GlobalProperty*> m_properties; ///< All global properties loaded from .cfg files.

};

#endif /* SCRIPTPACKAGE_H_ */
