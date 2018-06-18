/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option_l.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/14 15:13:32 by aguillot          #+#    #+#             */
/*   Updated: 2018/06/14 15:13:34 by aguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ls.h"

void	get_longest(args_node *elem, longest *longest)
{
	if (ft_nblen(elem->hardlinks) > longest->hardlinks)
		longest->hardlinks = ft_nblen(elem->hardlinks);
	if (ft_strlen(elem->uid) > longest->uid)
		longest->uid = ft_strlen(elem->uid);
	if (ft_strlen(elem->gid) > longest->gid)
		longest->gid = ft_strlen(elem->gid);
	if (ft_nblen(elem->size) > longest->size)
		longest->size = ft_nblen(elem->size);
}

void	get_perm(args_node *elem)
{
	char		*path;
	struct stat sb;
	struct stat sb2;

	if (elem->path == NULL)
			path = ft_strdup(elem->content);
		else
			path = ft_strdup(elem->path);
	if (stat(path, &sb) == -1 || lstat(path, &sb2) == -1)
		return;
	if (S_ISLNK(sb2.st_mode) == 1)
	{
		elem->perm[0] = 'l';
		sb = sb2;
	}
	if (S_ISDIR(sb.st_mode) == 1)
		elem->perm[0] = 'd';
	else if (S_ISREG(sb.st_mode) == 1)
		elem->perm[0] = '-';
	else if (S_ISCHR(sb.st_mode) == 1)
		elem->perm[0] = 'c';
	else if (S_ISBLK(sb.st_mode) == 1)
		elem->perm[0] = 'b';
	else if (S_ISFIFO(sb.st_mode) ==1)
		elem->perm[0] = 'p';
	else if (S_ISLNK(sb.st_mode) == 1)
		elem->perm[0] = 'l';
	else if (S_ISSOCK(sb.st_mode) == 1)
		elem->perm[0] = 's';
	elem->perm[1] = (sb.st_mode & S_IRUSR) ? 'r' : '-';
	elem->perm[2] = (sb.st_mode & S_IWUSR) ? 'w' : '-';
	elem->perm[3] = (sb.st_mode & S_IXUSR) ? 'x' : '-';
	elem->perm[4] = (sb.st_mode & S_IRGRP) ? 'r' : '-';
	elem->perm[5] = (sb.st_mode & S_IWGRP) ? 'w' : '-';
	elem->perm[6] = (sb.st_mode & S_IXGRP) ? 'x' : '-';
	elem->perm[7] = (sb.st_mode & S_IROTH) ? 'r' : '-';
	elem->perm[8] = (sb.st_mode & S_IWOTH) ? 'w' : '-';
	elem->perm[9] = (sb.st_mode & S_IXOTH) ? 'x' : '-';
	free(path);
}

void	get_correct_date(args_node *elem)
{
	char 	*correct_date;
	int 	i;
	int		j;

	i = 4;
	j = 0;
	correct_date = (char*)malloc(sizeof(char) * (13));
	while (i < 16)
	{
		correct_date[j++] = elem->mtimefull[i++];
	}
	correct_date[j] = '\0';
	free(elem->mtimefull);
	elem->mtimefull = ft_strdup(correct_date);
	free(correct_date);
}

void get_infos(args_node *elem)
{
	char			*path;
	struct stat 	sb;
	struct stat 	sb2;
	struct passwd 	*p;
	struct group 	*p2;

	if (elem->path == NULL)
			path = ft_strdup(elem->content);
		else
			path = ft_strdup(elem->path);
	if (stat(path, &sb) == -1 || lstat(path, &sb2) == -1)
		return;
	if (S_ISLNK(sb2.st_mode) == 1)
		sb = sb2;
	if ((p = getpwuid(sb.st_uid)) != NULL)
		elem->uid = ft_strdup(p->pw_name);
	if ((p2 = getgrgid(sb.st_gid)) != NULL)
		elem->gid = ft_strdup(p2->gr_name);
	elem->hardlinks = sb.st_nlink;
	elem->mtimefull = ft_strdup(ft_strtrim(ctime(&sb.st_mtime)));
	get_correct_date(elem);
	elem->size = sb.st_size;
	get_perm(elem);	
	free(path);
}

void	option_l(S_list *list)
{
	args_node 	*elem;
	longest 	longest;

	elem = list->head;
	longest.hardlinks = 0;
	longest.uid = 0;
	longest.gid = 0;
	longest.size = 0;
	while (elem)
	{
		get_infos(elem);
		get_longest(elem, &longest);
		//printf("LINKS: %zu, UID: %zu, GID: %zu, SIZE: %zu\n", longest.hardlinks, longest.uid, longest.gid, longest.size);
		elem = elem->next;
	}
	//printf("FIN\n");
	print_list_l(list, &longest);
}