#!/usr/bin/env bash

(
	cd "core"
	"./build_script.sh"
)

(
	cd "client"
	"./build_script.sh"
)

(
	cd "application"
	"./build_script.sh"
)
