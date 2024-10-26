#!/bin/bash
HELP=$(cat << EOF
Usage: $(basename "$0") [options] {[i]nstall,[c]lean,[r]einstall}

  -h, --help            Show this message
  -s, --systemd-dir     Set systemd directory
  -b, --localbin-dir    Set localbin directory
  -U, --user            Set user to configure for
  -u, --umask           Set umask
  -i, --interactive     Run interactively

Requires superuser.
Install will (in order):
    (1) Creates and registers service to run startup script at boot
    (2) Creates startup script
    (3) Enables lingering for chosen user such that their user services are
        started at boot.
Clean will (not in order):
    (1) Delete installed files
    (2) Unregister services
    (3) Restore linger status
EOF
)
LOG=.setupparam.log
SRV=irm-jetston.service

RUN_USER=${USER}
SYSTEMD_DIR=/etc/systemd/system
LOCALBIN_DIR=/usr/local/bin
STARTUP_SCRIPT=irm_jetson_startup.sh
UMASK_MODE=0022
user_run_user=
user_systemd_dir=
user_localbin_dir=
user_startup_script=
user_umask_mode=

script_name=$(basename "$0")
script_file=$(realpath "$0")
script_dir=$(realpath "${script_file}" | xargs dirname)
script_log=${script_dir}/${LOG}

function printhelp() {
    echo "$HELP"
    exit ${1:-0}
}

