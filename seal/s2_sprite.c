/*
* Copyright (C) 2016 Tang Yiyang
*
* This software may be modified and distributed under the terms
* of the MIT license.  See BELOW for details.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/


#include "s2_sprite.h"

void s2_node_init(struct s2_node* self, enum s2_node_type type)
{
    self->__id = ++s2_game_G()->__node_counter; // make debug easier.
    self->__type = type;
    self->x = 0;
    self->y = 0;
    self->anchor_x = 0;
    self->anchor_y = 0;
    self->scale_x = 1.0f;
    self->scale_y = 1.0f;
    self->rotation = 0.0f;
    self->width = 0.0f;
    self->height = 0.0f;

    self->visible = true;
    self->touchable = false;

    s2_affine_identify(&self->local_transform);

    self->parent = NULL;
    self->children = NULL;
    self->children_count = 0;
}

static void s2_node_update_local_transform(struct s2_node* self)
{
    float rcos = cosf(self->rotation);
    float rsin = sinf(self->rotation);
    float a = self->scale_x *  rcos;
    float b = self->scale_x *  rsin;
    float c = self->scale_y * -rsin;
    float d = self->scale_y *  rcos;
    float x = self->x - self->anchor_x * a - self->anchor_y * c;
    float y = self->y - self->anchor_x * b - self->anchor_y * d;

    s2_affine_set(&self->local_transform, a, b, c, d, x, y);
}

static struct s2_node* s2_node_get_root(struct s2_node* self)
{
    // the root node has no parent.
    // we could improve this by cache the root node in GAME.
    struct s2_node* parent = self;
    while(parent->parent)
    {
        parent = parent->parent;
    }
    return parent;
}

// simple implemention first,
// FIXME: how we could aviod copy here, proper memory management.
static struct s2_affine s2_node_transform_to(struct s2_node* self, struct s2_node* to)
{
    if (self == to) {
        return self->local_transform;
    }

    struct s2_affine ret;
    s2_affine_identify(&ret);

    struct s2_node* from = self;
    while(from != to)
    {
        s2_affine_concat(&ret, &from->local_transform);
        from = from->parent;
    }
    
    s2_affine_concat(&ret, &to->local_transform);
    return ret;
}

// FIXME: how we could aviod copy here, proper memory management.
static struct s2_affine s2_node_update_transform(struct s2_node* self)
{
    s2_node_update_local_transform(self);

    return s2_node_transform_to(self, s2_node_get_root(self));
}

static void s2_node_draw(struct s2_node* self)
{
    struct s2_affine model_transform = s2_node_update_transform(self);

    switch (self->__type) {
        case S2_NODE_SPRITE_IMAGE:
        {
            // safely cast due to the struct memory layout. :)
            s2_sprite_image_draw((struct s2_sprite_image*)self, &model_transform);
            break;
        }

        default:
            break;
    }
}

void s2_node_visit(struct s2_node* self)
{
    s2_node_draw(self);

    // WC: 先draw自己还是先draw childs?
    for (int i = 0; i < self->children_count; ++i) {
        struct s2_node* child = self->children[i];
        s2_assert(child);
        s2_node_visit(child);
    }
}

void s2_node_add_child(struct s2_node* self, struct s2_node* child)
{
    node_ptr_t* children = self->children;
    if (children == NULL) {
        children = s2_malloc(sizeof(node_ptr_t) * 4);
        children[0] = child;
        self->children_capacity = 4;
        self->children = children;
        LOGI("add Child");
    } else {
        if (self->children_count == self->children_capacity) {
            node_ptr_t* new_location = s2_realloc(children,
                                  sizeof(node_ptr_t)*self->children_capacity*2);
            if (new_location == children) {
                s2_assert(false, "allocate new children failed.");
                return;
            }
            self->children = new_location;
            self->children_capacity *= 2;
        }

        children[self->children_count] = child;
    }

    child->parent = self;
    self->children_count++;
}

void s2_node_destroy(struct s2_node* self)
{
    switch (self->__type) {
        case S2_NODE_SPRITE_IMAGE:
            s2_sprite_destroy((struct s2_sprite_image*)self);
            break;

        default:
            break;
    }
}

void s2_node_remove_child(struct s2_node* self, struct s2_node* child)
{
    int found = -1;
    for (int i = self->children_count-1; i >= 0 ; --i) {
        if (self->children[i] == child) {
            self->children[i] = NULL;
            found = i;
            break;
        }
    }

    if (found != -1) {
        for (int i = found; i < self->children_count-1; ++i) {
            self->children[i] = self->children[i+1];
        }
        s2_node_destroy(child);
    } else {
        LOGP("child (%p) not found when being removed.", child);
    }
}

void s2_node_remove_all_child(struct s2_node* self)
{
    for (int i = self->children_count-1; i >= 0; --i) {
        s2_node_remove_from_parent(self->children[i]);
    }
}

void s2_node_remove_from_parent(struct s2_node* self)
{
    s2_assert(self->parent);
    s2_node_remove_child(self->parent, self);
}


struct s2_sprite_image* s2_sprite_image_create_with_texture(struct s2_texture* texture)
{
    struct s2_sprite_image* sprite = s2_malloc(sizeof(*sprite));
    //TODO: cast may better?(struct s2_node*)(sprite)
    s2_node_init(&sprite->__super, S2_NODE_SPRITE_IMAGE);

    sprite->texture = s2_texture_retain(texture);
    sprite->texture_rect.x = 0;
    sprite->texture_rect.y = 0;
    sprite->texture_rect.w = texture->width;
    sprite->texture_rect.h = texture->height;

    /*
     * Quad layout:
     *      2-----3
     *      |     |
     *      |     |
     *      0-----1
     *
     */
    sprite->__quad[0].uv.u = 0.0f;
    sprite->__quad[0].uv.v = 1.0f - 0.0f;

    sprite->__quad[1].uv.u = 1.0f;
    sprite->__quad[1].uv.v = 1.0f - 0.0f;

    sprite->__quad[2].uv.u = 0.0f;
    sprite->__quad[2].uv.v = 1.0f - 1.0f;
    
    sprite->__quad[3].uv.u = 1.0f;
    sprite->__quad[3].uv.v = 1.0f - 1.0f;

    for (int i = 0; i < 4; ++i)
    {
        sprite->__quad[i].color.r = 255;
        sprite->__quad[i].color.g = 255;
        sprite->__quad[i].color.b = 255;
        sprite->__quad[i].color.a = 255;
    }

    return sprite;
}

