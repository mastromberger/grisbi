/* ************************************************************************** */
/*                                                                            */
/*                                  gsb_real                                  */
/*                                                                            */
/*     Copyright (C)    2000-2007 Cédric Auger (cedric@grisbi.org)            */
/*          2003-2008 Benjamin Drieu (bdrieu@april.org)	                      */
/*                      2009 Pierre Biava (grisbi@pierre.biava.name)          */
/*                      2009 Mickaël Remars (grisbi@remars.com)               */
/*          http://www.grisbi.org                                             */
/*                                                                            */
/*  This program is free software; you can redistribute it and/or modify      */
/*  it under the terms of the GNU General Public License as published by      */
/*  the Free Software Foundation; either version 2 of the License, or         */
/*  (at your option) any later version.                                       */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                              */
/*                                                                            */
/*  You should have received a copy of the GNU General Public License         */
/*  along with this program; if not, write to the Free Software               */
/*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                            */
/* ************************************************************************** */

/**
 * \file gsb_real.c
 * grisbi use a special structure to describe a real number
 * all is defined her
 */


#include "include.h"
#include <assert.h>

/*START_INCLUDE*/
#include "gsb_real.h"
#include "./gsb_data_currency.h"
/*END_INCLUDE*/

gsb_real null_real = { 0 , 0 };
gsb_real error_real = { 0x80000000, 0 };

glong gsb_real_power_10[] = { 1, 10, 100, 1000, 10000, 100000,
                            1000000, 10000000, 100000000, 1000000000 };

#define sizeofarray(x) (sizeof(x)/sizeof(*x))

#ifdef _MSC_VER
typedef struct _lldiv_t
{
	long long 	quot;
	long long 	rem;
} lldiv_t;

lldiv_t lldiv(long long numerator, long long denominator)
{
	//TODO find a standard/efficient impl for this
	lldiv_t result;
	result.quot = numerator / denominator;
	result.rem = numerator % denominator;
	return result;
}
#define lrint(x) (floor(x + 0.5))
#define rint(x) (floor(x + 0.5))
#endif//_MSC_VER

/*START_STATIC*/
static gchar *gsb_real_format_string ( gsb_real number,
                        gint currency_number,
                        gboolean show_symbol );
static gsb_real gsb_real_get_from_string_normalized ( const gchar *string, gint default_exponent );
static gboolean gsb_real_grow_exponent( gsb_real *num, guint target_exponent );
static void gsb_real_minimize_exponent ( gsb_real *num );
static void gsb_real_raw_minimize_exponent ( gint64 *mantissa, gint *exponent );
static gboolean gsb_real_raw_truncate_number ( gint64 *mantissa, gint *exponent );
static gdouble gsb_real_real_to_double ( gsb_real number );
/*END_STATIC*/

/*START_EXTERN*/
/*END_EXTERN*/


/**
 * Return the real in a formatted string, according to the currency 
 * regarding decimal separator, thousands separator and positive or
 * negative sign.
 * this is directly the number coded in the real wich is returned
 * usually, gsb_real_get_string_with_currency is better to adapt the format
 * 	of the number to the currency format
 * 
 * \param number	Number to format.
 *
 * \return		A newly allocated string of the number (this
 *			function will never return NULL) 
*/
gchar *gsb_real_get_string ( gsb_real number )
{
    return gsb_real_format_string ( number, 0, FALSE );
}

/**
 * format a gsb_real into a string from gsb_real_get_string
 *	adapt the format of the real to the currency (nb digits after , ...)
 *	show if asked the currency symbol
 *
 * \param number		A number to format.
 * \param currency_number	Currency to use.
 *
 * \return a newly allocated string of the number
 * */
gchar *gsb_real_get_string_with_currency ( gsb_real number,
                        gint currency_number,
                        gboolean show_symbol )
{
    gchar *string;

    string = gsb_real_format_string (number,
				     currency_number,
				     show_symbol);
    return string;
}


