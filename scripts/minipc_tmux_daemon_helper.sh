#!/bin/bash
HELP=$(
  cat <<EOF
Usage: $(basename "$0") [options] {[i]nstall,[c]lean,[r]einstall}

  -h, --help            Show this message
  -t, --tmux-dir        Set tmux directory
  -s, --systemd-dir     Set user systemd directory
  -r, --systemd-req     Set user systemd required service
  -m, --minipc-dir      Set minipc script directory
  -u, --umask           Set umask
  -i, --interactive     Run interactively

The included scripts/system-setup-helper.sh which will setup dependancies and
cause the service to be started at boot.

After installation, run \`systemctl --user start ${SRV}\` to start the
minipc tmux daemon. To attach to the session, run \`tmux -L mtd a -t mtd\`. You
may want to add \`alias minipc='tmux -L mtd attach -t mtd'\` to your .*rc. To
kill the daemon, either stop the attached session manually or run
\`systemctl --user stop ${SRV}\`.

[Ctrl+c] within the session restarts the minipc script.

Do not remove this file or ${LOG}. If this file is moved, move the log
with it. Clean before reinstalling.

Default key bindings (Meta is likely Alt):
  M-f   -> send-prefix (begin command)
  :     -> command-prompt (tmux command prompt)
  z     -> kill-window
  d     -> detach
  r     -> source-file /path/to/${CNF}

Examples:
  To detach the connected session, press [Alt+f] then press [d].
  To kill the connected session, press [Alt+f] then press [z].

For some terminals, the meta key has to be configured to act as an escape key.
E.g. set 'XTerm*metaSendsEscape: true' for \`xterm\`
EOF
)
LOG=.mtdparam.log
SRV=minipctd.service
CNF=tmux-mtd.conf

script_name=$(basename "$0")
script_file=$(realpath "$0")
script_dir=$(realpath "${script_file}" | xargs dirname)
script_log=${script_dir}/${LOG}

CONF_DIR=${XDG_CONFIG_HOME:-$HOME/.config}
TMUX_DIR=${CONF_DIR}/tmux
SYSTEMD_DIR=${CONF_DIR}/systemd/user
SYSTEMD_REQ=irm-jetson.service
MINIPC_DIR=$(dirname "${script_dir}")
MINIPC_TARGET=python/main.py
UMASK_MODE=0022
user_tmux_dir=
user_systemd_dir=
user_systemd_req=
user_minipc_dir=
user_minipc_target=
user_umask_mode=

function printhelp() {
  echo "$HELP"
  exit ${1:-0}
}

function install() {
  interactive=${1/-/}
  if [ -f "${script_log}" ]; then
    echo "${script_log}" already exists.
    echo Run \`${script_name} clean\` before reinstalling.
    exit 1
  fi
  if [ ${interactive} ]; then
    echo Choose \`tmux\` configuration directory.
    echo -n [default=${TMUX_DIR}]:\ 
    read user_tmux_dir
    echo

    echo Choose user systemd directory.
    echo -n [default=${SYSTEMD_DIR}]:\ 
    read user_systemd_dir
    echo

    echo Choose systemd service requirement.
    echo -n [default=${SYSTEMD_REQ}]:\ 
    read user_systemd_req
    echo

    echo Choose minipc directory.
    echo -n [default=${MINIPC_DIR}]:\ 
    read user_minipc_dir
    echo

    echo Choose minipc script name.
    echo -n [default=${MINIPC_TARGET}]:\ 
    read user_minipc_target
    echo

    echo Choose umask.
    echo -n [default=${UMASK_MODE}]\ 
    read user_umask_mode
    echo
  fi

  echo Selected configuration:
  echo "TMUX_DIR:      ${user_tmux_dir:=$TMUX_DIR}"
  echo "SYSTEMD_DIR:   ${user_systemd_dir:=$SYSTEMD_DIR} (user)"
  echo "MINIPC_DIR:    ${user_minipc_dir:=$MINIPC_DIR}"
  echo "MINIPC_TARGET: ${user_minipc_target:=$MINIPC_TARGET}"
  echo "UMASK_MODE:    ${user_umask_mode:=$UMASK_MODE}"
  echo
  echo "SERVICE:       ${user_systemd_dir}/${SRV}"
  echo "SCRIPT:        ${script_file}"
  echo "LOG:           ${script_log}"
  echo

  if [ ${interactive} ]; then
    echo -n Continue? [,y/n]\ 
    read cont
    [ "$cont" == n ] && exit 0
    echo
  fi

  trap 'rm -rf "$tmpdir"' EXIT
  tmpdir=$(mktemp -d /tmp/smtd.XXXXX)
  # Files
  f_mtdparamlog=${script_log}
  f_tmuxconf=${user_tmux_dir}/${CNF}
  f_systemdservice=${user_systemd_dir}/${SRV}
  # Temporary files
  tf_mtdparamlog=${tmpdir}/"${f_mtdparamlog}"
  tf_tmuxconf=${tmpdir}/"${f_tmuxconf}"
  tf_systemdservice=${tmpdir}/"${f_systemdservice}"

  mkdir -p "${tf_tmuxconf%\/*}" && >"${tf_tmuxconf}"
  echo +++ Write tmux configuration to "${tf_tmuxconf}"
  cat <<EOF | tee "${tf_tmuxconf}"
# config for minipctd in detached tmux session
unbind-key -a

set -g escape-time 0
set -g mouse off

set -g visual-activity off
set -g visual-bell off
set -g visual-silence off
set -g monitor-activity off
set -g monitor-bell off

set -g prefix M-f
bind M-f send-prefix

bind : command-prompt
bind z kill-window
bind d detach
bind r source-file ${f_tmuxconf}
EOF
  echo
  echo

  mkdir -p "${tf_systemdservice%\/*}" && >"${tf_systemdservice}"
  echo +++ Write systemd user service to "${tf_systemdservice}"
  cat <<EOF | tee "${tf_systemdservice}"
[Unit]
Description=minipctd
Requires=${user_systemd_req}

[Service]
Type=forking
ExecStart=/bin/tmux -f "${f_tmuxconf}" -L mtd new-ses -s mtd -n minipctd -c "${user_minipc_dir}" -d -- "${script_file}" jobcontrol "${user_minipc_target}" --dummy --skip-tests
ExecStop=/usr/bin/tmux -L mtd kill-ses
ExecReload=/usr/bin/tmux -L mtd send-keys C-c

[Install]
WantedBy=multi-user.target
EOF
  echo
  echo

  mkdir -p "${tf_mtdparamlog%\/*}" && >"${tf_mtdparamlog}"
  echo +++ Log installed files and directories to "${tf_mtdparamlog}"
  # Files
  for f in ${!f_@}; do
    echo "${f}=${!f}" | tee -a "${tf_mtdparamlog}"
  done
  # Directories
  for d in ${!d_@}; do
    echo "${d}=${!d}" | tee -a "${tf_mtdparamlog}"
  done
  echo
  echo

  echo Created files:
  sed -e '/^[^f]/d' -e "s|^f_.*=\(.*\)|${tmpdir}/\1|" "${tf_mtdparamlog}" |
    xargs ls -ld
  echo

  if [ ${interactive} ]; then
    echo -n Copy to system? [,y/n]\ 
    read cont
    [ "$cont" ] && exit 0
    echo
  fi

  echo Copy to system \(umask ${user_umask_mode}\)
  function copy_to_system() (
    tmpdir=$1
    script_file=$2
    script_dir=$3
    script_log=$4
    umask_mode=$5
    umask "${umask_mode}"

    source "${tmpdir}/${script_log}"

    # Directories
    for d in ${!d_@}; do
      mkdir -d "${!d}"
    done
    # Files
    for f in ${!f_@}; do
      cat "${tmpdir}/${!f}" >"${!f}"
    done
  )
  copy_to_system "${tmpdir}" \
    "${script_file}" \
    "${script_dir}" \
    "${script_log}" \
    "${user_umask_mode}"
  cat "${script_log}" | cut -d= -f2 | xargs ls -l
  echo
  # Changes
  echo Reload systemd configuration with \`systemctl --user daemon-reload\`
  systemctl --user daemon-reload
  echo Enable ${SRV}
  systemctl --user enable "${SRV}"

  echo
  echo Done.
}

function clean() {
  interactive=${1/-/}
  reinstall=${2/-/}
  if [ ! -f "${script_log}" ]; then
    echo "${script_log}" does not exist.
    echo Nothing to be cleaned.
    [ ${reinstall} ] &&
      return 0 ||
      exit 0
  fi
  if [ ${interactive} ]; then
    cat "${script_log}"
    echo
    echo -n Remove these? [,y/n]\ 
    read cont
    [ "$cont" == n ] && exit 0
    echo
  fi

  source "${script_log}"
  # Files
  for f in ${!f_@}; do
    [ -f "${!f}" ] &&
      rm -fv "${!f}"
  done
  # Directories
  for d in ${!d_@}; do
    rm -rfv "${!d}"
  done
  echo Disable ${SRV}
  systemctl --user disable "${SRV}"

  echo
  echo Done.
}

function jobcontrol() {
  #echo $@
  echo Job started $(date)
  job_target=$1
  job_mode=${2:---dummy}
  shift 2
  returncode=$?
  # Restart if killed with C-c
  if [ ${returncode} -eq 130 ]; then
    deactivate
    clear
    jobcontrol "${job_target}" --display-menu "$@"
  else
    deactivate
    echo Not killed with C-c, restarting in 1s.
    sleep 1
    clear
    jobcontrol "${job_target}" --dummy "$@"
  fi
}

interactive=-
reinstall=-
function run() {
  while [ $# -gt 0 ]; do
    case $1 in
    -t | --tmux-dir)
      user_tmux_dir=$2
      shift
      shift
      ;;
    -s | --systemd-dir)
      user_systemd_dir=$2
      shift
      shift
      ;;
    -r | --systemd-req)
      user_systemd_req=$2
      shift
      shift
      ;;
    -m | --minipc-dir)
      user_minipc_dir=$2
      shift
      shift
      ;;
    -u | --umask)
      user_umask_mode=$2
      shift
      shift
      ;;
    -i | --interactive)
      interactive=1
      shift
      ;;
    i | install)
      install ${interactive}
      shift
      ;;
    c | clean)
      clean ${interactive}
      shift
      ;;
    r | reinstall)
      reinstall=1
      clean ${interactive} ${reinstall}
      install ${interactive}
      shift
      ;;
    jobcontrol)
      shift
      jobcontrol "$@"
      ;;
    *)
      printhelp
      shift
      ;;
    esac
  done
}

run "${@:-help}"
