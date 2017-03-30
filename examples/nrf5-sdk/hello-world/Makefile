PROJECT_NAME     := hello_world_nrf52832-mdk
TARGETS          := nrf52832_xxaa
OUTPUT_DIRECTORY := _build

SDK_ROOT := ../../../nrf_sdks/nRF5_SDK_13.0.0_04a0bfd
PROJ_DIR := .

$(OUTPUT_DIRECTORY)/nrf52832_xxaa.out: \
  LINKER_SCRIPT  := hello_world_gcc_nrf52.ld

# Source files common to all targets
SRC_FILES += $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c
SRC_FILES += $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c
SRC_FILES += $(SDK_ROOT)/components/boards/boards.c
SRC_FILES += $(SDK_ROOT)/components/libraries/util/app_error.c
SRC_FILES += $(SDK_ROOT)/components/libraries/util/app_error_weak.c
SRC_FILES += $(SDK_ROOT)/components/libraries/scheduler/app_scheduler.c
SRC_FILES += $(SDK_ROOT)/components/libraries/timer/app_timer.c
SRC_FILES += $(SDK_ROOT)/components/libraries/util/app_util_platform.c
SRC_FILES += $(SDK_ROOT)/components/libraries/hardfault/hardfault_implementation.c
SRC_FILES += $(SDK_ROOT)/components/libraries/util/nrf_assert.c
SRC_FILES += $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c
SRC_FILES += $(SDK_ROOT)/components/drivers_nrf/clock/nrf_drv_clock.c
SRC_FILES += $(SDK_ROOT)/components/drivers_nrf/common/nrf_drv_common.c
SRC_FILES += $(SDK_ROOT)/components/drivers_nrf/uart/nrf_drv_uart.c
SRC_FILES += $(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd/nrf_nvic.c
SRC_FILES += $(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd/nrf_soc.c
SRC_FILES += $(PROJ_DIR)/src/main.c
SRC_FILES += $(SDK_ROOT)/components/toolchain/gcc/gcc_startup_nrf52.S
SRC_FILES += $(SDK_ROOT)/components/toolchain/system_nrf52.c

# Include folders common to all targets
INC_FOLDERS += $(PROJ_DIR)/inc
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/comp
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/twi_master
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_ancs_c
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_ias_c
INC_FOLDERS += $(SDK_ROOT)/components/libraries/pwm
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd/class/cdc/acm
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd/class/hid/generic
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd/class/msc
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd/class/hid
INC_FOLDERS += $(SDK_ROOT)/components/libraries/log
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_gls
INC_FOLDERS += $(SDK_ROOT)/components/libraries/fstorage
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/i2s
INC_FOLDERS += $(SDK_ROOT)/components/libraries/gpiote
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/gpiote
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd
INC_FOLDERS += $(SDK_ROOT)/components/boards
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/common
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_advertising
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_bas_c
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_hrs_c
INC_FOLDERS += $(SDK_ROOT)/components/libraries/queue
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_dtm
INC_FOLDERS += $(SDK_ROOT)/components/toolchain/cmsis/include
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_rscs_c
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/uart
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd/class/hid/kbd
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_lls
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/wdt
INC_FOLDERS += $(SDK_ROOT)/components/libraries/bsp
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_bas
INC_FOLDERS += $(SDK_ROOT)/components/libraries/experimental_section_vars
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_ans_c
INC_FOLDERS += $(SDK_ROOT)/components/libraries/slip
INC_FOLDERS += $(SDK_ROOT)/external/segger_rtt
INC_FOLDERS += $(SDK_ROOT)/components/libraries/csense_drv
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/hal
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_nus_c
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/rtc
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_ias
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd/class/hid/mouse
INC_FOLDERS += $(SDK_ROOT)/components/libraries/ecc
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/ppi
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_dfu
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/twis_slave
INC_FOLDERS += $(SDK_ROOT)/components
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/usbd
INC_FOLDERS += $(SDK_ROOT)/components/libraries/scheduler
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_lbs
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_hts
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/delay
INC_FOLDERS += $(SDK_ROOT)/components/libraries/crc16
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/timer
INC_FOLDERS += $(SDK_ROOT)/components/libraries/util
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/pwm
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd/class/cdc
INC_FOLDERS += $(SDK_ROOT)/components/libraries/csense
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/rng
INC_FOLDERS += $(SDK_ROOT)/components/libraries/low_power_pwm
INC_FOLDERS += $(SDK_ROOT)/components/libraries/hardfault
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_cscs
INC_FOLDERS += $(SDK_ROOT)/components/libraries/uart
INC_FOLDERS += $(SDK_ROOT)/components/libraries/hci
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/spi_slave
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/lpcomp
INC_FOLDERS += $(SDK_ROOT)/components/libraries/timer
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/power
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd/config
INC_FOLDERS += $(SDK_ROOT)/components/toolchain
INC_FOLDERS += $(SDK_ROOT)/components/libraries/led_softblink
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/qdec
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_cts_c
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/spi_master
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_nus
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_hids
INC_FOLDERS += $(SDK_ROOT)/components/libraries/strerror
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/pdm
INC_FOLDERS += $(SDK_ROOT)/components/libraries/crc32
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd/class/audio
INC_FOLDERS += $(SDK_ROOT)/components/ble/peer_manager
INC_FOLDERS += $(SDK_ROOT)/components/libraries/mem_manager
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_tps
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_dis
INC_FOLDERS += $(SDK_ROOT)/components/device
INC_FOLDERS += $(SDK_ROOT)/components/ble/nrf_ble_qwr
INC_FOLDERS += $(SDK_ROOT)/components/libraries/button
INC_FOLDERS += $(SDK_ROOT)/components/libraries/usbd
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/saadc
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_lbs_c
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_racp
INC_FOLDERS += $(SDK_ROOT)/components/toolchain/gcc
INC_FOLDERS += $(SDK_ROOT)/components/libraries/fds
INC_FOLDERS += $(SDK_ROOT)/components/libraries/twi
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/clock
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_rscs
INC_FOLDERS += $(SDK_ROOT)/components/drivers_nrf/swi
INC_FOLDERS += $(SDK_ROOT)/components/ble/ble_services/ble_hrs
INC_FOLDERS += $(SDK_ROOT)/components/libraries/log/src

# Libraries common to all targets
LIB_FILES += \

# C flags common to all targets
CFLAGS += -DBOARD_CUSTOM
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DNRF52
CFLAGS += -DNRF52832_XXAA
CFLAGS += -DNRF52_PAN_12
CFLAGS += -DNRF52_PAN_15
CFLAGS += -DNRF52_PAN_20
CFLAGS += -DNRF52_PAN_31
CFLAGS += -DNRF52_PAN_36
CFLAGS += -DNRF52_PAN_51
CFLAGS += -DNRF52_PAN_54
CFLAGS += -DNRF52_PAN_55
CFLAGS += -DNRF52_PAN_58
CFLAGS += -DNRF52_PAN_64
CFLAGS += -DNRF52_PAN_74
CFLAGS += -DSWI_DISABLE0
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS +=  -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums 

# C++ flags common to all targets
CXXFLAGS += \

# Assembler flags common to all targets
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DBOARD_CUSTOM
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DNRF52
ASMFLAGS += -DNRF52832_XXAA
ASMFLAGS += -DNRF52_PAN_12
ASMFLAGS += -DNRF52_PAN_15
ASMFLAGS += -DNRF52_PAN_20
ASMFLAGS += -DNRF52_PAN_31
ASMFLAGS += -DNRF52_PAN_36
ASMFLAGS += -DNRF52_PAN_51
ASMFLAGS += -DNRF52_PAN_54
ASMFLAGS += -DNRF52_PAN_55
ASMFLAGS += -DNRF52_PAN_58
ASMFLAGS += -DNRF52_PAN_64
ASMFLAGS += -DNRF52_PAN_74
ASMFLAGS += -DSWI_DISABLE0

# Linker flags
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys


.PHONY: $(TARGETS) default all clean help flash 

# Default target - first one defined
default: nrf52832_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo 	nrf52832_xxaa

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

# Flash the program
flash: $(OUTPUT_DIRECTORY)/nrf52832_xxaa.hex
	@echo Flashing: $<
	pyocd-flashtool -d debug -t nrf52 -se $<

# Flash softdevice
flash_softdevice:
	@echo Flashing: s132_nrf52_4.0.2_softdevice.hex
	pyocd-flashtool -d debug -t nrf52 -se $(SDK_ROOT)/components/softdevice/s132/hex/s132_nrf52_4.0.2_softdevice.hex

# chip erase
erase:
	pyocd-flashtool -d debug -t nrf52 -ce


