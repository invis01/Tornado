#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>

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
    //Vector3 scale; commented out for now because scale is fucking cringe
};

struct PartComponent{
    Vector3 size;
    Color color;
};

struct CharacterComponent{
    char username[20];
};

struct CameraComponent{
    Vector3 forward;
    float fov;
    float distance;
    bool isfirst;
    Camera camera;
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

unsigned int ComponentMasks[] = {
    COMPONENT_TRANSFORM,
    COMPONENT_PART,
    COMPONENT_CHARACTER,
    COMPONENT_CAMERA,
};

void add_component(struct GameEntity *entity, enum ComponentIndex component, void **componentlists){

    unsigned int mask = ComponentMasks[component];

    if (entity->componentMask & mask)
            return;

    void *storage = componentlists[component];

    switch(component) {
    default:
            break;
    case INDEX_TRANSFORM: {
        ((struct TransformComponent*)storage)[entity->id] = (struct TransformComponent){0};
        break;
        }
    case INDEX_PART: {
        ((struct PartComponent*)storage)[entity->id] = (struct PartComponent){.size={2, 0.5, 1}, .color=PURPLE};
        break;
        }
    case INDEX_CHARACTER: {
        ((struct CharacterComponent*)storage)[entity->id] = (struct CharacterComponent){0};
        break;
        }
    case INDEX_CAMERA: {
        ((struct CameraComponent*)storage)[entity->id] = (struct CameraComponent){0};
        break;
        }
    }

    entity->componentMask |= mask;
}

struct GameEntity* add_world_entity(unsigned int *entitycount) {
    static struct GameEntity world;
    world.id = 0;
    world.parent = NULL;
    world.children = malloc(sizeof(struct GameEntity*) * 16);
    world.childCount = 0;
    world.componentMask = 0;
    (*entitycount)++;
    return &world;
}

void register_entity(struct GameEntity *parent, struct GameEntity *newentity, unsigned int *entitycount) {
    newentity->id = *entitycount;
    newentity->parent = parent;
    newentity->children = NULL;
    newentity->childCount = 0;
    newentity->componentMask = 0;
    parent->children[parent->childCount] = newentity;
    parent->childCount++;
    (*entitycount)++;
}

void initialize_components(void **componentlists, int MAX_ENTITIES) {
    componentlists[INDEX_TRANSFORM] = malloc(sizeof(struct TransformComponent) * MAX_ENTITIES);
    componentlists[INDEX_PART] = malloc(sizeof(struct PartComponent) * MAX_ENTITIES);
    componentlists[INDEX_CHARACTER] = malloc(sizeof(struct CharacterComponent) * MAX_ENTITIES);
    componentlists[INDEX_CAMERA] = malloc(sizeof(struct CameraComponent) * MAX_ENTITIES);
}
