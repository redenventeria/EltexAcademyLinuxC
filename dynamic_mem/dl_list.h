
struct dl_list {
	struct dl_list *prev;
	struct dl_list *next;
	size_t elem_size;
	void *elem;
};
typedef struct dl_list dl_list;

typedef int(*comp)(void*, void*);

dl_list* new_list(void *elem, size_t elem_size);
void add_tail(dl_list *list, void *elem, size_t elem_size);
void delete_node(dl_list *p);
void find_all_nodes(dl_list **buf, size_t n, dl_list *list, void* value, comp eq);
