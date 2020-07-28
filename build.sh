#!/bin/bash

DIR=$(dirname $0)
SYS=$1
TRG=$2
LIB=lib
SRC=src

CORE=$SRC/core
CMDZ=$SRC/cmdz
OPTZ=$SRC/optz

cleanUp()
{
  echo "cleaning up..."
  if [ -e .tmp.hpp ]; then
    rm -rf .tmp.hpp
  fi
}

if [ "$SYS" = "" ]; then
  echo "Usage: $0 package-manager [bin-target]"
  exit 1
fi

trap cleanUp EXIT

if [ "$TRG" = "" ]; then
	TRG="kewl"
fi

VER=$(cat VERSION)
if [ "$VER" = "" ]; then
  echo cant read version
	exit 1
fi

echo "checking libraries..."
if [ ! -f $LIB/$SYS.hpp  ]; then
  echo unsupported libraries
	exit 1
fi

if [ -e .tmp.hpp ]; then
	if ! rm -Rf .tmp.hpp; then
		exit 1
	fi
fi

touch .tmp.hpp
if [ $? != 0 ]; then
	exit 1
fi

echo "collecting source code..."

echo "namespace basis {" >> .tmp.hpp
echo "#include \"$SRC/basis/cursed.cpp\"" >> .tmp.hpp
echo "}" >> .tmp.hpp

HPP=$(< $CORE/.INCLUDE)
echo "namespace core {" >> .tmp.hpp
for i in $HPP; do
	cat $CORE/$i.hpp >> .tmp.hpp
done
for i in $HPP; do
	for j in $(ls -r $CORE/$i); do
		echo "#include \"$CORE/$i/$j\"" >> .tmp.hpp
	done
done
echo "}" >> .tmp.hpp

echo "namespace cmdz {" >> .tmp.hpp
for i in $(ls -r $CMDZ); do
	echo "#include \"$CMDZ/$i\"" >> .tmp.hpp
done
echo "}" >> .tmp.hpp
echo "namespace optz {" >> .tmp.hpp
for i in $(ls -r $OPTZ); do
	echo "#include \"$OPTZ/$i\"" >> .tmp.hpp
done
echo "}" >> .tmp.hpp

echo "compiling..."

g++ -std=c++11 \
    $SRC/main.cpp \
    -DVERSION=$VER \
    -lncursesw \
    -lpthread \
    -lssl \
    -lcrypto \
    -L/usr/lib \
    -include $LIB/$SYS.hpp \
    -include .tmp.hpp \
    -o $TRG

exit $?
