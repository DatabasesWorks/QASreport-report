CONFIG(debug, debug|release) {
    DEST_DIRECTORY = $$PWD/../build/debug
}
CONFIG(release, debug|release) {
    DEST_DIRECTORY = $$PWD/../build/release
}
