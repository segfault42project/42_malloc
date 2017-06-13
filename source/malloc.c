#include "../include/malloc.h"

t_block	*g_block = NULL;
t_block	*ctrl = NULL;

/*void	*realloc(void *ptr, size_t size)*/
/*{*/
	/*(void)size;*/
	/*return (ptr);*/
/*}*/

void	free(void *addr)
{
	t_block	*tmp;
	size_t	DEBUG_i = 1;

	tmp = g_block;
	DEBUG_calling_free_message();
	DEBUG_address_to_free(addr);
	while (tmp)
	{
		/*RC;*/
		DEBUG_free_current_addr(tmp->ptr, DEBUG_i);
		if (tmp->ptr == addr)
		{
			/*ft_putstr(YELLOW"");*/
			/*print_hexa((unsigned long)tmp->ptr);*/
			/*ft_putendl("tmp->ptr = addr");*/
			tmp->free = 0;
			/*tmp->ptr = NULL;*/
			tmp->size = 0;
			tmp->flag = 0;
			break ;
		}
		tmp = tmp->next;
		/*ft_putendl("next");*/
		++DEBUG_i;
	}
	if (DEBUG_ADDRESS_TO_FREE == 1)
		ft_putendl(END"");
}

bool	alloc_ptr_node(size_t size, t_block *node)
{
	if (size <= TINY)
		node->ptr = mmap(0, TINY, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	else if (size <= SMALL)
		node->ptr = mmap(0, SMALL, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	else
		node->ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (node->ptr == MAP_FAILED)
	{
		DEBUG_show_ret_mmap((int)node->ptr);
		ft_putstr_fd("MMAP FAILED TO ALLOCATE PTR : ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (false);
	}
	if (size <= TINY)
		ft_memset(node->ptr, 0, TINY);
	else if (size <= SMALL)
		ft_memset(node->ptr, 0, SMALL);
	else
		ft_memset(node->ptr, 0, size);
	return (true);
}

void	*create_node()
{
	t_block *tmp;

	tmp = mmap(0, sizeof(t_block), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (tmp == MAP_FAILED)
	{
		DEBUG_show_ret_mmap((int)tmp->ptr);
		ft_putstr_fd("MMAP FAILED TO ALLOCATE NEW NODE : ",2);
		ft_putendl_fd(strerror(errno),2);
		return (NULL);
	}
	memset(tmp, 0, sizeof(t_block));
	return (tmp);
}

bool	lst_push_back(size_t size)
{
	t_block	*tmp;

	tmp = g_block;
	if (g_block == NULL)
	{
		ft_putendl("if");
		if ((g_block = create_node()) == NULL)
			return (false);
		if ((alloc_ptr_node(size, g_block)) == false)
			return (NULL);
		ctrl = g_block;
		DEBUG_print_node(g_block, 1);
		sleep(3);
	}
	else
	{
		ft_putendl("else");
		while (tmp->next)
			tmp = tmp->next;
		if ((tmp->next = create_node()) == NULL)
			return (false);
		if ((alloc_ptr_node(size, tmp->next)) == false)
			return (NULL);
	}
	return (true);
}

bool	check_list_filled()
{
	t_block	*tmp;

	tmp = g_block;
	while (tmp->next && tmp->free == 1)
		tmp = tmp->next;
	if (tmp->next == NULL)
		return (LST_FULL);
	return (LST_NOT_FULL);
}

void	*init_node(size_t size)
{
	t_block	*tmp;

	tmp = g_block;
	DEBUG_print_ctrl_g_block(ctrl, g_block);
	while (tmp->next && tmp->free == 1)
		tmp = tmp->next;
	tmp->free = 1;
	tmp->size = size;
	if (size <= TINY)
		tmp->flag = 't';
	else if (size <= SMALL)
		tmp->flag = 's';
	else
		tmp->flag = 'l';
	return (tmp->ptr);
}

void	*alloc_tiny_small(size_t size)
{
	int i = 0;

	if (g_block == NULL || check_list_filled() == LST_FULL)
	{
		while (i < NB_ZONES)
		{
			if (lst_push_back(size) == false)
				return (NULL);
			++i;
		}
	}
	return (init_node(size));
}


void	*malloc(size_t size)
{
	void	*alloc = NULL;
	size_t DEBUG_iter_malloc_call = 1;

	DEBUG_calling_malloc();
	/*if (size <= SMALL)*/
		alloc = alloc_tiny_small(size);
	/*else*/
		/*return (alloc_large(size));*/
	DEBUG_print_info_node(DEBUG_iter_malloc_call);
	return (alloc);
 }
