/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julessainthorant <marvin@42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:58:03 by julessainthor     #+#    #+#             */
/*   Updated: 2024/05/24 11:32:54 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */    
#include <limits.h>
#include <stdio.h>
# include <unistd.h>
# include <stdio.h>
# include <stdarg.h>
//# define NORM_MACRO(x, is_ui_)	x < 0 && is_ui_ == 0 ? 1 : 0

typedef unsigned int			t_ui_;
typedef unsigned long long int	t_uli_;
typedef long long int			t_li_;

static inline int	ft_putchar_l(char i)
{
	write(1, &(i), 1);
	return (1);
}
static inline int	norm_macro(int x, int is)
{
	if (x < 0 && is == 0)
		return (1);
	return (0);
}

static int	ft_putstr_l(char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (s[i] != '\0')
	{
		write(1, &s[i], 1);
		i++;
	}
	return (i);
}

static int	ft_putnbr_base_l(t_li_ nbr, char *base, size_t is_unsigned)
{
	int	size_base;
	int	nbr_final[200];
	int	i;
	int	j;

	j = norm_macro(nbr, is_unsigned);
	i = 0;
	size_base = 0;
	if (nbr < 0 && is_unsigned == 0)
	{
		nbr = -nbr;
		ft_putchar_l('-');
	}
	while (base[size_base])
		size_base++;
	while (nbr)
	{
		nbr_final[i] = nbr % size_base;
		nbr = nbr / size_base;
		i++;
	}
	j += i;
	while (--i >= 0)
		ft_putchar_l(base[nbr_final[i]]);
	return (j);
}

static int	ft_put_ptr_l(t_uli_ i)
{
	t_uli_	hex;
	char	p[40];
	int		o;
	int		q;

	o = 0;
	if (i == 0)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	write(1, "0x", 2);
	while (i > 0)
	{
		hex = i % 16;
		i /= 16;
		if (hex > 9)
			p[o++] = (char)((hex - 10) + 'a');
		else
			p[o++] = (char)(hex + '0');
	}
	q = o;
	while (o-- > 0)
		write(1, &p[o], 1);
	return (q + 2);
}

static int	ft_arg(const char f_, void *arg)
{
	int	l;

	l = 0;
	if ((f_ == 'd' || f_ == 'i' || f_ == 'u' || f_ == 'x'
			|| f_ == 'X') && ((int)((size_t) arg)) == 0)
	{
		write(1, "0", 1);
		return (1);
	}
	if (f_ == 'c')
		l += ft_putchar_l((int)((size_t) arg));
	else if (f_ == 's')
		l += ft_putstr_l((char *) arg);
	else if (f_ == 'p')
		l += ft_put_ptr_l((t_uli_) arg);
	else if ((f_ == 'd' || f_ == 'i'))
		l += ft_putnbr_base_l((int)((size_t) arg), "0123456789", 0);
	else if (f_ == 'u')
		l += ft_putnbr_base_l((t_ui_)((size_t) arg), "0123456789", 1);
	else if (f_ == 'x')
		l += ft_putnbr_base_l((t_ui_)((size_t) arg), "0123456789abcdef", 1);
	else if (f_ == 'X')
		l += ft_putnbr_base_l((t_ui_)((size_t) arg), "0123456789ABCDEF", 1);
	else if (f_ == 'l')
		l += ft_putnbr_base_l((t_ui_)(( unsigned long) arg), "0123456789", 1);
	return (l);
}

int	ft_printf(const char *s, ...)
{
	int		i;
	int		ft_printf_len;
	va_list	args;

	i = 0;
	ft_printf_len = 0;
	va_start(args, s);
	while (s[i])
	{
		if (s[i] == '%' && s[i + 1] == 'l' && s[i + 2] == 'u')
		{
			ft_printf_len += ft_arg(s[i + 1], va_arg(args, void *));
			i += 2;
		}
		else if (s[i] == '%' && s[i + 1] != '%' && s[i + 1] != '\0')
		{
			ft_printf_len += ft_arg(s[i + 1], va_arg(args, void *));
			i++;
		}
		else
		{
			write(1, &s[i], 1);
			if (s[i + 1] == '%' && s[i] == '%')
				i++;
			ft_printf_len++;
		}
		i++;
	}
	va_end(args);
	return (ft_printf_len);
}