/**
 * Return the real in a formatted string with an optional currency
 * symbol, according to the given locale regarding decimal separator,
 * thousands separator and positive or negative sign.
 * 
 * \param number		    Number to format.
 * \param conv      		the locale obtained with localeconv(), or built manually
 * \param currency_symbol 	the currency symbol
 *
 * \return		A newly allocated string of the number (this
 *			function will never return NULL) 
 */
gchar *gsb_real_raw_format_string (gsb_real number,
                        struct lconv *conv,
                        const gchar *currency_symbol )
{
    gchar format[40];
    gchar *result = NULL;
	const gchar *cs_start;
    const gchar *cs_start_space;
    const gchar *sign;
    const gchar *mon_decimal_point;
    const gchar *cs_end_space;
    const gchar *cs_end;
	ldiv_t units;

	if ( (number.exponent < 0)
    || (number.exponent >= sizeofarray ( gsb_real_power_10 ) )
    || (number.mantissa == error_real.mantissa) )
      return g_strdup ( "###ERR###" );

    cs_start = (currency_symbol && conv->p_cs_precedes) ? currency_symbol : "";
    cs_start_space = (currency_symbol && conv->p_cs_precedes && conv->p_sep_by_space) ? " " : "";
    sign = (number.mantissa < 0) ? conv->negative_sign : conv->positive_sign;
    mon_decimal_point = conv->mon_decimal_point && *conv->mon_decimal_point ? conv->mon_decimal_point : ".";
    cs_end_space = (currency_symbol && !conv->p_cs_precedes && conv->p_sep_by_space) ? " " : "";
    cs_end = (currency_symbol && !conv->p_cs_precedes) ? currency_symbol : "";

    units = ldiv ( labs (number.mantissa), gsb_real_power_10[number.exponent] );
    if ( units.quot < 1000 )
    {
        g_snprintf (format, sizeof(format), "%s%d%s",
                                           "%s%s%s%d%s%0",
                                           number.exponent,
                                           "d%s%s" );
        result = g_strdup_printf ( format, 
                        cs_start,
                        cs_start_space,
                        sign,
                        units.quot,
                        mon_decimal_point,
                        units.rem,
                        cs_end_space,
                        cs_end );
    }
    else
    {
        gchar *mon_thousands_sep = g_locale_to_utf8 ( conv->mon_thousands_sep, -1,
                                                        NULL, NULL, NULL );
        div_t thousands = div ( units.quot, 1000 );
        if ( thousands.quot < 1000 )
        {
            g_snprintf ( format, sizeof (format), "%s%d%s",
                                                "%s%s%s%d%s%03d%s%0",
                                                number.exponent,
                                                "d%s%s");
            result = g_strdup_printf ( format, 
                            cs_start,
                            cs_start_space,
                            sign,
                            thousands.quot,
                            mon_thousands_sep,
                            thousands.rem,
                            mon_decimal_point,
                            units.rem,
                            cs_end_space,
                            cs_end );
        }
        else
        {
            div_t millions = div ( thousands.quot, 1000 );
            if ( millions.quot < 1000 )
            {
                g_snprintf ( format, sizeof (format), "%s%d%s",
                                                    "%s%s%s%d%s%03d%s%03d%s%0",
                                                    number.exponent,
                                                    "d%s%s");
                result = g_strdup_printf ( format, 
                                cs_start,
                                cs_start_space,
                                sign,
                                millions.quot,
                                mon_thousands_sep,
                                millions.rem,
                                mon_thousands_sep,
                                thousands.rem,
                                mon_decimal_point,
                                units.rem,
                                cs_end_space,
                                cs_end);
            }
            else
            {
                div_t billions = div ( millions.quot, 1000 );
                g_snprintf ( format, sizeof (format), "%s%d%s",
                                                    "%s%s%s%d%s%03d%s%03d%s%03d%s%0",
                                                    number.exponent,
                                                    "d%s%s" );
                result = g_strdup_printf ( format, 
                                cs_start,
                                cs_start_space,
                                sign,
                                billions.quot,
                                mon_thousands_sep,
                                billions.rem,
                                mon_thousands_sep,
                                millions.rem,
                                mon_thousands_sep,
                                thousands.rem,
                                mon_decimal_point,
                                units.rem,
                                cs_end_space,
                                cs_end );
            }
        }
        g_free ( mon_thousands_sep );
    }
    return result;
}


