import os,sys

# read sphinx conf.py file
from openalea.misc.sphinx_configuration import *
from openalea.misc.sphinx_tools import sphinx_check_version
from openalea.deploy.metainfo import read_metainfo, compulsary_words
from openalea.sconsx.util.config_deploy import HexVersion

sphinx_check_version()                      # check that sphinx version is recent

def getMetaInfo():
    metainfo = {}
    execfile(os.path.join(os.pardir, 'src','plantgl','__init__.py'),{},metainfo)
    return metainfo['__metainfo__']

globals().update(getMetaInfo())


def getPGLVersionString():
    p = os.path.join(os.pardir, 'src', 'cpp', 'plantgl', 'version.h')
    return HexVersion.from_cpp_define('PGL_VERSION',p).to_string()

version = getPGLVersionString()

# by product that need to be updated:
latex_documents = [('contents', 'main.tex', project + ' documentation', authors, 'manual')]

project = pkg_name

html_theme = "sphinx_rtd_theme"
html_theme_path = ["_themes"]
