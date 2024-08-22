## ****************************************************************************
## ****************************************************************************
## Copyright SoC Design Research Group, All rights reserved.    
## Electronics and Telecommunications Research Institute (ETRI)
##
## THESE DOCUMENTS CONTAIN CONFIDENTIAL INFORMATION AND KNOWLEDGE 
## WHICH IS THE PROPERTY OF ETRI. NO PART OF THIS PUBLICATION IS 
## TO BE USED FOR ANY OTHER PURPOSE, AND THESE ARE NOT TO BE 
## REPRODUCED, COPIED, DISCLOSED, TRANSMITTED, STORED IN A RETRIEVAL 
## SYSTEM OR TRANSLATED INTO ANY OTHER HUMAN OR COMPUTER LANGUAGE, 
## IN ANY FORM, BY ANY MEANS, IN WHOLE OR IN PART, WITHOUT THE 
## COMPLETE PRIOR WRITTEN PERMISSION OF ETRI.
## ****************************************************************************
## 2024-08-12
## Kyuseung Han (han@etri.re.kr)
## ****************************************************************************
## ****************************************************************************

ifdef RVX_MINI_HOME
	include ${RVX_MINI_HOME}/rvx_config.mh
else
	include ${RVX_DEVKIT_HOME}/rvx_config.mh
endif

TEMPLATE_FILE = ${RVX_ENV}/util/Makefile.npx.template
include ${RVX_UTIL_HOME}/template_makefile.mh

ifndef FILE_LIST
FILE_LIST=${CURDIR}/target/*
endif

PLATFORM_DIR = $(call absolute_path,${CURDIR}/../..)

UTIL_DIR = ${PLATFORM_DIR}/util
FAKEFIE_DIR = ${UTIL_DIR}/fakefile
USER_DIR = ${PLATFORM_DIR}/user

APP_CFG_DIR=${CURDIR}/app
USER_APP_CFG_LIST=$(wildcard ${APP_CFG_DIR}/*.cfg)
USER_APP_CFG_NAME_LIST=$(basename $(notdir ${USER_APP_CFG_LIST}))

check:
	@echo ${USER_APP_CFG_LIST}

clean:

NUM_EPOCH=10
NUM_REPEAT=5
NUM_SAMPLE=3

include ./each_npx.mh

all: train tv file

train:
	cd ${XARVIS_NPX_TRANIER_HOME} && make riscv CFG_FILE=${USER_APP_CFG_LIST} RESULT_DIR=${CURDIR}/result NUM_EPOCH=${NUM_EPOCH} NUM_REPEAT=${NUM_REPEAT}

tv:
	cd ${XARVIS_NPX_TRANIER_HOME} && make tv CFG_FILE=${USER_APP_CFG_LIST} RESULT_DIR=${CURDIR}/result NUM_EPOCH=${NUM_EPOCH} NUM_REPEAT=${NUM_REPEAT} NUM_SAMPLE=${NUM_SAMPLE}

ALL_APP_CFG_LIST=$(wildcard ${APP_CFG_DIR}/*.cfg)
ALL_APP_CFG_NAME_LIST=$(basename $(notdir ${ALL_APP_CFG_LIST}))

$(addsuffix .file_clean, ${ALL_APP_CFG_NAME_LIST}):%.file_clean:
	cd ${FAKEFIE_DIR} && make remove REMOVE_FILE_LIST=$(*).cfg
	cd ${FAKEFIE_DIR} && make remove REMOVE_FILE_LIST=$(*)*.bin

file_clean: $(addsuffix .file_clean, ${ALL_APP_CFG_NAME_LIST})

$(addsuffix .file_copy, ${ALL_APP_CFG_NAME_LIST}):%.file_copy:
	cd ${UTIL_DIR} && make fakefile
	cd ${FAKEFIE_DIR} && make add ADD_FILE_LIST=${APP_CFG_DIR}/$(*).cfg
	cd ${FAKEFIE_DIR} && make add ADD_FILE_LIST=${CURDIR}/result/$(*)/riscv/$(*)*.bin

file_gen:
	cd ${FAKEFIE_DIR} && make all

file: file_clean $(addsuffix .file_copy, ${USER_APP_CFG_NAME_LIST}) file_gen

.PHONY:
