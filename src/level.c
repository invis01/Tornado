#include "components.h"
#include "rendering.h"
#include <cjson/cJSON.h>
#include <stdlib.h>

#define TEST_MAP "[{\"Tr\":0,\"P\":[0,0,0],\"S\":[200,2,200],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"7d7d85\"},{\"Tr\":0,\"P\":[0,7,-10],\"S\":[2,12,2],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"63452c\"},{\"Tr\":0,\"P\":[0,3.5,-10],\"S\":[8,1,8],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"},{\"Tr\":0,\"P\":[0,5.5,-10],\"S\":[7,1,7],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"},{\"Tr\":0,\"P\":[0,7.5,-10],\"S\":[6,1,6],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"},{\"Tr\":0,\"P\":[0,9.5,-10],\"S\":[5,1,5],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"},{\"Tr\":0,\"P\":[0,11.5,-10],\"S\":[4,1,4],\"R\":[0,0,0],\"T\":\"Part\",\"Shape\":\"Block\",\"C\":\"26a269\"}]"


void loadMap(struct GameEntity *world, unsigned int *entitycount, void **componentlists, struct Renderer *renderer, struct GameEntity *mapparts) {
    cJSON *map = cJSON_Parse(TEST_MAP);

    int count = cJSON_GetArraySize(map);

    for (int i = 0; i < count; i++) {
        cJSON *part = cJSON_GetArrayItem(map, i);

        register_entity(world, &mapparts[i], entitycount);

        add_component(&mapparts[i], INDEX_RENDERABLE, componentlists);
        add_component(&mapparts[i], INDEX_TRANSFORM, componentlists);

        Vector3 position = {cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "P"), 0)->valuedouble, cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "P"), 1)->valuedouble, cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "P"), 2)->valuedouble};
        Vector3 scale = {cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "S"), 0)->valuedouble, cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "S"), 1)->valuedouble, cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(part, "S"), 2)->valuedouble};
        unsigned int value = strtoul(cJSON_GetObjectItemCaseSensitive(part, "C")->valuestring, NULL, 16);
        value = (value << 8) | 0xFF;
        Color color = GetColor(value);

        ((struct RenderableComponent*)componentlists[INDEX_RENDERABLE])[mapparts[i].id].color = color;
        ((struct TransformComponent*)componentlists[INDEX_TRANSFORM])[mapparts[i].id].translation = position;
        ((struct TransformComponent*)componentlists[INDEX_TRANSFORM])[mapparts[i].id].size = scale;

        renderer->renderables[renderer->renderablecount] = mapparts[i].id;
        renderer->renderablecount++;
    }

    cJSON_Delete(map);
}
