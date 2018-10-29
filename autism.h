#pragma src /sys/src/libautism
#pragma lib "libautism.a"

extern	int	erfork(int);
extern	void*	emalloc(ulong);
extern	char*	esmprint(char*, ...);
extern	char*	estrdup(char*);
#pragma	varargck argpos esmprint 1

extern	int	eopen(char*, int);
extern	vlong	eseek(int, vlong, int);

extern	double	eatof(char*);
extern	int	eatoi(char*);
extern	long	eatol(char*);
extern	vlong	eatoll(char*);
/* extern	double	echarstod(int(*)(void*), void*);
 * not bothering with charstod */
extern	double	estrtod(char*, char**);
extern	long	estrtol(char*, char**, int);
extern	ulong	estrtoul(char*, char**, int);
extern	vlong	estrtoll(char*, char**, int);
extern	uvlong	estrtoull(char*, char**, int);

extern	void*	fget(int, ulong, int*);
extern	long	fdcp(int, int, ulong, vlong, vlong*);
