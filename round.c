#include <u.h>
#include <libc.h>
#include <String.h>
#include <bio.h>

#include "autism.h"

/*
07:28 < joe9> when n=410.00, and I do (unsigned long) of it, it is converting to 409 (probably to do with ieee). Any suggestions on how to correct that?
07:29 < joe9> this is with a C program native on 9front.
07:29 < joe9> print("toquote: n=%f => %lud,%f\n", ja->val->first->val->n, (unsigned long) (ja->val->first->val->n * 100), (ja->val->first->val->n * 100));
07:29 < joe9> n=4.100000 => 409,410.000000
07:30 < piroko> #define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
07:30 < piroko> something like that
07:32 < joe9> thanks.

09:52 < aiju> 14:30 < piroko> #define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
09:52 < aiju> NOOO
09:53 < aiju> this code is broken for x=0.4999something :)
09:54 < aiju> 14:28 < joe9> when n=410.00, and I do (unsigned long) of it, it is converting to 409 (probably to do with ieee). Any suggestions on how to correct that?
09:54 < aiju> 14:29 < joe9> this is with a C program native on 9front.
09:54 < aiju> 14:29 < joe9> print("toquote: n=%f => %lud,%f\n", ja->val->first->val->n, (unsigned long) (ja->val->first->val->n * 100), (ja->val->first->val->n * 100));
09:54 < aiju> that's because it's not 410.00
09:54 < aiju> it's probably more like 409.99999
09:55 < aiju> unfortunately, there is no round() function on plan 9 that does this right
09:55 < aiju> i thought mischief added it to APE but apparently not
10:01 < piroko> aiju: interesting. thanks for the correction
10:01 < piroko> I see that round() is more complicated to handle that
10:02 < joe9> aiju: what do you recommend for fixing such round() issues?
10:02 < joe9> aiju: i do not want to use round() from ape just for this.
10:04 < aiju> http://okturing.com/src/5613/body
#include <u.h>
#include <libc.h>

double
round(double x)
{
	double f, i;

	if(isNaN(x) || isInf(x, 1) || isInf(x, -1))
		return x;
	f = modf(x, &i);
	if(f <= -0.5)
		i--;
	else if(f >= 0.5)
		i++;
	return i;
}

10:04 < aiju> i believe this is correct
10:04 < aiju> it should probably be added to libc
10:04 < joe9> aiju, thanks.
*/

double
round(double x)
{
	double f, i;

	if(isNaN(x) || isInf(x, 1) || isInf(x, -1))
		return x;
	f = modf(x, &i);
	if(f <= -0.5)
		i--;
	else if(f >= 0.5)
		i++;
	return i;
}
