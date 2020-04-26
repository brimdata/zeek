#!/bin/bash
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

declare -a REPOS=(
    'https://github.com/salesforce/ja3.git'
    'https://github.com/salesforce/hassh.git'
)

for REPO in "${REPOS[@]}"; do
    TMPDIR=$(mktemp -d)
    ZSCRIPTS_DIR="$SCRIPT_DIR/../scripts/site"
    SCRIPTNAME="${REPO/\.git/}"
    SCRIPTNAME="${SCRIPTNAME/*\//}"
    git clone "$REPO" "$TMPDIR"
    for DIR in zeek bro scripts; do
        if [ -d "$TMPDIR/$DIR" ]; then
            mv "$TMPDIR/$DIR" "$ZSCRIPTS_DIR/$SCRIPTNAME"
        fi
    done

    # This commented-out stuff below are things I came up with when I was
    # hacking with other random Zeek scripts that were still "Bro"-ified and
    # had to be brought current to work with the newer Zeek version we use.
    # It's certainly our preference to get the upstream projects to
    # Zeek-ify for the good of the community before we depend on them, but I'm
    # leaving the dormant script code here just in case we ever want to add a
    # script desperately enough, as this will help us either bring in the
    # modified scripts as a one-off or to create changes we'd commit to our own
    # forked copy of a script repo.
    #
    # for BRO_FILE in $(find "$ZSCRIPTS_DIR" -name \*.bro); do
    #     mv -- "$BRO_FILE" "${BRO_FILE%.bro}.zeek"
    # done
    # for LOAD_SCRIPT in $(find "$ZSCRIPTS_DIR" -name __load__.zeek); do
    #     perl -i -pe "s/\.bro$/.zeek/" "$LOAD_SCRIPT"
    #     perl -i -pe "s/@load packages/@load site/" "$LOAD_SCRIPT"
    # done
    # for ZSCRIPT in $(find "$ZSCRIPTS_DIR" -name \*.zeek); do
    #     perl -i -pe "s/bro_init\(\)/zeek_init()/" "$ZSCRIPT"
    # done

    echo "@load ./$SCRIPTNAME" >> "$ZSCRIPTS_DIR/local.zeek"
done
