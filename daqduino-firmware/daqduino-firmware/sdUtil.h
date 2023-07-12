/*  SD card Utility Functions 

 - If using classes these would be private


*/

#pragma once



//extracts the integer value from filename format XX_log.txt
int extractInt(char *filename) {
  char *token = strtok(filename, "_");  // Split the filename at the delimiter "_"
  //  Serial.println(token);
  int fileNum = atoi(token);  // Extract filenumber
  //   Serial.println(fileNum);
 // free(filename);    // compiler warning for this, seen advice that says dont do this
  return fileNum;
}



 
// Print the directory found on the SD card. Modified from Arduino Example listFiles
void printDirectory(File dir, int numTabs) {
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



// build filename using number passed. Output Format XX_log.txt
void makeLogFileName(uint16_t logNumber) {
  sprintf(logFileName, "%02i%s", logNumber, logName);
  Serial.print("New File Created: ");
  Serial.println(logFileName);
}