USEMARCON
=========

USEMARCON is a rule-based MARC record manipulation program and library. It is free, open source, written in C++. USEMARCON can be used interactively with USEMARCON GUI (see below), in batch processing using the command line version and it's also possible to integrate the library with other software (this has been done e.g. with [YAZ Proxy](http://www.indexdata.com/yazproxy)).

USEMARCON has native support for UTF-8. It can also convert MARC-8 to UTF-8 and handle other character set conversions with conversion tables.

USEMARCON can read and write ISO2709 (the MARC exchange format), MARCXML and MarcXchange (input and output formats independently).

Compiled binaries, example rulesets, the USEMARCON GUI and other information can be found at the [USEMARCON page of the National Library of Finland](http://www.nationallibrary.fi/libraries/format/usemarcon.html).

[The USEMARCON page at the British Library](http://www.bl.uk/bibliographic/usemarcon.html) contains more information on USEMARCON and the background on the project.


License
-------

USEMARCON is provided under a fairly liberal license. Please see the LICENSE file in the distribution for details.


Files
-----

CHANGES - change history

BUILDING - instructions for building the library and executable

LICENSE - the license


Directories
-----------

library - the sources of the library

program - a command line program that uses the library

documentation - documentation files

win - Windows build files

win32dll - Windows 32bit dll build files

uni2uk - sample barebones conversion ruleset