/**
 * Return the real in a formatted string with an optional currency
 * symbol, according to the locale regarding decimal separator,
 * thousands separator and positive or negative sign.
 * 
 * \param number		Number to format.
 * \param currency_number 	the currency we want to adapt the number, 0 for no adaptation
 * \param show_symbol 		TRUE to add the currency symbol in the string
 *
 * \return		A newly allocated string of the number (this
 *			function will never return NULL) 
 */
gchar *gsb_real_format_string ( gsb_real number,
                        gint currency_number,
                        gboolean show_symbol )
{
    struct lconv * conv = localeconv ( );
    gint floating_point;

    const gchar *currency_symbol = (currency_number && show_symbol)
                                   ? gsb_data_currency_get_code_or_isocode (currency_number)
                                   : NULL;

    /* First of all if number = 0 I return 0 with the symbol of the currency if necessary */
    if (number.mantissa == 0)
    {
        if (currency_symbol && conv -> p_cs_precedes)
            return g_strdup_printf ( "%s %s", currency_symbol, "0" );
        else if (currency_symbol && ! conv -> p_cs_precedes)
            return g_strdup_printf ( "%s %s", "0", currency_symbol );
        else
            return g_strdup ("0");
    }

    /* first we need to adapt the exponent to the currency */
    /* if the exponent of the real is not the same of the currency, need to adapt it */
    floating_point = gsb_data_currency_get_floating_point ( currency_number );
    if ( currency_number && number.exponent != floating_point )
        number = gsb_real_adjust_exponent ( number, floating_point );

    return gsb_real_raw_format_string ( number, localeconv(), currency_symbol );
}


/**
 * get a real number from a string
 * the string can be formatted :
 * - handle , or . as separator
 * - spaces are ignored
 * - another character makes a 0 return
 *
 *   there is no ask for any exponent, so the gsb_real will be exactly the
 *   same as the string
 *
 * \param string
 *
 * \return the number in the string transformed to gsb_real
 * */
gsb_real gsb_real_get_from_string ( const gchar *string )
{
    return gsb_real_get_from_string_normalized ( string, -1 );
}



/**
 * get a real number from an integer
 *
 * \param mantissa
 * \param exponent -1 for no limit
 *
 * \return a gsb_real from the integer
 */
gsb_real gsb_real_new ( gint mantissa, gint exponent )
{
    gsb_real number = null_real;
    number.mantissa = mantissa;
    number.exponent = exponent;
    return number;
}


/**
 * get a gsb_real number from a string
 * the string can be formatted :
 * - spaces and the given utf8-encoded thousands separators are ignored
 * - handle ",", "." and the given utf8-encoded decimal separator
 * - another character makes a error_real return
 *
 * \param string
 * \param mon_thousands_sep, can be NULL or empty, but only one utf8 sequence
 * \param mon_decimal_point, can be NULL or empty, but only one utf8 sequence
 *
 * \return the number in the string transformed to gsb_real
 */
