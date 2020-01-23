
bool ynfunc(char *buf) {
	return (strcmp(buf, "true") == 0 ||
		strcmp(buf, "True") == 0 ||
		strcmp(buf, "TRUE") == 0 ||
		strcmp(buf, "T") == 0 ||
		strcmp(buf, "t") == 0 ||
		strcmp(buf, "yes") == 0 ||
		strcmp(buf, "Yes") == 0 ||
		strcmp(buf, "YES") == 0 ||
		strcmp(buf, "Y") == 0 ||
		strcmp(buf, "y") == 0 ||
		strcmp(buf, "1") == 0
		);
}

void settingLoad() {

	FILE *sfp = fopen("config.ini", "r");
	// 開けない時はデフォルト値
	if(sfp == NULL) {
		
		// 開けたら読み込む
	} else {
		char buf[64];
		fscanf(sfp, "UseBluetooth = %s\n", buf);
		usebt = ynfunc(buf);

		fscanf(sfp, " ButtonEnable = %s\n", buf);
		buttonEnable = ynfunc(buf);

		fscanf(sfp, " LineTraceEnable = %s\n", buf);
		lineTraceEnable = ynfunc(buf);

		fscanf(sfp, " BlockBingoEnable = %s\n", buf);
		blockBingoEnable = ynfunc(buf);

		// FILE *readlog = fopen("readresult.log", "w");
		// fprintf(readlog, "UseBluetooth = %d\n", usebt);
		// fclose(readlog);
		settingLoaded = true;
	}

	if(sfp != nullptr) fclose(sfp);
}