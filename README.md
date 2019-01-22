# Book Solution Pack

## Introduction

Allows for the creation of book and page objects (islandora:bookCModel,
islandora:pageCModel). Books are essentially a collection of pages, containing
only metadata and optionally a PDF file of the book. A book's PDF can be
generated from each page. Pages are based on an uploaded tiff of the page. From
the uploaded TIFF its possible to generate images for use in the "Islandora
Internet Archive Bookreader". It is also possible to generate PDF files per
page. OCR and OCR coordinate data can also be generated from the uploaded TIFF.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/discoverygarden/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Paged
Content](https://github.com/discoverygarden/islandora_paged_content)
* [Islandora Large Image Solution
Pack](https://github.com/discoverygarden/islandora_solution_pack_large_image)

## Installation

Install as
[usual](https://www.drupal.org/docs/8/extending-drupal-8/installing-drupal-8-modules).

## Configuration

Select configuration options for page derivatives, Parent Solr Field, and select
a viewer for the book object and page objects in Configuration » Islandora »
Solution pack configuration (admin/config/islandora/solution_pack_config/book).

![Configuration](https://camo.githubusercontent.com/e913af25f82dd8ff640dd11b337f64b5a9dea62f/687474703a2f2f692e696d6775722e636f6d2f3749434a66655a2e706e67)

## Documentation

Further documentation for this module is available at [our
wiki](https://wiki.duraspace.org/display/ISLANDORA/Book+Solution+Pack).

## Troubleshooting/Issues

Having problems or solved one? Create an issue, check out the Islandora Google
groups.

* [Users](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Devs](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

or contact [discoverygarden](http://support.discoverygarden.ca).

## Maintainers/Sponsors

Current maintainers:

* [discoverygarden](http://www.discoverygarden.ca)

## Development

If you would like to contribute to this module, please check out the helpful
[Documentation](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers),
[Developers](http://islandora.ca/developers) section on Islandora.ca and create
an issue, pull request and or contact
[discoverygarden](http://support.discoverygarden.ca).

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
