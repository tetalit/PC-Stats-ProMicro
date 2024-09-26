void DisplayStyle1B () { // HSMv1.3

  //-------------------------------------------Clearing Box ----------------------------------------------------

  //Clearing Boxes, eg: display.fillRect(<X>, ^Y^, W, H, Color);*/
  display.fillRect (0, 0, 128, 64, BLACK); 

  //--------------------------------------- Display Background ----------------------------------------------------
  display.setTextSize(2); 
  display.setCursor(1, 12);
  display.println("CPU");
  display.setCursor(1, 38);
  display.println("GPU");
  display.setTextSize(1);
  display.setCursor(1, 56);
  display.println("SYSRAM: ");

  //---------------------------------------CPU & GPU Hardware ID----------------------------------------------------

  /*получение id CPU и GPU*/
  display.setTextSize(0); // string font size
  String cpuName = "";
  display.println(cpuName);
  
  display.setTextSize(0); // string font size
  String gpuName = "";
  display.println(gpuName);

  if (inputString.indexOf("CPU") > -1)
  {
    String cpuName = "";
    display.setTextSize(1);
    //display.setCursor(-58, 1); // for my INTEL CORE i7-12700
    display.setCursor(-7, 1); //for usial cpu

    int cpuNameStart = inputString.indexOf("CPU:");
    if (inputString.indexOf( cpuNameStart) > -1) {
      cpuNameStart = cpuNameStart + 10;
    }
    else {
      cpuNameStart = cpuNameStart + 8;
    }
    if (inputString.indexOf("GPU") > -1)
    {
      cpuName = inputString.substring(cpuNameStart, inputString.indexOf("GPU:"));
    }
    else
      cpuName = inputString.substring(cpuNameStart);
    display.println(cpuName);

  }
  if (inputString.indexOf("GPU") > -1)
  {
    display.setTextSize(0);

    //display.setCursor(0, 28);
    display.setCursor(-41, 28); // без надписи NVIDIA
    //display.setCursor(-53, 28); // Negative spacing so, Nvidia doesn't cause a rollover, on the next line
    int gpuNameStart = inputString.indexOf("GPU:");
    if (inputString.indexOf(gpuNameStart) > -1) {
      gpuNameStart = gpuNameStart + 11;
    }
    else {
      gpuNameStart = gpuNameStart + 8;
    }

    int gpuNameEnd = inputString.indexOf("|", gpuNameStart);
    String gpuName = inputString.substring(gpuNameStart, gpuNameEnd);
    display.println(gpuName);
  }

//------------------------------------------ CPU Load/Temp -------------------------------------------------

  /*CPU строка*/
  int cpuStringStart = inputString.indexOf("C");
  int cpuDegree = inputString.indexOf("c");
  int cpuStringLimit = inputString.indexOf("|");
  String cpuString1 = inputString.substring(cpuStringStart + 1, cpuDegree);
  String cpuString2 = inputString.substring(cpuDegree + 1, cpuStringLimit - 1);

  /*CPU темпиратура*/
  display.setTextSize(2);
  display.setCursor(42, 12);
  display.print(cpuString1);
  display.setTextSize(1);

#ifdef noDegree
  display.print("C");   
#else
  display.print((char)247);
  display.print("C");     
#endif

  /*загрузка CPU по всем ядрам*/
  display.setTextSize(2);
  display.print(cpuString2);
  display.setTextSize(1);
  display.println("%");   

  //------------------------------------------ GPU Load/Temp -------------------------------------------------

  /*GPU настройки вывода*/
  int gpuStringStart = inputString.indexOf("G", cpuStringLimit);
  int gpuDegree = inputString.indexOf("c", gpuStringStart);
  int gpuStringLimit = inputString.indexOf("|", gpuStringStart);
  String gpuString1 = inputString.substring(gpuStringStart + 1, gpuDegree);
  String gpuString2 = inputString.substring(gpuDegree + 1, gpuStringLimit - 1);

  /*темпиратура GPU*/
  display.setTextSize(2);
  display.setCursor(42, 38);
  display.print(gpuString1);
  display.setTextSize(1);

#ifdef noDegree
  display.print("C");      
#else
  display.print((char)247);
  display.print("C");       
#endif

  /*загрузка GPU*/
  display.setTextSize(2);
  display.print(gpuString2);
  display.setTextSize(1);
  display.println("%");      

  //---------------------------------------------Total GPU Memory-----------------------------------------------------------

  int gpuMemoryStart = inputString.indexOf("GMT") + 3;
  int gpuMemoryEnd = inputString.indexOf("|", gpuMemoryStart);
  String gpuMemoryString = inputString.substring(gpuMemoryStart, gpuMemoryEnd);

  double totalGPUmem = atof(gpuMemoryString.c_str());
  double totalGPUmemSum = totalGPUmem / 1024;   
  float  totalGPUmemSumDP = totalGPUmemSum ;    

  //----------------------------------------SYSTEM  RAM TOTAL---------------------------------------------------
  /*SYSTEM RAM строка*/
  int ramStringStart = inputString.indexOf("R", gpuStringLimit);
  int ramStringLimit = inputString.indexOf("|", ramStringStart);
  String ramString = inputString.substring(ramStringStart + 1 , ramStringLimit);


  /*SYSTEM RAM AVALABLE доступная память*/
  int AramStringStart = inputString.indexOf("RA", ramStringLimit);
  int AramStringLimit = inputString.indexOf("|", AramStringStart);
  String AramString = inputString.substring(AramStringStart + 2 , AramStringLimit);

  /*SYSTEM RAM TOTAL всего оперативки*/
  double intRam = atof(ramString.c_str());
  double intAram = atof(AramString.c_str());
  //double  intRamSum = intRam + intAram;
  float  intRamSum = intRam + intAram; //float to handle the decimal point when printed (intRamSum,0)

  /*RAM USED/TOTAL*/
  display.setCursor(42, 56);
  display.print(intRamSum, 0); display.print(" / "); display.print(ramString); //display.print("GB");

  //--------------------------Trigger an event when CPU or GPU threshold is met ---------------------------------

#define enableCustomThesholdtriggers

//  CustomTriggerCPU_temp( cpuString1.toInt() ); // Neopixel CPU  Temperature
  //CustomTriggerCPU_load( cpuString2.toInt() ); // Neopixel CPU  Load

  //CustomTriggerGPU_temp
  rgbTape( gpuString1.toInt() ); // Neopixel GPU  Temperature
  //CustomTriggerGPU_load( gpuString2.toInt() ); // Neopixel GPU  Load

  oledDraw = 1;

}
