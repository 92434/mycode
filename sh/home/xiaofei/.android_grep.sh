#!/bin/sh

function grepfile() {
	find . -type f | grep --color -n "$@"
}

function greprsrc() {
	find . -type f -name "*.xml" | xargs grep --color -n "$@"
}

function grepjava() {
	find . -type f -name "*.java" | xargs grep --color -n "$@"
}

function grepstr() {
	find . -type f | xargs grep --color -n "$@"
}
