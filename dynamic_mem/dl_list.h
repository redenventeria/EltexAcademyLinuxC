
struct dl_list {
	struct dl_list *prev;
	struct dl_list *next;
	size_t elem_size;
	void *elem;
};

typedef struct dl_list dl_list;

dl_list* new_list(void *elem, size_t elem_size);
void add_tail(dl_list *list, void *elem, size_t elem_size);
void delete_node(dl_list *p);
void search_node(dl_list *list, void *value, int(*comp)(void*, void*));
