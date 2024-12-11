#include "pch.h"
#include "LuaIntegration.h"
#include "../Components/PositionComponents/PositionComponent.h"
#include "../Components/ComponentManager.h"
#include "../Components/ComponentHandler/ComponentHandler.h"
#include "../Components/Entity/EntityHandler/EntityHandler.h"


void LuaIntegration::lua()
{
    // initialize Lua interpreter
    L = luaL_newstate();

    // load Lua base libraries (print / math / etc)
    luaL_openlibs(L);

	lua_register(L, "createEntity",LuaIntegration::createEntity);
    luaL_dofile(L, "Initializer/lua.lua");


    // Cleanup:  Deallocate all space assocatated with the lua state */
    lua_close(L);
}

int LuaIntegration::createEntity(lua_State* _L)
{
	std::vector<Entity>& entities = EntityHandler::getInstance().entities;
    ComponentManager<PositionComponent>& positionManager = ComponentHandler::getInstance().positionManager;
    ComponentManager<VelocityComponent>& velocityManager = ComponentHandler::getInstance().VelocityManager;

	entities.emplace_back(0);
	entities.emplace_back(1);
	entities.emplace_back(2);

	positionManager.addComponent(0, PositionComponent(glm::vec3(100.f)));
	positionManager.addComponent(1, PositionComponent(glm::vec3(50.f)));
	positionManager.addComponent(2, PositionComponent(glm::vec3(10.f)));
	velocityManager.addComponent(0, VelocityComponent(glm::vec3(1.f)));
	velocityManager.addComponent(1, VelocityComponent(glm::vec3(2.f)));
	velocityManager.addComponent(2, VelocityComponent(glm::vec3(15.f)));

    return 0;
}