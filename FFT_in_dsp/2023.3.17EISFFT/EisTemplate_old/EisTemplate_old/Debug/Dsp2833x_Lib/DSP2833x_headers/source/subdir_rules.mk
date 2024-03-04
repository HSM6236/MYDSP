################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Dsp2833x_Lib/DSP2833x_headers/source/DSP2833x_GlobalVariableDefs.obj: ../Dsp2833x_Lib/DSP2833x_headers/source/DSP2833x_GlobalVariableDefs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS/ccsv6/tools/compiler/ti-cgt-c2000_18.12.8/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/CCS/ccsv6/tools/compiler/ti-cgt-c2000_18.12.8/include" --include_path="F:/CCS_Pro/EisTemplate/Dsp2833x_Lib/DSP2833x_headers/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Dsp2833x_Lib/DSP2833x_headers/source/DSP2833x_GlobalVariableDefs.d" --obj_directory="Dsp2833x_Lib/DSP2833x_headers/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


