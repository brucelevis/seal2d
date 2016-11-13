#ifndef __seal__affine__
#define __seal__affine__

struct mat4;
struct affine {
    float a, b, c, d;
    float x, y;
};

void af_identify(struct affine* out);

void af_srt(struct affine* out,
            float x, float y,
            float scale_x, float scale_y,
            float rotation_x, float rotation_y);

void af_copy(struct affine* out, struct affine* in);

void af_set_translate(struct affine* out, float x, float y);
void af_set_scale(struct affine* out, float scale_x, float scale_y);
void af_set_rotation(struct affine* out, float rotate_x, float rotate_y);

void af_concat(struct affine* out, struct affine* in);

void af_tostring(struct affine* self, char* buff);

void af_transfer_vec2(struct affine* out, float *x, float* y);
void af_transfer_rect(struct affine* out, int *x, int* y, int* w, int* h);
void af_transfer_invert(struct affine* in, struct affine* out);

void af_to_mat4(struct affine* in, struct mat4* mat);

#endif
