#ifndef __NM_LIST_H__
#define __NM_LIST_H__

#define container_of(ptr, type, member) \
		((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
		list_entry((ptr)->next, type, member)

#define NM_LIST_POISION1 ((void *)0x00100100)
#define NM_LIST_POISION2 ((void *)0x00200200)

struct list_head {
	struct list_head *next, *prev;
};

static inline void __list_add(struct list_head *new,
			      struct list_head *prev, struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NM_LIST_POISION1;
	entry->prev = NM_LIST_POISION2;
}
#endif
