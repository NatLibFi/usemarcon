FROM alpine:3.7
ENV PATH="/usemarcon/bin:${PATH}"
CMD ["usemarcon"]

RUN apk update
RUN apk add autoconf make gcc g++ wget
RUN adduser -S -h /usemarcon usemarcon usemarcon

USER usemarcon

WORKDIR usemarcon
RUN mkdir build
WORKDIR build

RUN wget https://github.com/NatLibFi/usemarcon/archive/v3.17.tar.gz
RUN tar --strip-components 1 -xf v3.17.tar.gz
RUN rm v3.17.tar.gz

WORKDIR pcre

RUN chmod +x CleanTxt config.guess config.sub configure depcomp Detrail install-sh perltest.pl PrepareRelease RunGrepTest RunTest 132html
RUN ./configure --enable-utf8 --enable-unicode-properties --disable-shared --disable-cpp
RUN make

WORKDIR /usemarcon/build

RUN chmod +x Makefile.am buildconf.sh config.guess config.sub configure install-sh missing mkinstalldirs usemarcon-config.in
RUN ./configure --prefix=/usemarcon
RUN make
RUN make install

WORKDIR /usemarcon
RUN rm -r build
