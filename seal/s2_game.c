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

    struct s2_node* root = (struct s2_node*)s2_sprite_image_create_tex(NULL);
    GAME->root = root;
}

void s2_game_update(/* WC: pass game in */)
{
    static struct s2_mat4 mat;
    s2_mat4_identify(&mat);

    bgfx_set_transform(&mat, 1);
    s2_sprite_renderer_begin(GAME->sprite_renderer);

    s2_node_visit(GAME->root);

    s2_sprite_renderer_end(GAME->sprite_renderer);
}

#ifdef __cplusplus
}
#endif
