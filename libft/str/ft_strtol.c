/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 19:11:36 by david             #+#    #+#             */
/*   Updated: 2025/01/15 03:08:08 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_convert_digit(char c)
{
	if (ft_isdigit(c))
		return (c - '0');
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
	return (-1);
}

long	ft_handle_overflow(long result, int digit, int base, int sign)
{
	if (sign == 1 && result > (LONG_MAX - digit) / base)
		return (LONG_MAX);
	if (sign == -1 && result > (-(LONG_MIN + digit)) / base)
		return (LONG_MIN);
	return (result * base + digit);
}

int	validate_base(int base, const char **str, char **endptr)
{
	if (base < 2 || base > 36)
	{
		if (endptr)
			*endptr = (char *)*str;
		return (0);
	}
	return (1);
}

void	skip_whitespace_and_sign(const char **str, int *sign)
{
	while (**str && ft_isspace(**str))
		(*str)++;
	if (**str == '+' || **str == '-')
	{
		if (**str == '-')
			*sign = -1;
		(*str)++;
	}
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	int	sign ;

	sign = 1;
	if (!validate_base(base, &str, endptr))
		return (0);
	skip_whitespace_and_sign(&str, &sign);
	base = determine_base(&str, base);
	return (handle_conversion(&str, base, sign, endptr));
}
