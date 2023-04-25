#!/bin/bash

yum install -y cmake python3 python3-devel python3-pip7
yum groupinstall -y "Development Tools"


# Build wheels for Python 3.6, 3.7, 3.8, 3.9 (adjust the list as needed)
for PYTHON in cp36-cp36 cp37-cp37 cp38-cp38 cp39-cp39 cp310-cp310 cp311-cp311; do
    /opt/python/${PYTHON}/bin/pip install -r requirements.txt
    /opt/python/${PYTHON}/bin/python setup.py bdist_wheel
done


# Repair the generated wheels to make them manylinux-compliant
for whl in dist/*.whl; do
    auditwheel repair "$whl" -w dist/manylinux/
done
