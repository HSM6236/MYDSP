################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
App/da5620/%.obj: ../App/da5620/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"E:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="E:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="D:/课题组/project/sogi_in_dsp/newEisTemplate/App/fft" --include_path="D:/课题组/project/sogi_in_dsp/newEisTemplate/App/da5620" --include_path="D:/课题组/project/sogi_in_dsp/newEisTemplate/App/ad7606" --include_path="D:/课题组/project/sogi_in_dsp/newEisTemplate/App/sci-x" --include_path="D:/课题组/project/sogi_in_dsp/newEisTemplate/App/timer" --include_path="D:/课题组/project/sogi_in_dsp/newEisTemplate/Dsp2833x_Lib/include" --include_path="D:/课题组/project/sogi_in_dsp/newEisTemplate/App/beep" --include_path="D:/课题组/project/sogi_in_dsp/newEisTemplate/App/sogi" -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="App/da5620/$(basename $(<F)).d_raw" --obj_directory="App/da5620" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


