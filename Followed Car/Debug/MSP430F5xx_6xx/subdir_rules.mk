################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MSP430F5xx_6xx/%.obj: ../MSP430F5xx_6xx/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"H:/Software/CCS/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="H:/Study/2022ds/Followed Car/Function/pwm" --include_path="H:/Study/2022ds/Followed Car/MSP430F5xx_6xx" --include_path="H:/Study/2022ds/Followed Car/MSP430F5xx_6xx/inc" --include_path="H:/Software/CCS/ccs/ccs_base/msp430/include" --include_path="H:/Study/2022ds/Followed Car" --include_path="H:/Software/CCS/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/include" --advice:power="all" --define=__MSP430F5529__ -g --c11 --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="MSP430F5xx_6xx/$(basename $(<F)).d_raw" --obj_directory="MSP430F5xx_6xx" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