gsb_real gsb_real_raw_get_from_string ( const gchar *string,
                                        const gchar *mon_thousands_sep,
                                        const gchar *mon_decimal_point )
{
    static const gchar *space_chars = " ";
    static const gchar *decimal_chars = ".,";
    static const gchar *positive_chars = "+";
    static const gchar *negative_chars = "-";

	unsigned mts_len;
	unsigned mdp_len;
    unsigned nb_digits = 0;
    gint64 mantissa = 0;
    gint8 sign = 0;
    gint8 dot_position = -1;
    const gchar *p = string;

    assert ( !mon_thousands_sep || ( g_utf8_strlen ( mon_thousands_sep, -1 ) <= 1 ) );
    assert ( !mon_decimal_point || ( g_utf8_strlen ( mon_decimal_point, -1 ) <= 1 ) );

    if ( !string)
        return error_real;

    mts_len = mon_thousands_sep
                       ? strlen ( mon_thousands_sep )
                       : 0;
    mdp_len = mon_decimal_point ? strlen ( mon_decimal_point ) : 0;

    for ( ; ; )
    {
        if ( g_ascii_isdigit ( *p ) )
        {
            mantissa *= 10;
            mantissa += ( *p - '0' );
            if ( mantissa > G_MAXLONG )
                return error_real;
            if ( sign == 0 ) sign = 1; // no sign found yet ==> positive
            ++nb_digits;
            ++p;
        }
        else if ( *p == 0 ) // terminal zero
        {
			gsb_real result;
			result.mantissa = sign * mantissa;
            result.exponent = ( dot_position >= 0 )
                              ? nb_digits - dot_position
                              : 0;
            return result;
        }
        else if ( strchr ( space_chars, *p )
             || ( mts_len && ( strncmp ( p, mon_thousands_sep, mts_len ) == 0 ) ) )
        {
            // just skip spaces and thousands separators
            p = g_utf8_find_next_char ( p, NULL );
        }
        else if ( strchr ( decimal_chars, *p )
             || ( mdp_len && ( strncmp ( p, mon_decimal_point, mdp_len ) == 0 ) ) )
        {
            if ( dot_position >= 0 ) // already found a decimal separator
                return error_real;
            dot_position = nb_digits;
            p = g_utf8_find_next_char ( p, NULL );
        }
        else if ( strchr ( negative_chars, *p ) )
        {
            if ( sign != 0 ) // sign already set
                return error_real;
            sign = -1;
            ++p;
        }
        else if ( strchr ( positive_chars, *p ) )
        {
            if ( sign != 0 ) // sign already set
                return error_real;
            sign = 1;
            ++p;
        }
        else // unknown char ==> error
        {
            return error_real;
        }
    }
}


/**
 * get a real number from a string
 * the string can be formatted :
 * - handle , or . as separator
 * - spaces are ignored
 * - another character makes a 0 return
 *
 *   the gsb_real will have the exponent given in default_exponent, except if default_exponent = -1
 *
 * \param string
 * \param default_exponent -1 for no limit
 *
 * \return the number in the string transformed to gsb_real
 */
gsb_real gsb_real_get_from_string_normalized ( const gchar *string, gint default_exponent )
{
    struct lconv *conv = localeconv ( );
    gchar *mon_thousands_sep = g_locale_to_utf8 (
                             conv->mon_thousands_sep, -1, NULL, NULL, NULL );
    gchar *mon_decimal_point = g_locale_to_utf8 (
                             conv->mon_decimal_point, -1, NULL, NULL, NULL );
    gsb_real result =  gsb_real_raw_get_from_string ( string, mon_thousands_sep, mon_decimal_point );
    g_free ( mon_thousands_sep );
    g_free ( mon_decimal_point );
    return result;
}


/**
 * compare 2 gsb_real and return the result (-1, 0, 1)
 *
 * \param number_1
 * \param number_2
 *
 * \return -1 if number_1 < number_2 ; 0 if number_1 = number_2 ; 1 if number_1 > number_2
 * */
G_MODULE_EXPORT gint gsb_real_cmp ( gsb_real number_1,
                        gsb_real number_2 )
{
    gsb_real_normalize ( &number_1,
			 &number_2 );
    if (number_1.mantissa < number_2.mantissa)
	return -1;
    if (number_1.mantissa == number_2.mantissa)
	return 0;

    return 1;
}

/**
 * reduce the exponent to its smallest possible value,
 * without losing any precision
 *
 * \param mantissa a pointer to the 64 bits mantissa to be reduced
 * \param exponent a pointer to the exponent to be reduced
 **/
void gsb_real_raw_minimize_exponent ( gint64 *mantissa, gint *exponent )
{
    while ( *exponent > 0 )
    {
        lldiv_t d = lldiv ( *mantissa, 10 );
        if ( d.rem != 0 )
            return;
        *mantissa = d.quot;
        --*exponent;
    }
}

/**
 * reduce the exponent to its smallest possible value,
 * without losing any precision
 *
 * \param num a pointer to the number to be reduced
 **/
