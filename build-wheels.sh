#!/bin/bash

# Compile wheels
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install -r /io/dev-requirements.txt
    "${PYBIN}/pip" wheel /io/ --no-deps -w wheelhouse/
done

# Repair the generated wheels to make them manylinux-compliant
for whl in dist/*.whl; do
    auditwheel repair "$whl" -w dist/manylinux/
done
