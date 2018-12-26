</$objtype/mkfile

P=autism

LIB=lib$P.$O.a
OFILES=\
	efunc.$O \
	round.$O \
	fdcp.$O \
	fget.$O \


HFILES=/sys/include/$P.h
</sys/src/cmd/mksyslib

install:V:	$LIB
	cp $LIB /$objtype/lib/lib$P.a
	cp $P.h /sys/include/$P.h

uninstall:V:
	rm -f /$objtype/lib/lib$P.a /sys/include/$P.h

$O.%:	%.$O $OFILES $LIB $TESTLIB
	$LD $LDFLAGS -o $target $prereq
