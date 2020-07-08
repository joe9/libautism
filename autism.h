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
	KiB = 1024 * sizeof(uchar),     /* kibi byte */
	MiB = KiB * KiB,        /* mebi byte */
};

/* below types from BurnZeZ's http://feline.systems/tmp/stype.h
Size of Boolean type is 1 byte(s)

Number of bits in a character: 8
Size of character types is 1 byte
Signed char min: -128 max: 127
Unsigned char min: 0 max: 255
Default char is unsigned

Size of short int types is 2 bytes
Signed short min: -32768 max: 32767
Unsigned short min: 0 max: 65535

Size of int types is 4 bytes
Signed int min: -2147483648 max: 2147483647
Unsigned int min: 0 max: 4294967295

Size of long int types is 4 bytes
Signed long min: -2147483648 max: 2147483647
Unsigned long min: 0 max: 4294967295

Size of long long types is 8 bytes
Signed long long min: -9223372036854775808 max: 9223372036854775807
Unsigned long long min: 0 max: 18446744073709551615
 */
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

extern s32 erfork(s32);
extern void *emalloc(u32);
extern s8 *esmprint(s8 *, ...);
extern s8 *estrdup(s8 *);

#pragma	varargck argpos esmprint 1

extern s32 eopen(s8 *, s32);
extern s64 eseek(s32, s64, s32);

extern double eatof(s8 *);
extern s32 eatoi(s8 *);
extern s32 eatol(s8 *);
extern s64 eatoll(s8 *);

/* extern	double	echarstod(int(*)(void*), void*);
 * not bothering with charstod */
extern double estrtod(s8 *, s8 **);
extern s32 estrtol(s8 *, s8 **, int);
extern u32 estrtoul(s8 *, s8 **, int);
extern s64 estrtoll(s8 *, s8 **, int);
extern u64 estrtoull(s8 *, s8 **, int);

extern void *fget(int, u32, int *);
extern s32 fdcp(int, int, u32, s64, s64 *);

/* these 2 from spew of #cat-v */
int esnprint(s8 *buf, int sz, const s8 *fmt, ...);
s8 *eseprint(s8 *buf, s8 *ebuf, const s8 *fmt, ...);

/* below utilities from acme/wiki/src/util.c */
s8 *estrstrdup(s8 *s, s8 *t);
s8 *eappend(s8 *s, s8 *sep, s8 *t);
s8 *egrow(s8 *s, s8 *sep, s8 *t);

void error(s8 *fmt, ...);
void warn(s8 *fmt, ...);

/* check grid/testdebug.c for details on why this is the best */
#define D if (debug == 0) {} else fprint

double round(double x);

/* from env.c */
s8 *readfile(s8*);
void setenv(s8*, s8*);
void cpenv(s8*, s8*);
