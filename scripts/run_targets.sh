#! /bin/bash -e


ctest --test-dir tests -V

cmake --build . --target docs
chromium doc/docs_doxygen/html/index.html &

cmake --build . --target coverage
chromium coverage/index.html &
