#pragma lib "libautism.a"

enum Bool {
	False = 0,
	True = 1
};

typedef enum Bool Bool;

/* got these codes from
   ~/dev/apps/plan9/plan9front/sys/src/cmd/postscript/posttek/posttek.h */
#define NUL '\000'
#define SOH '\001'
#define STX '\002'
#define ETX '\003'
#define EOT '\004'
#define ENQ '\005'
#define ACK '\006'
#define BEL '\007'
#define BS  '\010'
#define HT  '\011'
#define NL  '\012'
#define VT  '\013'
#define FF  '\014'
#define CR  '\015'
#define SO  '\016'
#define SI  '\017'
#define DLE '\020'
#define DC1 '\021'
#define DC2 '\022'
#define DC3 '\023'
#define DC4 '\024'
#define NAK '\025'
#define SYN '\026'
#define ETB '\027'
#define CAN '\030'
#define EM  '\031'
#define SUB '\032'
#define ESC '\033'
#define FS  '\034'
#define GS  '\035'
#define RS  '\036'
#define US  '\037'
#define DEL '\177'

enum {
	KiB = 1024 * sizeof(uchar), /* kibi byte */
	MiB = KiB * KiB, /* mebi byte */
};

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

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

/* these 2 from spew of #cat-v */
int	esnprint(char *buf, int sz, const char *fmt, ...);
char*	eseprint(char *buf, char *ebuf, const char *fmt, ...);

/* below utilities from acme/wiki/src/util.c */
char *estrstrdup(char *s, char *t);
char *eappend(char *s, char *sep, char *t);
char *egrow(char *s, char *sep, char *t);
void error(char *fmt, ...);

/* check grid/testdebug.c for details on why this is the best */
#define D if (debug == 0) {} else fprint
