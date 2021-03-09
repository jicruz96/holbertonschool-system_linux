#!/bin/bash
trap "echo Nope" SIGABRT SIGIO SIGTERM
