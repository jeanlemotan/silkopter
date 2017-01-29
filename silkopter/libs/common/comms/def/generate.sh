#!/bin/bash

../../../../../def_lang/bin/pc/debug/generator --def gs_comms.def --namespace silk::gs_comms --xheader gen_support.h --enum-hashes true
../../../../../def_lang/bin/pc/debug/generator --def messages.def --namespace silk::messages --xheader gen_support.h --enum-hashes true
