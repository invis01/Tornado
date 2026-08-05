#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER

#include <raylib.h>

struct GameEntity{
    unsigned int id;
    struct GameEntity* parent;
    struct GameEntity** children;
    int childCount;
    unsigned int componentMask;
};

struct TransformComponent{
    Vector3 translation;
    Vector3 rotation;
    Vector3 scale;
};

struct PartComponent{
    Vector3 size;
};

struct CharacterComponent{
    char username[20];
};

struct CameraComponent{
    Vector3 forward;
    float fov;
    float distance;
    bool isfirst;
};

enum ComponentType {
    COMPONENT_TRANSFORM = 1 << 0,
    COMPONENT_PART = 1 << 1,
    COMPONENT_CHARACTER = 1 << 2,
    COMPONENT_CAMERA = 1 << 3,
};

enum ComponentIndex {
    INDEX_TRANSFORM,
    INDEX_PART,
    INDEX_CHARACTER,
    INDEX_CAMERA,
    COMPONENT_COUNT
};

void add_component(struct GameEntity *entity, enum ComponentIndex component, void *componentlists);
struct GameEntity* add_world_entity(unsigned int *entitycount);
void register_entity(struct GameEntity *parent, struct GameEntity *newentity, unsigned int *entitycount);

#endif
