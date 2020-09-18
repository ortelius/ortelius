#!/bin/bash
# (c) Artur.Klauser@computer.org
#
# This script checks if all software requirements are met in a Linux environment
# in order to use 'docker buildx' to build multi-architecture images.
# For more information see:
# https://nexus.eddiesinentropy.net/2020/01/12/Building-Multi-architecture-Docker-Images-With-Buildx/

function error() {
  echo "ERROR: $*"
  exit 1
}

function ok() {
  echo "OK: $*"
}

function version() {
  printf '%02d' $(echo "$1" | tr . ' ' | sed -e 's/ 0*/ /g') 2>/dev/null
}

function check_qemu_binfmt() {
  # Docker
  if ! command -v docker >/dev/null 2>&1; then
    error "Can't find docker." \
	 "Install with 'sudo apt-get install docker-ce' or docker.io."
  fi
  docker_version="$(docker --version | cut -d' ' -f3 | tr -cd '0-9.')"
  if [[ "$(version "$docker_version")" < "$(version '19.03')" ]]; then
    error "docker $docker_version too old. Need >= 19.03"
  fi
  docker_experimental="$(docker version | \
                         awk '/^ *Experimental:/ {print $2 ; exit}')"
  if [[ "$docker_experimental" != 'true' ]]; then
    error "docker experimental flag not enabled:"\
          "Set with 'export DOCKER_CLI_EXPERIMENTAL=enabled'"
  else
    ok "docker $docker_version supports buildx experimental feature."
  fi

  # Kernel
  kernel_version="$(uname -r)"
  if [[ "$(version "$kernel_version")" < "$(version '4.8')" ]]; then
    error "Kernel $kernel_version too old - need >= 4.8." \
          " Install a newer kernel."
  else
    ok "kernel $kernel_version has binfmt_misc fix-binary (F) support."
  fi

  # binfmt_misc file system
  if [[ "$(mount | grep -c '/proc/sys/fs/binfmt_misc')" == '0' ]]; then
    error '/proc/sys/fs/binfmt_misc not mounted. Mount with' \
	  "'sudo mount -t binfmt_misc binfmt_misc /proc/sys/fs/binfmt_misc'"
  else
    ok "/proc/sys/fs/binfmt_misc is mounted"
  fi

  # binfmt-support
  if ! command -v update-binfmts >/dev/null 2>&1; then
    error "Can't find update-binfmts." \
	 "Install with 'sudo apt-get install binfmt-support'."
  fi
  binfmt_version="$(update-binfmts --version | awk '{print $NF}')"
  if [[ "$(version "$binfmt_version")" < "$(version '2.1.7')" ]]; then
    error "update-binfmts $binfmt_version too old. Need >= 2.1.7"
  else
    ok "update-binfmts $binfmt_version has fix-binary (F) support."
  fi

  # QEMU
  if [[ ! -e '/proc/sys/fs/binfmt_misc/qemu-aarch64' ]]; then
    # Skip this test if QEMU isn't registered with binfmt_misc. It might
    # come from a docker image rather than the host file system.
    if [[ ! -e '/usr/bin/qemu-aarch64-static' ]]; then
      error "Missing QEMU." \
            " Install with 'sudo apt-get install qemu-user-static'."
    else
      ok "QEMU installed"
    fi
  fi

  if [[ ! -e '/proc/sys/fs/binfmt_misc/qemu-aarch64' ]]; then
    error 'QEMU not registered in binfmt_misc.'
  fi
  flags="$(grep 'flags:' /proc/sys/fs/binfmt_misc/qemu-aarch64 | \
	   cut -d' ' -f2)"
  if [[ "$(echo "$flags" | grep -c F)" == '0' ]]; then
    error 'QEMU not registered in binfmt_misc with fix-binary (F) flag.'
  else
    ok "QEMU registered in binfmt_misc with flags $flags (F is required)."
  fi

  echo "Host looks good for docker buildx multi-architecture support".
}

set -e
check_qemu_binfmt "$@"