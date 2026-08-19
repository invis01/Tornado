#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "components.h"

unsigned int ComponentMasks[] = {
    COMPONENT_TRANSFORM,
    COMPONENT_RENDERABLE,
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
        ((struct TransformComponent*)storage)[entity->id] = (struct TransformComponent){.translation={0.0f, 0.0f, 0.0f}, .rotation={0.0f, 0.0f, 0.0f}, .size={2.0f, 0.5f, 1.0f}};
        break;
        }
    case INDEX_RENDERABLE: {
        ((struct RenderableComponent*)storage)[entity->id] = (struct RenderableComponent){.color=PINK, .isPart=true};
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
    componentlists[INDEX_RENDERABLE] = malloc(sizeof(struct RenderableComponent) * MAX_ENTITIES);
    componentlists[INDEX_CHARACTER] = malloc(sizeof(struct CharacterComponent) * MAX_ENTITIES);
    componentlists[INDEX_CAMERA] = malloc(sizeof(struct CameraComponent) * MAX_ENTITIES);
}