void s2_sprite_destroy(struct s2_sprite_image* self)
{
    s2_texture_release(self->texture);
    s2_free(self);
}

// WC: Vertices should pre-mulitply model matrix before their submission to the renderer
void s2_sprite_image_draw(struct s2_sprite_image* self, struct s2_affine* mt)
{
    struct s2_node* super = &self->__super;
    struct s2_vertex* v = self->__quad;

    float w0 = super->width * (1 - super->anchor_x);
    float w1 = super->width * (0 - super->anchor_x);

    float h0 = super->height * (1 - super->anchor_y);
    float h1 = super->height * (0 - super->anchor_y);
    float a = mt->a;
    float b = mt->b;
    float c = mt->c;
    float d = mt->d;
    float tx = mt->x;
    float ty = mt->y;

    v[0].pos.x = a * w1 + c * h1 + tx;
    v[0].pos.y = d * h1 + b * w1 + ty;

    v[1].pos.x = a * w0 + c * h1 + tx;
    v[1].pos.y = d * h1 + b * w0 + ty;

    v[2].pos.x = a * w1 + c * h0 + tx;
    v[2].pos.y = d * h0 + b * w1 + ty;

    v[3].pos.x = a * w0 + c * h0 + tx;
    v[3].pos.y = d * h0 + b * w0 + ty;

    s2_sprite_renderer_draw(s2_game_G()->sprite_renderer, self->__quad, self->texture);

//    LOGP("{%.2f, %.2f}, {%.2f, %.2f}, {%.2f, %.2f}, {%.2f, %.2f}",
//         v[0].pos.x, v[0].pos.y,
//         v[1].pos.x, v[1].pos.y,
//         v[2].pos.x, v[2].pos.y,
//         v[3].pos.x, v[3].pos.y);
}




