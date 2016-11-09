#ifndef touch_handler_h
#define touch_handler_h

#define MAX_TOUCH_SEQ (8)
struct sprite;
struct touch_event;

struct touch_handler {
    int n_visited;
    int n_touched;
    struct sprite* __visiting[MAX_TOUCH_SEQ];
    struct sprite* __touched[MAX_TOUCH_SEQ];
};

struct touch_handler* touch_handler_new();
void touch_handler_free(struct touch_handler* self);

void touch_handler_push(struct touch_handler* self, struct sprite* sprite);
void touch_handler_visit(struct touch_handler* self, struct touch_event* event);

#endif