function install() {
    interactive=${1/-}
    if [ -f "${script_log}" ]; then
        echo "${script_log}" already exists.
        echo Run \`${script_name} clean\` before reinstalling.
        exit 1
    fi
    if [ ${interactive} ]; then
        echo Choose user which to run for.
        echo -n [default=${RUN_USER}]:\ 
        read user_run_user
        echo

        echo Choose system systemd directory.
        echo -n [default=${SYSTEMD_DIR}]:\ 
        read user_systemd_dir
        echo

        echo Choose local bin directory.
        echo -n [default=${LOCALBIN_DIR}]:\ 
        read user_localbin_dir
        echo

        echo Choose startup script name.
        echo -n [default=${STARTUP_SCRIPT}]:\ 
        read user_startup_script
        echo

        echo Choose umask.
        echo -n [default=${UMASK_MODE}]\ 
        read user_umask_mode
        echo
    fi

    echo Selected configuration:
    echo "RUN_USER:         ${user_run_user:=$RUN_USER}"
    echo "SYSTEMD_DIR:      ${user_systemd_dir:=$SYSTEMD_DIR} (system)"
    echo "LOCALBIN_DIR:     ${user_localbin_dir:=$LOCALBIN_DIR}"
    echo "STARTUP_SCRIPT:   ${user_startup_script:=$STARTUP_SCRIPT}"
    echo "UMASK_MODE:       ${user_umask_mode:=$UMASK_MODE}"
    echo
    echo "SERVICE:          ${user_systemd_dir}/${SRV}"
    echo "LOG:              ${script_log}"
    echo

    if [ "${user_run_user}" == root ]; then
        echo Selected user should not be root.
        echo Run again with -U.
        exit 1
    fi

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
    f_systemdservice=${user_systemd_dir}/${SRV}
    f_startupscript=${user_localbin_dir}/${user_startup_script}
    # Temporary files
    tf_mtdparamlog=${tmpdir}/"${f_mtdparamlog}"
    tf_systemdservice=${tmpdir}/"${f_systemdservice}"
    tf_startupscript=${tmpdir}/"${f_startupscript}"

    mkdir -p "${tf_systemdservice%\/*}" && > "${tf_systemdservice}"
    echo +++ Write systemd user service to "${tf_systemdservice}"
    cat << EOF | tee "${tf_systemdservice}"
[Unit]
Description=Run iRM startup script for Jetson.

[Service]
Type=oneshot
ExecStart=${user_localbin_dir}/${user_startup_script}

[Install]
WantedBy=multi-user.target
EOF
    echo
    echo

    mkdir -p "${tf_startupscript%\/*}" && > "${tf_startupscript}"
    echo +++ Write startup script to "${tf_startupscript}"
    cat << EOF | tee "${tf_startupscript}"
#!/bin/bash
#
# Startup script for Jetson.
#
# Do not directly add \`can\` \`can_raw\` and \`mttcan\` as modules,
# we need to modify registers frist before the setup
busybox devmem 0x0c303018 w 0xc458
busybox devmem 0x0c303010 w 0xc400
busybox devmem 0x0c303008 w 0xc458
busybox devmem 0x0c303000 w 0xc400
modprobe can
modprobe can_raw
modprobe mttcan
ip link set down can0
ip link set can0 type can bitrate 1000000
ip link set up can0
EOF
    echo
    echo

    mkdir -p "${tf_mtdparamlog%\/*}" && > "${tf_mtdparamlog}"
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

    echo +++ Log system states to "${tf_mtdparamlog}"
    # User
    echo "RUN_USER=${user_run_user}" | tee -a "${tf_mtdparamlog}"
    # User linger status
    loginctl show-user "${user_run_user}" -p Linger | tee -a "${tf_mtdparamlog}"

    echo Created files:
    sed -e '/^[^f]/d' -e "s|^f_.*=\(.*\)|${tmpdir}/\1|" "${tf_mtdparamlog}" \
        | xargs ls -ld
    echo
    echo Saved statuses:
    sed -e '/^[fd]_/d' "${tf_mtdparamlog}"

    if [ ${interactive} ]; then
        echo -n Copy to system and make changes? [,y/n]\ 
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
            cat "${tmpdir}/${!f}" > "${!f}"
        done
    )
    copy_to_system "${tmpdir}" \
                   "${script_file}" \
                   "${script_dir}" \
                   "${script_log}" \
                   "${user_umask_mode}"
    grep "^[fd]_" "${script_log}" | cut -d= -f2 | xargs ls -l
    echo

    echo Make changes
    function make_changes() (
        tmpdir=$1
        script_file=$2
        script_dir=$3
        script_log=$4

        source "${tmpdir}/${script_log}"

        # Lingering
        echo =================${Linger}
        if [ "${Linger}" == no ]; then
            loginctl enable-linger "${RUN_USER}"
            echo Enabled lingering for ${RUN_USER}
        fi
    )
    make_changes "${tmpdir}" \
                 "${script_file}" \
                 "${script_dir}" \
                 "${script_log}"


    echo Make ${f_startupscript} executable
    chmod +x "${f_startupscript}"
    echo Reload systemd configuration with \`systemctl --user daemon-reload\`
    systemctl daemon-reload
    echo Enable ${SRV}
    systemctl enable "${SRV}"

    echo
    echo Done.
}


function clean() {
    interactive=${1/-}
    reinstall=${2/-}
    if [ ! -f "${script_log}" ]; then
        echo "${script_log}" does not exist.
        echo Nothing to be cleaned.
        [ ${reinstall} ] \
            && return 0 \
            || exit 0
    fi
    if [ ${interactive} ]; then
        cat "${script_log}"
        echo
        echo -n Remove these and revert? [,y/n]\ 
        read cont
        [ "$cont" == n ] && exit 0
        echo
    fi

    source "${script_log}"
    # Files
    for f in ${!f_@}; do
        [ -f "${!f}" ] && \
            rm -v "${!f}"
    done
    # Directories
    for d in ${!d_@}; do
        rm -rfv "${!d}"
    done
    # Changes
    echo Disable ${SRV}
    systemctl disable "${SRV}"
    if [ "${Linger}" == no ]; then
        if [ "$(loginctl show-user "${RUN_USER}" -P Linger)" = yes ]; then
            loginctl disable-linger "${RUN_USER}"
            echo Disabled lingering for "${RUN_USER}"
        fi
    fi

    echo
    echo Done.
}

interactive=-
reinstall=-
function run() {
    [ "${EUID}" -ne 0 ] && printhelp 1
    while [ $# -gt 0 ]; do
        case $1 in
            -s|--systemd-dir)
                user_systemd_dir=$2;
                shift; shift;;
            -b|--localbin-dir)
                user_localbin_dir=$2;
                shift; shift;;
            -U|--user)
                user_run_user=$2;
                shift; shift;;
            -u|--umask)
                user_umask_mode=$2;
                shift; shift;;
            -i|--interactive)
                interactive=1;
                shift;;
            i|install)
                install ${interactive};
                shift;;
            c|clean)
                clean ${interactive};
                shift;;
            r|reinstall)
                reinstall=1
                clean ${interactive} ${reinstall};
                install ${interactive};
                shift;;
            *)
                printhelp;
                shift;;
            esac
    done
}

run "${@:-help}"