//
//#include "seal.h"
//
//
//EXTERN_GAME;
//
//// dirty flags
//#define SPRITE_TRANSFORM_DIRTY  (1 << 0)
//#define SPRITE_SCALE_DIRTY      (1 << 1)
//#define SPRITE_ROTATION_DIRTY   (1 << 2)
//#define SPRITE_COLOR_DIRTY      (1 << 3)
//#define SPRITE_FRAME_DIRTY      (1 << 4)
//#define SPRITE_ZORDER_DIRTY     (1 << 5)
//
//#define SPRITE_SRT_DIRTY        (SPRITE_TRANSFORM_DIRTY | SPRITE_SCALE_DIRTY | SPRITE_ROTATION_DIRTY)
//
//#define SPRITE_ALL_DIRTY        (SPRITE_SRT_DIRTY | SPRITE_COLOR_DIRTY | SPRITE_FRAME_DIRTY)
//
//static unsigned int __sprite_id = 0;
//static struct render* R = NULL;
//static struct sprite_frame_cache* C = NULL;
//void sprite_init_render(struct render* render)
//{
//    R = render;
//}
//
//static int hash_str(void* key)
//{
//    return hashmapHash(key, strlen(key));
//}
//
//static bool hash_equal(void* a, void* b)
//{
//    return strcmp(a, b) == 0;
//}
//
//
//static void transform_vertex(struct primitive_vertex* vo,
//    struct primitive_vertex* vi,
//    struct affine* transform)
//{
//    float x = vi->position[0], y = vi->position[1];
//    af_transfer_vec2(transform, &x, &y);
//    vo->position[0] = x;
//    vo->position[1] = y;
//}
//
//static void sprite_update_primitive_line_transform(struct sprite* self,
//    struct affine* transform)
//{
//    for (int i = 0; i < 2; ++i) {
//        transform_vertex(&self->primitive_data.primitive_vertex[i],
//            &self->primitive_data.primitive_vertex_origin[i],
//            transform);
//    }
//}
//
//static void sprite_update_primitive_rect_transform(struct sprite* self,
//    struct affine* transform)
//{
//    for (int i = 0; i < 4; ++i) {
//        transform_vertex(&self->primitive_data.primitive_vertex[i],
//            &self->primitive_data.primitive_vertex_origin[i],
//            transform);
//    }
//}
//
//static void sprite_sort_zorder(struct sprite* self)
//{
//    if (self->dirty & SPRITE_ZORDER_DIRTY) {
//        int n = array_size(self->children);
//        for (int i = 0; i < n - 1; ++i) {
//            for (int j = i + 1; j < n; ++j) {
//                struct sprite* a = array_at(self->children, i);
//                struct sprite* b = array_at(self->children, j);
//                if (a && b && a->zorder > b->zorder) {
//                    array_swap(self->children, i, j);
//                }
//            }
//        }
//
//        self->dirty &= (~SPRITE_ZORDER_DIRTY);
//    }
//}
//
//static void sprite_update_anim(struct sprite* self, float dt)
//{
//    if (self->type == SPRITE_TYPE_PIC) {
//        struct anim* anim = self->sprite_data.anim;
//        if (anim) {
//            anim_update(anim, dt);
//            sprite_set_sprite_frame(self, anim_current_frame(anim));
//        }
//    }
//}
//
//static void sprite_update_scale9(struct sprite* self)
//{
//    struct sprite_frame* frame = self->scale9_data.frame;
//    struct rect* inset = &(self->scale9_data.inset);
//    int width = frame->source_size.w;
//    int height = frame->source_size.h;
//
//    float ox = -self->w * self->anchor_x;
//    float oy = self->h * self->anchor_y;
//
//    struct scale9_data* data = &(self->scale9_data);
//
//    int l = inset->x;
//    int c = inset->w;
//    int r = width - (inset->x + inset->w);
//    int p = self->w - r;
//    float ps = (self->w - (l + r)) * 1.0f / c;
//
//    int t = inset->y;
//    int m = inset->h;
//    int b = height - (inset->y + inset->h);
//    int q = self->h - b;
//    float qs = (self->h - (t + b)) * 1.0f / m;
//
//    sprite_set_scale_x(data->tc, ps);
//    sprite_set_scale_x(data->mc, ps);
//    sprite_set_scale_x(data->bc, ps);
//
//    sprite_set_scale_y(data->ml, qs);
//    sprite_set_scale_y(data->mc, qs);
//    sprite_set_scale_y(data->mr, qs);
//
//    sprite_set_pos(data->tl, ox + 0, oy + 0);
//    sprite_set_pos(data->tc, ox + l, oy + 0);
//    sprite_set_pos(data->tr, ox + p, oy + 0);
//    sprite_set_pos(data->ml, ox + 0, oy - t);
//    sprite_set_pos(data->mc, ox + l, oy - t);
//    sprite_set_pos(data->mr, ox + p, oy - t);
//    sprite_set_pos(data->bl, ox + 0, oy - q);
//    sprite_set_pos(data->bc, ox + l, oy - q);
//    sprite_set_pos(data->br, ox + p, oy - q);
//}
//
//static void sprite_update_transform(struct sprite* self)
//{
//    // pass the dirty flags to the children
//    for (int i = 0; i < array_size(self->children); ++i) {
//        struct sprite* child = (struct sprite*)array_at(self->children, i);
//        if (child) {
//            // we should only pass the SRT dirty to the children
//            child->dirty |= (self->dirty & SPRITE_SRT_DIRTY);
//        }
//    }
//
//    // TODO: we could improve performance by seprating transform from SRT,
//    // but for simpler implemention, we have use SRT_DIRTY right now. :)
//    if (self->dirty & SPRITE_SRT_DIRTY) {
//        struct affine* local = &self->local_srt;
//        af_srt(local, self->x, self->y, self->scale_x, self->scale_y, self->rotation, self->rotation);
//
//        struct affine tmp;
//        af_identify(&tmp);
//        af_concat(&tmp, &self->local_srt);
//
//        if (self->parent) {
//            af_concat(&tmp, &(self->parent->world_srt));
//        }
//
//        // TODO: refactor here someday. doesn't have any good idea right now.
//        if (self->type == SPRITE_TYPE_PRIMITVE) {
//            switch (self->primitive_data.primitive_type) {
//            case PRIMITIVE_LINE:
//                sprite_update_primitive_line_transform(self, &tmp);
//                break;
//            case PRIMITIVE_RECT:
//                sprite_update_primitive_rect_transform(self, &tmp);
//                break;
//            default:
//                break;
//            }
//        }
//        else if (self->type == SPRITE_TYPE_SCALE9) {
//            sprite_update_scale9(self);
//        }
//        else {// if (self->type == SPRITE_TYPE_PIC) {
//            float w0 = self->w * (1 - self->anchor_x);
//            float w1 = self->w * (0 - self->anchor_x);
//
//            float h0 = self->h * (1 - self->anchor_y);
//            float h1 = self->h * (0 - self->anchor_y);
//            float a = tmp.a;
//            float b = tmp.b;
//            float c = tmp.c;
//            float d = tmp.d;
//            float tx = tmp.x;
//            float ty = tmp.y;
//
//            float x0 = a * w1 + c * h1 + tx;
//            float y0 = d * h1 + b * w1 + ty;
//            float x1 = a * w0 + c * h1 + tx;
//            float y1 = d * h1 + b * w0 + ty;
//            float x2 = a * w0 + c * h0 + tx;
//            float y2 = d * h0 + b * w0 + ty;
//            float x3 = a * w1 + c * h0 + tx;
//            float y3 = d * h0 + b * w1 + ty;
//
//            struct glyph* g = &self->sprite_data.glyph;
//            SET_VERTEX_POS(g->bl, x0, y0);
//            SET_VERTEX_POS(g->br, x1, y1);
//            SET_VERTEX_POS(g->tr, x2, y2);
//            SET_VERTEX_POS(g->tl, x3, y3);
//
//            // use custom width & height
//            //self->w = fabs(x1 - x0);
//            //self->h = fabs(y3 - y1);
//        }
//        self->world_srt = tmp;
//    }
//    self->dirty &= (~SPRITE_SRT_DIRTY);
//}
//
//static void sprite_update_color(struct sprite* self)
//{
//    if (self->dirty & SPRITE_COLOR_DIRTY) {
//
//        // todo: other type wants his bev
//        switch (self->type) {
//        case SPRITE_TYPE_SCALE9:
//        {
//            struct scale9_data* data = &(self->scale9_data);
//            for (int i = 0; i < 9; ++i) {
//                sprite_set_color(data->sprites[i], self->color);
//            }
//            break;
//        }
//        default:
//        {
//            struct glyph* g = &self->sprite_data.glyph;
//            SET_VERTEX_COLOR_UINT(g->bl, self->color);
//            SET_VERTEX_COLOR_UINT(g->br, self->color);
//            SET_VERTEX_COLOR_UINT(g->tr, self->color);
//            SET_VERTEX_COLOR_UINT(g->tl, self->color);
//            break;
//        }
//        }
//        self->dirty &= (~SPRITE_COLOR_DIRTY);
//    }
//}
//
//struct sprite_frame_cache* sprite_frame_cache_new()
//{
//    struct sprite_frame_cache* c = STRUCT_NEW(sprite_frame_cache);
//    c->cache = hashmapCreate(128, hash_str, hash_equal);
//
//    C = c;
//    return c;
//}
//
//void sprite_frame_cache_free(struct sprite_frame_cache* self)
//{
//    hashmapFree(self->cache);
//    s_free(self);
//}
//
//void sprite_frame_cache_add(struct sprite_frame_cache* self,
//    struct sprite_frame* frame)
//{
//    hashmapPut(self->cache, frame->key, frame);
//}
//
//struct sprite_frame* sprite_frame_cache_get(struct sprite_frame_cache* self,
//    const char* key)
//{
//    struct sprite_frame* f = hashmapGet(self->cache, (void*)key);
//    if (!f) {
//        f = sprite_frame_new(key);
//        sprite_frame_cache_add(GAME->sprite_frame_cache, f);
//    }
//    return f;
//}
//
//struct sprite_frame* sprite_frame_new(const char* key)
//{
//    struct sprite_frame* f = STRUCT_NEW(sprite_frame);
//    memset(f, 0, sizeof(struct sprite_frame));
//
//    size_t len = strlen(key);
//    f->key = s_malloc(len + 1);
//    memset(f->key, 0, len + 1);
//    strcpy(f->key, key);
//
//    return f;
//}
//
//static struct sprite_frame* sprite_frame_new_raw()
//{
//    struct sprite_frame* f = STRUCT_NEW(sprite_frame);
//    memset(f, 0, sizeof(struct sprite_frame));
//    return f;
//}
//
//void sprite_frame_free(struct sprite_frame* self)
//{
//    s_free(self->key);
//    s_free(self);
//}
//
//void sprite_frame_set_texture_id(struct sprite_frame* self, GLuint tex_id)
//{
//    self->tex_id = tex_id;
//}
//
//void sprite_frame_init_uv(struct sprite_frame* self,
//    float texture_width, float texture_height) {
//    struct rect* frame_rect = &self->frame_rect;
//    self->uv.u = frame_rect->x / texture_width;
//    self->uv.v = 1.0f - (frame_rect->y + frame_rect->h) / texture_height; // left corner is (0, 0)
//    self->uv.w = frame_rect->w / texture_width;
//    self->uv.h = frame_rect->h / texture_height;
//}
//
//void sprite_frame_init_subuv(struct sprite_frame* self, struct sprite_frame* parent)
//{
//    int tex_width = parent->frame_rect.w / parent->uv.w;
//    int tex_height = parent->frame_rect.h / parent->uv.h;
//    sprite_frame_init_uv(self, tex_width, tex_height);
//}
//
//void sprite_frame_tostring(struct sprite_frame* self, char* buff)
//{
//    sprintf(buff, "{key = \"%s\",\n"
//        "frame_rect = {%d, %d, %d, %d},\n"
//        "source_rect = {%d, %d, %d, %d},\n"
//        "size = {%d, %d},\n"
//        "text_id = %d,\n"
//        "rotated = %s,\n"
//        "trimmed = %s,\n"
//        "uv = {%.2f, %.2f, %.2f, %.2f}}\n",
//        self->key,
//        self->frame_rect.x, self->frame_rect.y,
//        self->frame_rect.w, self->frame_rect.h,
//        self->source_rect.x, self->source_rect.y,
//        self->source_rect.w, self->source_rect.h,
//        self->source_size.w, self->source_size.h,
//        self->tex_id,
//        stringfy_bool(self->rotated),
//        stringfy_bool(self->trimmed),
//        self->uv.u, self->uv.v, self->uv.w, self->uv.h);
//}
//
//static void sprite_init(struct sprite* self,
//    enum sprite_type type,
//    float width, float height)
//{
//    self->__id = ++__sprite_id;
//    self->__lua_handler = 0;
//    self->parent = NULL;
//    self->dirty = SPRITE_ALL_DIRTY;
//    self->zorder = 0;
//    self->scale_x = self->scale_y = 1.0f;
//    self->rotation = 0.0f;
//    self->x = self->y = 0.0f;
//    self->anchor_x = self->anchor_y = 0.5f;
//    self->w = width;
//    self->h = height;
//    self->ow = width;
//    self->oh = height;
//    self->color = C4B_COLOR(255, 255, 255, 255);
//    self->visible = true;
//    self->touchable = false;
//    self->swallow = false;
//    self->children = array_new(16);
//
//    af_identify(&self->local_srt);
//    af_identify(&self->world_srt);
//}
//
//void sprite_set_glyph(struct sprite* self, struct rect* rect,
//    struct uv* uv, GLuint tex_id)
//{
//    struct glyph* g = &(self->sprite_data.glyph);
//
//    SET_VERTEX_POS(g->bl, 0.0f, 0.0f);
//    SET_VERTEX_COLOR(g->bl, 255, 255, 255, 255);
//
//    SET_VERTEX_POS(g->br, rect->w, 0.0f);
//    SET_VERTEX_COLOR(g->br, 255, 255, 255, 255);
//
//    SET_VERTEX_POS(g->tl, 0.0f, rect->h);
//    SET_VERTEX_COLOR(g->tl, 255, 255, 255, 255);
//
//    SET_VERTEX_POS(g->tr, rect->w, rect->h);
//    SET_VERTEX_COLOR(g->tr, 255, 255, 255, 255);
//
//    if (uv) {
//        SET_VERTEX_UV(g->bl, uv->u, uv->v);
//        SET_VERTEX_UV(g->br, uv->u + uv->w, uv->v);
//        SET_VERTEX_UV(g->tl, uv->u, uv->v + uv->h);
//        SET_VERTEX_UV(g->tr, uv->u + uv->w, uv->v + uv->h);
//    }
//}
//
//struct sprite* sprite_new(struct sprite_frame* frame)
//{
//    struct sprite* s = STRUCT_NEW(sprite);
//    s->type = SPRITE_TYPE_PIC;
//    s->sprite_data.anim = NULL;
//    s->sprite_data.frame = frame;
//    sprite_init(s, SPRITE_TYPE_PIC, frame->source_rect.w, frame->source_rect.h);
//
//    sprite_set_glyph(s, &frame->frame_rect, &frame->uv, frame->tex_id);
//
//    // TODO: should we do it here? can this make the glyph right?
//    // update the sprite once, so that we can get glyph right after we new it
//    // sprite_update_transform(s);
//    return s;
//}
//
//struct sprite* sprite_new_label(const char* label)
//{
//    struct sprite* s = STRUCT_NEW(sprite);
//    s->type = SPRITE_TYPE_TTF_LABEL;
//
//    //TODO: implement this later
//    s_assert(false);
//    return s;
//}
//
//struct sprite* sprite_new_bmfont_label(const char* label,
//    const char* fnt_path,
//    int line_width)
//{
//    struct rect r = { 0, 0, line_width, 0 };
//    struct sprite* s = sprite_new_container(&r);
//    s->type = SPRITE_TYPE_BMFONT_LABEL;
//
//    struct bmfont* bmfont = bmfont_cache_get(GAME->bmfont_cache, fnt_path);
//
//    s->bmfont_data.line_width = line_width;
//    s->bmfont_data.text = NULL;
//    s->bmfont_data.bmfont = bmfont;
//    sprite_set_text(s, label);
//
//    return s;
//}
//
//struct sprite* sprite_new_container(struct rect* r)
//{
//    struct sprite* s = STRUCT_NEW(sprite);
//    s->type = SPRITE_TYPE_CONTAINER;
//
//    sprite_init(s, SPRITE_TYPE_CONTAINER, r->w, r->h);
//    sprite_set_glyph(s, r, NULL, 0);
//    s->x = r->x;
//    s->y = r->y;
//    return s;
//}
//
//#if defined (SEAL_USE_SPINE)
//struct sprite* sprite_new_spine(const char* atlas_path,
//    const char* spine_data_path,
//    float scale)
//{
//    struct sprite* s = STRUCT_NEW(sprite);
//    s->type = SPRITE_TYPE_SPINE;
//
//    struct rect r;
//    struct spine_anim* spine_anim = spine_anim_new(atlas_path,
//        spine_data_path,
//        scale);
//    spine_get_boundingbox(spine_anim, &r);
//    sprite_init(s, SPRITE_TYPE_SPINE, r.w, r.h);
//    s->spine_data.spine_anim = spine_anim;
//    return s;
//}
//#endif
//
//struct sprite* sprite_new_clip(struct rect* r)
//{
//    struct sprite* s = STRUCT_NEW(sprite);
//    s->type = SPRITE_TYPE_CLIP;
//
//    sprite_init(s, SPRITE_TYPE_CLIP, r->w, r->h);
//    sprite_set_glyph(s, r, NULL, 0);
//
//    s->x = r->x;
//    s->y = r->y;
//    return s;
//}
//
//// vertex: float[4], start point:(v[0], v[1]), end point: (v[1], v[2])
//struct sprite* sprite_new_line(float* vertex, float width, color line_color)
//{
//    struct sprite* s = STRUCT_NEW(sprite);
//
//    struct rect rect = {
//        0, 0, vertex[2] - vertex[0], width
//    };
//
//    s->type = SPRITE_TYPE_PRIMITVE;
//    sprite_init(s, SPRITE_TYPE_PRIMITVE, rect.w, rect.h);
//    sprite_set_color(s, line_color);
//
//    s->primitive_data.primitive_type = PRIMITIVE_LINE;
//    struct primitive_vertex* v = s_malloc(PRIMITIVE_VERTEX_SIZE * 2);
//    SET_VERTEX_POS(v[0], vertex[0], vertex[1]);
//    SET_VERTEX_POS(v[1], vertex[2], vertex[3]);
//
//    SET_VERTEX_COLOR_UINT(v[0], line_color);
//    SET_VERTEX_COLOR_UINT(v[1], line_color);
//
//    s->primitive_data.primitive_vertex = v;
//
//    struct primitive_vertex* v_origin = s_malloc(PRIMITIVE_VERTEX_SIZE * 4);
//    memcpy(v_origin, v, PRIMITIVE_VERTEX_SIZE * 4);
//
//    s->primitive_data.primitive_vertex = v;
//    s->primitive_data.primitive_vertex_origin = v_origin;
//    return s;
//}
//
//struct sprite* sprite_new_rect(struct rect* rect,
//    unsigned int rect_flag,
//    color fill_color, color outline_color)
//{
//    struct sprite* s = STRUCT_NEW(sprite);
//    s->type = SPRITE_TYPE_PRIMITVE;
//    struct primitive_data* primitive_data = &s->primitive_data;
//    primitive_data->primitive_type = PRIMITIVE_RECT;
//
//    sprite_init(s, SPRITE_TYPE_PRIMITVE, rect->w, rect->h);
//    sprite_set_glyph(s, rect, NULL, 0);
//    sprite_set_color(s, fill_color);
//
//    struct primitive_vertex* v = s_malloc(PRIMITIVE_VERTEX_SIZE * 4);
//    float l = rect->x;
//    float r = rect->x + rect->w;
//    float b = rect->y;
//    float t = rect->y + rect->h;
//
//    // 4 lines forms an rect.
//    SET_VERTEX_POS(v[0], l, b);
//    SET_VERTEX_POS(v[1], r, b);
//    SET_VERTEX_POS(v[2], r, t);
//    SET_VERTEX_POS(v[3], l, t);
//
//    SET_VERTEX_COLOR_UINT(v[0], fill_color);
//    SET_VERTEX_COLOR_UINT(v[1], fill_color);
//    SET_VERTEX_COLOR_UINT(v[2], fill_color);
//    SET_VERTEX_COLOR_UINT(v[3], fill_color);
//
//    struct primitive_vertex* v_origin = s_malloc(PRIMITIVE_VERTEX_SIZE * 4);
//    memcpy(v_origin, v, PRIMITIVE_VERTEX_SIZE * 4);
//
//    primitive_data->primitive_vertex = v;
//    primitive_data->primitive_vertex_origin = v_origin;
//
//    primitive_data->rect_flag = rect_flag;
//    primitive_data->fill_color = fill_color;
//    primitive_data->outline_color = outline_color;
//
//    return s;
//}
//
//static struct sprite* sprite_new_scale9_item(struct sprite* self,
//    int x, int y,
//    int width, int height,
//    int tex_width, int tex_height,
//    GLuint tex_id,
//    const char* tag)
//{
//    struct sprite_frame* frame = sprite_frame_new_raw();
//    frame->frame_rect.x = x;
//    frame->frame_rect.y = y;
//    frame->frame_rect.w = width;
//    frame->frame_rect.h = height;
//
//    frame->source_rect = frame->frame_rect;
//
//    frame->source_size.w = width;
//    frame->source_size.h = height;
//
//    frame->tex_id = tex_id;
//
//    sprite_frame_init_uv(frame, tex_width, tex_height);
//    frame->__initialized = true;
//
//    struct sprite* s = sprite_new(frame);
//    sprite_set_anchor(s, 0, 1);
//    sprite_add_child(self, s, 0);
//    return s;
//}
//
//struct sprite* sprite_new_scale9(struct sprite_frame* frame, struct rect* inset)
//{
//    int t_width = frame->frame_rect.w / frame->uv.w;
//    int t_height = frame->frame_rect.h / frame->uv.h;
//
//    int width = frame->source_size.w;
//    int height = frame->source_size.h;
//
//    struct rect rect = { 0, 0, width, height };
//    struct sprite* s = sprite_new_container(&rect);
//    sprite_set_anchor(s, 0.5f, 0.5f);
//    s->type = SPRITE_TYPE_SCALE9;
//
//    int x = frame->frame_rect.x;
//    int y = frame->frame_rect.y;
//
//    int l = inset->x;
//    int c = inset->w;
//    int r = width - (inset->x + inset->w);
//    int p = l + c;
//
//    int t = inset->y;
//    int m = inset->h;
//    int b = height - (inset->y + inset->h);
//    int q = t + m;
//
//    GLuint tex_id = frame->tex_id;
//    struct scale9_data* data = &(s->scale9_data);
//    data->frame = frame;
//    data->inset = *inset;
//
//    data->tl = sprite_new_scale9_item(s, x + 0, y + 0, l, t, t_width, t_height, tex_id, "tlf");
//    data->tc = sprite_new_scale9_item(s, x + l, y + 0, c, t, t_width, t_height, tex_id, "tcf");
//    data->tr = sprite_new_scale9_item(s, x + p, y + 0, r, t, t_width, t_height, tex_id, "trf");
//
//    data->ml = sprite_new_scale9_item(s, x + 0, y + t, l, m, t_width, t_height, tex_id, "mlf");
//    data->mc = sprite_new_scale9_item(s, x + l, y + t, c, m, t_width, t_height, tex_id, "mcf");
//    data->mr = sprite_new_scale9_item(s, x + p, y + t, r, m, t_width, t_height, tex_id, "mrf");
//
//    data->bl = sprite_new_scale9_item(s, x + 0, y + q, l, b, t_width, t_height, tex_id, "blf");
//    data->bc = sprite_new_scale9_item(s, x + l, y + q, c, b, t_width, t_height, tex_id, "bcf");
//    data->br = sprite_new_scale9_item(s, x + p, y + q, r, b, t_width, t_height, tex_id, "brf");
//
//    for (int i = 0; i < 9; ++i) {
//        data->frames[i] = data->sprites[i]->sprite_data.frame;
//    }
//
//    return s;
//}
//
//void sprite_free(struct sprite* self)
//{
//    switch (self->type) {
//    case SPRITE_TYPE_PIC:
//    {
//        if (self->sprite_data.anim) {
//            anim_free(self->sprite_data.anim);
//        }
//        break;
//    }
//
//    case SPRITE_TYPE_BMFONT_LABEL:
//    {
//        if (self->bmfont_data.text) {
//            s_free(self->bmfont_data.text);
//        }
//        break;
//    }
//
//#if defined (SEAL_USE_SPINE)
//    case SPRITE_TYPE_SPINE:
//    {
//        if (self->spine_data.spine_anim) {
//            spine_anim_free(self->spine_data.spine_anim);
//        }
//        break;
//    }
//#endif
//
//    case SPRITE_TYPE_PRIMITVE:
//    {
//        if (self->primitive_data.primitive_vertex) {
//            s_free(self->primitive_data.primitive_vertex);
//        }
//        if (self->primitive_data.primitive_vertex) {
//            s_free(self->primitive_data.primitive_vertex_origin);
//        }
//        break;
//    }
//
//    case SPRITE_TYPE_SCALE9:
//    {
//        struct scale9_data* data = &(self->scale9_data);
//        for (int i = 0; i < 9; ++i) {
//            sprite_frame_free(data->frames[i]);
//        }
//
//        break;
//    }
//
//    case SPRITE_TYPE_CONTAINER:
//    {
//        break;
//    }
//
//    default:
//        break;
//    }
//    lua_handler_clean(GAME->lua_handler, GAME->lstate, self);
//    array_free(self->children);
//    s_free(self);
//}
//
//void sprite_set_text(struct sprite* self, const char* label)
//{
//    struct bmfont_data* bmfont_data = &self->bmfont_data;
//    char* text = bmfont_data->text;
//    struct bmfont* bmfont = bmfont_data->bmfont;
//    if (text && (!strcmp(text, label))) {
//        return;
//    }
//
//    if (self->type == SPRITE_TYPE_BMFONT_LABEL && bmfont) {
//        sprite_remove_all_child(self);
//        const char* fnt_path = bmfont->fnt_file;
//        char* p = strrchr(fnt_path, '/');
//        char tmp[128] = "";
//        strncpy(tmp, fnt_path, p - fnt_path);
//        char pagefile[128] = "";
//        snprintf(pagefile, 128, "%s/%s", tmp, bmfont->page.file);
//        struct texture* tex = texture_cache_load(GAME->texture_cache, pagefile);
//
//        // not support chinese yet.
//        char label_key[128] = "";
//        int len = strlen(label);
//        float x = 0.0f;
//        float y = 0.0f;
//
//        int width = bmfont_data->line_width;
//        int height = bmfont->common.lineHeight;
//        bool auto_calc_width = width == 0;
//
//        // TODO: we should have an bmfont render, cause it wastes a lot of sprite visit
//        // to render a simple label, anyway it's simpler right now. :)
//        for (int i = 0; i < len; ++i) {
//
//            // TODO: currently we only support ** ENGLISH **, we need have a UTF-8 character split function
//            // to split this label into characters, so that we were able to support Chinese, Japanese..O
//            char c = label[i];
//            snprintf(label_key, 128, "%s_%c", fnt_path, c);
//
//            char str[4] = { c, 0, 0, 0 };
//
//            struct charc* character = bmfont_load_charc(bmfont, str);
//
//            struct sprite_frame* frame = sprite_frame_cache_get(C, label_key);
//            if (!frame->__initialized) {
//
//                frame->frame_rect.x = character->x;
//                frame->frame_rect.y = character->y;
//                frame->frame_rect.w = character->width;
//                frame->frame_rect.h = character->height;
//
//                frame->source_rect = frame->frame_rect;
//
//                frame->source_size.w = character->width;
//                frame->source_size.h = character->height;
//
//                frame->tex_id = tex->id;
//
//                sprite_frame_init_uv(frame, tex->width, tex->height);
//                frame->__initialized = true;
//            }
//
//            struct sprite* c_sprite = sprite_new(frame);
//            int yoffset = bmfont->common.lineHeight - character->yoffset - character->height;
//            sprite_set_pos(c_sprite, x + character->xoffset, y + yoffset);
//            sprite_set_anchor(c_sprite, 0, 0);
//            sprite_add_child(self, c_sprite, 0);
//
//            // coord caculation
//            x += character->xadvance;
//            if (!auto_calc_width) {
//                if (x > width) {
//                    x = 0;
//                    y -= bmfont->common.lineHeight;
//                    height += bmfont->common.lineHeight;
//                }
//            }
//            else {
//                width = x;
//            }
//        }
//
//        sprite_set_size(self, width, height);
//
//        int label_count = strlen(label);
//        if (!text) {
//            text = s_malloc(label_count + 1);
//
//        }
//        else if (label_count > strlen(text)) {
//            text = s_realloc(text, label_count + 1);
//        }
//        strcpy(text, label);
//        self->bmfont_data.text = text;
//    }
//}
//
//void sprite_add_child(struct sprite* self, struct sprite* child, int zorder)
//{
//    s_assert(child && child != self);
//
//    array_push_back(self->children, child);
//    child->zorder = zorder;
//    child->parent = self;
//
//    self->dirty |= SPRITE_ZORDER_DIRTY;
//}
//
//void sprite_remove_from_parent(struct sprite* self)
//{
//    s_assert(self->parent);
//    scheduler_stop_target(GAME->scheduler, self);
//    // we only remove the child, but we don't move the array
//    array_remove(self->parent->children, self);
//    sprite_remove_all_child(self);
//    sprite_free(self);
//}
//
//void sprite_remove_child(struct sprite* self, struct sprite* child)
//{
//    // here we should release the memory??? yes.
//    if (child) {
//        sprite_remove_from_parent(child);
//    }
//}
//
//void sprite_remove_all_child(struct sprite* self)
//{
//    struct array* children = self->children;
//    int n = array_size(children);
//    for (int i = n - 1; i >= 0; --i) {
//        sprite_remove_child(self, array_at(children, i));
//    }
//    //    LOGP("after remove all child children = %d", array_size(children));
//}
//
//static int touch_event_set_func(lua_State* L, void* ud)
//{
//    struct touch_event* event = (struct touch_event*)ud;
//    lua_pushstring(L, SPRITE_EVENT_TYPE);
//    lua_pushinteger(L, event->type);
//    lua_pushinteger(L, event->x);
//    lua_pushinteger(L, event->y);
//    return 4;
//}
//
//void sprite_visit_touch(struct sprite* self, struct touch_handler* handler, struct touch_event* touch_event)
//{
//    struct array* children = self->children;
//    for (int i = 0;i < array_size(children); ++i) {
//        struct sprite* child = (struct sprite*)array_at(children, i);
//        if (child) { // NULL indicates that the child has been removed
//                     // recursively visit the children.
//            sprite_visit_touch(child, handler, touch_event);
//        }
//    }
//
//    bool contains = sprite_contains(self, touch_event->x, touch_event->y);
//    bool visible = self->visible;
//    //    LOGP("__id = %d, contains = %s, visible = %s", self->__id, stringfy_bool(contains), stringfy_bool(visible));
//    if (touch_event->type == TOUCH_BEGIN && contains && visible) {
//        touch_handler_push(handler, self);
//    }
//}
//
//void sprite_touch(struct sprite* self, struct touch_event* touch_event)
//{
//    seal_call_func(self, touch_event_set_func, touch_event, false);
//}
//
//bool sprite_contains(struct sprite* self, float x, float y)
//{
//    if (self->w < FLT_EPSILON || self->h < FLT_EPSILON) return false;
//    //struct glyph* g = &self->sprite_data.glyph;
//    struct rect world = {
//        0 - self->w * self->anchor_x,//g->bl.position[0],
//        0 - self->h * self->anchor_y,//g->bl.position[1],
//        self->w,
//        self->h,
//    };
//
//    af_transfer_rect(&self->world_srt, &world.x, &world.y, &world.w, &world.h);
//
//    return rect_contains(&world, x, y);
//}
//
//void sprite_run_action(struct sprite* self, struct action* action)
//{
//    scheduler_schedule(GAME->scheduler, self, action);
//}
//
//void sprite_stop_all_actions(struct sprite* self)
//{
//    scheduler_stop_target(GAME->scheduler, self);
//}
//
//static void sprite_draw_pic(struct sprite* self)
//{
//    render_switch(R, RENDER_TYPE_SPRITE);
//    sprite_renderer_func_draw(R, self);
//}
//
//static void sprite_draw_clip(struct sprite* self)
//{
//    render_flush(R);
//
//    struct rect sr = {
//        self->x, self->y, self->w, self->h
//    };
//
//    af_transfer_rect(&self->world_srt, &sr.x, &sr.y, &sr.w, &sr.h);
//
//    render_set_scissors(R, sr.x, sr.y, sr.w, sr.h);
//}
//
//#if defined (SEAL_USE_SPINE)
//static void sprite_draw_spine(struct sprite* self, float dt)
//{
//    struct spine_anim* anim = self->spine_data.spine_anim;
//    render_switch(R, RENDER_TYPE_SPINE);
//    spine_anim_update(anim, dt);
//    spine_anim_draw(anim, R, self->world_srt.x, self->world_srt.y);
//}
//#endif
//
//static void sprite_draw_primitive(struct sprite* self)
//{
//    render_switch(R, RENDER_TYPE_PRIMITIVE);
//    primitive_render_func_draw(R,
//        self->primitive_data.primitive_type,
//        self);
//}
//
//static void sprite_draw(struct sprite* self, float dt)
//{
//    if (!self->visible) {
//        return;
//    }
//
//    switch (self->type) {
//    case SPRITE_TYPE_PIC:
//        sprite_draw_pic(self);
//        break;
//    case SPRITE_TYPE_CLIP:
//        sprite_draw_clip(self);
//        break;
//#if defined (SEAL_USE_SPINE)
//    case SPRITE_TYPE_SPINE:
//        sprite_draw_spine(self, dt);
//        break;
//#endif
//    case SPRITE_TYPE_PRIMITVE:
//        sprite_draw_primitive(self);
//        break;
//    case SPRITE_TYPE_CONTAINER:
//        // do nothing.
//        break;
//    default:
//        break;
//    }
//}
//
//static void sprite_after_visit(struct sprite* self)
//{
//    if (!self->visible) {
//        return;
//    }
//
//    switch (self->type) {
//        case SPRITE_TYPE_CLIP:
//        {
//            render_flush(R);
//            render_clean_scissors(R);
//            break;
//        }
//        default:
//            break;
//    }
//}
//
//void sprite_visit(struct sprite* self, float dt)
//{
//    sprite_sort_zorder(self);
//    sprite_update_transform(self);
//    sprite_update_anim(self, dt);
//    sprite_update_color(self);
//    sprite_draw(self, dt);
//
//    struct array* children = self->children;
//    for (int i = 0;i < array_size(children); ++i) {
//        struct sprite* child = (struct sprite*)array_at(children, i);
//        if (child) { // NULL indicates that the child has been removed
//                     // recursively visit the children.
//            sprite_visit(child, dt);
//        }
//    }
//    sprite_after_visit(self);
//}
//
//void sprite_draw_label(struct sprite* self)
//{
//
//}
//
//void sprite_set_sprite_frame(struct sprite* self, struct sprite_frame* frame)
//{
//    struct glyph* g = sprite_get_glyph(self);
//    struct uv* uv = &frame->uv;
//    SET_VERTEX_UV(g->bl, uv->u, uv->v);
//    SET_VERTEX_UV(g->br, uv->u + uv->w, uv->v);
//    SET_VERTEX_UV(g->tl, uv->u, uv->v + uv->h);
//    SET_VERTEX_UV(g->tr, uv->u + uv->w, uv->v + uv->h);
//
//    self->sprite_data.frame = frame;
//
//    self->dirty |= SPRITE_FRAME_DIRTY;
//}
//
//void sprite_set_anim(struct sprite* self, struct anim* anim)
//{
//    struct anim* origin = self->sprite_data.anim;
//    if (origin != anim) {
//        if (origin) {
//            anim_free(origin);
//        }
//
//        self->sprite_data.anim = anim;
//        anim_play(anim);
//    }
//}
//
//#if defined (SEAL_USE_SPINE)
//void sprite_set_spine_anim(struct sprite* self, const char* anim_name, int track, bool loop)
//{
//    spine_anim_set_anim(self->spine_data.spine_anim, anim_name, track, loop);
//}
//#endif
//
//void sprite_set_visible(struct sprite* self, bool visible)
//{
//    self->visible = visible;
//}
//
//void sprite_set_pos(struct sprite* self, float x, float y)
//{
//    self->x = x;
//    self->y = y;
//
//    self->dirty |= SPRITE_TRANSFORM_DIRTY;
//}
//
//void sprite_set_anchor(struct sprite* self, float x, float y)
//{
//    self->anchor_x = x;
//    self->anchor_y = y;
//    self->dirty |= SPRITE_TRANSFORM_DIRTY;
//}
//
//void sprite_set_rotation(struct sprite* self, float rotation)
//{
//    self->rotation = rotation;
//    self->dirty |= SPRITE_ROTATION_DIRTY;
//}
//
//void sprite_set_scale(struct sprite* self, float scale)
//{
//    self->scale_x = self->scale_y = scale;
//    self->dirty |= SPRITE_SCALE_DIRTY;
//}
//
//void sprite_set_scale_x(struct sprite* self, float scale_x)
//{
//    self->scale_x = scale_x;
//    self->dirty |= SPRITE_SCALE_DIRTY;
//}
//
//void sprite_set_scale_y(struct sprite* self, float scale_y)
//{
//    self->scale_y = scale_y;
//    self->dirty |= SPRITE_SCALE_DIRTY;
//}
//
//void sprite_set_color(struct sprite* self, color color)
//{
//    self->color = color;
//    self->dirty |= SPRITE_COLOR_DIRTY;
//}
//
//void sprite_set_opacity(struct sprite* self, unsigned char opacity)
//{
//    self->color = (self->color & 0xffffff00) | opacity;
//    self->dirty |= SPRITE_COLOR_DIRTY;
//}
//
//void sprite_set_size(struct sprite* self, float width, float height)
//{
//    self->w = width;
//    self->h = height;
//
//    if (self->type == SPRITE_TYPE_BMFONT_LABEL) {
//        // adjust anchor point
//        float ax = width * self->anchor_x;
//        float ay = height * self->anchor_y;
//
//        struct array* children = self->children;
//        for (int i = 0;i < array_size(children); ++i) {
//            struct sprite* child = (struct sprite*)array_at(children, i);
//            if (child) { // NULL indicates that the child has been removed
//                         // recursively visit the children.
//                sprite_set_pos(child, child->x - ax, child->y - ay);
//            }
//        }
//    }
//
//    self->dirty |= SPRITE_SRT_DIRTY;
//}
//
//struct glyph* sprite_get_glyph(struct sprite* self)
//{
//    return &self->sprite_data.glyph;
//}
//
//void sprite_dump_children(struct sprite* self)
//{
//    struct array* children = self->children;
//    for (int i = 0; i < array_size(children); ++i) {
//        struct sprite* s = array_at(children, i);
//        if (s) {
//            LOGP("[sprite] : addr(%p)"
//                "__id(%d) \n"
//                "children(%lu)",
//                s,
//                s->__id,
//                s->children != NULL ? array_size(s->children) : 0);
//        }
//        else {
//            LOGP("[sprite] : NULL");
//        }
//    }
//}