void gsb_real_minimize_exponent ( gsb_real *num )
{
    gint64 mantissa = num->mantissa;

    gsb_real_raw_minimize_exponent ( &mantissa, &num->exponent);
    num->mantissa = mantissa;
}

/**
 * grow the exponent up to target_exponent
 * (only if possible without losing precision)
 *
 * \param num a pointer to the number
 * \param target_exponent the desired exponent
 **/
gboolean gsb_real_grow_exponent( gsb_real *num, guint target_exponent )
{
    gint64 mantissa = num->mantissa;
    gint exponent = num->exponent;
    gboolean succes = TRUE;

    while ( exponent < target_exponent )
    {
        gint64 new_mantissa = mantissa * 10;
        if ( ( new_mantissa > G_MAXLONG ) || ( new_mantissa < G_MINLONG ) )
        {
            succes = FALSE;
            break;
        }
        mantissa = new_mantissa;
        ++exponent;
    }
    num->mantissa = mantissa;
    num->exponent = exponent;

    return succes;
}

/**
 * normalize the 2 numbers to be able to add/substract/compare them later
 * for that transform the 2 numbers to have the same exponent
 * and after that we can work on the mantissa
 *
 * \param number_1 a pointer to gsb_real wich contains the number_1 to transform
 * \param number_2 a pointer to gsb_real wich contains the number_2 to transform
 *
 * \return TRUE if normalization occured
 * FALSE if exponents can't be the same without loss of precision
 * */
gboolean gsb_real_normalize ( gsb_real *number_1, gsb_real *number_2 )
{
    gsb_real_minimize_exponent ( number_1 );
    gsb_real_minimize_exponent ( number_2 );

    if ( number_1->exponent < number_2->exponent )
    {
        if ( !gsb_real_grow_exponent ( number_1, number_2->exponent ) )
		{
			while ( number_2 -> exponent > number_1 -> exponent )
            {
                number_2 -> exponent--;
                number_2 -> mantissa = number_2 -> mantissa / 10;
            }
		}
    }
    else if ( number_2->exponent < number_1->exponent )
    {
        if ( !gsb_real_grow_exponent ( number_2, number_1->exponent ) )
		{
            while ( number_1 -> exponent > number_2 -> exponent )
            {
                number_1 -> exponent--;
                number_1 -> mantissa = number_1 -> mantissa / 10;
            }
		}
    }

    return ( number_1->exponent == number_2->exponent );
}


/**
 * give the absolute value of the number
 *
 * \param number
 *
 * \return a gsb_real, the absolute value of the given number
 * */
gsb_real gsb_real_abs ( gsb_real number )
{
    number.mantissa = labs (number.mantissa);
    return number;
}


/**
 * modify the number to adjust the exponent wanted
 *
 * \param number
 * \param return_exponent the exponent we want to have for the returned number, or -1 if no limit
 *
 * \return the transformed number
 * */
gsb_real gsb_real_adjust_exponent ( gsb_real number,
                        gint return_exponent )
{
    gdouble tmp;

    if (return_exponent == -1)
	return number;

    tmp = number.mantissa;

    while ( number.exponent != return_exponent )
    {
	if (number.exponent < return_exponent)
	{
	    tmp = tmp * 10;
	    number.exponent++;
	}
	else
	{
	    tmp = tmp / 10;
	    number.exponent--;
	}
    }
    number.mantissa = lrint (tmp);
    return number;
}



/**
 * add 2 gsb_real
 * when an overflow occurs, error_real is returned
 *
 * \param number_1
 * \param number_2
 *
 * \return a gsb_real = number_1 + number_2, or error_real when an error occured
 * */
