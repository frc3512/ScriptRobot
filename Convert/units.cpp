#include "units.h"
#include <math.h>
#include "../add_on/vxw_assert.h"

#define M_PI 3.14159265359

void registerConvertUnits(asIScriptEngine* engine)
{
	assert(engine->RegisterGlobalFunction("float toRadians(float)", asFUNCTION(toRadians), asCALL_CDECL) >= 0);
	assert(engine->RegisterGlobalFunction("float toDegrees(float)", asFUNCTION(toDegrees), asCALL_CDECL) >= 0);

}


float toRadians(float a)
{
	return a*M_PI/180;

}

float toDegrees(float a)
{
	return (-(a*180/M_PI))+90;

}
