</$objtype/mkfile

P=autism

LIB=lib$P.$O.a
OFILES=\
	efunc.$O \
	sfunc.$O \
	fdcp.$O \
	fget.$O \

HFILES=/sys/include/$P.h

UPDATE=\
	mkfile\
	$HFILES\
	${OFILES:%.$O=%.c}\
	${LIB:/$objtype/%=/386/%}\

</sys/src/cmd/mksyslib

install:V:	$LIB
	cp $LIB /$objtype/lib/lib$P.a
	cp $P.h /sys/include/$P.h

uninstall:V:
	rm -f /$objtype/lib/lib$P.a /sys/include/$P.h

clean:V:
	rm -f *.[$OS] [$OS].* $LIB

$O.%:	%.$O $OFILES $LIB $TESTLIB
	$LD $LDFLAGS -o $target $prereq
