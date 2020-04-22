#include <u.h>
#include <libc.h>
#include <String.h>
#include <bio.h>

#include "autism.h"

s32
fdcp(s32 fd, s32 ofd, u32 bs, s64 bn, s64 * bw)
{
	u8 *buf;
	s32 n;

	if(bn == 0) {
		werrstr("can't write 0 bytes");
		return 1;
	}

	if(bs == 0)
		if((bs = iounit(fd)) <= 0)
			bs = 8192;
	if(bn > 0 && bs > bn)
		bs = bn;

	if((buf = malloc(bs)) == nil)
		return 1;

	if(bw != nil)
		*bw = 0;
	while((n = read(fd, buf, bs)) > 0) {
		if(write(ofd, buf, n) != n) {
			n = -1;
			break;
		}
		if(bw != nil)
			*bw += n;
		if(bn > 0)
			bn -= n;

		if(bn == 0) {
			n = 0;
			break;
		}

		if(bn > 0 && bn < bs)
			bs = bn;
	}

	free(buf);
	return n;
}
