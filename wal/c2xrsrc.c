#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define HOME "HOME"
#define WAL ".cache/wal"
#define CFILE "colors-wal-dwm.h"
#define XRSRC "colors.Xresources"

#define SEP "/"
#define SEPLEN 1

#define arrlen(arr) (sizeof(arr) / sizeof(*(arr)))

/* remember static memory is 0 initialized */
unsigned char *userhome;
unsigned char *walcache;
unsigned char *cpath;
unsigned char *xpath;

/*
 * working with library instead of mmaping or straight open/read/write/close
 * Why not mmap:
 *	files are rather small and way below pagesize (in my machine 4Kb) and default M_MMAP_THRESHOLD (128*1024 bytes)
 * Why not open/read/write/close:
 *	many calls to straight syscall wrappers without any buffering or stuff should be a bit slower than libc implementation which
 *	already takes care of a lot of that. I guess I could try doing some of that on my own but meh
 */
#define FILEBUFSIZE 1024
unsigned char filebuf[FILEBUFSIZE];

#define NUMOFATTR 6 /* amount of attributes expected */
#define CFVARLEN 31 /* as for the C standard */
#define XATTRLEN 63 /* arbitrary choice of length */
#define HEXCOLLEN 7 /* only reading #ffffff and alike, not including alpha in hex */

struct c2xrsrc {
	/* allocated sizes are the respective lengths + 1 because of null terminator */
	unsigned char varname[CFVARLEN + 1];
	unsigned char xattr[XATTRLEN + 1];
	unsigned char hexcolor[HEXCOLLEN + 1];
};
#define C2SIZE sizeof(struct c2xrsrc)

struct c2xrsrc *c2ptrs[NUMOFATTR];

unsigned char *xattributes[NUMOFATTR] = {
	"dwm.normfgcolor", "dwm.normbgcolor", "dwm.normbordercolor",
	"dwm.selfgcolor", "dwm.selbgcolor", "dwm.selbordercolor",
};
/*
 * yes i know size is not needed when defined like this.
 * but is makes a nice documentation in this case i think
 */
unsigned char *varnames[NUMOFATTR] = {
	"norm_fg", "norm_bg", "norm_border",
	"sel_fg", "sel_bg", "sel_border",
};


/*
 * frees memory and stuff before exiting
 * @retcode:	program exit code
 */
void cleanup(int retcode)
{
	int i;

	if (walcache != NULL)
		free(walcache);
	if(cpath != NULL)
		free(cpath);
	if(xpath != NULL)
		free(xpath);

	for (i = 0; i < NUMOFATTR; i++) {
		if (c2ptrs[i] != NULL)
			free(c2ptrs[i]);
	}

	exit(retcode);
}

static void xfile_append(void)
{
	FILE* xhandle;
	int i;
	xhandle = fopen(xpath, "a");
	if (xhandle == NULL)
		goto failxopen;

	for (i = 0; i < NUMOFATTR; i++) {
		memset(filebuf, 0, FILEBUFSIZE);

		strncpy(filebuf, c2ptrs[i]->xattr, FILEBUFSIZE - strlen(filebuf) - 3);
		filebuf[strlen(filebuf)] = ':';
		strncpy(filebuf + strlen(filebuf), c2ptrs[i]->hexcolor, FILEBUFSIZE - strlen(filebuf) - 2);
		filebuf[strlen(filebuf)] = '\n';

		fwrite(filebuf, sizeof(unsigned char), strlen(filebuf), xhandle);
	}
	return;
failxappend:
	fclose(xhandle);
failxopen:
	cleanup(1);
}

/*
 * returns the address of the next hex color of the form #ffffff in a buffer
 * or NULL if it is not found
 * @buf:	pointer to the beggining of the bufer were the search must start
 * @limit:	pointer to the last address reachable by this function (presumably end of buffer addr)
 */
static unsigned char *getnexthex(unsigned char *buf, unsigned char *limit)
{
	unsigned char *ptr;
	unsigned char c;
	unsigned int i;

	ptr = buf;
	while ((ptr = strchr(ptr, '#')) != NULL) {
		for (i = 1; ptr + i < limit && i < HEXCOLLEN; i++) {
			c = ptr[i];
			if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
				continue;
			ptr++; /* skip current '#' and continue the search */
			break;
		}
		if (i == HEXCOLLEN)
			break; /* found a hex color */
	}
	return ptr;
}

/*
 * read CFILE and fill c2xrsrc structs hexcolor field along the way
 * if failure occurs then cleanup is called
 * expects filebuf to be 0ed
 * notice this expects variables to be initialized with the value to extract
 * so definition and post assignment would break this search
 * Luckily CFILE is OK with these constraints.
 */
