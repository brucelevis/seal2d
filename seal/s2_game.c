#ifdef __cplusplus
extern "C" {
#endif

#include "s2_game.h"

static struct s2_game* GAME = NULL;

struct s2_game* s2_game_G()
{
    return GAME;
}

static void s2_game_init_bgfx(/* WC: pass game in */ uint32_t width, uint32_t height)
{
    bool inited = bgfx_init(BGFX_RENDERER_TYPE_OPENGL, BGFX_PCI_ID_NONE, 0, NULL, NULL);
    if (!inited) {
        s2_assert(false, "init bgfx failed.");
        return;
    }

    bgfx_reset(width, height, BGFX_RESET_VSYNC);
    bgfx_set_view_clear(0
                        , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                        , 0x303030ff
                        , 1.0f
                        , 0
                        );
#if DEBUG
    bgfx_set_debug(BGFX_DEBUG_TEXT);
#endif
}

void s2_game_init(/* WC: pass game in */ struct s2_game_config* config)
{
    s2_game_init_bgfx(config->design_height, config->design_height);

    GAME = s2_malloc(sizeof(struct s2_game));
    GAME->__node_counter = 0;
    GAME->sprite_renderer = s2_sprite_renderer_create();

    //TODO: remove later, tmp test code
    struct s2_texture* tex = s2_texture_create("res/unpack/bunny_bounding.png");
    struct s2_node* root = (struct s2_node*)s2_sprite_image_create_tex(tex);
    root->x = 0;
    root->y = 0;
    root->width = 26; //config->design_width;
    root->height = 37;//config->design_height;

    struct s2_node* child = (struct s2_node*)s2_sprite_image_create_tex(tex);
    child->x = 26;
    child->y = 37;
    child->anchor_x = 0;
    child->anchor_y = 0;
    child->width = 26;
    child->height = 37;

    s2_node_add_child(root, child);

    GAME->root = root;
}

void s2_game_update(/* WC: pass game in */)
{
    s2_node_visit(GAME->root);
}

void s2_game_shutdown()
{
    s2_sprite_renderer_destroy(GAME->sprite_renderer);
}

#ifdef __cplusplus
}
#endif
