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
## 2022-11-14
## Kyuseung Han (han@etri.re.kr)
## ****************************************************************************
## ****************************************************************************

ifdef RVX_MINI_HOME
	include ${RVX_MINI_HOME}/rvx_config.mh
else
	include ${RVX_DEVKIT_HOME}/rvx_config.mh
endif

TEMPLATE_FILE = ${RVX_ENV}/util/Makefile.fakefile.template
include ${RVX_UTIL_HOME}/template_makefile.mh

ifndef FILE_LIST
FILE_LIST=${CURDIR}/target/*
endif

ifndef ROM_NAME
ROM_NAME=NONE
endif

PLATFORM_DIR = $(call absolute_path,${CURDIR}/../..)

USER_DIR = ${PLATFORM_DIR}/user
ROM_TARGET_DIR=${PLATFORM_DIR}/util/rom/target

clean:
	-@rm -rf *.c *.h

convert: clean
	${PYTHON3_CMD} ${RVX_ENV}/util/convert_file.py -i ${FILE_LIST} -o ${CURDIR} -r ${ROM_NAME}

user:
	cd ${PLATFORM_DIR} && make user
	-@cp -f *.c *.h ${USER_DIR}/api

all: convert user

.PHONY:
