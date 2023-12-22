# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import datetime
current_year = datetime.datetime.now().year

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Mosiwi Wiki Subproject'
copyright = '2022 - {}, Shenzhen Mosiwi Technology Co., Ltd'.format(current_year)
author = 'Mosiwi'
release = 'v1.0'


# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration
# https://myst-parser.readthedocs.io/en/latest/
extensions = [
    # https://myst-parser.readthedocs.io/en/latest/intro.html
    "myst_parser",         
    "sphinx_design",
    "sphinx_copybutton",   # Add a copy button to your code blocks
    # "sphinx_tippy",      # Add tooltips to your documentation
]

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}
source_parsers = {'.md': 'recommonmark.parser.CommonMarkParser'}

# https://myst-parser.readthedocs.io/en/latest/syntax/optional.html#syntax-extensions
myst_enable_extensions = [
    "colon_fence",
    "dollarmath",        # Displays mathematical formulas
]

templates_path = ['_templates']
exclude_patterns = []

# Language to be used for generating the HTML full-text search index.
# Sphinx supports the following languages:
#   'da', 'de', 'en', 'es', 'fi', 'fr', 'hu', 'it', 'ja'
#   'nl', 'no', 'pt', 'ro', 'ru', 'sv', 'tr', 'zh'
#
html_search_language = 'en'


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
# html_theme = "sphinx_book_theme"     # Another nice theme     

html_static_path = ['_static']
# set font
html_css_files = [
    '_static/css_style/my_css-style.css',
]

html_logo = '_static/web_logo/logo_mosiwi.png'
html_favicon = '_static/web_logo/my_favicon.png'
html_show_sphinx = False


# -- Options for EPUB output
epub_show_urls = 'footnote'

