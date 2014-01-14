#ifndef GLOBALPROPERTY_H_
#define GLOBALPROPERTY_H_

#include <angelscript.h>
#include <string>

/**GlobalProperty class.
 * Creates an angelscript global property, manages and releases it when
 * the instance of this class is destroyed. The pointer to the property must be kept
 * alive for the duration of the instance of this class containing it. The pointer should
 * then be freed by the application writer.
 */
class GlobalProperty
{
public:
    /**GlobalProperty constructor.
     * Creates an empty instance of a GlobalProperty class.
     * with ptr set to NULL.
     */
    GlobalProperty();

    /**GlobalProperty destructor.
     * calls release().
     * @see release()
     */
    ~GlobalProperty();

    /**Sets the members of a GlobalProperty.
     * If the members have been previously set this wont work.
     * @param definition the line in a config where this property was pulled from.
     * @param type the angelscript string representation of the property type.
     * @param name the name of the property as to be accessed in angelscript code.
     * @param a pointer to the corresponding property.
     */
    void setup(std::string definition, std::string type, std::string name, void* ptr);

    /**Registers this property with the angelscript engine.
     * This will only work if setup() was called before.
     * @param engine a valid instance of an anglescript engine.
     * @return true on success.
     */
    bool registerProperty(asIScriptEngine* engine, bool configGroup = true);

    /**Releases the global property.
     * Removes the angelscript dynamic config group containing the property.
     */
    void release();

    /**Returns the pointer to the property.
     * @return the pointer to the property.
     */
    void* getPtr();

    /**Returns the property definition.
     * @return the property definition.
     */
    std::string getDefinition();

    /**Returns the property type.
     * @return the property type.
     */
    std::string getType();

    /**Returns the property name.
     * @return the property name.
     */
    std::string getName();

private:
    asIScriptEngine* m_engine; ///< The angelscript engine the property was registered under.
    void* m_ptr; ///< A pointer to the property registered in the angelscript engine.

    std::string m_definition; ///< The line in a config that generated this property.
    std::string m_type; ///< The property type as registered in angelscript.
    std::string m_name; ///< The property name as registered in angelscript.

};

#endif /* GLOBALPROPERTY_H_ */
