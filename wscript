#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'bnb'
VERSION = '2.0.0'


def build(bld):

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_BNB_VERSION="{}"'.format(
            VERSION))

    bld(name='bnb_includes',
        includes='./src',
        export_includes='./src',
        use=['endian_includes', 'bitter_includes'])

    if bld.is_toplevel():

        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.recurse('test')
