

struct dl_list_node {
	struct dl_list_node *prev;
	struct dl_list_node *next;
	size_t elem_size;
	void *elem;
};
typedef struct dl_list_node dl_list_node;

struct dl_list {
  dl_list_node *head;
  dl_list_node *tail;
  size_t len;
};
typedef struct dl_list dl_list;

typedef int(*comp)(void*, void*);

void new_list(dl_list *list);
void add_tail(dl_list *list, void *elem, size_t elem_size);
void delete_node(dl_list *list, dl_list_node *node);
void find_all_nodes(dl_list_node **buf, size_t buf_len, dl_list *list, void* value, comp eq);
void free_list(dl_list *list);
