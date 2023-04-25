#!/bin/bash
set -e -u -x

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        auditwheel repair "$wheel" --plat "$PLAT" -w /io/wheelhouse/
    fi
}

# Install a system package required by our library
yum install -y cmake

# Compile wheels
for PYTHON in cp39-cp39; do
    # Install python packages
    # "${PYBIN}/pip" install pybind11 pybind11-global
    "/opt/python/${PYTHON}/bin/pip" install -r /io/requirements.txt
    "/opt/python/${PYTHON}/bin/pip" wheel /io/ --no-deps -w wheelhouse/
done

# Bundle external shared libraries into the wheels
for whl in wheelhouse/*.whl; do
    repair_wheel "$whl"
done

# Install packages and test
# for PYTHON in cp36-cp36 cp37-cp37 cp38-cp38 cp39-cp39 cp310-cp310 cp311-cp311; do
#     "/opt/python/${PYTHON}/bin/pip" install python-manylinux-demo --no-index -f /io/wheelhouse
#     (cd "$HOME"; "${PYBIN}/nosetests" pymanylinuxdemo)
# done