G_MODULE_EXPORT gsb_real gsb_real_add ( gsb_real number_1,
                        gsb_real number_2 )
{
    gint64 mantissa;

    if ( ( number_1.mantissa == error_real.mantissa )
      || ( number_2.mantissa == error_real.mantissa )
      || !gsb_real_normalize ( &number_1, &number_2 ) )
        return error_real;

	mantissa = ( gint64 ) number_1.mantissa + number_2.mantissa;
    if ( ( mantissa > G_MAXLONG ) || ( mantissa < G_MINLONG ) )
    {
        if ( ! gsb_real_raw_truncate_number (&mantissa, &number_1.exponent ) )
            return error_real;
    }
    number_1.mantissa = mantissa;

    return number_1;
}

/**
 * substract between 2 gsb_real : number_1 - number_2
 *
 * \param number_1
 * \param number_2
 *
 * \return a gsb_real = number_1 - number_2
 * */
gsb_real gsb_real_sub ( gsb_real number_1,
                        gsb_real number_2 )
{
    number_2.mantissa = -number_2.mantissa;
    return gsb_real_add ( number_1, number_2 );
}


/**
 * return the opposite of the number
 * ie 5 returns -5 in gsb_real number
 *
 * \param number
 *
 * \return its opposite
 * */
G_MODULE_EXPORT gsb_real gsb_real_opposite ( gsb_real number )
{
    number.mantissa = -number.mantissa;
    return number;
}


/**
 * multiply 2 gsb_reals. no overflow possible
 *
 * \param number_1
 * \param number_2
 *
 * \return the multiplication between the 2
 * */
gsb_real gsb_real_mul ( gsb_real number_1,
                        gsb_real number_2 )
{
    gint64 mantissa;

    mantissa = ( gint64 ) number_1.mantissa * number_2.mantissa;
    number_1.exponent += number_2.exponent;

    gsb_real_raw_minimize_exponent ( &mantissa, &number_1.exponent );

    if ( ( mantissa > G_MAXLONG ) || ( mantissa < G_MINLONG ) )
    {
        if ( ! gsb_real_raw_truncate_number (&mantissa, &number_1.exponent ) )
            return error_real;
    }
    number_1.mantissa = mantissa;

    return number_1;
}


/**
 * divide 2 gsb_reals
 *
 * \param number_1
 * \param number_2
 *
 * \return the div between the 2
 * */
gsb_real gsb_real_div ( gsb_real number_1,
                        gsb_real number_2 )
{
    gsb_real number;

    number = gsb_real_double_to_real ( gsb_real_real_to_double (number_1)
				       /
				       gsb_real_real_to_double (number_2));

    return number;
}


/**
 * convert a gsb_real to a double
 *
 * \param number a gsb_real number
 *
 * \return the number in double format
 * */
gdouble gsb_real_real_to_double ( gsb_real number )
{
    gdouble double_number;
    gint i;

    double_number = number.mantissa;
    for ( i=0 ; i<number.exponent ; i++ )
	double_number = double_number / 10;
    return double_number;
}


/**
 * convert a double to a gsb_real
 *
 * \param number a gdouble number
 *
 * \return the number in gsb_real format
 * */
G_MODULE_EXPORT gsb_real gsb_real_double_to_real ( gdouble number )
{
    gdouble tmp_double;
    gdouble maxlong;

    gsb_real real_number = null_real;
    maxlong = G_MAXLONG / 10;

    while (modf (number, &tmp_double))
    {
	number = number * 10;
	real_number.exponent++;

	if (fabs (number) > maxlong)
	    number = rint (number);
    }
    real_number.mantissa = number;
    return real_number;
}


/**
 * truncate the number. WARNING there loss of accuracy
 *
 * \param mantissa a pointer to the 64 bits mantissa to be truncate
 * \param exponent a pointer to the exponent to be truncate
 **/
gboolean gsb_real_raw_truncate_number ( gint64 *mantissa, gint *exponent )
{
    if ( *mantissa > G_MAXLONG )
    {
        do
        {
            --*exponent;
            *mantissa = *mantissa / 10;
        } while ( *mantissa > G_MAXLONG );
        return TRUE;
    }
    else if ( *mantissa < G_MINLONG )
    {
        do
        {
            --*exponent;
            *mantissa = *mantissa / 10;
        } while ( *mantissa < G_MINLONG );
        return TRUE;
    }

    return FALSE;
}


/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */

