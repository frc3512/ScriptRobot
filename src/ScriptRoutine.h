#ifndef SCRIPTROUTINE_H_
#define SCRIPTROUTINE_H_

#include <list>
#include <string>
#include <vector>
#include <angelscript.h>

/**ScriptRoutine class.
 * The ScriptRoutine class fetches angelscript function pointers
 * from script modules specified in the scripts vector. The
 * hook that is fetched is specified by the ScriptRoutine type.
 * the angelscript function pointers are then used to execute
 * the robot code.
 */
class ScriptRoutine
{
public:
    /**ScriptRoutine constructor.
     * Creates an empty instance of a ScriptRoutine class
     * with name set to "" and the type set to ScriptRoutine::None.
     */
    ScriptRoutine();

    /**ScriptRoutine destructor.
     * calls release()
     * @see release()
     */
    ~ScriptRoutine();

    /**Releases angelscript objects help by a Routine.
     * Decreases the ref count of all stored angelscript function pointers.
     */
    void release();

    /**ScriptRoutine::Type enum.
     * Determines the ScriptRoutine's type and what hook the routine will
     * fetch from the module or engine.
     */
    typedef enum Type
    {
        None = 0, ///< The ScriptRoutine has no type and will not fetch any hooks.
        Init = 1, ///< This is an Init ScriptRoutine. hook void onInit(Robot@).
        Disabled = 2, ///< This is a Disabled ScriptRoutine. hook: void onDisabled(Robot@).
        Autonomous = 3, ///< This is an Autonomous ScriptRoutine. hook: void onAutonomous(Robot@).
        OperatorControl = 4, ///< This is an OperatorControl ScriptRoutine. hook void onOperatorControl(Robot@).
        Test = 5 ///< This is a Test ScriptRoutine. hook void onTest(Robot@).

    } Type;

    /**Sets the name and type of a ScriptRoutine.
     * This will only work if the name or type were not previously set.
     * @param name the name of the ScriptRoutine.
     * @param type the ScriptRoutine type.
     */
    void setup(std::string name, ScriptRoutine::Type type);

    /**Sets the scripts vector.
     * @param scripts the vector containing all scripts used by this ScriptRoutine.
     */
    void setScripts(std::vector<std::string> scripts);

    /**Loads all the hooks using an angelscript engine.
     * If the name and type of the ScriptRoutine are not set this will return.
     * Checks if a module has been loaded with the name of a script this
     * ScriptRoutine has. Then checks if the module has a hook of this ScriptRoutine's type.
     * @param engine an angelscript engine with all the scripts loaded.
     */
    void loadHooksFromEngine(asIScriptEngine* engine);

    /**Sets the name of the ScriptRoutine.
     * This can only be done if the name was not previously set.
     * @param name the name of the ScriptRoutine.
     */
    void setName(std::string name);

    /**Returns the name of the ScriptRoutine.
     * @return the name of the ScriptRoutine.
     */
    std::string getName();

    /**Sets the type of the ScriptRoutine.
     * This can only be done if the type was not previously set.
     * @param type the ScriptRoutine type.
     */
    void setType(ScriptRoutine::Type type);

    /**Returns the ScriptRoutine type.
     * @return the ScriptRoutine type.
     */
    ScriptRoutine::Type getType();

    /**Adds a script to the ScriptRoutine.
     * The script is added if the script is not already in the vector.
     * @param script the name of the script to add.
     */
    void addScript(std::string script);

    /**Removes a script from the ScriptRoutine.
     * @param script the name of the script to remove.
     */
    void remScript(std::string script);

    /**Returns true if the ScriptRoutine has this script.
     * @param script the name of the script to check.
     * @return true if the ScriptRoutine has this script.
     */
    bool hasScript(std::string script);

    /**Returns the number of scripts in this ScriptRoutine.
     * @return the number of scripts in this ScriptRoutine.
     */
    unsigned int getNumOfScripts();

    /**Returns a script by index.
     * If "i" exceeds the getNumOfScripts() then the return will be "".
     * @see getNumOfScripts()
     * @param i the index of the script to return.
     * @return the name of the script at the index.
     */
    std::string getScript(unsigned int i);

    /**Returns the scripts of a ScriptRoutine.
     * @return the scripts of a ScriptRoutine.
     */
    std::vector<std::string> getScripts();

    /**Adds a hook using an angelscript module.
     * If the name and type of the ScriptRoutine are not set this will return false.
     * Checks if the module has been loaded with the name of a script this
     * ScriptRoutine has. Then checks if the module has a hook of this ScriptRoutine's type.
     */
    bool addHookFromModule(asIScriptModule* module);

    /**Returns the number of hooks loaded in this ScriptRoutine.
     * @return the number of hooks loaded in this ScriptRoutine.
     */
    unsigned int getNumOfHooks();

    /**Returns a hook by index.
     * If "i" exceeds getNumOfHooks() then the return will be NULL.
     * @see getNumOfHooks()
     * @param i the index of the hook to return.
     * @return the angelscript function pointer at the index.
     */
    asIScriptFunction* getHook(unsigned int i);

    /**Retursn the hook deceleration as a string.
     * @return the hook deceleration as a string.
     */
    std::string getHookDecl();

private:
    std::string m_name; ///< The name of the ScriptRoutine.
    Type m_typet; ///< The ScriptRoutine type.
    std::vector<std::string> m_scripts; ///< The scripts utilized in this ScriptRoutine.
    std::list<asIScriptFunction*> m_hooks; ///< The angelscript function pointers used by this ScriptRoutine.

};

#endif /* SCRIPTROUTINE_H_ */
