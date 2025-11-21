# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'OConfigurator'
copyright = '2025, Léo Cluzel'
author = 'Léo Cluzel'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
	'sphinx.ext.graphviz',
	'sphinx.ext.autodoc',
	'breathe'
]
pygments_style = 'sphinx'

templates_path = []#['_templates']
exclude_patterns = []





# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = []#['_static']

# -- GraphViz configuration ----------------------------------
graphviz_output_format = 'svg'


breathe_projects = {
	"OConfigurator": "../build/doxygen/xml"
}

breathe_default_project = "OConfigurator"

primary_domain = 'cpp'

highlight_language = 'cpp'