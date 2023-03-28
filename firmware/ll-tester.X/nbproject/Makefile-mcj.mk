#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-mcj.mk)" "nbproject/Makefile-local-mcj.mk"
include nbproject/Makefile-local-mcj.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=mcj
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ll-tester.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/ll-tester.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../lcd_drv/lcd_drv.c ../lcd_drv/ChrFont0.c ../lcd_drv/FillPat.c ../lcd_drv/OledChar.c ../lcd_drv/OledDriver.c ../lcd_drv/OledGrph.c ../lcd_drv/dogm-graphic.c ../lcd_drv/eadog.c ../lcd_drv/foo.c ../src/config/bma400/peripheral/adchs/plib_adchs.c ../src/config/bma400/peripheral/canfd/plib_canfd1.c ../src/config/bma400/peripheral/clk/plib_clk.c ../src/config/bma400/peripheral/coretimer/plib_coretimer.c ../src/config/bma400/peripheral/dmac/plib_dmac.c ../src/config/bma400/peripheral/evic/plib_evic.c ../src/config/bma400/peripheral/gpio/plib_gpio.c ../src/config/bma400/peripheral/mcpwm/plib_mcpwm.c ../src/config/bma400/peripheral/qei/plib_qei2.c ../src/config/bma400/peripheral/rtcc/plib_rtcc.c ../src/config/bma400/peripheral/spi/spi_master/plib_spi1_master.c ../src/config/bma400/peripheral/spi/spi_master/plib_spi2_master.c ../src/config/bma400/peripheral/tmr/plib_tmr9.c ../src/config/bma400/peripheral/tmr/plib_tmr5.c ../src/config/bma400/peripheral/tmr/plib_tmr6.c ../src/config/bma400/peripheral/uart/plib_uart1.c ../src/config/bma400/stdio/xc32_monitor.c ../src/config/bma400/initialization.c ../src/config/bma400/interrupts.c ../src/config/bma400/exceptions.c ../src/bma490l.c ../src/timers.c ../src/sca3300.c ../src/gfx.c ../src/imu.c ../src/canfd.c ../src/pid.c ../src/do_fft.c ../src/fft.c ../src/host.c ../src/main.c ../src/cmd_scanner.c ../src/remote_cmd.c ../src/imupic32mcj.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1684788505/lcd_drv.o ${OBJECTDIR}/_ext/1684788505/ChrFont0.o ${OBJECTDIR}/_ext/1684788505/FillPat.o ${OBJECTDIR}/_ext/1684788505/OledChar.o ${OBJECTDIR}/_ext/1684788505/OledDriver.o ${OBJECTDIR}/_ext/1684788505/OledGrph.o ${OBJECTDIR}/_ext/1684788505/dogm-graphic.o ${OBJECTDIR}/_ext/1684788505/eadog.o ${OBJECTDIR}/_ext/1684788505/foo.o ${OBJECTDIR}/_ext/1932426826/plib_adchs.o ${OBJECTDIR}/_ext/1930658663/plib_canfd1.o ${OBJECTDIR}/_ext/896314349/plib_clk.o ${OBJECTDIR}/_ext/1811813999/plib_coretimer.o ${OBJECTDIR}/_ext/2015971584/plib_dmac.o ${OBJECTDIR}/_ext/2016010272/plib_evic.o ${OBJECTDIR}/_ext/2016064100/plib_gpio.o ${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o ${OBJECTDIR}/_ext/896327584/plib_qei2.o ${OBJECTDIR}/_ext/2016395447/plib_rtcc.o ${OBJECTDIR}/_ext/305547027/plib_spi1_master.o ${OBJECTDIR}/_ext/305547027/plib_spi2_master.o ${OBJECTDIR}/_ext/896330724/plib_tmr9.o ${OBJECTDIR}/_ext/896330724/plib_tmr5.o ${OBJECTDIR}/_ext/896330724/plib_tmr6.o ${OBJECTDIR}/_ext/2016467043/plib_uart1.o ${OBJECTDIR}/_ext/1887586581/xc32_monitor.o ${OBJECTDIR}/_ext/1927388145/initialization.o ${OBJECTDIR}/_ext/1927388145/interrupts.o ${OBJECTDIR}/_ext/1927388145/exceptions.o ${OBJECTDIR}/_ext/1360937237/bma490l.o ${OBJECTDIR}/_ext/1360937237/timers.o ${OBJECTDIR}/_ext/1360937237/sca3300.o ${OBJECTDIR}/_ext/1360937237/gfx.o ${OBJECTDIR}/_ext/1360937237/imu.o ${OBJECTDIR}/_ext/1360937237/canfd.o ${OBJECTDIR}/_ext/1360937237/pid.o ${OBJECTDIR}/_ext/1360937237/do_fft.o ${OBJECTDIR}/_ext/1360937237/fft.o ${OBJECTDIR}/_ext/1360937237/host.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/cmd_scanner.o ${OBJECTDIR}/_ext/1360937237/remote_cmd.o ${OBJECTDIR}/_ext/1360937237/imupic32mcj.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1684788505/lcd_drv.o.d ${OBJECTDIR}/_ext/1684788505/ChrFont0.o.d ${OBJECTDIR}/_ext/1684788505/FillPat.o.d ${OBJECTDIR}/_ext/1684788505/OledChar.o.d ${OBJECTDIR}/_ext/1684788505/OledDriver.o.d ${OBJECTDIR}/_ext/1684788505/OledGrph.o.d ${OBJECTDIR}/_ext/1684788505/dogm-graphic.o.d ${OBJECTDIR}/_ext/1684788505/eadog.o.d ${OBJECTDIR}/_ext/1684788505/foo.o.d ${OBJECTDIR}/_ext/1932426826/plib_adchs.o.d ${OBJECTDIR}/_ext/1930658663/plib_canfd1.o.d ${OBJECTDIR}/_ext/896314349/plib_clk.o.d ${OBJECTDIR}/_ext/1811813999/plib_coretimer.o.d ${OBJECTDIR}/_ext/2015971584/plib_dmac.o.d ${OBJECTDIR}/_ext/2016010272/plib_evic.o.d ${OBJECTDIR}/_ext/2016064100/plib_gpio.o.d ${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o.d ${OBJECTDIR}/_ext/896327584/plib_qei2.o.d ${OBJECTDIR}/_ext/2016395447/plib_rtcc.o.d ${OBJECTDIR}/_ext/305547027/plib_spi1_master.o.d ${OBJECTDIR}/_ext/305547027/plib_spi2_master.o.d ${OBJECTDIR}/_ext/896330724/plib_tmr9.o.d ${OBJECTDIR}/_ext/896330724/plib_tmr5.o.d ${OBJECTDIR}/_ext/896330724/plib_tmr6.o.d ${OBJECTDIR}/_ext/2016467043/plib_uart1.o.d ${OBJECTDIR}/_ext/1887586581/xc32_monitor.o.d ${OBJECTDIR}/_ext/1927388145/initialization.o.d ${OBJECTDIR}/_ext/1927388145/interrupts.o.d ${OBJECTDIR}/_ext/1927388145/exceptions.o.d ${OBJECTDIR}/_ext/1360937237/bma490l.o.d ${OBJECTDIR}/_ext/1360937237/timers.o.d ${OBJECTDIR}/_ext/1360937237/sca3300.o.d ${OBJECTDIR}/_ext/1360937237/gfx.o.d ${OBJECTDIR}/_ext/1360937237/imu.o.d ${OBJECTDIR}/_ext/1360937237/canfd.o.d ${OBJECTDIR}/_ext/1360937237/pid.o.d ${OBJECTDIR}/_ext/1360937237/do_fft.o.d ${OBJECTDIR}/_ext/1360937237/fft.o.d ${OBJECTDIR}/_ext/1360937237/host.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/cmd_scanner.o.d ${OBJECTDIR}/_ext/1360937237/remote_cmd.o.d ${OBJECTDIR}/_ext/1360937237/imupic32mcj.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1684788505/lcd_drv.o ${OBJECTDIR}/_ext/1684788505/ChrFont0.o ${OBJECTDIR}/_ext/1684788505/FillPat.o ${OBJECTDIR}/_ext/1684788505/OledChar.o ${OBJECTDIR}/_ext/1684788505/OledDriver.o ${OBJECTDIR}/_ext/1684788505/OledGrph.o ${OBJECTDIR}/_ext/1684788505/dogm-graphic.o ${OBJECTDIR}/_ext/1684788505/eadog.o ${OBJECTDIR}/_ext/1684788505/foo.o ${OBJECTDIR}/_ext/1932426826/plib_adchs.o ${OBJECTDIR}/_ext/1930658663/plib_canfd1.o ${OBJECTDIR}/_ext/896314349/plib_clk.o ${OBJECTDIR}/_ext/1811813999/plib_coretimer.o ${OBJECTDIR}/_ext/2015971584/plib_dmac.o ${OBJECTDIR}/_ext/2016010272/plib_evic.o ${OBJECTDIR}/_ext/2016064100/plib_gpio.o ${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o ${OBJECTDIR}/_ext/896327584/plib_qei2.o ${OBJECTDIR}/_ext/2016395447/plib_rtcc.o ${OBJECTDIR}/_ext/305547027/plib_spi1_master.o ${OBJECTDIR}/_ext/305547027/plib_spi2_master.o ${OBJECTDIR}/_ext/896330724/plib_tmr9.o ${OBJECTDIR}/_ext/896330724/plib_tmr5.o ${OBJECTDIR}/_ext/896330724/plib_tmr6.o ${OBJECTDIR}/_ext/2016467043/plib_uart1.o ${OBJECTDIR}/_ext/1887586581/xc32_monitor.o ${OBJECTDIR}/_ext/1927388145/initialization.o ${OBJECTDIR}/_ext/1927388145/interrupts.o ${OBJECTDIR}/_ext/1927388145/exceptions.o ${OBJECTDIR}/_ext/1360937237/bma490l.o ${OBJECTDIR}/_ext/1360937237/timers.o ${OBJECTDIR}/_ext/1360937237/sca3300.o ${OBJECTDIR}/_ext/1360937237/gfx.o ${OBJECTDIR}/_ext/1360937237/imu.o ${OBJECTDIR}/_ext/1360937237/canfd.o ${OBJECTDIR}/_ext/1360937237/pid.o ${OBJECTDIR}/_ext/1360937237/do_fft.o ${OBJECTDIR}/_ext/1360937237/fft.o ${OBJECTDIR}/_ext/1360937237/host.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/cmd_scanner.o ${OBJECTDIR}/_ext/1360937237/remote_cmd.o ${OBJECTDIR}/_ext/1360937237/imupic32mcj.o

# Source Files
SOURCEFILES=../lcd_drv/lcd_drv.c ../lcd_drv/ChrFont0.c ../lcd_drv/FillPat.c ../lcd_drv/OledChar.c ../lcd_drv/OledDriver.c ../lcd_drv/OledGrph.c ../lcd_drv/dogm-graphic.c ../lcd_drv/eadog.c ../lcd_drv/foo.c ../src/config/bma400/peripheral/adchs/plib_adchs.c ../src/config/bma400/peripheral/canfd/plib_canfd1.c ../src/config/bma400/peripheral/clk/plib_clk.c ../src/config/bma400/peripheral/coretimer/plib_coretimer.c ../src/config/bma400/peripheral/dmac/plib_dmac.c ../src/config/bma400/peripheral/evic/plib_evic.c ../src/config/bma400/peripheral/gpio/plib_gpio.c ../src/config/bma400/peripheral/mcpwm/plib_mcpwm.c ../src/config/bma400/peripheral/qei/plib_qei2.c ../src/config/bma400/peripheral/rtcc/plib_rtcc.c ../src/config/bma400/peripheral/spi/spi_master/plib_spi1_master.c ../src/config/bma400/peripheral/spi/spi_master/plib_spi2_master.c ../src/config/bma400/peripheral/tmr/plib_tmr9.c ../src/config/bma400/peripheral/tmr/plib_tmr5.c ../src/config/bma400/peripheral/tmr/plib_tmr6.c ../src/config/bma400/peripheral/uart/plib_uart1.c ../src/config/bma400/stdio/xc32_monitor.c ../src/config/bma400/initialization.c ../src/config/bma400/interrupts.c ../src/config/bma400/exceptions.c ../src/bma490l.c ../src/timers.c ../src/sca3300.c ../src/gfx.c ../src/imu.c ../src/canfd.c ../src/pid.c ../src/do_fft.c ../src/fft.c ../src/host.c ../src/main.c ../src/cmd_scanner.c ../src/remote_cmd.c ../src/imupic32mcj.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-mcj.mk ${DISTDIR}/ll-tester.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MK0512MCJ048
MP_LINKER_FILE_OPTION=,--script="../src/config/bma400/p32MK0512MCJ048.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1684788505/lcd_drv.o: ../lcd_drv/lcd_drv.c  .generated_files/flags/mcj/1e767bef26f447075843bb16a403b9ee016d1de0 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/lcd_drv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/lcd_drv.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/lcd_drv.o.d" -o ${OBJECTDIR}/_ext/1684788505/lcd_drv.o ../lcd_drv/lcd_drv.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/ChrFont0.o: ../lcd_drv/ChrFont0.c  .generated_files/flags/mcj/5babdb9a28fe5986205f05f1832b037bbcd5a31e .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/ChrFont0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/ChrFont0.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/ChrFont0.o.d" -o ${OBJECTDIR}/_ext/1684788505/ChrFont0.o ../lcd_drv/ChrFont0.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/FillPat.o: ../lcd_drv/FillPat.c  .generated_files/flags/mcj/f506d3834ebcceae538721e80d2d04c601fa3c60 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/FillPat.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/FillPat.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/FillPat.o.d" -o ${OBJECTDIR}/_ext/1684788505/FillPat.o ../lcd_drv/FillPat.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/OledChar.o: ../lcd_drv/OledChar.c  .generated_files/flags/mcj/6e80f1fc5aae30a9bef783bc17150972a1cb352e .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledChar.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledChar.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/OledChar.o.d" -o ${OBJECTDIR}/_ext/1684788505/OledChar.o ../lcd_drv/OledChar.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/OledDriver.o: ../lcd_drv/OledDriver.c  .generated_files/flags/mcj/93aa5083dd5acfb14d3c70cff46754e8910c7a4c .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledDriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/OledDriver.o.d" -o ${OBJECTDIR}/_ext/1684788505/OledDriver.o ../lcd_drv/OledDriver.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/OledGrph.o: ../lcd_drv/OledGrph.c  .generated_files/flags/mcj/5dcc44187fd8245fe9e7053381fe4c549bff9986 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledGrph.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledGrph.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/OledGrph.o.d" -o ${OBJECTDIR}/_ext/1684788505/OledGrph.o ../lcd_drv/OledGrph.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/dogm-graphic.o: ../lcd_drv/dogm-graphic.c  .generated_files/flags/mcj/d4c98e37c45ad7513d89b76954b56cfd97bc05f .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/dogm-graphic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/dogm-graphic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/dogm-graphic.o.d" -o ${OBJECTDIR}/_ext/1684788505/dogm-graphic.o ../lcd_drv/dogm-graphic.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/eadog.o: ../lcd_drv/eadog.c  .generated_files/flags/mcj/222005abffc0e7ee0e7ee9689a65aff54e82e280 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/eadog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/eadog.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/eadog.o.d" -o ${OBJECTDIR}/_ext/1684788505/eadog.o ../lcd_drv/eadog.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/foo.o: ../lcd_drv/foo.c  .generated_files/flags/mcj/49687687d723397f5dfa169712c6b22584cb9c32 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/foo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/foo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/foo.o.d" -o ${OBJECTDIR}/_ext/1684788505/foo.o ../lcd_drv/foo.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1932426826/plib_adchs.o: ../src/config/bma400/peripheral/adchs/plib_adchs.c  .generated_files/flags/mcj/3a780831b6ab005bb046bdb4524e82161499b55d .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1932426826" 
	@${RM} ${OBJECTDIR}/_ext/1932426826/plib_adchs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1932426826/plib_adchs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1932426826/plib_adchs.o.d" -o ${OBJECTDIR}/_ext/1932426826/plib_adchs.o ../src/config/bma400/peripheral/adchs/plib_adchs.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1930658663/plib_canfd1.o: ../src/config/bma400/peripheral/canfd/plib_canfd1.c  .generated_files/flags/mcj/a392999ea27375fae6a66e54a1c1d0bd8ba4bcd8 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1930658663" 
	@${RM} ${OBJECTDIR}/_ext/1930658663/plib_canfd1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1930658663/plib_canfd1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1930658663/plib_canfd1.o.d" -o ${OBJECTDIR}/_ext/1930658663/plib_canfd1.o ../src/config/bma400/peripheral/canfd/plib_canfd1.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896314349/plib_clk.o: ../src/config/bma400/peripheral/clk/plib_clk.c  .generated_files/flags/mcj/b3eba2d3f6be1ab2b568e5760b55bece440154f9 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896314349" 
	@${RM} ${OBJECTDIR}/_ext/896314349/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/896314349/plib_clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896314349/plib_clk.o.d" -o ${OBJECTDIR}/_ext/896314349/plib_clk.o ../src/config/bma400/peripheral/clk/plib_clk.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1811813999/plib_coretimer.o: ../src/config/bma400/peripheral/coretimer/plib_coretimer.c  .generated_files/flags/mcj/6da46802690e283a425feac8995b2c636ec0a7d5 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1811813999" 
	@${RM} ${OBJECTDIR}/_ext/1811813999/plib_coretimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811813999/plib_coretimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1811813999/plib_coretimer.o.d" -o ${OBJECTDIR}/_ext/1811813999/plib_coretimer.o ../src/config/bma400/peripheral/coretimer/plib_coretimer.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2015971584/plib_dmac.o: ../src/config/bma400/peripheral/dmac/plib_dmac.c  .generated_files/flags/mcj/a359048d260eac696fe5095b428a4d23cd300d24 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2015971584" 
	@${RM} ${OBJECTDIR}/_ext/2015971584/plib_dmac.o.d 
	@${RM} ${OBJECTDIR}/_ext/2015971584/plib_dmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2015971584/plib_dmac.o.d" -o ${OBJECTDIR}/_ext/2015971584/plib_dmac.o ../src/config/bma400/peripheral/dmac/plib_dmac.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2016010272/plib_evic.o: ../src/config/bma400/peripheral/evic/plib_evic.c  .generated_files/flags/mcj/191e3820955d8a7b68315e3eca3e9a1a248bae97 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2016010272" 
	@${RM} ${OBJECTDIR}/_ext/2016010272/plib_evic.o.d 
	@${RM} ${OBJECTDIR}/_ext/2016010272/plib_evic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2016010272/plib_evic.o.d" -o ${OBJECTDIR}/_ext/2016010272/plib_evic.o ../src/config/bma400/peripheral/evic/plib_evic.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2016064100/plib_gpio.o: ../src/config/bma400/peripheral/gpio/plib_gpio.c  .generated_files/flags/mcj/fa25bfff639de1efcf292f36018654810489ae27 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2016064100" 
	@${RM} ${OBJECTDIR}/_ext/2016064100/plib_gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/2016064100/plib_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2016064100/plib_gpio.o.d" -o ${OBJECTDIR}/_ext/2016064100/plib_gpio.o ../src/config/bma400/peripheral/gpio/plib_gpio.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o: ../src/config/bma400/peripheral/mcpwm/plib_mcpwm.c  .generated_files/flags/mcj/95b46d712ef0266b48bae03e5fb991c2c24b0267 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1921361413" 
	@${RM} ${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o.d" -o ${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o ../src/config/bma400/peripheral/mcpwm/plib_mcpwm.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896327584/plib_qei2.o: ../src/config/bma400/peripheral/qei/plib_qei2.c  .generated_files/flags/mcj/3e1e5b611e811c442144c2f65cafe896496ee3bc .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896327584" 
	@${RM} ${OBJECTDIR}/_ext/896327584/plib_qei2.o.d 
	@${RM} ${OBJECTDIR}/_ext/896327584/plib_qei2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896327584/plib_qei2.o.d" -o ${OBJECTDIR}/_ext/896327584/plib_qei2.o ../src/config/bma400/peripheral/qei/plib_qei2.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2016395447/plib_rtcc.o: ../src/config/bma400/peripheral/rtcc/plib_rtcc.c  .generated_files/flags/mcj/8812925c0510db680bf0337a9aed56580b98a826 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2016395447" 
	@${RM} ${OBJECTDIR}/_ext/2016395447/plib_rtcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2016395447/plib_rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2016395447/plib_rtcc.o.d" -o ${OBJECTDIR}/_ext/2016395447/plib_rtcc.o ../src/config/bma400/peripheral/rtcc/plib_rtcc.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/305547027/plib_spi1_master.o: ../src/config/bma400/peripheral/spi/spi_master/plib_spi1_master.c  .generated_files/flags/mcj/67a21effeea8d6d83fbea8aebe741012efbc795a .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/305547027" 
	@${RM} ${OBJECTDIR}/_ext/305547027/plib_spi1_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/305547027/plib_spi1_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/305547027/plib_spi1_master.o.d" -o ${OBJECTDIR}/_ext/305547027/plib_spi1_master.o ../src/config/bma400/peripheral/spi/spi_master/plib_spi1_master.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/305547027/plib_spi2_master.o: ../src/config/bma400/peripheral/spi/spi_master/plib_spi2_master.c  .generated_files/flags/mcj/7e3e0c1f0641ef31dcbd3927a98c95d75aeff967 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/305547027" 
	@${RM} ${OBJECTDIR}/_ext/305547027/plib_spi2_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/305547027/plib_spi2_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/305547027/plib_spi2_master.o.d" -o ${OBJECTDIR}/_ext/305547027/plib_spi2_master.o ../src/config/bma400/peripheral/spi/spi_master/plib_spi2_master.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896330724/plib_tmr9.o: ../src/config/bma400/peripheral/tmr/plib_tmr9.c  .generated_files/flags/mcj/5c6c7f9116f82f29413c00ebcd4edea09972a8f2 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896330724" 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr9.o.d 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr9.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896330724/plib_tmr9.o.d" -o ${OBJECTDIR}/_ext/896330724/plib_tmr9.o ../src/config/bma400/peripheral/tmr/plib_tmr9.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896330724/plib_tmr5.o: ../src/config/bma400/peripheral/tmr/plib_tmr5.c  .generated_files/flags/mcj/46536cd8d5eecd58b54f464a83cf68c5888d7eb7 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896330724" 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr5.o.d 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr5.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896330724/plib_tmr5.o.d" -o ${OBJECTDIR}/_ext/896330724/plib_tmr5.o ../src/config/bma400/peripheral/tmr/plib_tmr5.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896330724/plib_tmr6.o: ../src/config/bma400/peripheral/tmr/plib_tmr6.c  .generated_files/flags/mcj/52cac384acf308549b8d83350ac1f83820b87985 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896330724" 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr6.o.d 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr6.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896330724/plib_tmr6.o.d" -o ${OBJECTDIR}/_ext/896330724/plib_tmr6.o ../src/config/bma400/peripheral/tmr/plib_tmr6.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2016467043/plib_uart1.o: ../src/config/bma400/peripheral/uart/plib_uart1.c  .generated_files/flags/mcj/c0c2563af5ebdedaeebd37b537efda0c83c00b18 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2016467043" 
	@${RM} ${OBJECTDIR}/_ext/2016467043/plib_uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/2016467043/plib_uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2016467043/plib_uart1.o.d" -o ${OBJECTDIR}/_ext/2016467043/plib_uart1.o ../src/config/bma400/peripheral/uart/plib_uart1.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1887586581/xc32_monitor.o: ../src/config/bma400/stdio/xc32_monitor.c  .generated_files/flags/mcj/6ec0af3110980f341246235c6068cad936068a08 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1887586581" 
	@${RM} ${OBJECTDIR}/_ext/1887586581/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1887586581/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1887586581/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/1887586581/xc32_monitor.o ../src/config/bma400/stdio/xc32_monitor.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1927388145/initialization.o: ../src/config/bma400/initialization.c  .generated_files/flags/mcj/7bafd435cc7c17b43fe0d6eb94fa08bc3735eb12 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1927388145" 
	@${RM} ${OBJECTDIR}/_ext/1927388145/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1927388145/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1927388145/initialization.o.d" -o ${OBJECTDIR}/_ext/1927388145/initialization.o ../src/config/bma400/initialization.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1927388145/interrupts.o: ../src/config/bma400/interrupts.c  .generated_files/flags/mcj/ab597201a799c6683af1272dcedaa2f52e46f845 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1927388145" 
	@${RM} ${OBJECTDIR}/_ext/1927388145/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1927388145/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1927388145/interrupts.o.d" -o ${OBJECTDIR}/_ext/1927388145/interrupts.o ../src/config/bma400/interrupts.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1927388145/exceptions.o: ../src/config/bma400/exceptions.c  .generated_files/flags/mcj/330440859581c8901a04fd7bf54fb2f82d0a7e81 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1927388145" 
	@${RM} ${OBJECTDIR}/_ext/1927388145/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1927388145/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1927388145/exceptions.o.d" -o ${OBJECTDIR}/_ext/1927388145/exceptions.o ../src/config/bma400/exceptions.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/bma490l.o: ../src/bma490l.c  .generated_files/flags/mcj/7be5de0c3f4f7b1579240ea2aeda0a35ea02bfed .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bma490l.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bma490l.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bma490l.o.d" -o ${OBJECTDIR}/_ext/1360937237/bma490l.o ../src/bma490l.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/timers.o: ../src/timers.c  .generated_files/flags/mcj/136e8ece6464dc4d0428858699cfcdf3ea24a8e7 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/timers.o ../src/timers.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/sca3300.o: ../src/sca3300.c  .generated_files/flags/mcj/649ccddba8abba20351daeebf9f96f5518fd05f8 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sca3300.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sca3300.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/sca3300.o.d" -o ${OBJECTDIR}/_ext/1360937237/sca3300.o ../src/sca3300.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/gfx.o: ../src/gfx.c  .generated_files/flags/mcj/fad95ad62305a6a615e4df9cf6bef3522983b657 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gfx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gfx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/gfx.o.d" -o ${OBJECTDIR}/_ext/1360937237/gfx.o ../src/gfx.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/imu.o: ../src/imu.c  .generated_files/flags/mcj/19bbbe2fe6e625adb7e2d7993a553c68ccab1322 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/imu.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/imu.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/imu.o.d" -o ${OBJECTDIR}/_ext/1360937237/imu.o ../src/imu.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/canfd.o: ../src/canfd.c  .generated_files/flags/mcj/b194b810cf4eff02471d9a056df7304dbf0124be .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/canfd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/canfd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/canfd.o.d" -o ${OBJECTDIR}/_ext/1360937237/canfd.o ../src/canfd.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/pid.o: ../src/pid.c  .generated_files/flags/mcj/8838068df07fa3565fe9d5a0d1c9689272aed92a .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pid.o.d" -o ${OBJECTDIR}/_ext/1360937237/pid.o ../src/pid.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/do_fft.o: ../src/do_fft.c  .generated_files/flags/mcj/d612919cce4006b490ad88a70dcba4a69b10af57 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/do_fft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/do_fft.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/do_fft.o.d" -o ${OBJECTDIR}/_ext/1360937237/do_fft.o ../src/do_fft.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/fft.o: ../src/fft.c  .generated_files/flags/mcj/b7e8ca7f8007717eaf667b75014452af89a38947 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/fft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/fft.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/fft.o.d" -o ${OBJECTDIR}/_ext/1360937237/fft.o ../src/fft.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/host.o: ../src/host.c  .generated_files/flags/mcj/bd5fb049e9cb5d786bf102df9e1a62a9bf4f6251 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/host.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/host.o.d" -o ${OBJECTDIR}/_ext/1360937237/host.o ../src/host.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/mcj/1da6e21b60286e6fc619fc437ccb561d41bb7138 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/cmd_scanner.o: ../src/cmd_scanner.c  .generated_files/flags/mcj/69437c1a15768fbfc63dd533694f5424291dd031 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cmd_scanner.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cmd_scanner.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/cmd_scanner.o.d" -o ${OBJECTDIR}/_ext/1360937237/cmd_scanner.o ../src/cmd_scanner.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/remote_cmd.o: ../src/remote_cmd.c  .generated_files/flags/mcj/c8ccc06b6b1e8e675dd887c61e46e797cf87dbae .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/remote_cmd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/remote_cmd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/remote_cmd.o.d" -o ${OBJECTDIR}/_ext/1360937237/remote_cmd.o ../src/remote_cmd.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/imupic32mcj.o: ../src/imupic32mcj.c  .generated_files/flags/mcj/55125f60bda31f879fbc06746419112fbc593594 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/imupic32mcj.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/imupic32mcj.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/imupic32mcj.o.d" -o ${OBJECTDIR}/_ext/1360937237/imupic32mcj.o ../src/imupic32mcj.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1684788505/lcd_drv.o: ../lcd_drv/lcd_drv.c  .generated_files/flags/mcj/7810cb3b9b66d525a810187b1d967c1d3eaaa8fd .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/lcd_drv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/lcd_drv.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/lcd_drv.o.d" -o ${OBJECTDIR}/_ext/1684788505/lcd_drv.o ../lcd_drv/lcd_drv.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/ChrFont0.o: ../lcd_drv/ChrFont0.c  .generated_files/flags/mcj/f5666145dd37c2cfae87a6dcb4ab1d606c8b903e .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/ChrFont0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/ChrFont0.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/ChrFont0.o.d" -o ${OBJECTDIR}/_ext/1684788505/ChrFont0.o ../lcd_drv/ChrFont0.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/FillPat.o: ../lcd_drv/FillPat.c  .generated_files/flags/mcj/3fb1e21e1a05f3c1066ccec53c91b215d38f6354 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/FillPat.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/FillPat.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/FillPat.o.d" -o ${OBJECTDIR}/_ext/1684788505/FillPat.o ../lcd_drv/FillPat.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/OledChar.o: ../lcd_drv/OledChar.c  .generated_files/flags/mcj/ba3e79b8b3e7dc9b71436530b17e805a9bded95e .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledChar.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledChar.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/OledChar.o.d" -o ${OBJECTDIR}/_ext/1684788505/OledChar.o ../lcd_drv/OledChar.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/OledDriver.o: ../lcd_drv/OledDriver.c  .generated_files/flags/mcj/3867d8395b2357754a9d393f7defcfc581118a18 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledDriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/OledDriver.o.d" -o ${OBJECTDIR}/_ext/1684788505/OledDriver.o ../lcd_drv/OledDriver.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/OledGrph.o: ../lcd_drv/OledGrph.c  .generated_files/flags/mcj/e1f3ec8840d4b7fa2e42a7e0cdaff961f3f3f662 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledGrph.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/OledGrph.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/OledGrph.o.d" -o ${OBJECTDIR}/_ext/1684788505/OledGrph.o ../lcd_drv/OledGrph.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/dogm-graphic.o: ../lcd_drv/dogm-graphic.c  .generated_files/flags/mcj/cedc8ae236e8d0f60342148de81b184e36d61088 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/dogm-graphic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/dogm-graphic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/dogm-graphic.o.d" -o ${OBJECTDIR}/_ext/1684788505/dogm-graphic.o ../lcd_drv/dogm-graphic.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/eadog.o: ../lcd_drv/eadog.c  .generated_files/flags/mcj/a5c4c5bf020339d5517856448fde0b03415d5529 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/eadog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/eadog.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/eadog.o.d" -o ${OBJECTDIR}/_ext/1684788505/eadog.o ../lcd_drv/eadog.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1684788505/foo.o: ../lcd_drv/foo.c  .generated_files/flags/mcj/d54c26ccf076a5af876d3cd2d0d74df23f4a4f4f .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1684788505" 
	@${RM} ${OBJECTDIR}/_ext/1684788505/foo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1684788505/foo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1684788505/foo.o.d" -o ${OBJECTDIR}/_ext/1684788505/foo.o ../lcd_drv/foo.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1932426826/plib_adchs.o: ../src/config/bma400/peripheral/adchs/plib_adchs.c  .generated_files/flags/mcj/117f7d2698aed3906470e8ba6cc340aca5368223 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1932426826" 
	@${RM} ${OBJECTDIR}/_ext/1932426826/plib_adchs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1932426826/plib_adchs.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1932426826/plib_adchs.o.d" -o ${OBJECTDIR}/_ext/1932426826/plib_adchs.o ../src/config/bma400/peripheral/adchs/plib_adchs.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1930658663/plib_canfd1.o: ../src/config/bma400/peripheral/canfd/plib_canfd1.c  .generated_files/flags/mcj/c8e037f6b64d0f348da4cc097306ffa2e6b909f8 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1930658663" 
	@${RM} ${OBJECTDIR}/_ext/1930658663/plib_canfd1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1930658663/plib_canfd1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1930658663/plib_canfd1.o.d" -o ${OBJECTDIR}/_ext/1930658663/plib_canfd1.o ../src/config/bma400/peripheral/canfd/plib_canfd1.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896314349/plib_clk.o: ../src/config/bma400/peripheral/clk/plib_clk.c  .generated_files/flags/mcj/3d5c0269e2e984f818967d79279d97eb83ed85ed .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896314349" 
	@${RM} ${OBJECTDIR}/_ext/896314349/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/896314349/plib_clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896314349/plib_clk.o.d" -o ${OBJECTDIR}/_ext/896314349/plib_clk.o ../src/config/bma400/peripheral/clk/plib_clk.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1811813999/plib_coretimer.o: ../src/config/bma400/peripheral/coretimer/plib_coretimer.c  .generated_files/flags/mcj/1127feb501c514c153cffc715507317c87f09844 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1811813999" 
	@${RM} ${OBJECTDIR}/_ext/1811813999/plib_coretimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1811813999/plib_coretimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1811813999/plib_coretimer.o.d" -o ${OBJECTDIR}/_ext/1811813999/plib_coretimer.o ../src/config/bma400/peripheral/coretimer/plib_coretimer.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2015971584/plib_dmac.o: ../src/config/bma400/peripheral/dmac/plib_dmac.c  .generated_files/flags/mcj/f3cf02f0c17174ee002d144602316158f156c131 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2015971584" 
	@${RM} ${OBJECTDIR}/_ext/2015971584/plib_dmac.o.d 
	@${RM} ${OBJECTDIR}/_ext/2015971584/plib_dmac.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2015971584/plib_dmac.o.d" -o ${OBJECTDIR}/_ext/2015971584/plib_dmac.o ../src/config/bma400/peripheral/dmac/plib_dmac.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2016010272/plib_evic.o: ../src/config/bma400/peripheral/evic/plib_evic.c  .generated_files/flags/mcj/e7a7f6507d00d409342073d34649b6c9c4b92afe .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2016010272" 
	@${RM} ${OBJECTDIR}/_ext/2016010272/plib_evic.o.d 
	@${RM} ${OBJECTDIR}/_ext/2016010272/plib_evic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2016010272/plib_evic.o.d" -o ${OBJECTDIR}/_ext/2016010272/plib_evic.o ../src/config/bma400/peripheral/evic/plib_evic.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2016064100/plib_gpio.o: ../src/config/bma400/peripheral/gpio/plib_gpio.c  .generated_files/flags/mcj/f916502be1e295236ea13d7a6f85e1aff120cc31 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2016064100" 
	@${RM} ${OBJECTDIR}/_ext/2016064100/plib_gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/2016064100/plib_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2016064100/plib_gpio.o.d" -o ${OBJECTDIR}/_ext/2016064100/plib_gpio.o ../src/config/bma400/peripheral/gpio/plib_gpio.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o: ../src/config/bma400/peripheral/mcpwm/plib_mcpwm.c  .generated_files/flags/mcj/444a3af597da9581010f66e3dc3b29a354701e19 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1921361413" 
	@${RM} ${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o.d" -o ${OBJECTDIR}/_ext/1921361413/plib_mcpwm.o ../src/config/bma400/peripheral/mcpwm/plib_mcpwm.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896327584/plib_qei2.o: ../src/config/bma400/peripheral/qei/plib_qei2.c  .generated_files/flags/mcj/cc91ae6c65fd834c633419588f06485f7a85a255 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896327584" 
	@${RM} ${OBJECTDIR}/_ext/896327584/plib_qei2.o.d 
	@${RM} ${OBJECTDIR}/_ext/896327584/plib_qei2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896327584/plib_qei2.o.d" -o ${OBJECTDIR}/_ext/896327584/plib_qei2.o ../src/config/bma400/peripheral/qei/plib_qei2.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2016395447/plib_rtcc.o: ../src/config/bma400/peripheral/rtcc/plib_rtcc.c  .generated_files/flags/mcj/dafe90d8795ba78443adffacdfc486944e8f1504 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2016395447" 
	@${RM} ${OBJECTDIR}/_ext/2016395447/plib_rtcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2016395447/plib_rtcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2016395447/plib_rtcc.o.d" -o ${OBJECTDIR}/_ext/2016395447/plib_rtcc.o ../src/config/bma400/peripheral/rtcc/plib_rtcc.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/305547027/plib_spi1_master.o: ../src/config/bma400/peripheral/spi/spi_master/plib_spi1_master.c  .generated_files/flags/mcj/3c79a36a697a757b6ef9c5b56e9ae1b4b57e6995 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/305547027" 
	@${RM} ${OBJECTDIR}/_ext/305547027/plib_spi1_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/305547027/plib_spi1_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/305547027/plib_spi1_master.o.d" -o ${OBJECTDIR}/_ext/305547027/plib_spi1_master.o ../src/config/bma400/peripheral/spi/spi_master/plib_spi1_master.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/305547027/plib_spi2_master.o: ../src/config/bma400/peripheral/spi/spi_master/plib_spi2_master.c  .generated_files/flags/mcj/11c6954433f6a459e11e565724b9ca7eaa9dbc87 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/305547027" 
	@${RM} ${OBJECTDIR}/_ext/305547027/plib_spi2_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/305547027/plib_spi2_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/305547027/plib_spi2_master.o.d" -o ${OBJECTDIR}/_ext/305547027/plib_spi2_master.o ../src/config/bma400/peripheral/spi/spi_master/plib_spi2_master.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896330724/plib_tmr9.o: ../src/config/bma400/peripheral/tmr/plib_tmr9.c  .generated_files/flags/mcj/b29c355395274189de5310bd5d1afd0cfbc8b6c4 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896330724" 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr9.o.d 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr9.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896330724/plib_tmr9.o.d" -o ${OBJECTDIR}/_ext/896330724/plib_tmr9.o ../src/config/bma400/peripheral/tmr/plib_tmr9.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896330724/plib_tmr5.o: ../src/config/bma400/peripheral/tmr/plib_tmr5.c  .generated_files/flags/mcj/4aba487b2403a268ba02412a1be3ed6feace753c .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896330724" 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr5.o.d 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr5.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896330724/plib_tmr5.o.d" -o ${OBJECTDIR}/_ext/896330724/plib_tmr5.o ../src/config/bma400/peripheral/tmr/plib_tmr5.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/896330724/plib_tmr6.o: ../src/config/bma400/peripheral/tmr/plib_tmr6.c  .generated_files/flags/mcj/a5120457151f691bf775ca6f91cab722f90cc652 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896330724" 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr6.o.d 
	@${RM} ${OBJECTDIR}/_ext/896330724/plib_tmr6.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/896330724/plib_tmr6.o.d" -o ${OBJECTDIR}/_ext/896330724/plib_tmr6.o ../src/config/bma400/peripheral/tmr/plib_tmr6.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2016467043/plib_uart1.o: ../src/config/bma400/peripheral/uart/plib_uart1.c  .generated_files/flags/mcj/3448a6568d061bebfd3f8a1c328687c54db9cec5 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2016467043" 
	@${RM} ${OBJECTDIR}/_ext/2016467043/plib_uart1.o.d 
	@${RM} ${OBJECTDIR}/_ext/2016467043/plib_uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2016467043/plib_uart1.o.d" -o ${OBJECTDIR}/_ext/2016467043/plib_uart1.o ../src/config/bma400/peripheral/uart/plib_uart1.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1887586581/xc32_monitor.o: ../src/config/bma400/stdio/xc32_monitor.c  .generated_files/flags/mcj/a2add09b291d58cd86a0ee6d286e9d5ec346320a .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1887586581" 
	@${RM} ${OBJECTDIR}/_ext/1887586581/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1887586581/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1887586581/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/1887586581/xc32_monitor.o ../src/config/bma400/stdio/xc32_monitor.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1927388145/initialization.o: ../src/config/bma400/initialization.c  .generated_files/flags/mcj/edd3eeef59933e0773423fa909ba50babb2570a .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1927388145" 
	@${RM} ${OBJECTDIR}/_ext/1927388145/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1927388145/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1927388145/initialization.o.d" -o ${OBJECTDIR}/_ext/1927388145/initialization.o ../src/config/bma400/initialization.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1927388145/interrupts.o: ../src/config/bma400/interrupts.c  .generated_files/flags/mcj/e5247921dfd73f14a00224f621a4de9f8361ce0a .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1927388145" 
	@${RM} ${OBJECTDIR}/_ext/1927388145/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1927388145/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1927388145/interrupts.o.d" -o ${OBJECTDIR}/_ext/1927388145/interrupts.o ../src/config/bma400/interrupts.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1927388145/exceptions.o: ../src/config/bma400/exceptions.c  .generated_files/flags/mcj/e8e43acd9dcdc4ec2d9b5b4e1b2c6815105fc5b6 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1927388145" 
	@${RM} ${OBJECTDIR}/_ext/1927388145/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1927388145/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1927388145/exceptions.o.d" -o ${OBJECTDIR}/_ext/1927388145/exceptions.o ../src/config/bma400/exceptions.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/bma490l.o: ../src/bma490l.c  .generated_files/flags/mcj/76a94865f31577675a005d80030391e95f25ecb9 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bma490l.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bma490l.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bma490l.o.d" -o ${OBJECTDIR}/_ext/1360937237/bma490l.o ../src/bma490l.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/timers.o: ../src/timers.c  .generated_files/flags/mcj/8da699af2d74d168d80867eb713c5a6ecb38f2ca .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/timers.o ../src/timers.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/sca3300.o: ../src/sca3300.c  .generated_files/flags/mcj/d5d40f590c41122dfbd2de6702c7b589cc767452 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sca3300.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/sca3300.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/sca3300.o.d" -o ${OBJECTDIR}/_ext/1360937237/sca3300.o ../src/sca3300.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/gfx.o: ../src/gfx.c  .generated_files/flags/mcj/4b1a42d402802e62dbd95709e3f88beb133cf333 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gfx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gfx.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/gfx.o.d" -o ${OBJECTDIR}/_ext/1360937237/gfx.o ../src/gfx.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/imu.o: ../src/imu.c  .generated_files/flags/mcj/53206c4d9ffb79bc5944064d023e7e89b92ba898 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/imu.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/imu.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/imu.o.d" -o ${OBJECTDIR}/_ext/1360937237/imu.o ../src/imu.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/canfd.o: ../src/canfd.c  .generated_files/flags/mcj/4afbec9f5329bec7cd0faef7b60ab43315744932 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/canfd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/canfd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/canfd.o.d" -o ${OBJECTDIR}/_ext/1360937237/canfd.o ../src/canfd.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/pid.o: ../src/pid.c  .generated_files/flags/mcj/68b29b94ee3b6183b7999b03ffaa94cf2d41cf82 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pid.o.d" -o ${OBJECTDIR}/_ext/1360937237/pid.o ../src/pid.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/do_fft.o: ../src/do_fft.c  .generated_files/flags/mcj/5a4ecdffe6167464ec90cd5e69c16d4514c5dc76 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/do_fft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/do_fft.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/do_fft.o.d" -o ${OBJECTDIR}/_ext/1360937237/do_fft.o ../src/do_fft.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/fft.o: ../src/fft.c  .generated_files/flags/mcj/52ee9069a16f89cd33bc6a33df95504bda3e2153 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/fft.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/fft.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/fft.o.d" -o ${OBJECTDIR}/_ext/1360937237/fft.o ../src/fft.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/host.o: ../src/host.c  .generated_files/flags/mcj/e4647b2fd86b72731aa66dae9f554af786869dc7 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/host.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/host.o.d" -o ${OBJECTDIR}/_ext/1360937237/host.o ../src/host.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/mcj/93a8c14c41a3d69aac6820b14346ca61a199ae87 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/cmd_scanner.o: ../src/cmd_scanner.c  .generated_files/flags/mcj/fb5b283369165f5f5376861f7378e8a374006388 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cmd_scanner.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cmd_scanner.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/cmd_scanner.o.d" -o ${OBJECTDIR}/_ext/1360937237/cmd_scanner.o ../src/cmd_scanner.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/remote_cmd.o: ../src/remote_cmd.c  .generated_files/flags/mcj/8f8bdec49c692bce91341f6114264daf95226a74 .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/remote_cmd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/remote_cmd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/remote_cmd.o.d" -o ${OBJECTDIR}/_ext/1360937237/remote_cmd.o ../src/remote_cmd.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/imupic32mcj.o: ../src/imupic32mcj.c  .generated_files/flags/mcj/a00212d8111192b61ba709cc07da9ceb7d3436da .generated_files/flags/mcj/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/imupic32mcj.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/imupic32mcj.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O2 -fno-common -I"../src" -I"../src/config/mcj" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/imupic32mcj.o.d" -o ${OBJECTDIR}/_ext/1360937237/imupic32mcj.o ../src/imupic32mcj.c    -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/ll-tester.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../src/config/bma400/p32MK0512MCJ048.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/ll-tester.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x36F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/ll-tester.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../src/config/bma400/p32MK0512MCJ048.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/ll-tester.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_mcj=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=512,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex ${DISTDIR}/ll-tester.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
