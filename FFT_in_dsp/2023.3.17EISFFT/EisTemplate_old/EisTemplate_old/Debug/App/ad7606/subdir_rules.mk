################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
App/ad7606/%.obj: ../App/ad7606/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"E:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="E:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="D:/project/EisTemplate_old/EisTemplate_old/App/da5620" --include_path="D:/project/EisTemplate_old/EisTemplate_old/App/ad7606" --include_path="D:/project/EisTemplate_old/EisTemplate_old/App/sci-x" --include_path="D:/project/EisTemplate_old/EisTemplate_old/App/timer" --include_path="D:/project/EisTemplate_old/EisTemplate_old/Dsp2833x_Lib/include" --include_path="D:/project/EisTemplate_old/EisTemplate_old/App/beep" --include_path="D:/project/EisTemplate_old/EisTemplate_old/App/fft" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="App/ad7606/$(basename $(<F)).d_raw" --obj_directory="App/ad7606" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


