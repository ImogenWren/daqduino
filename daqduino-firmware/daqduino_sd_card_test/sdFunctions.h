
#pragma once


bool sd_available;

char logName[9] = { "_log.txt" };  // File must use INT_xxxx.xxx filename structure for delimiting later
char logFileName[12];
File root;

void SD_setup() {
  Serial.print("Initializing SD card...");
  int attempts;
  while (!SD.begin(SD_SELECT_PIN)) {
    Serial.print("Attempt: ");
    Serial.print(attempts);
    Serial.println(" SD Card initialization failed!");
    attempts++;
    delay(500);
  }
  Serial.println("SD Card initialization done.");
}


int extractInt(char *filename) {
  char *token = strtok(filename, "_");  // Split the filename at the delimiter "_"
  //  Serial.println(token);
  int fileNum = atoi(token);  // Extract filenumber
  //   Serial.println(fileNum);
  free(filename);
  return fileNum;
}


int fileNoArray[64];  // Avoid the need for dynamic array INSTEAD:
int no_indexes = 0;    // Instead save the number of useful indexes seperately. Feels exceptionally clunky but it should work
 
int *printDirectory(File dir, int numTabs) {
  Serial.println("\nChecking SD Card Contents");

  int arrayIndex = 0;  // used it iterate array? This feels like a for loop but without knowing the size of the loop?
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      Serial.println("No More Files Found");
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    if (entry.isDirectory()) {
      Serial.println("/");
      //  printDirectory(entry, numTabs + 1);
    } else {

      // Extract the integer numbers from the filename to see which is the next free integer to use
      //  Serial.print(entry.name());  // This is the origional line to prints the file name so is the target to extract the integer
      char *filename = entry.name();  // Get the name of the file as a char string, do we need to worry about memory malloc? maybe? lets see if we run into issues without it.
      Serial.print(filename);
      Serial.print("   ");
      int fileNum = extractInt(filename);
     // Serial.print(fileNum);
     // Serial.print("   ");
     // free(filename);    // seeing online resources saying dont do this in arduino - Also was getting compiler warnings so removed.


      // Assuming that no directories are added to card - will have to investigate to see if that causes unexpected behaviour - removing avoids need for recursion
      //if (entry.isDirectory()) {
      //  Serial.println("/");
      //  printDirectory(entry, numTabs + 1);
      // } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
      entry.close();

      fileNoArray[arrayIndex] = fileNum;      
      arrayIndex++;  // Forgot to iterate the iteration variable! oops
      no_indexes = arrayIndex;  // Used in setup() to go back through array and only check for new entries // This can go after as index 1 means 2 indexes so needs to be iterated one more time
    }
  }
}





// build filename using number passed
void makeLogFileName(uint16_t logNumber) {
  sprintf(logFileName, "%02i%s", logNumber, logName);
  Serial.print("New File Created: ");
  Serial.println(logFileName);
}