static void cfile_extract(void)
{
	FILE *chandle;
	unsigned char *ptr;
	unsigned char *hex;
	unsigned int rb;
	int i;

	chandle = fopen(cpath, "r");
	if (chandle == NULL)
		goto failcopen;

	/* ensure null byte at the end of filebuf */
	rb = fread(filebuf, sizeof(unsigned char), FILEBUFSIZE - 1, chandle);
	if (feof(chandle) == 0 && rb == FILEBUFSIZE - 1)
		goto failextract; /* file did not fit into filebuf */

	for (i = 0; i < NUMOFATTR; i++) {
		ptr = strstr(filebuf, c2ptrs[i]->varname);
		if (ptr == NULL)
			goto failextract;

		hex = getnexthex(ptr, filebuf + FILEBUFSIZE);
		if (hex == NULL)
			goto failextract;

		strncpy(c2ptrs[i]->hexcolor, hex, HEXCOLLEN);
	}

	fclose(chandle);
	return;
failextract:
	fclose(chandle);
failcopen:
	cleanup(1);
}

/*
 * allocates memory for a c2xrsrc struct and sets known values.
 * returns a pointer to the obtained memory for the struct or NULL if failure occurs
 * @var:	a NON NULL pointer to a string containing the variable name in the Cfile
 * @attr:	a NON NULL pointer to a string containing the expected variable name in the XRSRC file
 */
struct c2xrsrc *init_c2(unsigned char *var, unsigned char *attr)
{
	struct c2xrsrc *c2;
	c2 = malloc(C2SIZE);
	if (c2 == NULL)
		goto outc2init;
	memset(c2, 0, C2SIZE);
	strncpy(c2->varname, var, CFVARLEN);
	strncpy(c2->xattr, attr, XATTRLEN);
outc2init:
	return c2;
}

/*
 * initializes struct pointers in c2ptrs array
 * does not return but will call cleanup() if failure occurs
 */
void init_c2ptrs(void)
{
	int i;
	unsigned int xsize;
	unsigned int namessize;

	namessize = arrlen(varnames);
	xsize = arrlen(xattributes);

	if (xsize != namessize || xsize != NUMOFATTR)
		goto failc2ptrs;

	for (i = 0; i < NUMOFATTR; i++) {
		c2ptrs[i] = init_c2(varnames[i], xattributes[i]);
		if (c2ptrs[i] == NULL)
			goto failc2ptrs;
	}

	return;
failc2ptrs:
	cleanup(1);
}

/*
 * checks if path is valid
 * @path:	an unsigned char pointer to the variable containing the path string
 */
#define pathexists(path)		\
	access(path, F_OK)

/*
 * assumes space allocated suffices
 * @src:	a pointer to source string in concatenation
 * @dst:	a pointer to destination string in concatenation
 */
static unsigned char *pathcat(unsigned char *dst, unsigned char *src)
{
	if (strlen(dst) != 0) {
		dst[strlen(dst) + SEPLEN] = '\0';
		dst[strlen(dst)] = '/';
	}
	strncat(dst, src, strlen(src));
	return dst;
}

/*
 * allocates space for resulting string including '/' and '\0'.
 * returns the number of bytes allocated in dst.
 * @src:	a NON NULL pointer to source string in concatenation
 * @dst:	a pointer to a unsigned char array. may be NULL or not
 */
static int pathalloc(unsigned char **dst, unsigned char *src)
{
	unsigned int size;
	unsigned int dstlen;

	dstlen = 0;
	size = 1; /* 1 byte always allocated for '\0' */
	size += strlen(src);

	if (*dst != NULL) {
		dstlen = strlen(*dst);
		size += dstlen;
		size++; /* another byte allocated for '/' */
	}

	*dst = realloc(*dst, size); /* if *dst == NULL then it's just like malloc */
	if (*dst == NULL)
		return 0;
	memset(*dst + dstlen, 0, size - dstlen); /* 0 out newly allocated memory */

	return size;
}

/*
 * wrapper around path building functions
 * returns a pointer to the resulting path string
 * @prefix:	an unsigned char pointer to the first part of the path
 * @suffix:	an unsigned char pointer to a string to be appended to the first part
 */
static unsigned char* buildpath(unsigned char *prefix, unsigned char *suffix)
{
	int size;
	if (suffix == NULL)
		goto failedbuild; /* why building a path if the part to be appended is NULL */

	size = pathalloc(&prefix, suffix);
	if (!size)
		goto failedbuild;

	pathcat(prefix, suffix);
	if (size - 1 != strlen(prefix))
		goto failedbuild;

	if (!pathexists(prefix) == -1)
		goto failedbuild;

	return prefix;
failedbuild:
	cleanup(1);
}


int main()
{
	unsigned int ret;

	ret = 0;
	userhome = getenv(HOME);
	if (userhome == NULL) {
		ret = 1;
		goto out;
	}

	walcache = buildpath(walcache, userhome);
	walcache = buildpath(walcache, WAL);

	cpath = buildpath(cpath, walcache);
	cpath = buildpath(cpath, CFILE);

	xpath = buildpath(xpath, walcache);
	xpath = buildpath(xpath, XRSRC);

	init_c2ptrs();

	cfile_extract();

	xfile_append();
out:
	cleanup(ret);
}
