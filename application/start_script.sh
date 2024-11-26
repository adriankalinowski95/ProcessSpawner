#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

CURRENT_DIR="$PWD"
PROCESS_MANAGER_PATH="$CURRENT_DIR/build/process_manager/process_manager"
CHILD_PROCESS_PATH="$CURRENT_DIR/build/child_process/child_process"

DEFAULT_PROCESS_MANAGER_SERVER_IP="localhost"
DEFAULT_PROCESS_MANAGER_SERVER_PORT=50051
DEAFULT_PROCESS_MANAGER_NAME="default_process_manager"
DEFAULT_PROCESS_MANAGER_PROCESS_NAME="process_manager"

DEFAULT_CORE_SERVER_IP="localhost"
DEFAULT_CORE_SERVER_PORT=5002

DEFAULT_CHILD_PROCESS_IP="localhost"
DEFAULT_CHILD_PROCESS_PORT=8081
DEFAULT_CHILD_PROCESS_PATH=CHILD_PROCESS_PATH
DEFAULT_CHILD_PROCESS_PROCESS_NAME="child_process"

usage() {
    cat << EOF
Usage: $0 [options]

Options with example:
  --process_manager_server_ip="localhost"
  --process_manager_server_port=50051                              
  --process_manager_name="default_process_manager"
  --process_manager_process_name="process_manager"
  --core_server_ip="localhost"
  --core_server_port=5002
  --child_process_ip="localhost"
  --child_process_port=8081
  --child_process_path="/build/child_process/child_process"
  --child_process_process_name="child_process"
  --help                                                                 

EOF
    exit 0
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --process_manager_server_ip=*)
            DEFAULT_PROCESS_MANAGER_SERVER_IP="${1#*=}"
            shift
            ;;
        --process_manager_server_port=*)
            DEFAULT_PROCESS_MANAGER_SERVER_PORT="${1#*=}"
            shift
            ;;
        --process_manager_name=*)
            DEAFULT_PROCESS_MANAGER_NAME="${1#*=}"
        	shift
            ;;
        --process_manager_process_name=*)
            DEAFULT_PROCESS_MANAGER_NAME="${1#*=}"
        	shift
            ;;
        --core_server_ip=*)
            DEFAULT_CORE_SERVER_IP="${1#*=}"
        	shift
            ;;
        --core_server_port=*)
            DEAFULT_PROCESS_MANAGER_NAME="${1#*=}"
        	shift
            ;;
        --child_process_ip=*)
            DEFAULT_CORE_SERVER_PORT="${1#*=}"
        	shift
            ;;
        --child_process_port=*)
            DEFAULT_CHILD_PROCESS_PORT="${1#*=}"
        	shift
            ;;
        --child_process_path=*)
            DEFAULT_CHILD_PROCESS_PATH="${1#*=}"
        	shift
            ;;
        --child_process_process_name=*)
            DEFAULT_CHILD_PROCESS_PROCESS_NAME="${1#*=}"
        	shift
            ;;
        --help|-h)
            usage
            ;;
        *)
            echo "Unkown param: $1"
            usage
            ;;
    esac
done

add_json_field() {
    local key="$1"
    local value="$2"
    if [[ -n "$value" ]]; then
        if [[ "$key" == "age" ]]; then
            echo "\"$key\": $value"
        else
            value=$(echo "$value" | sed 's/\\/\\\\/g; s/"/\\"/g')
            echo "\"$key\": \"$value\""
        fi
    fi
}

JSON="{"

FIELDS=()
FIELDS+=("$(add_json_field "process_manager_server_ip" "$DEFAULT_PROCESS_MANAGER_SERVER_IP")")
FIELDS+=("$(add_json_field "process_manager_server_port" "$DEFAULT_PROCESS_MANAGER_SERVER_PORT")")
FIELDS+=("$(add_json_field "process_manager_name" "$DEAFULT_PROCESS_MANAGER_NAME")")
FIELDS+=("$(add_json_field "process_manager_process_name" "$DEFAULT_PROCESS_MANAGER_PROCESS_NAME")")
FIELDS+=("$(add_json_field "core_server_ip" "$DEFAULT_CORE_SERVER_IP")")
FIELDS+=("$(add_json_field "core_server_port" "$DEFAULT_CORE_SERVER_PORT")")
FIELDS+=("$(add_json_field "child_process_ip" "$DEFAULT_CHILD_PROCESS_IP")")
FIELDS+=("$(add_json_field "child_process_port" "$DEFAULT_CHILD_PROCESS_PORT")")
FIELDS+=("$(add_json_field "child_process_path" "$DEFAULT_CHILD_PROCESS_PATH")")
FIELDS+=("$(add_json_field "child_process_process_name" "$DEFAULT_CHILD_PROCESS_PROCESS_NAME")")

JSON+=$(IFS=,; echo "${FIELDS[*]}")

JSON+="}"

"$PROCESS_MANAGER_PATH" "$JSON"