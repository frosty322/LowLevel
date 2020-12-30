#include <stdlib.h>

struct l_list {
	__int32_t value;
	struct l_list *next;
};


struct l_list *list_create(__int32_t  val);
struct l_list *list_add_front(struct l_list** m_list, __int32_t value);
struct l_list* list_add_back(struct l_list** m_list, __int32_t value);

struct l_list* list_add_after(struct l_list* node, __int32_t value);

struct l_list* list_node_at(struct l_list* list, size_t index);
__int32_t list_get(struct l_list* list, size_t index);
void list_free(struct l_list* list);
size_t list_lenght(struct l_list* list);
__int32_t list_sum(struct l_list* list);

void foreach(struct l_list* list, void (*consumer)(__int32_t));
struct l_list* map(struct l_list* list, __int32_t (*operator)(__int32_t));
struct l_list* map_mut(struct l_list** list, __int32_t (*operator)(__int32_t));


__int32_t foldl(struct l_list* list, __int32_t acc, __int32_t (*operator)(__int32_t, __int32_t));
struct l_list* iterate(__int32_t value, size_t length, __int32_t (*operator)(__int32_t));
__int32_t save(struct l_list* list, const char* filename);
__int32_t load(struct l_list** list, const char* filename);
__int32_t serialize(struct l_list* list, const char* filename);
__int32_t deserialize(struct l_list** list, const char* filename);
FILE* fileOpen(const char* filename, const char* mode);
void fileClose(FILE* f);
__int32_t fileEOF(FILE* f);
