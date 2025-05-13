# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os

project = 'Inventory System'
copyright = '2025, Rafael Correa'
author = 'Rafael Correa'
release = '2.3.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    "sphinx_rtd_dark_mode",
    "sphinxcontrib.googleanalytics",
    # "myst_parser",
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

googleanalytics_id = "G-TBJYX8K1T0"
myst_heading_anchors = 3

default_dark_mode = False

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']
