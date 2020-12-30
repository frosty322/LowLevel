#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include "linked_list.h"

static void shift(struct l_list* iter)
{
    while (iter->next != NULL) iter = iter->next;
}

static size_t index_shift(struct l_list* count, size_t index)
{
    size_t i = 0;

    while (count->next != NULL)
    {
        if (index == i) break;

        count = count->next;
        i += 1;
    }

    return i;
}

struct l_list* list_create(__int32_t val) 
{

	struct l_list* list = malloc(sizeof(struct l_list));
	list->value = val;
	list->next = NULL;
	return list;

}

struct l_list* list_add_front(struct l_list** m_list, __int32_t value)
{
    	struct l_list *list = list_create(value);
	list->next = *m_list;
	*m_list = list;

	return list;
}

struct l_list* list_add_back(struct l_list** m_list, __int32_t value)
{
    struct l_list* count = *m_list;
	
	if (*m_list == NULL)
	{
		*m_list = count;
	} else {
        struct l_list *iter = *m_list;
        shift(iter);
		iter->next = count;
	}

	return count;
}

struct l_list* list_add_after(struct l_list* node, __int32_t value)
{
    struct l_list* new_node = list_create(value);

    if (node != NULL) 
    {
        new_node->next = node->next;
        node->next = new_node;
    }

    return new_node;
}

struct l_list* list_node_at(struct l_list* list, size_t index)
{
    struct l_list* count = list;

    size_t i = index_shift(count, index);

	if (index != i) return 0;

	return count;
}

__int32_t list_get(struct l_list* list, size_t index)
{
    struct l_list* count = list;

    size_t i = index_shift(count, index);

	if (index != i) return 0;

	return count->value;
}

void list_free(struct l_list* list)
{
	while (list->next != NULL) 
    {
        struct l_list* died = list;
		list = list->next;

		free(died);
	}
}


size_t list_lenght(struct l_list* list)
{
    struct l_list* count;
    size_t len = 0;

	for (count = list; count != NULL; count = count->next)
		++len;

	return len;
}

__int32_t list_sum(struct l_list* list)
{
    struct l_list* count = list;
    __int32_t sum = 0;

	if (count == NULL) return sum;

	for (;;)
	{
		sum += count->value;
		if (count->next == NULL) break;
		count = count->next;
	}

	return sum;
}

void foreach(struct l_list* list, void (*procedure)(__int32_t))
{
    struct l_list* count;

	for (count = list; count != NULL; count = count->next)
		procedure(count->value);
}

struct l_list* map(struct l_list* list, __int32_t (*operator)(__int32_t))
{
    struct l_list* count, * new_count = NULL, * start = NULL;

	for (count = list; count != NULL; count = count->next) 
    {
		new_count = list_add_after(new_count, operator(count->value));

		if (start == NULL) start = new_count;
	}

	return start;
}

struct l_list* map_mut(struct l_list** list, __int32_t (*operator)(__int32_t))
{
    struct l_list* count;

	for (count = *list; count != NULL; count = count->next)
		count->value = operator(count->value);

	return *list;
}

__int32_t foldl(struct l_list* list, __int32_t acc, __int32_t (*operator)(__int32_t, __int32_t))
{
    struct l_list* count;

    for (count = list; count != NULL; count = count->next)
        acc = operator(count->value, acc);

    return acc;
}


struct l_list* iterate(__int32_t value, size_t length, __int32_t (*operator)(__int32_t))
{
    size_t i;
    struct l_list* start, * count;

    start = list_create(value);
    count = start;

    for (i = 1; i < length; i++) 
    {
        value = operator(value);
        count = list_add_after(count, value);
    }

    return start;
}

FILE* fileOpen(const char* filename, const char* mode)
{
    FILE* f;
    errno = false; 
    f = fopen(filename, mode);
    if (errno) return NULL;
    return f;
}

void fileClose(FILE* f)
{
    fclose(f);
}

__int32_t save(struct l_list* list, const char* filename)
{
    struct l_list* count;
        
    FILE* f = fileOpen(filename, "w");
    if (f == NULL) return 0;

    for (count = list; count != NULL; count = count->next) 
    {
        fprintf(f, "%d ", count->value);

        if (errno || ferror(f)) 
        {
            fileClose(f);
            return 0;
        }
    }

    fileClose(f);

    if (errno) return 0;
    return 1;
}

__int32_t fileEOF(FILE* f)
{
    return feof(f);
}

__int32_t load(struct l_list** list, const char* filename)
{
    struct l_list* count = NULL, * start = NULL;
    __int32_t value;
     
    FILE* f = fileOpen(filename, "r");
    if (f == NULL) return 0;

    for(;;)
    {
        fscanf(f, "%d", &value);

        if (fileEOF(f)) break;

        if (errno || ferror(f)) 
        {
            fileClose(f);
            return 0;
        }

        count = list_add_after(count, value);

        if (start == NULL) start = count;
    }

    *list = start;
    fileClose(f);

    if (errno) return 0;
    return 1;

}

__int32_t serialize(struct l_list* list, const char* filename)
{
    struct l_list* count;

    FILE* f = fileOpen(filename, "rb");
    if (f == NULL) return 0;

    for (count = list; count != NULL; count = count->next) 
    {
        fwrite(&count->value, sizeof(__int32_t), 1, f);

        if (errno || ferror(f)) 
        {
            fileClose(f);
            return 0;
        }
    }

    fileClose(f);

    if (errno) return 0;
    return 1;

}

__int32_t deserialize(struct l_list** list, const char* filename)
{
    struct l_list* count = NULL, * start = NULL;
    __int32_t value;

    FILE* f = fileOpen(filename, "r");
    if (f == NULL) return 0;

    for(;;)
    {
        fread(&value, sizeof(__int32_t), 1, f);

        if (fileEOF(f)) break;

        if (errno || ferror(f))
        {
            fileClose(f);
            return 0;
        }

        count = list_add_after(count, value);

        if (start == NULL) start = count;
    }

    *list = start;
    fileClose(f);

    if (errno) return 0;
    return 1;
}




