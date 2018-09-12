FROM alpine:3.8
ENTRYPOINT ["bin/usemarcon"]

COPY . build
WORKDIR usemarcon

RUN addgroup -S usemarcon \
  && adduser -S -h /usemarcon usemarcon usemarcon \
  && chown -R usemarcon:usemarcon /build \
  && apk add -U --no-cache gcc \
  && apk add --no-cache --virtual .build-deps \
    sudo autoconf perl automake libtool make g++ file \
  && sudo -u usemarcon sh -c 'cd /build/ && rm aclocal.m4 && aclocal \
    && ./buildconf.sh' \
  && sudo -u usemarcon sh -c 'cd /build/pcre && chmod +x configure depcomp \
      Detrail install-sh perltest.pl PrepareRelease RunGrepTest RunTest \
      132html && ./configure --enable-utf8 --enable-unicode-properties \
      --disable-shared --disable-cpp && make' \
  && sudo -u usemarcon sh -c 'cd /build && chmod +x configure install-sh \
        mkinstalldirs && ./configure --prefix=/usemarcon && \
         make' \
  && sh -c 'cd /build && make install' \
  && apk del .build-deps \
  && rm -rf /build tmp/* /var/cache/apk/*

USER usemarcon
