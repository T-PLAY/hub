
# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))
from sphinx.builders.html import StandaloneHTMLBuilder
import subprocess, os

# Doxygen
subprocess.call('doxygen Doxyfile.in', shell=True)
#  subprocess.call('doxygen Doxyfile.out', shell=True)

# -- Project information -----------------------------------------------------
#  sys.path.append(os.path.abspath('xml'))


project = 'hub'
copyright = '2021-2024, Gauthier Bouyjou'
author = 'Gauthier BOUYJOU'

#  def get_git_hub_version() -> str:
    #  return subprocess.check_output(['git', 'describe', '--tags', '--abbrev=0']).decode('ascii').strip()

version = 'v2.0.0'
#  version = get_git_hub_version()

root_doc = 'api/index'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    'sphinx.ext.autosectionlabel',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx_sitemap',
    'sphinx.ext.inheritance_diagram',
    'breathe',
    'sphinx.ext.githubpages',
    'sphinx_rtd_theme',
    #  'sphinx_mdinclude'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

highlight_language = 'c++'

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'canonical_url': '',
    'analytics_id': '',  #  Provided by Google in your dashboard
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': False,
    
    'logo_only': False,

    # Toc options
    'collapse_navigation': True,
    'sticky_navigation': True,
    'navigation_depth': 6,
    'includehidden': True,
    'titles_only': False
}
html_show_sphinx = False
# html_logo = ''
# github_url = ''
#  html_baseurl = ''

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# -- Breathe configuration -------------------------------------------------

breathe_projects = {
    "hub": "_build/xml/"
    #  "hub": "/home/gauthier/tmpOpenSource/build-hub-Desktop-Debug/doc/sphinx/html/doxygen"
	#  "hub": "xml/"
}
breathe_default_project = "hub"
breathe_default_members = ('members', 'undoc-members')



#  # Configuration file for the Sphinx documentation builder.
#  #
#  # For the full list of built-in configuration values, see the documentation:
#  # https://www.sphinx-doc.org/en/master/usage/configuration.html
#
#  # -- Project information -----------------------------------------------------
#  # https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information
#
#  project = 'Hub'
#  copyright = '2023, me'
#  author = 'me'
#
#  # -- General configuration ---------------------------------------------------
#  # https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration
#
#  extensions = []
#
#  templates_path = ['_templates']
#  exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']
#
#
#
#  # -- Options for HTML output -------------------------------------------------
#  # https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output
#
#  #  html_theme = 'alabaster'
#  # The theme to use for HTML and HTML Help pages.  See the documentation for
#  # a list of builtin themes.
#  #
#  html_theme = 'sphinx_rtd_theme'
#  html_theme_options = {
#      'canonical_url': '',
#      'analytics_id': '',
#      'display_version': True,
#      'prev_next_buttons_location': 'bottom',
#      'style_external_links': False,
#
#      'logo_only': False,
#
#      # Toc options
#      'collapse_navigation': True,
#      'sticky_navigation': True,
#      'navigation_depth': 4,
#      'includehidden': True,
#      'titles_only': False
#  }
#  # html_logo = ''
#  # github_url = ''
#  # html_baseurl = ''
#  html_static_path = ['_static']
