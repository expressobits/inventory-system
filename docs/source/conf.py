# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os

project = 'Inventory System'
copyright = '2025, Expresso Bits'
author = 'Rafael Correa'
release = '2.3.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    "sphinx_rtd_dark_mode",
    # "sphinxcontrib.googleanalytics",
    # "sphinx_tabs.tabs",
    # "notfound.extension",
    # "sphinxext.opengraph",
    # "sphinx_copybutton",
    # "sphinxcontrib.video",
    # "sphinxemoji.sphinxemoji",
]

# if os.getenv("GITHUB_ACTIONS"):
#   extensions.append("sphinxcontrib.googleanalytics")
#   googleanalytics_id = "G-TBJYX8K1T0"

# googleanalytics_id = "G-TBJYX8K1T0"
myst_heading_anchors = 3

default_dark_mode = False

templates_path = ['_templates']
exclude_patterns = []


supported_languages = {
    "en": "Inventory System documentation in English",
    "pt_BR": "Documentação do Inventory System em Português Brasileiro",
}

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

locale_dirs = ['locale/']   # path is example but recommended.
gettext_compact = False     # optional.
