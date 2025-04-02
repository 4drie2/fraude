/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 02:58:11 by david             #+#    #+#             */
/*   Updated: 2025/01/15 03:00:07 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

long	handle_conversion(const char **str,
	int base, int sign, char **endptr)
{
	long	result;
	int		digit;

	result = 0;
	while (**str)
	{
		digit = ft_convert_digit(**str);
		if (digit == -1 || digit >= base)
			break ;
		if (result > (LONG_MAX - digit) / base)
		{
			result = ft_handle_overflow(result, digit, base, sign);
			while (**str && ft_convert_digit(**str) != -1
				&& ft_convert_digit(**str) < base)
				(*str)++;
			break ;
		}
		result = result * base + digit;
		(*str)++;
	}
	if (endptr)
		*endptr = (char *)*str;
	return (result * sign);
}

int	determine_base(const char **str, int base)
{
	if (base == 0)
	{
		if (**str == '0' && ((*str)[1] == 'x' || (*str)[1] == 'X'))
		{
			*str += 2;
			return (16);
		}
		else if (**str == '0')
			return (8);
		else
			return (10);
	}
	else if (base == 16 && **str == '0'
		&& ((*str)[1] == 'x' || (*str)[1] == 'X'))
		*str += 2;
	return (base);
}
