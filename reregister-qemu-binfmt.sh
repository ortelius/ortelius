#!/bin/bash
# (c) Artur.Klauser@computer.org
#
# This script tries to reregister QEMU's binfmt_misc handlers with the
# fix-binary (F) flag in order to be usable with 'docker buildx' to build
# multi-architecture images.
# For more information see:
# https://nexus.eddiesinentropy.net/2020/01/12/Building-Multi-architecture-Docker-Images-With-Buildx/

function remove_binfmt() {
  local arch="$1"; shift
  local package="$1"; shift
  update-binfmts \
    --package "${package}" \
    --remove "qemu-${arch}" "/usr/bin/qemu-${arch}-static"
}

function install_binfmt() {
  local arch="$1"; shift
  local package="$1"; shift
  local interpreter="$1"; shift
  local offset="$1"; shift
  local magic="$1"; shift
  local mask="$1"; shift
  update-binfmts \
    --package "${package}" \
    --install "qemu-${arch}" "${interpreter}" \
    --offset "${offset}" \
    --magic "${magic}" \
    --mask "${mask}" \
    --credentials yes \
    --fix-binary yes
}

function reregister_qemu_binfmt() {
  # Reregister all qemu interpreters with fix-binary flag which makes them
  # available inside containers and chroots, e.g. docker buildx works for
  # architectures supported by qemu interpreters.
  for file in /proc/sys/fs/binfmt_misc/qemu-*; do
    arch="${file/*qemu-/}"
    package="$(head -1 "/var/lib/binfmts/qemu-${arch}")"
    # Pull arguments from current registration.
    eval $(awk ' \
      /^(interpreter|offset|magic|mask)/ {printf "%s=\"%s\"\n",$1,$2}' \
      "${file}")
    # Convert to binary strings.
    magic="$(echo $magic | sed 's/\(..\)/\\x\1/g')"
    mask="$(echo $mask | sed 's/\(..\)/\\x\1/g')"
    echo "Reregistering arch $arch"
    remove_binfmt "${arch}" "${package}"
    install_binfmt "${arch}" "${package}" "${interpreter}" \
      "${offset}" "${magic}" "${mask}"
  done
}

set -e
reregister_qemu_binfmt "$@"