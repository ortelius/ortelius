#!/bin/sh

OUT=`find $1 -printf "'%p' => {#'size' => '%s',#'path' => '%p',#'shortpath' => '',#'shortname' => '',#'drive' => ''#},#"`
OUT=`echo $OUT | sed "s|#'$1/|'|g" | sed "s|'$1'|'.'|"`
echo "{"
echo $OUT | sed "s/,#$//" | tr "#" "\n"
echo "}"
