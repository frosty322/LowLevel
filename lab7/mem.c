#include "mem.h"
#include <errno.h>
#include <string.h>

 
#define MMAP_PROT (PROT_READ | PROT_WRITE)
#define MMAP_FLAGS (0x02 | 0x20)


static void* start_address;
static void* end_address;


static struct mem* get_last_pointer(void) {
struct mem* head = (struct mem*) start_address;
while(head->next != NULL){
	head = head->next;}
return head;
}

static size_t get_size_pointer(size_t query){
query = query > MIN_SIZE ? query : MIN_SIZE;
query = query % 4096 == 0 ? query : query + (4096 - query%4096);
return query;
}


static struct mem* space(struct mem* head, size_t query) {
while (head != NULL){
	if (head->is_free == 1 && head->capacity >= query) return head;
	head = head->next;
}
return NULL;
}


void* heap_init(size_t initial_size) { 
struct mem* head;
void* pointer;
initial_size = get_size_pointer(initial_size);
pointer = mmap(NULL, initial_size + 1, MMAP_PROT, MMAP_FLAGS, -1, 0);

if (!start_address) start_address = pointer;

end_address = (char*) pointer + initial_size;
head = (struct mem*) pointer;
head->next = NULL;
head->capacity = initial_size - sizeof(struct mem);
head->is_free = 0;

printf("Initialized heap at %p, size %ld\n", (void*) head, initial_size);
return head;
}

struct mem* get_block(struct mem* head, size_t query){
char* pointer;
struct mem* other;
if (head->capacity - query <= sizeof(struct mem)){
	printf("Block is full at %p, seze %ld\n", (void*) head, head->capacity);
	head->is_free = 0;
	return head;
	}
pointer = (char*) head + sizeof(struct mem) + query;
other = (struct mem*) pointer;
other->capacity = head->capacity - query;
other->is_free = 1;
other->next = NULL;
head->next  = other;
head->is_free = 0;
head->capacity = query;
printf("Heap is split into two sections %p and %p, sizes %ld and %ld\n", (void*) head, (void*) other, head->capacity, other->capacity);
return head;
}

void* _malloc(size_t query) {
struct mem* head;
	
if (start_address == NULL) {
	head = (struct mem*) heap_init(query + sizeof(struct mem));
	head = get_block(head, query);
	return (char*) head + sizeof(struct mem);}
else{
	struct mem* block;
		
	head = (struct mem*) start_address;
	block = space(head, query + sizeof(struct mem));

	if (block != NULL) {
		block = get_block(block, query);
		return (char*) block + sizeof(struct mem);}
	else{
		char* pointer;
		char* tmp;
		size_t rest;
		struct mem* new_block;

		block = get_last_pointer();
		if (block->is_free == 1){
			pointer =(char*) end_address;
			rest = query + sizeof(struct mem) - block->capacity;
			rest = get_size_pointer(rest);
			tmp = (char*) mmap(pointer, rest + 1, MMAP_PROT, MAP_FIXED | MMAP_FLAGS, -1, 0);

			if (tmp != (void*) -1 && tmp != NULL) {
				printf("Heap has increased%s\n", "");
				end_address = pointer + rest;
				block->capacity = block->capacity + rest;
				block = get_block(block, query);
				return (char*) block + sizeof(struct mem);
				}
			}
		new_block = (struct mem*) heap_init(sizeof(struct mem) + query);
		if (new_block == NULL) return NULL;
		new_block = get_block(new_block, query);
		block->next = new_block;
		return (char*) new_block + sizeof(struct mem);
		}
	}
}

void _free(void* mem){
struct mem* memory = (struct mem*) start_address;
char* pointer = (char*) start_address;
while (pointer + sizeof(struct mem) != mem && pointer != NULL) {
	memory = memory->next;
	pointer = (char*) memory->next;}
while(memory!= NULL){
	printf("Space is free at %p\n", (void*) memory);
	memory->is_free = 1;
	if (memory->next != NULL && memory->next->is_free == 1 && ((char*) memory + sizeof(struct mem) + memory->capacity == (char*) memory->next)){
		printf("Spaces united: %p and %p\n", (void*) memory, (void*) memory->next);
		memory->capacity = memory->capacity + sizeof(struct mem) + memory->next->capacity;
		memory->next = memory->next->next;
		memory = memory->next;}
	else{break;}
	}
}

void memalloc_debug_struct_info(FILE* f, struct mem const* const address) {

	size_t i;

	fprintf(f, "start: %p\nsize: %lu\nis_free: %d\n",
			(void*) address,
			address->capacity,
			address->is_free);
	for (i = 0; i < DEBUG_FIRST_BYTES && i < address->capacity; ++i)
		printf("%hX", ((char*)address)[sizeof(struct mem) + i]);
	putc('\n', f);

}

void memalloc_debug_heap(FILE* f, struct mem const* ptr) {
	for (; ptr; ptr = ptr->next )
		memalloc_debug_struct_info(f, ptr);
}


