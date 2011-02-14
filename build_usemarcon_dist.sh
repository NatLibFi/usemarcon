#!/bin/csh

# A simple shell script for creating a distribution package

if ($#argv == 0) then
  echo "Usage: $0 usemarconxxx"
  exit 1
endif

set usemarcon=$argv[1]

echo "** Preparing the package"
if (! -e ${usemarcon}d.zip) then
  echo "${usemarcon}d.zip does not exist"
  exit 1
endif

if (-e $usemarcon) then
  echo "$usemarcon already exists"
  exit 1
endif

if (-e ${usemarcon}.zip) then
  echo "${usemarcon}.zip already exists"
  exit 1
endif
if (-e ${usemarcon}.tar.gz) then
  echo "${usemarcon}.tar.gz already exists"
  exit 1
endif

unzip ${usemarcon}d.zip

if (! -d ${usemarcon}) then
  echo "${usemarcon} directory does not exist"
  exit 1
endif

cd $usemarcon
chmod +x Makefile.am buildconf.sh config.guess config.sub configure install-sh missing mkinstalldirs usemarcon-config.in
./buildconf.sh
rm -rf autom4te.cache

cd pcre
chmod +x CleanTxt config.guess config.sub configure depcomp Detrail install-sh perltest.pl PrepareRelease RunGrepTest RunTest 132html
cd ..
cd ..

echo
echo "** Creating packages"
zip -r $usemarcon.zip $usemarcon
rm $usemarcon/usemarcon.exe
tar -czhf $usemarcon.tar.gz $usemarcon

echo
echo "** Testing build"
cd $usemarcon

cd pcre
./configure --enable-utf8 --enable-unicode-properties --disable-shared --disable-cpp
make
cd ..

./configure
make
cd ..

