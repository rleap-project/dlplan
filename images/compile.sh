#!/bin/bash

pdflatex thumbnail.tex
convert -density 300 thumbnail.pdf -background white -flatten -resize 1000x500 thumbnail.png
