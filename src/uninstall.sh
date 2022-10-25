#!/usr/bin/env bash
# Based on https://habr.com/ru/post/590021/

set -Eeuo pipefail
trap cleanup SIGINT SIGTERM ERR EXIT

PROG_NAME="3DViewer_v1.0"
EXE_FILE="viewer"

INSTALL_DIR="${HOME}/tmp/${PROG_NAME}"
script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)

usage() {
  cat <<EOF
Usage: $(basename "${BASH_SOURCE[0]}") [-h] [-v] [-f] [-t target_dir]

Uninstall ${PROG_NAME}

Available options:

-h, --help      Print this help and exit
-v, --verbose   Print script debug info
-f, --from      Installation directory (Default: ${INSTALL_DIR})
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
    -f | --from) # install dir
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

if [ -d "${INSTALL_DIR}" ]; then
    echo "Uninstalling from ${INSTALL_DIR}..."
  if [ -f "${INSTALL_DIR}/${EXE_FILE}" ]; then
    rm -f "${INSTALL_DIR}/${EXE_FILE}"
    rm -f ${INSTALL_DIR}/*.glade
    rm -f ${INSTALL_DIR}/*.conf
    rm -f ${INSTALL_DIR}/*.pdf

    rmdir "${INSTALL_DIR}"
    # If installation dir contains some customly added files, it will NOT be deleted

  else
    echo "No file '${EXE_FILE}' in directory '${INSTALL_DIR}'!"
    exit 1
  fi
else
  echo "Directory '${INSTALL_DIR}' does not exist!"
  exit 1
fi

echo "Uninstalled from'${INSTALL_DIR}'"
