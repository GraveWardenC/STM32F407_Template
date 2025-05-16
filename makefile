arm-none-eabi-gcc -o "Shell.elf" @"objects.list" $(USER_OBJS) $(LIBS) 
-mcpu=cortex-m4 
-T"C:\Users\15185\STM32CubeIDE\workspace_1.18.0\Shell\STM32F407VGTX_FLASH.ld" 
--specs=nosys.specs 
-Wl,
-Map="Shell.map" 
-Wl,
--gc-sections 
-static 
--specs=nano.specs 
-mfpu=fpv4-sp-d16 
-mfloat-abi=hard 
-mthumb 
-Wl,
--start-group 
-lc 
-lm 
-Wl,
--end-group
	@echo '已结束构建目标： $@'