#!/usr/bin/env bash
# Based on https://habr.com/ru/post/590021/

set -Eeuo pipefail
trap cleanup SIGINT SIGTERM ERR EXIT

PROG_NAME="3DViewer_v1.0"
EXE_FILE="viewer"
DOC_FILE="readme.pdf"

INSTALL_DIR="${HOME}/tmp/${PROG_NAME}"
script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)

usage() {
  cat <<EOF
Usage: $(basename "${BASH_SOURCE[0]}") [-h] [-v] [-f] [-t target_dir]

Install ${PROG_NAME}

Available options:

-h, --help      Print this help and exit
-v, --verbose   Print script debug info
-t, --to        Installation directory (Default: ${INSTALL_DIR})
EOF
  exit
}

cleanup() {
  trap - SIGINT SIGTERM ERR EXIT
  # script cleanup here
}

msg() {
  echo >&2 -e "${1-}"
}

die() {
  local msg=$1
  local code=${2-1} # default exit status 1
  msg "$msg"
  exit "$code"
}

parse_params() {

  while :; do
    case "${1-}" in
    -h | --help) usage ;;
    -v | --verbose) set -x ;;
    -t | --to) # install dir
      INSTALL_DIR="${2-}"
      shift
      ;;
    -?*) die "Unknown option: $1" ;;
    *) break ;;
    esac
    shift
  done

  return 0
}

parse_params "$@"

# script logic here

echo "Installation directory '${INSTALL_DIR}'"

mkdir -p "${INSTALL_DIR}"


cd "$script_dir"
cp "$EXE_FILE" "${INSTALL_DIR}/"
cp *.glade "${INSTALL_DIR}/"

if [ -f "${DOC_FILE}" ]; then
  cp "$DOC_FILE" "${INSTALL_DIR}/"
fi

echo "Installed to '${INSTALL_DIR}'"
# ls -lR "${INSTALL_DIR}"

